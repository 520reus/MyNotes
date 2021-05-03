/******************************************************************************
 * Copyright 2017 The Apollo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/**
 * @file dp_road_graph.cc
 **/

#include "modules/planning/tasks/optimizers/road_graph/dp_road_graph.h"

#include "cyber/task/task.h"

#include "modules/common/proto/error_code.pb.h"
#include "modules/planning/proto/planning_internal.pb.h"
#include "modules/planning/proto/planning_status.pb.h"

#include "cyber/common/log.h"
#include "modules/common/configs/vehicle_config_helper.h"
#include "modules/common/math/cartesian_frenet_conversion.h"
#include "modules/common/util/util.h"
#include "modules/map/hdmap/hdmap_util.h"
#include "modules/planning/common/path/frenet_frame_path.h"
#include "modules/planning/common/planning_context.h"
#include "modules/planning/common/planning_gflags.h"
#include "modules/planning/math/curve1d/quintic_polynomial_curve1d.h"

namespace apollo {
namespace planning {

DpRoadGraph::DpRoadGraph(const DpPolyPathConfig &config,               //proto定义的数据格式
                         const ReferenceLineInfo &reference_line_info,  //ReferenceLineInfo类
                         const SpeedData &speed_data)                  //SpeedData类
    : config_(config),
      reference_line_info_(reference_line_info),
      reference_line_(reference_line_info.reference_line()),  
      speed_data_(speed_data) {}
      
/**
 *  FindPathTunnel()主要分为3部分：先设置相关前提条件，然后查找代价最小路径，
 * 最后对每段代价最小路径采样以构造FrenetFramePath类的实例，并存入path_data中。
 * @param init_point,待求解路径的起始轨迹点
 * @param obstacles,障碍物序列
 * @param path_data,FindPathTunnel()的结果是依据若干level之间分段5次多项式的采样点，
                    保存在path_data.frenet_path_（SL系）和path_data.discretized_path_（XY系）中
 */  
bool DpRoadGraph::FindPathTunnel(const common::TrajectoryPoint &init_point,
                                 const std::vector<const Obstacle *> &obstacles,
                                 PathData *const path_data) {
  CHECK_NOTNULL(path_data);

  init_point_ = init_point; //XY系下的起始轨迹点
  //转换到SL坐标系下，S0，L0存入init_sl_point_
  if (!reference_line_.XYToSL(init_point_.path_point(), &init_sl_point_)) {
    AERROR << "Fail to create init_sl_point from : "
           << init_point.DebugString();
    return false;
  }
  //起始xy坐标点[x0,y0] to SL坐标点[s(0),l(0),dl(0),ddl(0)] 
  //TODO 搞清楚ReferenceLine类中XYToSL函数与GetFrenetPoint函数的作用,尤其是XY是如何映射到SL系下的？
  init_frenet_frame_point_ =
      reference_line_.GetFrenetPoint(init_point_.path_point());
  //采样函数初始化
  waypoint_sampler_->Init(&reference_line_info_, init_sl_point_,
                          init_frenet_frame_point_);
  waypoint_sampler_->SetDebugLogger(planning_debug_);

  std::vector<DpRoadGraphNode> min_cost_path; //存储最小cost的路径
  //查找代价最小的路径，结果存入min_cost_path
  if (!GenerateMinCostPath(obstacles, &min_cost_path)) {
    AERROR << "Fail to generate graph!";
    return false;
  }
  std::vector<common::FrenetFramePoint> frenet_path; //存储转换到SL系下的path信息
  double accumulated_s = min_cost_path.front().sl_point.s(); //s0
  const double path_resolution = config_.path_resolution(); //采样间隔
  //针对找到的最短路径
  for (size_t i = 1; i < min_cost_path.size(); ++i) {
    const auto &prev_node = min_cost_path[i - 1];
    const auto &cur_node = min_cost_path[i];
    //每段路径长
    const double path_length = cur_node.sl_point.s() - prev_node.sl_point.s();
    double current_s = 0.0;
    const auto &curve = cur_node.min_cost_curve;
    //对每一段curve采样
    while (current_s + path_resolution / 2.0 < path_length) {
      const double l = curve.Evaluate(0, current_s);//TODO 搞明白插值原理
      const double dl = curve.Evaluate(1, current_s);
      const double ddl = curve.Evaluate(2, current_s);
      common::FrenetFramePoint frenet_frame_point;
      frenet_frame_point.set_s(accumulated_s + current_s);
      frenet_frame_point.set_l(l);
      frenet_frame_point.set_dl(dl); 
      frenet_frame_point.set_ddl(ddl);
      frenet_path.push_back(std::move(frenet_frame_point));
      current_s += path_resolution;
    }
    if (i == min_cost_path.size() - 1) {
      accumulated_s += current_s;
    } else {
      accumulated_s += path_length;
    }
  }
  path_data->SetReferenceLine(&reference_line_);
  path_data->SetFrenetPath(FrenetFramePath(std::move(frenet_path)));
  return true;
}

/**
 * 查找代价最小路径的核心在于GenerateMinCostPath()，也是分为3部分：先采样，然后构造graph，
 * 最后查找从起点（自车当前位置）到终点（尽可能远的某个采样点）的代价最小路径。 
 * @param obstacles,障碍物序列
 * @param min_cost_path,用于存储生成的最小代价路径序列，路径上的每一个点都是一个DpRoadGraphNode类
 */
bool DpRoadGraph::GenerateMinCostPath(
    const std::vector<const Obstacle *> &obstacles,
    std::vector<DpRoadGraphNode> *min_cost_path) {    
  CHECK(min_cost_path != nullptr);
  //对所在车道横纵向采样，结果存入path_waypoints，
  //其中的每个元素是从横向一个level上采样得到的小点集
  std::vector<std::vector<common::SLPoint>> path_waypoints;
  if (!waypoint_sampler_->SamplePathWaypoints(init_point_, &path_waypoints) ||
      path_waypoints.size() < 1) {
    AERROR << "Fail to sample path waypoints! reference_line_length = "
           << reference_line_.Length();
    return false;
  }
  const auto &vehicle_config =
      common::VehicleConfigHelper::Instance()->GetConfig();
  //TODO 搞清楚TrajectoryCost类的作用
  TrajectoryCost trajectory_cost(
      config_, reference_line_, reference_line_info_.IsChangeLanePath(),
      obstacles, vehicle_config.vehicle_param(), speed_data_, init_sl_point_,
      reference_line_info_.AdcSlBoundary());

  //构造graph，graph_nodes中的每个元素是依据横向一个level上采样得到点集构成的
  std::list<std::list<DpRoadGraphNode>> graph_nodes;

  // find one point from first row
  const auto &first_row = path_waypoints.front();
  size_t nearest_i = 0;
  //找到第一个level上的离车辆当前位置最近的点
  for (size_t i = 1; i < first_row.size(); ++i) {
    if (std::fabs(first_row[i].l() - init_sl_point_.l()) <
        std::fabs(first_row[nearest_i].l() - init_sl_point_.l())) {
      nearest_i = i;
    }
  }
  //产生第一层level
  graph_nodes.emplace_back();
  //将第一个level上的离车辆当前位置最近的点添加到graph的第一个level的点集中
  graph_nodes.back().emplace_back(first_row[nearest_i], nullptr,
                                  ComparableCost());
  auto &front = graph_nodes.front().front();  //采样起点
  size_t total_level = path_waypoints.size(); //采样层数

  //一层层的来处理
  for (size_t level = 1; level < path_waypoints.size(); ++level) {
    const auto &prev_dp_nodes = graph_nodes.back();
    const auto &level_points = path_waypoints[level]; //每一层的采样点
    //graph下一层的level
    graph_nodes.emplace_back();
    std::vector<std::future<void>> results;
    //对该层每个节点，即cur_node进行处理
    for (size_t i = 0; i < level_points.size(); ++i) {
      const auto &cur_point = level_points[i];
      //
      graph_nodes.back().emplace_back(cur_point, nullptr);
      //构建路图消息结构，用于存储处理当前层节点需要的信息
      auto msg = std::make_shared<RoadGraphMessage>(
          prev_dp_nodes, level, total_level, &trajectory_cost, &front,
          &(graph_nodes.back().back()));  //graph_nodes.back().back()为当前node

      if (FLAGS_enable_multi_thread_in_dp_poly_path) {
        results.emplace_back(cyber::Async(&DpRoadGraph::UpdateNode, this, msg));
      } else {
        //更新当前操作节点node的cost，更新后，该node保存了起点到该点的最小代价、pre_node以及两者间的curve
       //prev node ---> node间用5次polynomial curve
        UpdateNode(msg);
      }
    }
    if (FLAGS_enable_multi_thread_in_dp_poly_path) {
      for (auto &result : results) {
        result.get();
      }
    }
  }
  //以上只是把每个node与起点的最小代价计算出来了，并没有寻找代价最小的路径
  // find best path 寻找最短路径
  DpRoadGraphNode fake_head;
  //在最后的level采样点中，寻找代价最小的节点，作为一次规划的终点，即fake_head的prev
  //其实其他level的采样点很可能代价更小，这里为什么从最后的level选呢？
  //因为规划的长度和时间越长越好，规划越长，相当于自车看到的越远
  for (const auto &cur_dp_node : graph_nodes.back()) {
    //取最后一层level的cost最小的点，即为DP处理后的路径终点，并用fake_head指向该路径终点
    fake_head.UpdateCost(&cur_dp_node, cur_dp_node.min_cost_curve, 
                         cur_dp_node.min_cost);
  }
  //下面从cost最小的路径的尾巴开始向前回溯，找到路径上的所有点
  const auto *min_cost_node = &fake_head;
  //min_cost_path没有把fake_head保存进去,fake_head->min_cost_prev_node才是真正的终点
  while (min_cost_node->min_cost_prev_node) {
    min_cost_node = min_cost_node->min_cost_prev_node;
    min_cost_path->push_back(*min_cost_node);    
  }
  if (min_cost_node != &graph_nodes.front().front()) { //最终一定处理到起始Node结束，否则处理失败
    return false;
  }

  std::reverse(min_cost_path->begin(), min_cost_path->end());  //把头尾翻转回来，改成从初始点开始的点序列

  for (const auto &node : *min_cost_path) {
    ADEBUG << "min_cost_path: " << node.sl_point.ShortDebugString();
    planning_debug_->mutable_planning_data()
        ->mutable_dp_poly_graph()
        ->add_min_cost_point()
        ->CopyFrom(node.sl_point);
  }
  return true;
}

/* 
对当前node进行更新，更新后，该node保存了起点到该点的最小代价、prev node、以及curve
该函数主要做了3件事：在current node与任一prev node间、以及current node与first node间，
构造5次polynomial curve；计算这2个node间的cost；更新current node的cost。
 */
void DpRoadGraph::UpdateNode(const std::shared_ptr<RoadGraphMessage> &msg) {
  CHECK_NOTNULL(msg);
  CHECK_NOTNULL(msg->trajectory_cost);
  CHECK_NOTNULL(msg->front);
  CHECK_NOTNULL(msg->cur_node);
  //对当前节点与前层所有采样点之间进行计算比较，然后更新当前node
  for (const auto &prev_dp_node : msg->prev_nodes) {
    const auto &prev_sl_point = prev_dp_node.sl_point;
    const auto &cur_point = msg->cur_node->sl_point;
    double init_dl = 0.0;
    double init_ddl = 0.0;
    // level=1 表示第一层只有当前车辆位置的节点，下一层才是采样层
    if (msg->level == 1) {
      //仅自车当前姿态有dl（角度朝向）,ddl，其余点的dl,ddl都是0
      init_dl = init_frenet_frame_point_.dl();
      init_ddl = init_frenet_frame_point_.ddl();
    }
    //纵向相邻level间的任意两点间，都是一条5次多项式曲线
    QuinticPolynomialCurve1d curve(prev_sl_point.l(), init_dl, init_ddl,
                                   cur_point.l(), 0.0, 0.0,
                                   cur_point.s() - prev_sl_point.s());

    if (!IsValidCurve(curve)) {
      continue;
    }
    const auto cost =
        msg->trajectory_cost->Calculate(curve, prev_sl_point.s(), cur_point.s(),
                                        msg->level, msg->total_level) +
        prev_dp_node.min_cost;       

    msg->cur_node->UpdateCost(&prev_dp_node, curve, cost); //更新cost，根据cost指定pre_node与相应curve
  }

  // try to connect the current point with the first point directly
  /* 
  自车当前点（即起始点）与后面任意采样点间都构造了一条5次多项式曲线
  目的是比较当前点到起点之间的曲线cost小，还是当前点与pre_node之间的cost+pre_node.min_cost更小！
  若从起点之间连接该点的曲线cost更小，则将该点的pre_node之间设置为起始节点！
  这样做的好处是不完全依靠前层DP积累的经验，直接从起点连接到该点生成一段曲线也不会耗费很多算力 
  */
  if (reference_line_info_.IsChangeLanePath() && msg->level >= 2) {
    const double init_dl = init_frenet_frame_point_.dl();
    const double init_ddl = init_frenet_frame_point_.ddl();
    QuinticPolynomialCurve1d curve(
        init_sl_point_.l(), init_dl, init_ddl, msg->cur_node->sl_point.l(), 0.0,
        0.0, msg->cur_node->sl_point.s() - init_sl_point_.s());
    if (!IsValidCurve(curve)) {
      return;    
    }
    const auto cost = msg->trajectory_cost->Calculate(
        curve, init_sl_point_.s(), msg->cur_node->sl_point.s(), msg->level,
        msg->total_level);
    msg->cur_node->UpdateCost(msg->front, curve, cost);
  }
}

//判断曲线是否有效
bool DpRoadGraph::IsValidCurve(const QuinticPolynomialCurve1d &curve) const {
  static constexpr double kMaxLateralDistance = 20.0;
  for (double s = 0.0; s < curve.ParamLength(); s += 2.0) {
    const double l = curve.Evaluate(0, s);
    if (std::fabs(l) > kMaxLateralDistance) {
      return false;
    }
  }
  return true;
}

//计算曲线cost
void DpRoadGraph::GetCurveCost(TrajectoryCost trajectory_cost,
                               const QuinticPolynomialCurve1d &curve,
                               const double start_s, const double end_s,
                               const uint32_t curr_level,
                               const uint32_t total_level,
                               ComparableCost *cost) {
  *cost =
      trajectory_cost.Calculate(curve, start_s, end_s, curr_level, total_level);
}

}  // namespace planning
}  // namespace apollo

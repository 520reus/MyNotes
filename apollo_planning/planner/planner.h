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
 * 文件中，定义了2个类：Planner类和 PlannerWithReferenceLine类。其中，Planner类是所有规划器的基类，
 * 重要函数有Init() 和 Plan()。PlannerWithReferenceLine类也是继承自Planner类，
 * 重要函数有PlanOnReferenceLine()。
 * planning 模块中有4种规划器，分别为：RTKReplayPlanner，PublicRoadPlanner，NaviPlanner，LatticePlanner。
 * 每个规划器针对不同的场景和问题。在这4个规划器中，都实现了继承自Planner类的Plan()函数和继承自
 * PlannerWithReferenceLine类的PlanOnReferenceLine()函数。在执行具体的规划任务时，都是在Plan()
 * 中调用PlanOnReferenceLine()，从而获得规划的轨迹结果。也就是说，最底层的规划方法，是在各规划器的
 * PlanOnReferenceLine()中实现。​​
 */

#pragma once

#include <string>

#include "modules/common/proto/pnc_point.pb.h"
#include "modules/planning/proto/planning_config.pb.h"

#include "modules/common/status/status.h"
#include "modules/planning/common/frame.h"
#include "modules/planning/scenarios/scenario.h"
#include "modules/planning/scenarios/scenario_manager.h"

/**
 * @namespace apollo::planning
 * @brief apollo::planning
 */
namespace apollo {
namespace planning {

/**
 * @class Planner
 * @brief Planner is a base class for specific planners.
 *        It contains a pure virtual function Plan which must be implemented in
 * derived class.
 */


class Planner {
 public:
  /**
   * @brief Constructor
   */
  Planner() = default;

  /**
   * @brief Destructor
   */
  virtual ~Planner() = default;

  virtual std::string Name() = 0;
  virtual apollo::common::Status Init(const PlanningConfig& config) = 0;

  /**
   * @brief Compute trajectories for execution.
   * @param planning_init_point The trajectory point where planning starts.
   * @param frame Current planning frame.
   * @return OK if planning succeeds; error otherwise.
   */
  virtual apollo::common::Status Plan(
      const common::TrajectoryPoint& planning_init_point, Frame* frame,
      ADCTrajectory* ptr_computed_trajectory) = 0;

  virtual void Stop() = 0;

 protected:
  PlanningConfig config_;
  scenario::ScenarioManager scenario_manager_;
  scenario::Scenario* scenario_ = nullptr;
};

class PlannerWithReferenceLine : public Planner {
 public:
  /**
   * @brief Constructor
   */
  PlannerWithReferenceLine() = default;

  /**
   * @brief Destructor
   */
  virtual ~PlannerWithReferenceLine() = default;

  /**
   * @brief Compute a trajectory for execution.
   * @param planning_init_point The trajectory point where planning starts.
   * @param frame Current planning frame.
   * @param reference_line_info The computed reference line.
   * @return OK if planning succeeds; error otherwise.
   */
  virtual apollo::common::Status PlanOnReferenceLine(
      const common::TrajectoryPoint& planning_init_point, Frame* frame,
      ReferenceLineInfo* reference_line_info) {
    CHECK_NOTNULL(frame);
    return apollo::common::Status::OK();
  }
};

}  // namespace planning
}  // namespace apollo

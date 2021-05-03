/******************************************************************************
 * Copyright 2018 The Apollo Authors. All Rights Reserved.
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
 * NaviPlannerDispatcher类继承自PlannerDispatcher类，实现了DispatchPlanner()。其从config文件
 * （apollo\modules\planning\conf\planning_config_navi.pb.txt）中读取planner_type，
 * 然后创建对应类型的planner。而配置文件中设定planner_type为“NAVI”，对应NaviPlanner。 
 */

#pragma once

#include <memory>

#include "modules/common/status/status.h"
#include "modules/common/util/factory.h"
#include "modules/planning/common/planning_gflags.h"
#include "modules/planning/planner/planner_dispatcher.h"

/**
 * @namespace apollo::planning
 * @brief apollo::planning
 */
namespace apollo {
namespace planning {

/**
 * @class planning
 *
 * @brief PlannerDispatcher module main class.
 */
//final关键字修饰类，让该类不能被继承。
class NaviPlannerDispatcher final : public PlannerDispatcher {
 public:
  NaviPlannerDispatcher() = default;
  virtual ~NaviPlannerDispatcher() = default;

  std::unique_ptr<Planner> DispatchPlanner() override;
};

}  // namespace planning
}  // namespace apollo

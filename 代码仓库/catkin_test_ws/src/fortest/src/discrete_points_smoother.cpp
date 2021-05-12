#include "discrete_points_smoother.h"
#include "discrete_points_smooth_ipopt_interface.h"

#include "IpIpoptApplication.hpp"
#include "IpSolveStatistics.hpp"

#include <iostream>
using std::cout;
using std::endl;



//Solve函数会根据配置文件调用QpWithOsqp，NlpWithIpopt和SqpWithOsqp三个函数
//如果考虑参考线的曲率约束，其优化问题是非线性的，可以使用ipopt非线性求解器求解（内点法），
//也可以使用osqp二次规划求解器来用SQP方法求解；如果不考虑曲率约束，则直接用osqp求解二次规划问题。
bool FemPosDeviationSmoother::Solve(
    const std::vector<std::pair<double, double>>& raw_point2d,
    const std::vector<double>& bounds, std::vector<double>* opt_x,
    std::vector<double>* opt_y) {
        
      return NlpWithIpopt(raw_point2d, bounds, opt_x, opt_y);
}

//考虑曲率约束，直接用IPOPT非线性求解（内点法）
bool FemPosDeviationSmoother::NlpWithIpopt(
    const std::vector<std::pair<double, double>>& raw_point2d,
    const std::vector<double>& bounds, std::vector<double>* opt_x,
    std::vector<double>* opt_y) {
  if (opt_x == nullptr || opt_y == nullptr) {
     cout<< "opt_x or opt_y is nullptr"<<endl;
    return false;
  }

  FemPosDeviationIpoptInterface* smoother =
      new FemPosDeviationIpoptInterface(raw_point2d, bounds);

  smoother->set_weight_fem_pos_deviation(weight_fem_pos_deviation);
  smoother->set_weight_path_length(weight_path_length);
  smoother->set_weight_ref_deviation(weight_ref_deviation);
  smoother->set_weight_curvature_constraint_slack_var(weight_curvature_constraint_slack_var);
  smoother->set_curvature_constraint(curvature_constraint);

  Ipopt::SmartPtr<Ipopt::TNLP> problem = smoother;

  // Create an instance of the IpoptApplication
  Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();

  app->Options()->SetIntegerValue("print_level",
                                  static_cast<int>(print_level));
  app->Options()->SetIntegerValue(
      "max_iter", static_cast<int>(max_iter));
  app->Options()->SetIntegerValue(
      "acceptable_iter",
      static_cast<int>(acceptable_iter));
  app->Options()->SetNumericValue("tol", tol);
  app->Options()->SetNumericValue("acceptable_tol", acceptable_tol);

  Ipopt::ApplicationReturnStatus status = app->Initialize();
  if (status != Ipopt::Solve_Succeeded) {
    cout << "*** Error during initialization!" << endl;
    return false;
  }

  status = app->OptimizeTNLP(problem);

  if (status == Ipopt::Solve_Succeeded ||
      status == Ipopt::Solved_To_Acceptable_Level) {
    // Retrieve some statistics about the solve
    Ipopt::Index iter_count = app->Statistics()->IterationCount();
    cout << "*** The problem solved in " << iter_count << " iterations!" << endl;
  } else {
    cout << "Solver fails with return code: " << static_cast<int>(status) << endl;
    return false;
  }
  smoother->get_optimization_results(opt_x, opt_y);
  return true;
}


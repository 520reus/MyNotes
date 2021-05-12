#pragma once

#include <utility>
#include <vector>

/*
 * @brief:
 * This class solve an optimization problem:
 * Y
 * |
 * |                       P(x1, y1)  P(x2, y2)
 * |            P(x0, y0)                       ... P(x(k-1), y(k-1))
 * |P(start)
 * |
 * |________________________________________________________ X
 *
 *
 * Given an initial set of points from 0 to k-1,  The goal is to find a set of
 * points which makes the line P(start), P0, P(1) ... P(k-1) "smooth".
 */
// algorithm peremeters
const double weight_fem_pos_deviation = 1e8;
const double weight_path_length = 1.0;
const double weight_ref_deviation = 50;
const double weight_curvature_constraint_slack_var = 1e5;
const double curvature_constraint = 0.2;
// ipopt options
const size_t max_iter = 500;
const size_t print_level = 5;
const size_t acceptable_iter = 15;
const double tol = 1e-8;
const size_t acceptable_tol = 1e-1;


class FemPosDeviationSmoother {
 public:
  FemPosDeviationSmoother() = default;

  bool Solve(const std::vector<std::pair<double, double>>& raw_point2d,
             const std::vector<double>& bounds, std::vector<double>* opt_x,
             std::vector<double>* opt_y);

  bool NlpWithIpopt(const std::vector<std::pair<double, double>>& raw_point2d,
                    const std::vector<double>& bounds,
                    std::vector<double>* opt_x, std::vector<double>* opt_y);



};
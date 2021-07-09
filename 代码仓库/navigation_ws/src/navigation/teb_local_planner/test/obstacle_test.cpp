#include <iostream>
#include <eigen3/Eigen/Dense>
#include "teb_local_planner/obstacles.h"
#include "teb_local_planner/timed_elastic_band.h"
#include "teb_local_planner/distance_calculations.h"

using namespace teb_local_planner;
using std::cout;
using std::endl;

int main()
{
    Point2dContainer v;
    v.push_back(Eigen::Vector2d(0,0));
    v.push_back(Eigen::Vector2d(1,0));
    v.push_back(Eigen::Vector2d(1,1));
    v.push_back(Eigen::Vector2d(0,1));
    PolygonObstacle obs_poly(v);
    Eigen::Vector2d check_point(1.31,0.5);
    cout << obs_poly.checkCollision(check_point,0.2) << endl;

    TimedElasticBand teb;
    Eigen::Vector2d res;
    Eigen::Vector2d line_start(0,0);
    Eigen::Vector2d line_end(1,1);
    Eigen::Vector2d ref_point(0.4,0.4);
    res = closest_point_on_line_segment_2d(ref_point,line_start,line_end);
    cout << "res = " << res << endl;
    
    return 0;
}
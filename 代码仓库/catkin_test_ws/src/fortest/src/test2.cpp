
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <eigen3/Eigen/Dense>
#include "matplotlibcpp.h"
 
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::vector;
using std::string;
using std::ios;
using std::cout;
using std::endl;

namespace plt = matplotlibcpp;

double fast_sigmoid(double x)
{
  return x / (1 + fabs(x));
}

double penaltyBoundToInterval(const double& var,const double& a,const double& epsilon)
{
  if (var < -a+epsilon)
  {
    return (-var - (a - epsilon));
  }
  if (var <= a-epsilon)
  {
    return 0.;
  }
  else
  {
    return (var - (a - epsilon));
  }
}

/**
 * @brief Linear penalty function for bounding \c var to the interval \f$ a < var < b \f$
 * @param var The scalar that should be bounded
 * @param a lower bound
 * @param b upper bound
 * @param epsilon safty margin (move bound to the interior of the interval)
 * @see penaltyBoundToIntervalDerivative
 * @return Penalty / cost value that is nonzero if the constraint is not satisfied
 */
inline double penaltyBoundToInterval(const double& var,const double& a, const double& b, const double& epsilon)
{
  if (var < a+epsilon)
  {
    return (-var + (a + epsilon));
  }
  if (var <= b-epsilon)
  {
    return 0.;
  }
  else
  {
    return (var - (b - epsilon));
  }
}


/**
 * @brief Linear penalty function for bounding \c var from below: \f$ a < var \f$
 * @param var The scalar that should be bounded
 * @param a lower bound
 * @param epsilon safty margin (move bound to the interior of the interval)
 * @see penaltyBoundFromBelowDerivative
 * @return Penalty / cost value that is nonzero if the constraint is not satisfied
 */
inline double penaltyBoundFromBelow(const double& var, const double& a,const double& epsilon)
{
  if (var >= a+epsilon)
  {
    return 0.;
  }
  else
  {
    return (-var + (a+epsilon));
  }
}

/**
 * @brief Derivative of the linear penalty function for bounding \c var to the interval \f$ -a < var < a \f$
 * @param var The scalar that should be bounded
 * @param a lower and upper absolute bound
 * @param epsilon safty margin (move bound to the interior of the interval)
 * @see penaltyBoundToInterval
 * @return Derivative of the penalty function w.r.t. \c var
 */
inline double penaltyBoundToIntervalDerivative(const double& var,const double& a, const double& epsilon)
{
  if (var < -a+epsilon)
  {
    return -1;
  }
  if (var <= a-epsilon)
  {
    return 0.;
  }
  else
  {
    return 1;		
  }
}

/**
 * @brief Derivative of the linear penalty function for bounding \c var to the interval \f$ a < var < b \f$
 * @param var The scalar that should be bounded
 * @param a lower bound
 * @param b upper bound
 * @param epsilon safty margin (move bound to the interior of the interval)
 * @see penaltyBoundToInterval
 * @return Derivative of the penalty function w.r.t. \c var
 */
inline double penaltyBoundToIntervalDerivative(const double& var,const double& a, const double& b, const double& epsilon)
{
  if (var < a+epsilon)
  {
    return -1;
  }
  if (var <= b-epsilon)
  {
    return 0.;
  }
  else
  {
    return 1;		
  }
}


/**
 * @brief Derivative of the linear penalty function for bounding \c var from below: \f$ a < var \f$
 * @param var The scalar that should be bounded
 * @param a lower bound
 * @param epsilon safty margin (move bound to the interior of the interval)
 * @see penaltyBoundFromBelow
 * @return Derivative of the penalty function w.r.t. \c var
 */
inline double penaltyBoundFromBelowDerivative(const double& var, const double& a,const double& epsilon)
{
  if (var >= a+epsilon)
  {
    return 0.;
  }
  else
  {
    return -1;
  }
}
 
int main(int argc, char* argv[])
{

   Eigen::Matrix2d down;
   down<<1,0,
         2,1;
   //P<<1,2
   //   2,5
   Eigen::Matrix2d P=down*down.transpose();
   std::cout<<"P"<<P<<std::endl;
   Eigen::Matrix2d ml=P.llt().matrixU();
   Eigen::Matrix2d testP=ml*ml.transpose();
   std::cout<<"mllt"<<ml<<std::endl;
   std::cout<<"testP"<<testP<<std::endl;


  double xxz,yyz,zz,
         tt,ss,rr;
  vector<int> sss(5,0);
  sss.clear();

	vector<double> xx,yy;
    double low = -10;
    double high = 10;
    double dt = 0.2;
    size_t size = static_cast<int>((high-low)/dt);
    for(size_t i = 0; i < size; i++)
    {
        xx.push_back(low + dt*i);
    }
    for(size_t i = 0; i < size; i++)
    {
        yy.push_back(penaltyBoundToIntervalDerivative(xx[i],2,1));
    }

    plt::plot(xx,yy);
    plt::show();



    Eigen::Vector2d a(5,6.001);
    cout << a.x() << endl;
    cout << a.y() << endl;
    
    Eigen::Vector2d b(3,4);

    double dot = a.dot(b);

    cout << "dot = " << dot << endl;
  
    double c = (a - b).squaredNorm();

    cout << "squaredNorm = " << c << endl;

    b.normalize();
    cout << "b = \n " << b << endl;

    cout << b.minCoeff() << endl;
    cout << b.maxCoeff() << endl;
    cout << b.mean() << endl;
    b.fill(7);
    cout << b << endl;
    Eigen::Matrix3d m  = Eigen::Matrix3d::Ones();
    cout << m << endl;
    Eigen::MatrixXd n  = Eigen::MatrixXd::Identity(5,5);
    cout << n << endl;
    cout << n.block(0,0,3,3) << endl;
    
    vector<int> x(10,0);
    x = {1,2,5,6,7,3,2,5,0,5,7};
    for(auto a:x)
        cout << a << " ";
    cout << endl;
    x.erase(remove(x.begin(),x.end(),5),x.end());
    for(auto a:x)
        cout << a << " ";
    cout << endl;


    


	return 0;
}
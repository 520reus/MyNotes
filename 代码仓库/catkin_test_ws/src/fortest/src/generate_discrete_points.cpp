#include<iostream>
#include "matplotlibcpp.h"
#include <fstream>
#include <sstream>  
#include <vector>
//include cppad for solve NLP 
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>

using std::cout; using std::endl; 

namespace plt = matplotlibcpp;

namespace {
using CppAD::AD;
class FG_eval {
    
    public:
    // 初始化模型参数
    FG_eval(std::vector<double> x,std::vector<double> y)
    {
        refx_ = std::move(x);
        refy_ = std::move(y);
        // Number of variables
        // Variables include 2D points and curvature constraints slack variable
        num_of_points_ = refx_.size();
        num_of_slack_var_ = num_of_points_ - 2;
        num_of_variables_ = num_of_points_*2 + num_of_slack_var_;
        // Number of constraints
        // Constraints includes positional constraints, curvature constraints and
        // slack variable constraints
        num_of_curvature_constr_ = num_of_points_ - 2;
        num_of_slack_constr_ = num_of_points_ - 2;
        num_of_constrains_ = num_of_points_*2 + num_of_curvature_constr_ + num_of_slack_constr_;
        // Indexes for variables and constraints,
        // Start index is actual first index and end index is one index after the
        // actual final index
        slack_var_start_index_ = num_of_points_ * 2;
        slack_var_end_index_ = slack_var_start_index_ + num_of_slack_var_;
        curvature_constr_start_index_ = num_of_points_ * 2;
        curvature_constr_end_index_ = curvature_constr_start_index_ + num_of_curvature_constr_;
        slack_constr_start_index_ = curvature_constr_end_index_;
        slack_constr_end_index_ = slack_constr_start_index_ + num_of_slack_constr_;

    }
    


    typedef CPPAD_TESTVECTOR(AD<double>) ADvector;
    void operator()(ADvector& fg, const ADvector& x)
    { 
        assert(fg.size() == num_of_constrains_+1); //number of constrains + obj
        assert(x.size() == num_of_variables_); //number of variables
        CPPAD_TESTVECTOR(AD<double>) variables(num_of_variables_);
        
        
        for(size_t i=0;i<num_of_variables_;i++)
            variables[i] = x[i];

        // f(x) objective function
        // Distance to refs 与参考点距离代价
        AD<double> dis_to_path_cost;
        for(size_t i=0;i<num_of_points_;i++)
        {
            size_t index = i*2;
            dis_to_path_cost += ((variables[index]-refx_[i])*(variables[index]-refx_[i]) + 
                            (variables[index+1]-refy_[i])*(variables[index+1]-refy_[i]));
        }
        // Fem_pos_deviation 平滑性代价
        AD<double> smooth_cost;
        for(size_t i=0;i+2<num_of_points_;i++)
        {
            size_t findex = i * 2;
            size_t mindex = findex + 2;
            size_t lindex = mindex + 2;
            smooth_cost += ((variables[findex]+variables[lindex]-2*variables[mindex])*
                            (variables[findex]+variables[lindex]-2*variables[mindex])+
                            (variables[findex+1]+variables[lindex+1]-2*variables[mindex+1])*
                            (variables[findex+1]+variables[lindex+1]-2*variables[mindex+1]));
        }
        // Total length 长度代价
        AD<double> length_cost;
        for (size_t i = 0; i + 1 < num_of_points_; ++i) {
        size_t findex = i * 2;
        size_t nindex = findex + 2;
        length_cost += ((variables[findex] - variables[nindex]) * 
                        (variables[findex] - variables[nindex]) +
                        (variables[findex + 1] - variables[nindex + 1]) * 
                        (variables[findex + 1] - variables[nindex + 1]));
        }
        // Slack variable minimization 对松弛变量的惩罚
        AD<double> slack_cost;
        for(size_t i = slack_var_start_index_;i<slack_var_end_index_;i++)
        {
            slack_cost += variables[i];
        }
        
        fg[0] = weight_ref_deviation_*dis_to_path_cost  + weight_path_length_*length_cost + 
                weight_fem_pos_deviation_*smooth_cost + weight_curvature_constraint_slack_var_*slack_cost;

        // constraints
        // a. positional deviation constraints 位置约束
        for (size_t i = 0; i < num_of_points_; ++i) {
            size_t index = i * 2;
            fg[index + 1] = variables[index];
            fg[index + 2] = variables[index + 1];
        }

        // b. curvature constraints 曲率约束
        for (size_t i = 0; i + 2 < num_of_points_; ++i) {
            size_t findex = i * 2;
            size_t mindex = findex + 2;
            size_t lindex = mindex + 2;
            fg[curvature_constr_start_index_ + i + 1] =
                (((variables[findex] + variables[lindex]) - 2.0 * variables[mindex]) *
                    ((variables[findex] + variables[lindex]) - 2.0 * variables[mindex]) +
                ((variables[findex + 1] + variables[lindex + 1]) - 2.0 * variables[mindex + 1]) *
                    ((variables[findex + 1] + variables[lindex + 1]) - 2.0 * variables[mindex + 1])) -
                variables[slack_var_start_index_ + i];
        }

        // c. slack var constraints 松弛变量的约束
        size_t slack_var_index = 0;
        for (size_t i = slack_constr_start_index_; i < slack_constr_end_index_; ++i) {
            fg[i + 1] = variables[slack_var_start_index_ + slack_var_index];
            ++slack_var_index;
        }

        
        return;
    }

    

    private:
        std::vector<double> refx_; //离散参考点
        std::vector<double> refy_;
        size_t num_of_points_ = 0; //离散点个数
        size_t num_of_variables_ = 0; //优化变量个数
        size_t num_of_constrains_ = 0; //约束方程个数
        size_t num_of_slack_var_ = 0;  //松弛变量个数（用于目标函数）
        size_t num_of_curvature_constr_ = 0; //曲率约束方程数
        size_t num_of_slack_constr_ = 0; //松弛变量方程数（用于约束方程）
        //index
        size_t slack_var_start_index_ = 0;
        size_t slack_var_end_index_ = 0;
        size_t curvature_constr_start_index_ = 0;
        size_t curvature_constr_end_index_ = 0;
        size_t slack_constr_start_index_ = 0;
        size_t slack_constr_end_index_ = 0;
        // Weights in optimization cost function
        double weight_fem_pos_deviation_ = 1.0e8; //平滑性
        double weight_path_length_ = 1.0; //路径长度
        double weight_ref_deviation_ = 1.0; //与参考点的偏差
        double weight_curvature_constraint_slack_var_ = 1.0e2; //松弛因子

        double curvature_constraint_ = 0.2;
        double limit_l = 0.01;
};
}

namespace {
using CppAD::AD;
class DiscetePointsSmoother {
    
    public:
    // 初始化模型参数
    DiscetePointsSmoother(std::vector<double> x,std::vector<double> y)
    {
        refx_ = std::move(x);
        refy_ = std::move(y);
        // Number of variables
        // Variables include 2D points and curvature constraints slack variable
        num_of_points_ = refx_.size();
        num_of_slack_var_ = num_of_points_ - 2;
        num_of_variables_ = num_of_points_*2 + num_of_slack_var_;
        // Number of constraints
        // Constraints includes positional constraints, curvature constraints and
        // slack variable constraints
        num_of_curvature_constr_ = num_of_points_ - 2;
        num_of_slack_constr_ = num_of_points_ - 2;
        num_of_constrains_ = num_of_points_*2 + num_of_curvature_constr_ + num_of_slack_constr_;
        // Indexes for variables and constraints,
        // Start index is actual first index and end index is one index after the
        // actual final index
        slack_var_start_index_ = num_of_points_ * 2;
        slack_var_end_index_ = slack_var_start_index_ + num_of_slack_var_;
        curvature_constr_start_index_ = num_of_points_ * 2;
        curvature_constr_end_index_ = curvature_constr_start_index_ + num_of_curvature_constr_;
        slack_constr_start_index_ = curvature_constr_end_index_;
        slack_constr_end_index_ = slack_constr_start_index_ + num_of_slack_constr_;

    }
    


    typedef CPPAD_TESTVECTOR(AD<double>) ADvector;
    void operator()(ADvector& fg, const ADvector& x)
    { 
        assert(fg.size() == num_of_constrains_+1); //number of constrains + obj
        assert(x.size() == num_of_variables_); //number of variables
        CPPAD_TESTVECTOR(AD<double>) variables(num_of_variables_);
        
        
        for(size_t i=0;i<num_of_variables_;i++)
            variables[i] = x[i];

        // f(x) objective function
        // Distance to refs 与参考点距离代价
        AD<double> dis_to_path_cost;
        for(size_t i=0;i<num_of_points_;i++)
        {
            size_t index = i*2;
            dis_to_path_cost += ((variables[index]-refx_[i])*(variables[index]-refx_[i]) + 
                            (variables[index+1]-refy_[i])*(variables[index+1]-refy_[i]));
        }
        // Fem_pos_deviation 平滑性代价
        AD<double> smooth_cost;
        for(size_t i=0;i+2<num_of_points_;i++)
        {
            size_t findex = i * 2;
            size_t mindex = findex + 2;
            size_t lindex = mindex + 2;
            smooth_cost += ((variables[findex]+variables[lindex]-2*variables[mindex])*
                            (variables[findex]+variables[lindex]-2*variables[mindex])+
                            (variables[findex+1]+variables[lindex+1]-2*variables[mindex+1])*
                            (variables[findex+1]+variables[lindex+1]-2*variables[mindex+1]));
        }
        // Total length 长度代价
        AD<double> length_cost;
        for (size_t i = 0; i + 1 < num_of_points_; ++i) {
        size_t findex = i * 2;
        size_t nindex = findex + 2;
        length_cost += ((variables[findex] - variables[nindex]) * 
                        (variables[findex] - variables[nindex]) +
                        (variables[findex + 1] - variables[nindex + 1]) * 
                        (variables[findex + 1] - variables[nindex + 1]));
        }
        // Slack variable minimization 对松弛变量的惩罚
        AD<double> slack_cost;
        for(size_t i = slack_var_start_index_;i<slack_var_end_index_;i++)
        {
            slack_cost += variables[i];
        }
        
        fg[0] = weight_ref_deviation_*dis_to_path_cost  + weight_path_length_*length_cost + 
                weight_fem_pos_deviation_*smooth_cost + weight_curvature_constraint_slack_var_*slack_cost;

        // constraints
        // a. positional deviation constraints 位置约束
        for (size_t i = 0; i < num_of_points_; ++i) {
            size_t index = i * 2;
            fg[index + 1] = variables[index];
            fg[index + 2] = variables[index + 1];
        }

        // b. curvature constraints 曲率约束
        for (size_t i = 0; i + 2 < num_of_points_; ++i) {
            size_t findex = i * 2;
            size_t mindex = findex + 2;
            size_t lindex = mindex + 2;
            fg[curvature_constr_start_index_ + i + 1] =
                (((variables[findex] + variables[lindex]) - 2.0 * variables[mindex]) *
                    ((variables[findex] + variables[lindex]) - 2.0 * variables[mindex]) +
                ((variables[findex + 1] + variables[lindex + 1]) - 2.0 * variables[mindex + 1]) *
                    ((variables[findex + 1] + variables[lindex + 1]) - 2.0 * variables[mindex + 1])) -
                variables[slack_var_start_index_ + i];
        }

        // c. slack var constraints 松弛变量的约束
        size_t slack_var_index = 0;
        for (size_t i = slack_constr_start_index_; i < slack_constr_end_index_; ++i) {
            fg[i + 1] = variables[slack_var_start_index_ + slack_var_index];
            ++slack_var_index;
        }

        
        return;
    }

    void discretePointsSmoother(std::vector<double>& smooth_x,std::vector<double>& smooth_y)
    {
    // object that computes objective and constraints

    size_t i;
    typedef CPPAD_TESTVECTOR(double) Dvector;

    size_t nx = num_of_variables_; // number of varibles
    size_t ng = num_of_constrains_; // number of constraints
    size_t num_of_points = num_of_points_;// number of points
    Dvector x0(nx); // initial condition of varibles
    for(size_t i=0;i<num_of_points_;i++)
    {
        size_t index = i*2;
        x0[index] = refx_[i];
        x0[index+1] = refy_[i];
    }
    for(size_t i=slack_var_start_index_;i<slack_var_end_index_;i++)
    {
        x0[i] = 0;
    }

    // lower and upper bounds for varibles
    Dvector xl(nx), xu(nx);
    for(size_t i = 0; i < num_of_points_; i++)
    {
        size_t index = i*2;
        //x
        xl[index] = -1e20;
        xu[index] = 1e20;
        //y
        xl[index+1] = -1e20;
        xu[index+1] = 1e20;
    }

    for(size_t i=slack_var_start_index_;i<slack_var_end_index_;i++)
    {
        xl[i] = -1e20;
        xu[i] = 1e20;
    }
    // constraints 约束
    // a. positional deviation constraints 点的上下左右边界约束
    Dvector gl(ng), gu(ng);
    for(size_t i=0;i<num_of_points_;i++)
    {
        size_t index = i*2;
        //x
        gl[index] = refx_[i] - limit_l;
        gu[index] = refx_[i] + limit_l;
        //y
        gl[index + 1] = refy_[i] - limit_l;
        gu[index + 1] = refy_[i] + limit_l;
    }
    // b. curvature constraints 曲率约束上下界
    double ref_total_length = 0.0;
    double pre_point_x = refx_.front();
    double pre_point_y = refy_.front();
    for (size_t i = 1; i < num_of_points_; ++i) {
        auto cur_point_x = refx_[i];
        auto cur_point_y = refy_[i];
        double x_diff = cur_point_x - pre_point_x;
        double y_diff = cur_point_y - pre_point_y;
        ref_total_length += std::sqrt(x_diff * x_diff + y_diff * y_diff);
        pre_point_x = cur_point_x;
        pre_point_y = cur_point_y;
    }
    double average_delta_s =
        ref_total_length / static_cast<double>(num_of_points_ - 1);
    double curvature_constr_upper =
        average_delta_s * average_delta_s * curvature_constraint_;

    for (size_t i = curvature_constr_start_index_;
        i < curvature_constr_end_index_; ++i) {
        gl[i] = -1e20;
        gu[i] = curvature_constr_upper * curvature_constr_upper;
    }

    // c. slack var constraints 松弛变量约束
    for (size_t i = slack_constr_start_index_; i < slack_constr_end_index_; ++i) {
        gl[i] = 0.0;
        gu[i] = 1e20;
    }

    FG_eval fg_eval(refx_,refy_);
        
    // options
    std::string options;
    // turn off any printing
    options += "Integer print_level  0\n";
    options += "String sb            yes\n";
    // maximum iterations
    options += "Integer max_iter     10\n";
    //approximate accuracy in first order necessary conditions;
    // see Mathematical Programming, Volume 106, Number 1,
    // Pages 25-57, Equation (6)
    options += "Numeric tol          1e-6\n";
    //derivative tesing
    options += "String derivative_test   second-order\n";
    // maximum amount of random pertubation; e.g.,
    // when evaluation finite diff
    options += "Numeric point_perturbation_radius   0.\n";


    CppAD::ipopt::solve_result<Dvector> solution; // solution
    CppAD::ipopt::solve<Dvector, FG_eval>(options, x0, xl, xu, gl, gu, fg_eval, solution); // solve the problem

    cout<<"solution: "<<solution.x<<endl;
    cout<<"min f(x)="<<solution.obj_value<<endl;

    };

    private:
        std::vector<double> refx_; //离散参考点
        std::vector<double> refy_;
        size_t num_of_points_ = 0; //离散点个数
        size_t num_of_variables_ = 0; //优化变量个数
        size_t num_of_constrains_ = 0; //约束方程个数
        size_t num_of_slack_var_ = 0;  //松弛变量个数（用于目标函数）
        size_t num_of_curvature_constr_ = 0; //曲率约束方程数
        size_t num_of_slack_constr_ = 0; //松弛变量方程数（用于约束方程）
        //index
        size_t slack_var_start_index_ = 0;
        size_t slack_var_end_index_ = 0;
        size_t curvature_constr_start_index_ = 0;
        size_t curvature_constr_end_index_ = 0;
        size_t slack_constr_start_index_ = 0;
        size_t slack_constr_end_index_ = 0;
        // Weights in optimization cost function
        double weight_fem_pos_deviation_ = 1.0e8; //平滑性
        double weight_path_length_ = 1.0; //路径长度
        double weight_ref_deviation_ = 1.0; //与参考点的偏差
        double weight_curvature_constraint_slack_var_ = 1.0e2; //松弛因子

        double curvature_constraint_ = 0.2;
        double limit_l = 0.01;
};
}

void generateDiscretePoints(std::vector<double>& x,std::vector<double>& y)
{
    //读文件
    std::ifstream inFile("src/fortest/data/traj3.csv",std::ios::in);
    std::string lineStr;

    //这些csv直接从第一行开始
    while(std::getline(inFile,lineStr)) //每次从csv中读取一行，该行内容存入lineStr，直到文件末尾
    {
        std::stringstream ss(lineStr); //创建流的输入输出
        //cout << ss.str() << endl;
        std::string str;
        std::getline(ss,str,','); //遇到','停止读入
        double point_x = atof(str.c_str());
        x.push_back(point_x);
        std::getline(ss,str,'\n'); //遇到行尾标志停止读入
        double point_y = atof(str.c_str());
        y.push_back(point_y); 
    }

}

void PointsSampling(const std::vector<double>& x,const std::vector<double>& y,
                          std::vector<double>& sam_x,std::vector<double>& sam_y)
{
    size_t n = x.size();
    size_t sampling_interval = 25;
    for(size_t i = 0;i < n;i += sampling_interval)
    {
        sam_x.push_back(x[i]);
        sam_y.push_back(y[i]);
    }
}


void plotPath(std::vector<double>& x,std::vector<double>& y)
{
    plt::plot(x,y);
    plt::show(); 
}

void plotPaths(const std::vector<double>& x,const std::vector<double>& y,
                                    std::vector<double>& smooth_x,std::vector<double>& smooth_y)
{
    plt::plot(x,y);
    plt::plot(smooth_x,smooth_y);
    plt::show();
}

inline double penaltyBoundToInterval(const double& var,const double& a,const double& epsilon)
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


int main() {
    
    /* std::vector<double> x,y,sam_x,sam_y,smooth_x,smooth_y;
    generateDiscretePoints(x,y);
    PointsSampling(x,y,sam_x,sam_y);
    //plotPath(sam_x,sam_y);
    DiscetePointsSmoother smoother(sam_x,sam_y);
    smoother.discretePointsSmoother(smooth_x,smooth_y);
    plotPaths(x,y,smooth_x,smooth_y); */
    int a = 5;
    int b = 10;
    cout << a/b << endl;
    cout << static_cast<double>(a)/b << endl;

    std::vector<double> t(200);
    std::vector<double> yt(200);
    for(int i=0;i<101;i++)
    {
        t[i] = static_cast<double>(i-50)/5;
        yt[i] = penaltyBoundToInterval(t[i],5,0);
    }
    plt::plot(t,yt);
    plt::show();
}



#include<iostream>
#include "matplotlibcpp.h"
#include <fstream>
#include <sstream>  
#include <vector>
#include <ctime>
#include <cmath>
#include <eigen3/Eigen/Dense>

#include "discrete_points_smoother.h"

using std::cout; using std::endl; 

namespace plt = matplotlibcpp;

const double point_bound = 0.05;


void generateDiscretePoints(std::vector<double>& x,std::vector<double>& y)
{
    //读文件
    std::ifstream inFile("src/fortest/data/traj1.csv",std::ios::in);
    std::string lineStr;

    //这些csv直接从第一行开始
    while(std::getline(inFile,lineStr)) //每次从csv中读取一行，该行内容存入lineStr，直到文件末尾
    {
        std::stringstream ss(lineStr); //创建流的输入输出
        //cout << ss.str() << endl;
        std::string str;
        std::getline(ss,str,','); //遇到','停止读入·
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

//计算离散点序列偏航角 deg
void calc_yaw(const std::vector<double>& x,const std::vector<double>& y,std::vector<double>* vec_yaw)
{
    size_t n = x.size();
    double yaw = 0;
    for(int i = 0;i < n - 1;i++){
        yaw = atan2(y[i+1] - y[i],x[i+1] - x[i])*180/M_PI;
        vec_yaw->push_back(yaw);
    }
    vec_yaw->push_back(yaw);
}

//知乎大佬的求曲率方法 https://zhuanlan.zhihu.com/p/72083902
double PJCurvature(const Eigen::Vector3f& x,const Eigen::Vector3f& y)
{
    double t_a = sqrt(pow(x[1]-x[0],2) + pow(y[1] - y[0],2));
    double t_b = sqrt(pow(x[2]-x[1],2) + pow(y[2] - y[1],2));

    Eigen::Matrix3f M;
    M << 1,-t_a,t_a*t_a,
         1,0,0,
         1,t_b,t_b*t_b;

    Eigen::Vector3f a;
    Eigen::Vector3f b;

    a = M.inverse() * x;
    b = M.inverse() * y;
    //曲率的方向问题：所选曲线法向量与曲线弯曲方向相同，曲率为正，曲线法向量与曲线弯曲方向相反，曲率为负。
    double kappa = -2*(a[2]*b[1] - a[1]*b[2])/pow(a[1]*a[1] + b[1]*b[1],3/2); 
    return kappa;
    
}

//计算离散点序列曲率
void calc_kappa(const std::vector<double>& x,const std::vector<double>& y,std::vector<double>* vec_kappa)
{
    size_t n = x.size();
    for(int i = 1;i < n - 1;i++){
        Eigen::Vector3f vx,vy;
        vx << x[i-1],x[i],x[i+1];
        vy << y[i-1],y[i],y[i+1];
        double kappa = PJCurvature(vx,vy);
        vec_kappa->at(i) = kappa;
    }
    vec_kappa->at(0) = vec_kappa->at(1);
    vec_kappa->at(n-1) = vec_kappa->at(n-2);
}

void plotPath(std::vector<double>& x,std::vector<double>& y)
{
    plt::plot(x,y);
    plt::show(); 
}


void plotResult(const std::vector<double>& x,const std::vector<double>& y,
               const std::vector<double>& smooth_x,const std::vector<double>& smooth_y)
{
    //*1.plot path
    plt::figure_size(1200,780); //必须写这句，否则title、lengend方法无法获得图像句柄
    plt::named_plot("ref path",x,y,"r-");
    plt::named_plot("smoothed path",smooth_x,smooth_y,"b--");
    plt::title("discrete points smoothing path");
    plt::legend();
    plt::xlabel("x[m]");
    plt::ylabel("y[m]");
    
    std::vector<double> origin_yaw,smooth_yaw,origin_kappa(x.size()),smooth_kappa(x.size());
    calc_yaw(x,y,&origin_yaw);
    calc_kappa(x,y,&origin_kappa);
    calc_yaw(smooth_x,smooth_y,&smooth_yaw);
    calc_kappa(smooth_x,smooth_y,&smooth_kappa);

    //*2.plot yaw
    plt::figure_size(1200,780);
    plt::named_plot("The yaw angle of ref path",x,origin_yaw,"-");
    plt::named_plot("The yaw angle of smoothed path",smooth_x,smooth_yaw,"--");
    plt::title("yaw angle of the path");
    plt::legend();
    plt::ylabel("yaw[deg]");

    //*3.plot kappa
    plt::figure_size(1200,780);
    plt::named_plot("The curvature of ref path",x,origin_kappa,"-");
    plt::named_plot("The curvature of smoothed path",smooth_x,smooth_kappa,"--");
    plt::title("The curvature of the path");
    plt::legend();
    plt::ylabel("kappa[m-1]");

    plt::show();
}



int main()
{
    /* //测试代码
    Eigen::Vector3f vx;vx << 0,5*sin(0.02),5*sin(0.04);
    Eigen::Vector3f vy;vy << 0,5-5*cos(0.02),5-5*cos(0.04);
    double kappa = PJCurvature(vx,vy); */

    clock_t start_time,end_time; // to calculate the time of algorithm
    std::vector<double> x,y,sam_x,sam_y,smooth_x,smooth_y;
    generateDiscretePoints(x,y);
    //PointsSampling(x,y,sam_x,sam_y); //采样离散点序列

    FemPosDeviationSmoother smoother;
    std::vector<std::pair<double,double>> ref_points; 
    for(size_t i = 0;i < x.size();i++)
    {
        ref_points.push_back(std::make_pair(x[i],y[i]));
    }
    
    std::vector<double> points_bound(x.size());
    for(size_t i = 0;i < x.size();i++)
    {
        points_bound.at(i) = point_bound;
    }
    start_time = clock();
    smoother.Solve(ref_points,points_bound,&smooth_x,&smooth_y);
    end_time = clock();
    cout<<"The run time of discrete points smoothing algorithm(using ipopt for NLP) is:"<<(double)(end_time-start_time)*1000/CLOCKS_PER_SEC<<"ms"<<endl;
    plotResult(x,y,smooth_x,smooth_y);
   
    //plotPaths(x,y,smooth_x,smooth_y);

}
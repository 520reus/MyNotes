#include<iostream>
#include<ros/ros.h>
#include <queue>
#include <deque>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>
#include <geometry_msgs/Twist.h>

using std::endl;
using std::cout;

std::recursive_mutex r_mutex_;
std::mutex mutex_;

void hello(std::string name) {
    r_mutex_.lock();
    cout << "Welcome to " << name << endl;
    r_mutex_.lock();
    cout << "sdf" << endl;
    r_mutex_.unlock();
    r_mutex_.unlock();
    
}

bool isAdjustTrajectory(const std::vector<double>& local_trajectory, 
                        const int& judge_num)
{
    if(local_trajectory.size() < 2)
        return false;
    size_t i = 0;
    double front_w, back_w;
    while(i < local_trajectory.size() - 1)
    {
        front_w = local_trajectory[i]; 
        back_w = local_trajectory[i+1];
        if(front_w*back_w < 0)
            break;
        i++;
    }
    return i+1 < judge_num ? true : false;
}

size_t findSignChangedIndex(const std::vector<double>& local_trajectory)
{
    double front_w, back_w;
    size_t i = 0;
    //找到符号改变的界限点
    while(i < local_trajectory.size() - 1)
    {
        front_w = local_trajectory[i]; 
        back_w = local_trajectory[i+1];
        if(front_w*back_w < 0)
            break;
        i++;
    }
    return i;
}

void foo(const int& x)
{
    cout << "x=" << x << endl;
}


int main(int argc,char* argv[])
{
   
    ros::init(argc,argv,"gnss_dy_rs422");
    ros::NodeHandle nh;
    ros::Timer Timer;
    ros::Rate sleep_time(100); //跟踪2s更新一次局部目标点
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("mypub",1);

    std::shared_ptr<int> p = std::make_shared<int>(8);
    cout << *p << endl;
    std::shared_ptr<int> x = p;
    cout << *x << endl;
    int *l = new int(11);
    p.reset(l);
    cout << *l << endl;

    void (*pf)(const int &x);
    pf = foo;
    int m = 77;
    pf(m);

    const int a = 10;
    const int& b = a;

    return 0;
}

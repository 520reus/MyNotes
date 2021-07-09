#include<iostream>
#include<ros/ros.h>
#include <serial/serial.h>
#include <queue>
#include <deque>
#include <vector>

using std::endl;
using std::cout;

void updateLocalGoal(const ros::TimerEvent& event)
{
    ROS_ERROR("WWWWWWWWW");
}

int main(int argc,char* argv[])
{
   
    ros::init(argc,argv,"gnss_dy_rs422");
    ros::NodeHandle nh;
    ros::Timer Timer;
    ros::Duration sleep_time(2); //跟踪2s更新一次局部目标点
    Timer = nh.createTimer(sleep_time,boost::bind(&updateLocalGoal,_1));
        
   
    ros::spin();
   
    
    

    
	
    return 0;
}

#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include <tutorial_dynamic_reconfigure/tutorialsConfig.h>

void callback(const tutorial_dynamic_reconfigure::tutorialsConfig &config,uint32_t level)
{
    ROS_INFO("Reconfigure Request:%d %d %s %d",
             config.planner_frequency,
             config.controller_frequency,
             config.planner_name.c_str(),
             config.isDone);
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"dynamic_reconfigure_server");
    ros::NodeHandle nh;
    dynamic_reconfigure::Server<tutorial_dynamic_reconfigure::tutorialsConfig> server;
    dynamic_reconfigure::Server<tutorial_dynamic_reconfigure::tutorialsConfig>::CallbackType f;
    f = boost::bind(&callback,_1,_2);
    
    server.setCallback(f);
    ROS_INFO("Spinng node");
    ros::spin();
    return 0;
}
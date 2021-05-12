// action服务器
#include <ros/ros.h>
#include <actionlib/server/simple_action_server.h>
#include "tutorial_action/DoDishesAction.h"

typedef actionlib::SimpleActionServer<tutorial_action::DoDishesAction> Server;

// 收到action的goal后调用的回调函数
void executeCB(const tutorial_action::DoDishesGoalConstPtr& goal,Server* as)
{
    ros::Rate rate(1);
    tutorial_action::DoDishesFeedback feedback;
    ROS_INFO("Dishwater %d is working.",goal->dish_id);
    // 洗盘子的进度，按照1hz的频率发布进度feedback
    for(int i=0;i<=10;i++){
        feedback.pecent_complete = i*10;
        as->publishFeedback(feedback);
        rate.sleep();
    }
    ROS_INFO("DishWater %d finish working.",goal->dish_id);
    as->setSucceeded();
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"action_server");
    ros::NodeHandle nh;
    // 定义一个服务器
    Server server(nh,"DoDishes",boost::bind(&executeCB,_1,&server),false);
    // 服务器开始运行
    server.start();
    ros::spin();

    return 0;
}
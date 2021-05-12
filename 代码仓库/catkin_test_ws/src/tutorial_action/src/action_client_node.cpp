// action客户端
#include <actionlib/client/simple_action_client.h>
#include "tutorial_action/DoDishesAction.h"
#include <ros/ros.h>

typedef actionlib::SimpleActionClient<tutorial_action::DoDishesAction> Client;
// 当action完成后会调用次回调函数一次
void doneCB(const actionlib::SimpleClientGoalState& state,
            const tutorial_action::DoDishesResultConstPtr& result)
{
    ROS_INFO("Yay!The dishes are now clean");
    ros::shutdown();
}
// 当action激活后会调用次回调函数一次
void activeCB()
{
    ROS_INFO("Goal just went active");
}
// 收到feedback后调用的回调函数
void feedbackCB(const tutorial_action::DoDishesFeedbackConstPtr& feedback)
{
    ROS_INFO("pecent_complete:%d",feedback->pecent_complete);
}

int main(int argc,char** argv)
{

    ros::init(argc,argv,"action_client");
    ros::NodeHandle nh;

    Client client("DoDishes",true);
    ROS_INFO("Waiting for action server to start.");
    client.waitForServer();
    ROS_INFO("Action server started,sending goal.");

    tutorial_action::DoDishesGoal goal;
    goal.dish_id = 1;
    // 发送action的goal给服务器端，并且设置回调函数
    client.sendGoal(goal,&doneCB,&activeCB,&feedbackCB);
    ros::spin();

    return 0;
}
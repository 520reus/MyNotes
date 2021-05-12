#include<ros/ros.h>
#include<geometry_msgs/Twist.h>

void velCB(const geometry_msgs::TwistConstPtr& vel)
{
    ROS_INFO("get cmd_vel!,vx=%0.2f,vtheta=%0.2f",vel->linear.x,vel->angular.z);
}

int main(int argc,char** argv)
{
    ros::init(argc,argv,"move_base");
    ros::NodeHandle nh("~");
    ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel",1);
    ros::Subscriber vel_sub = nh.subscribe<geometry_msgs::Twist>("cmd_vel",1,&velCB);

    geometry_msgs::Twist vel_msg;
    vel_msg.linear.x = 2.0;
    vel_msg.angular.z = 1.25;

    ros::Rate rate(10);
    while(ros::ok())
    {
        vel_pub.publish(vel_msg);
        rate.sleep();
        ros::spinOnce();
    }
    return 0;
}
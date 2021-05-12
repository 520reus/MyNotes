#include <ros/ros.h>
#include "nav_msgs/Odometry.h"

#include "tf/transform_listener.h"


int main(int argc,char** argv)
{
    ros::init(argc,argv,"test");
    ros::NodeHandle nh;
    tf::StampedTransform trans;
    tf::TransformListener listener;
    ros::Publisher odom_pub = nh.advertise<nav_msgs::Odometry>("/odom",100);
    ros::Rate loop_rate(1);
    while(ros::ok())
    {
        try{
        listener.waitForTransform("base_footprint","imu_link",ros::Time(0),ros::Duration(3));
        listener.lookupTransform("base_footprint","imu_link",ros::Time(0),trans);
    }
    catch(tf::TransformException& ex)
    {
        ROS_ERROR("%s",ex.what());
    }
    tf::Quaternion quet;
    quet.setX(trans.getRotation().getX());
    quet.setY(trans.getRotation().getY());
    quet.setZ(trans.getRotation().getZ());
    quet.setW(trans.getRotation().getW());

    
    geometry_msgs::Vector3 rpy;
    tf::Matrix3x3(quet).getRPY(rpy.x,rpy.y,rpy.z);

    geometry_msgs::Vector3 dp;
    dp.x = trans.getOrigin().getX();
    dp.y = trans.getOrigin().getY();
    dp.z = trans.getOrigin().getZ();

    nav_msgs::Odometry odom;
    odom.pose.pose.position.x = 5+dp.x;
    odom.pose.pose.position.y = 5+dp.y;
    odom.pose.pose.position.z = 5+dp.z;

    odom.pose.pose.orientation.x = 0+quet.getX();
    odom.pose.pose.orientation.y = 0+quet.getY();
    odom.pose.pose.orientation.z = 0+quet.getZ();
    odom.pose.pose.orientation.w = 0+quet.getW();

    tf::Quaternion q;
    tf::quaternionMsgToTF(odom.pose.pose.orientation,q);
    geometry_msgs::Vector3 rpy2;
    tf::Matrix3x3(q).getRPY(rpy2.x,rpy2.y,rpy2.z);
    ROS_INFO("roll is:%f",rpy2.x);
    ROS_INFO("pitch is:%f",rpy2.y);
    ROS_INFO("yaw is:%f",rpy2.z);

    odom_pub.publish(odom);

    loop_rate.sleep();
    }
    
    
    return 0;
}
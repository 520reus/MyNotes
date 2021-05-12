#include <ros/ros.h>
#include "tf/transform_broadcaster.h"

int main(int argc,char** argv)
{
    ros::init(argc,argv,"tf_map2odom");
    ros::NodeHandle nh;
    ros::Rate rate(100);
    while(ros::ok())
    {
        tf::TransformBroadcaster map_broadcaster; //1.定义一个tf广播
        tf::Transform transform;//2.声明一个变量用来存储转换信息
        transform.setOrigin(tf::Vector3(0, 0, 0));//3. 定义平移
        tf::Quaternion q;// 4.定义旋转
        q.setRPY(0, 0, 0);
        transform.setRotation(q);

        
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(0,0,0);
        map_broadcaster.sendTransform(tf::StampedTransform(transform,
                                      ros::Time::now(),
                                      "map",
                                      "odom"));

        rate.sleep();
        ros::spinOnce();
    }
    return 0;
}
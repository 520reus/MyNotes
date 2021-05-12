#include <math.h>
#include <stdio.h>
#include <ros/ros.h>

#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include <sensor_msgs/NavSatFix.h>
#include "tf/transform_broadcaster.h"

#include <crss_vdcar/GpsInfo.h>

double x =0;
double y =0;                                                             
double z =0;

crss_vdcar::GpsInfo gps_status;

void gps_status_chatterCallback(const crss_vdcar::GpsInfo msg)
{
    gps_status = msg;                         
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "odom_gps_estimate");	
	ros::NodeHandle n;	
	ros::Subscriber gps_status_sub_ = n.subscribe("/gps/status", 100, gps_status_chatterCallback);
    ros::Publisher  odom_pub_       = n.advertise<nav_msgs::Odometry>("/odom/by_gps_estimate", 100);
	ros::Rate loop_rate(100);
	
	nav_msgs::Odometry axisodom;
	ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time    = ros::Time::now();

	while(n.ok())
	{
		ros::spinOnce();                          //check here
		current_time = ros::Time::now();
		double dt = (current_time - last_time).toSec(); 
		//printf("dt %f\n ",dt);
		
		double delta_x =  gps_status.Vx * dt;
		double delta_y =  gps_status.Vy * dt;
		double delta_z =  gps_status.Vz * dt;
		//printf("delta_x %f\n ",delta_x);
		x += delta_x;
		y += delta_y;
		z += delta_z;
		
		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw( gps_status.Roll, gps_status.Pitch, gps_status.Heading );
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		//反馈给各个功能包的

		axisodom.header.stamp    = current_time;
		axisodom.header.frame_id = "gps_estimate_odom";
		axisodom.child_frame_id  = "gps_link";
		
		//set the position
		axisodom.pose.pose.position.x  = x;
		axisodom.pose.pose.position.y  = y;
		axisodom.pose.pose.position.z  = z;
		axisodom.pose.pose.orientation = odom_quat;
		
		//set the velocity m/s	
		axisodom.twist.twist.linear.x  = gps_status.Vx;   
		axisodom.twist.twist.linear.y  = gps_status.Vy;   
		axisodom.twist.twist.linear.z  = gps_status.Vz;
		//gps 没有角速度信息  deg/s
		axisodom.twist.twist.angular.x = 0;               
		axisodom.twist.twist.angular.y = 0;
		axisodom.twist.twist.angular.z = 0;   
		
		odom_pub_.publish(axisodom);
		last_time = current_time;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ros::spinOnce();
		loop_rate.sleep();
	}
	ros::spin();	
	return 0;
}

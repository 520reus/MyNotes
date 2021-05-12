#include <math.h>
#include <stdio.h>
#include "ros/ros.h"

#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "tf/transform_broadcaster.h"

double x =0;
double y =0;                                                             
double z =0;
//全局角度  默认0度  
//车以Y轴为初始
//double consttheta = M_PI/2;
//车以X轴为初始
double consttheta =0;

double getV   =0;
double getW   =0;

double getVx  =0;
double getVy  =0;

double  D   = 241.3/1000 ;
double  lLeftRtight = 602.8/1000;
double  lFrontRear  = 981.215/1000;
double  lturnFR     = 427.0/1000;

void chatterCallback(const geometry_msgs::Twist msg)
{
    //左转为正  逆时针  V rear -> m/s   W -> deg/s  
	getV  = msg.linear.x;     			 
	getW  = msg.angular.z;   		 	 
}

double forRotation(double angle)
{
	if(angle>(M_PI*2))
	{
		angle = angle -(M_PI*2);
	};
	if(angle<0)
	{
		angle = angle +(M_PI*2);
	}; 
	return angle;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "odom_wheel");	
	ros::NodeHandle n;	
	ros::Subscriber wheel_sub_ = n.subscribe("/wheel/data", 100, chatterCallback);
	ros::Publisher  odom_pub_  = n.advertise<nav_msgs::Odometry>("/odom/by_wheel", 100);	
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
		
		double delta_th = getW  * dt;             //rad
		consttheta     += delta_th;
		consttheta      = forRotation(consttheta);
		
		getVx = getV * cos(delta_th);             //m/s
		getVy = getV * sin(delta_th);
				
		double delta_x = (getVx * cos(consttheta) - getVy * sin(consttheta)) * dt;
		double delta_y = (getVx * sin(consttheta) + getVy * cos(consttheta)) * dt;		
  		//printf("delta_x %f\n ",delta_x);
		x += delta_x;
		y += delta_y;

		//2D pose (轮式里程计)    z轴 roll pitch 忽略
		//偏航角 -> 欧拉角转换四元数                  //rad
		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(0, 0, consttheta);		

		//axisodom.pose.covariance = ODOM_POSE_COVARIANCE;
		axisodom.pose.covariance[0]   = 0.01; 
		axisodom.pose.covariance[7]   = 0.01; 
		axisodom.pose.covariance[35]  = 0.01; 
		axisodom.pose.covariance[14]  = 1e-10; 
		axisodom.pose.covariance[21]  = 1e-10; 
		axisodom.pose.covariance[28]  = 1e-10; 

		//axisodom.twist.covariance = ODOM_TWIST_COVARIANCE;
		axisodom.twist.covariance[0]  = 0.01; 
		axisodom.twist.covariance[7]  = 0.01; 
		axisodom.twist.covariance[35] = 0.01; 
	
		axisodom.header.stamp = current_time;
		axisodom.header.frame_id = "wheel_odom";
		axisodom.child_frame_id  = "base_footprint";
		
		//set the position
		axisodom.pose.pose.position.x  = x;
		axisodom.pose.pose.position.y  = y;
		axisodom.pose.pose.position.z  = z;
		axisodom.pose.pose.orientation = odom_quat;
		
		//set the velocity m/s deg/s 		
		axisodom.twist.twist.linear.x  = getVx;  
		axisodom.twist.twist.linear.y  = getVy;
		axisodom.twist.twist.angular.z = getW;    

		odom_pub_.publish(axisodom);
		last_time = current_time;

		ros::spinOnce();
		loop_rate.sleep();
	}
	ros::spin();	
	return 0;
}

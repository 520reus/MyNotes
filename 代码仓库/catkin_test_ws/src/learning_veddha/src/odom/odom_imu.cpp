#include <math.h>
#include <stdio.h>
#include "ros/ros.h"

#include <sensor_msgs/Imu.h>
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "tf/transform_broadcaster.h"

double x =0;
double y =0;                                                             
double z =0;
//全局角度  默认0度  
//车以Y轴为初始
//double Yaw = M_PI/2;
//车以X轴为初始
double Yaw   =0;
double Pitch =0;
double Roll  =0;

double getV  =0;

double getVx =0;
double getVy =0;
double getVz =0;

double getWx =0;
double getWy =0;
double getWz =0;

sensor_msgs::Imu imu_data;

void imu_data_chatterCallback(const sensor_msgs::Imu msg)
{
	//已滤波只取四元数得 偏航角 俯视角
    imu_data = msg;                                  
}
void own_vel_chatterCallback(const geometry_msgs::Twist msg)
{
    //左转为正  逆时针  V rear -> m/s 只要速度
	getV = msg.linear.x;     			    
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "odom_imu");	
	ros::NodeHandle n;	
	ros::Subscriber imu_sub_   = n.subscribe("/imu/data", 100, imu_data_chatterCallback);
	ros::Subscriber wheel_sub_ = n.subscribe("/wheel/data", 100, own_vel_chatterCallback);	
    ros::Publisher  odom_pub_  = n.advertise<nav_msgs::Odometry>("/odom/by_imu", 100);
	ros::Rate loop_rate(100);
	
	tf::Quaternion quat;
	nav_msgs::Odometry axisodom;
	ros::Time current_time, last_time;
	current_time = ros::Time::now();
	last_time    = ros::Time::now();

	//初始化
	quat.setRPY( 0, 0, 0 );
	imu_data.orientation.x = 0;
	imu_data.orientation.y = 0;
	imu_data.orientation.z = 0;
	imu_data.orientation.w = 1;
	
	while(n.ok())
	{
	    ros::spinOnce();//check here 
		current_time = ros::Time::now();
		double dt = (current_time - last_time).toSec();
        //printf("dt %f\n ",dt);

		//四元数转欧拉角 
		tf::quaternionMsgToTF(imu_data.orientation, quat);
		tf::Matrix3x3(quat).getRPY(Roll, Pitch, Yaw);
		
		//printf("Yaw : %f\n", Yaw);
		//printf("Pitch : %f\n", Pitch);
		//printf("Roll : %f\n", Roll);
		
		//速度由ros_get提供
		getVx = getV*(  cos(Yaw)*cos(Pitch) + sin(Pitch)*sin(Roll)*sin(Yaw) );
		getVy = getV*(  sin(Yaw)*cos(Roll) );	
		getVz = getV*( -sin(Pitch)*cos(Yaw) + cos(Pitch)*sin(Roll)*sin(Yaw) );
		
		//getVx = getV*( cos(Pitch)*cos(Yaw) );
		//getVy = getV*( cos(Pitch)*sin(Yaw) );	
		//getVz = getV*( sin(Pitch) );
		
		double delta_x =  getVx * dt;
		double delta_y =  getVy * dt;
		double delta_z =  getVz * dt;
		//printf("delta_x %f\n ",delta_x)
		x += delta_x;
		y += delta_y;
		z += delta_z;		 

		//geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(Yaw);
		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(Pitch,Roll,Yaw);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//反馈给各个功能包的
		
		axisodom.header.stamp = current_time;
		axisodom.header.frame_id = "imu_odom";
		axisodom.child_frame_id  = "imu_link";
		
		//set the position
		axisodom.pose.pose.position.x  = x;
		axisodom.pose.pose.position.y  = y;
		axisodom.pose.pose.position.z  = z;
		axisodom.pose.pose.orientation = odom_quat;
		
		//set the velocity		
		axisodom.twist.twist.linear.x  = getVx;        
		axisodom.twist.twist.linear.y  = getVy;        
		axisodom.twist.twist.linear.z  = getVz;    
		//imu 角速度信息暂未计算  deg/s 
		axisodom.twist.twist.angular.x = getWx;        
		axisodom.twist.twist.angular.y = getWy;        
		axisodom.twist.twist.angular.z = getWz; 

		axisodom.twist.twist.angular.x = imu_data.angular_velocity.x;        
		axisodom.twist.twist.angular.y = imu_data.angular_velocity.y;        
		axisodom.twist.twist.angular.z = imu_data.angular_velocity.z; 		       
		
		odom_pub_.publish(axisodom);
		last_time = current_time;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ros::spinOnce();
		loop_rate.sleep();
	}
	ros::spin();	
	return 0;
}

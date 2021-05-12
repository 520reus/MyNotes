#include <math.h>
#include <stdio.h>
#include <ros/ros.h>

#include "std_msgs/String.h"
#include <sensor_msgs/Imu.h>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include <sensor_msgs/NavSatFix.h>
#include "tf/transform_broadcaster.h"

#include <crss_vdcar/GpsInfo.h>

double  x =0;
double  y =0;                                                             
double  z =0;
int count =0;
double _A = 6378137.0000;   //地球椭球体长半轴，a=6378137.0000，单位为米(m);
double _B = 6356752.3142;   //地球椭球体短半轴，b=6356752.3142，单位为米(m);
double B0 = 28.18615523  * M_PI/180;    //投影基准纬度，Bo =0，弧度(rad);
double L0 = 112.94491422 * M_PI/180;    //坐标原点经度，Lo =0，弧度(rad);
double Z0 = 37.0;
double B  =0;
double L  =0;
double f, e1, e2, NB0, K, dtemp;
//f  ：扁率；
//e1 ：第一偏心率；
//e2 ：第二偏心率；
//NB0：卯酉圈曲率半径，单位为米（m）；

sensor_msgs::Imu imu_data;
crss_vdcar::GpsInfo gps_status;
sensor_msgs::NavSatFix gps_data;

void gps_status_chatterCallback(const crss_vdcar::GpsInfo msg)
{
    gps_status = msg;                         
}
void gps_data_chatterCallback(const sensor_msgs::NavSatFix msg)
{
	gps_data = msg;
}
void imu_data_chatterCallback(const sensor_msgs::Imu msg)
{
	//已滤波只取四元数得 偏航角 俯视角
    imu_data = msg;                                  
}
void initial()
{
	f = (_A-_B)/_A;
	dtemp = 1-(_B/_A)*(_B/_A);
	e1 = sqrt(dtemp);
	dtemp = (_A/_B)*(_A/_B)-1;
	e2 = sqrt(dtemp);
	NB0 = ( (_A*_A)/_B )/sqrt( 1+e2*e2*cos(B0)*cos(B0) );
	K = NB0 * cos(B0);
}
void BLToXY()
{
	x = K * (L-L0);
	y = K * log( tan(M_PI/4 + B/2) * pow( (1-e1*sin(B))/(1+e1*sin(B)) , e1/2 ) );
	printf("F %f\n",log( tan(M_PI/4 + B/2) * pow( (1-e1*sin(B))/(1+e1*sin(B)) , e1/2 ) ));
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "odom_gps");	
	ros::NodeHandle n;	
	ros::Subscriber gps_status_sub_ = n.subscribe("/gps/status", 100, gps_status_chatterCallback);
	ros::Subscriber gps_sub_        = n.subscribe("/gps/data", 100, gps_data_chatterCallback);
	ros::Subscriber imu_sub_        = n.subscribe("/imu/data", 100, imu_data_chatterCallback);
    ros::Publisher  odom_pub_       = n.advertise<nav_msgs::Odometry>("/odom/by_gps", 100);
	ros::Rate loop_rate(100);
	
	nav_msgs::Odometry axisodom;
	ros::Time current_time, last_time;
	B0 = 0 * M_PI/180;    //投影基准纬度，Bo =0，弧度(rad);
	L0 = 0 * M_PI/180;    //坐标原点经度，Lo =0，弧度(rad);
	initial();

	//初始化
	tf::Quaternion quat;
	quat.setRPY( 0, 0, 0 );
	imu_data.orientation.x = 0;
	imu_data.orientation.y = 0;
	imu_data.orientation.z = 0;
	imu_data.orientation.w = 1;
	
	while(n.ok())
	{
		ros::spinOnce();                          //check here
		current_time = ros::Time::now();
		
		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(gps_status.Roll,gps_status.Pitch,gps_status.Heading);

		printf("e1 %f\n",e1);
		printf("e2 %f\n",e2);
		printf("x %f\n",x);
		printf("K %f\n",K);
		printf("y %f\n",y);
		axisodom.header.stamp    = current_time;
		axisodom.header.frame_id = "gps_odom";
		axisodom.child_frame_id  = "gps_link";
		
		B = gps_data.latitude  * M_PI/180;
		L = gps_data.longitude * M_PI/180;
		BLToXY();
		
		if( gps_status.status == 42 || gps_status.status == 52 )
{
		axisodom.pose.pose.position.x  = x-12572950.0;
		axisodom.pose.pose.position.y  = y-3252284.0;
		axisodom.pose.pose.position.z  = gps_data.altitude-Z0;
		axisodom.pose.pose.orientation = odom_quat;

		axisodom.twist.twist.linear.x  = gps_status.Vx;   
		axisodom.twist.twist.linear.y  = gps_status.Vy;   
		axisodom.twist.twist.linear.z  = gps_status.Vz;
		//gps 没有角速度信息  deg/s
		axisodom.twist.twist.angular.x = 0;               
		axisodom.twist.twist.angular.y = 0;
		axisodom.twist.twist.angular.z = 0;
}
  	else
{




}	
		
		odom_pub_.publish(axisodom);

		ros::spinOnce();
		loop_rate.sleep();
	}
	ros::spin();	
	return 0;
}

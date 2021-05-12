#include <math.h>
#include <stdio.h>
#include <ros/ros.h>

#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include <sensor_msgs/NavSatFix.h>
#include "tf/transform_broadcaster.h"
#include "tf/transform_datatypes.h"

#include <learning_veddha/GpsInfo.h>

double  x =0;
double  y =0;                                                             
double  z =0;
int count =0;
double _A = 6378137.0000;    //地球椭球体长半轴，a=6378137.0000，单位为米(m);
double _B = 6356752.3142;    //地球椭球体短半轴，b=6356752.3142，单位为米(m);
double B0 = 0 * M_PI/180;    //投影基准纬度，Bo =0，弧度(rad);
double L0 = 0 * M_PI/180;    //坐标原点经度，Lo =0，弧度(rad);
//double B0 = 30.76206773  * M_PI/180;    //投影基准纬度，Bo =0，弧度(rad);
//double L0 = 121.14874974 * M_PI/180;    //坐标原点经度，Lo =0，弧度(rad);
//changsha->12572950.0;
//           3252284.0;
//                33.0;
//changsha->112.94504538;
//           28.18617172;
//                  21.3;
// double X0 = 13486217.0;
// double Y0 = 3580036.0;
// double Z0 = 3.0;
//latitude: 30.76075412
//longitude: 121.14877723
//altitude: 14.69
//////////////////////////////
//double X0 = 12572984.9404;
//double Y0 = 3252288.33642;
//double Z0 = 21.3;
double X0 = 28.18592496;
double Y0 = 112.94467007;
double Z0 = 37.56;
//orientation: 
double qx = 0.00239049694335;
double qy = -0.00606204053923;
double qz = -0.019996122782;
double qw = -0.999778821671;
//////////////////////////////
double B  = 0;
double L  = 0;
double f, e1, e2, NB0, K, dtemp;
//f  ：扁率；
//e1 ：第一偏心率；
//e2 ：第二偏心率；
//NB0：卯酉圈曲率半径，单位为米（m）；

learning_veddha::GpsInfo gps_status;
sensor_msgs::NavSatFix gps_data;

void gps_status_chatterCallback(const learning_veddha::GpsInfo msg)
{
    gps_status = msg;                         
}
void gps_data_chatterCallback(const sensor_msgs::NavSatFix msg)
{
	gps_data = msg;
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
void BLToXY(double B, double L)
{
	x = K * (L-L0);
	y = K * log( tan(M_PI/4 + B/2) * pow( (1-e1*sin(B))/(1+e1*sin(B)) , e1/2 ) );
	//printf("F %f\n",y);
}

int main(int argc, char** argv)
{

	ros::init(argc, argv, "odom_gps");	
	ros::NodeHandle n;	
	ros::Subscriber gps_status_sub_ = n.subscribe("/gps/info", 100, gps_status_chatterCallback);
	ros::Subscriber gps_sub_        = n.subscribe("/gps/data", 100, gps_data_chatterCallback);
    ros::Publisher  odom_pub_       = n.advertise<nav_msgs::Odometry>("/odom/by_gps", 100);
	ros::Rate loop_rate(100);
	
	nav_msgs::Odometry axisodom;
	ros::Time current_time;

	//初始化
	axisodom.pose.pose.orientation.x = 0;
	axisodom.pose.pose.orientation.y = 0;
	axisodom.pose.pose.orientation.z = 0;
	axisodom.pose.pose.orientation.w = 1;

	B0 = 0 * M_PI/180;    //投影基准纬度，Bo =0，弧度(rad);
	L0 = 0 * M_PI/180;    //坐标原点经度，Lo =0，弧度(rad);
	initial();
	
	while(n.ok())
	{
		ros::spinOnce();                          //check here
		current_time = ros::Time::now();

		geometry_msgs::Quaternion gps_quat;
		// gps_quat.x = qx;
		// gps_quat.y = qy;
		// gps_quat.z = qz;
		// gps_quat.w = qw;
		gps_quat.x = 0;
		gps_quat.y = 0;
		gps_quat.z = 0;
		gps_quat.w = 1;
 		tf::Quaternion quat;
     	tf::quaternionMsgToTF(gps_quat, quat);		

 		double roll, pitch, yaw;//定义存储r\p\y的容器
     	tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);//进行转换
		roll=pitch=yaw=0;
		double flag_roll = gps_status.Roll - roll;
		double flag_pitch = gps_status.Pitch - pitch;
		double flag_yaw = gps_status.Heading - yaw;

		//geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(flag_roll, flag_pitch, flag_yaw);
		geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(0, 0, flag_yaw);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
		//反馈给各个功能包的
		//printf("e1 %f\n",e1);
		//printf("e2 %f\n",e2);
		//printf("x %f\n",x);
		//printf("K %f\n",K);
		//printf("y %f\n",y);
		axisodom.header.stamp    = current_time;
		axisodom.header.frame_id = "gps_odom";
		axisodom.child_frame_id  = "gps_link";
		
		B = gps_data.latitude  * M_PI/180;
		L = gps_data.longitude * M_PI/180;
		BLToXY(B, L);
		
//		axisodom.pose.pose.position.x  = (x-X0)/1.162;
//		axisodom.pose.pose.position.y  = y-Y0/1.162;
//		axisodom.pose.pose.position.z  = (gps_data.altitude-Z0)/1.162;
		axisodom.pose.pose.position.x  = x-X0;
		axisodom.pose.pose.position.y  = y-Y0;
		axisodom.pose.pose.position.z  = gps_data.altitude-Z0;
		axisodom.pose.pose.position.z  = 0;
		axisodom.pose.pose.orientation = odom_quat;

		axisodom.twist.twist.linear.x  = gps_status.Vx;   
		axisodom.twist.twist.linear.y  = gps_status.Vy;   
		//axisodom.twist.twist.linear.z  = gps_status.Vz;
		axisodom.twist.twist.linear.z  = 0;
		//gps 没有角速度信息  deg/s
		axisodom.twist.twist.angular.x = 0;               
		axisodom.twist.twist.angular.y = 0;
		axisodom.twist.twist.angular.z = 0;  		

		odom_pub_.publish(axisodom);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ros::spinOnce();
		loop_rate.sleep();
	}
	ros::spin();	
	return 0;
}

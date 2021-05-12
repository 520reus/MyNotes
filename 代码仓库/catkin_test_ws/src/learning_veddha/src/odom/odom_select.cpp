#include <math.h>
#include <stdio.h>
#include "ros/ros.h"

#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include <sensor_msgs/NavSatFix.h>
#include "tf/transform_broadcaster.h"

#include <learning_veddha/GpsInfo.h>

double x =0;
double y =0;                                                             
double z =0;

double Yaw   =0;
double Pitch =0;
double Roll  =0;

double getV   =0;
double getWz  =0;

double getVv  =0;
double getVx  =0;
double getVy  =0;

int    count  =0;
int    flag   =0;
int    g_p_s  =0;

float ODOM_POSE_COVARIANCE_dynamic[6][6]  = {1e-3,    0,    0,   0,   0,    0, 
                                                0, 1e-3,    0,   0,   0,    0,
                                                0,    0,  1e6,   0,   0,    0,
                                                0,    0,    0, 1e6,   0,    0,
                                                0,    0,    0,   0, 1e6,    0,
                                                0,    0,    0,   0,   0,  1e3};
float ODOM_POSE_COVARIANCE_static[6][6]   = {1e-9,    0,    0,   0,   0,    0, 
                                                0, 1e-3, 1e-9,   0,   0,    0,
                                                0,    0,  1e6,   0,   0,    0,
                                                0,    0,    0, 1e6,   0,    0,
                                                0,    0,    0,   0, 1e6,    0,
                                                0,    0,    0,   0,   0, 1e-9};
float ODOM_TWIST_COVARIANCE_dynamic[6][6] = {1e-3,    0,    0,   0,   0,    0, 
                                                0, 1e-3,    0,   0,   0,    0,
                                                0,    0,  1e6,   0,   0,    0,
                                                0,    0,    0, 1e6,   0,    0,
                                                0,    0,    0,   0, 1e6,    0,
                                                0,    0,    0,   0,   0,  1e3};
float ODOM_TWIST_COVARIANCE_static[6][6]  = {1e-9,    0,    0,   0,   0,    0, 
                                                0, 1e-3, 1e-9,   0,   0,    0,
                                                0,    0,  1e6,   0,   0,    0,
                                                0,    0,    0, 1e6,   0,    0,
                                                0,    0,    0,   0, 1e6,    0,
                                                0,    0,    0,   0,   0, 1e-9};

nav_msgs::Odometry imu_odom, wheel_odom, gps_odom, gps_estimate_odom;
learning_veddha::GpsInfo gps_status;

void wheel_odom_chatterCallback(const nav_msgs::Odometry msg)
{
	wheel_odom = msg;
	//flag = 0;
}
void imu_odom_chatterCallback(const nav_msgs::Odometry msg)
{
	imu_odom   = msg;
	//flag = 1;
}
void gps_estimate_odom_chatterCallback(const nav_msgs::Odometry msg)
{
	gps_estimate_odom   = msg;
	//flag = 2;
}
void gps_odom_chatterCallback(const nav_msgs::Odometry msg)
{
	gps_odom   = msg;
	flag = 3;
}
void gps_status_chatterCallback(const learning_veddha::GpsInfo msg)
{
    gps_status = msg;
	//if (gps_status.status == 11 || gps_status.status == 12 || gps_status.status == 13 || 
	//	gps_status.status == 31 || gps_status.status == 32 || gps_status.status == 33 || 
	//	gps_status.status == 41 || gps_status.status == 42 || gps_status.status == 43 ||
	//	gps_status.status == 51 || gps_status.status == 52 || gps_status.status == 53 ||
	//	gps_status.status == 61 || gps_status.status == 62 || gps_status.status == 63 ||
	//	gps_status.status == 81 || gps_status.status == 82 || gps_status.status == 83 ||
	//	gps_status.status == 91 || gps_status.status == 92 || gps_status.status == 93 )  
	//{
	//	g_p_s = 1;
	//}                       
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "odom_actor");	
	ros::NodeHandle h;	
	ros::Subscriber gps_status_sub_   = h.subscribe("/gps/info", 100, gps_status_chatterCallback);
	ros::Subscriber wheel_sub_        = h.subscribe("/odom/by_wheel", 100, wheel_odom_chatterCallback);
	ros::Subscriber imu_sub_          = h.subscribe("/odom/by_imu", 100, imu_odom_chatterCallback);
	ros::Subscriber gps_estimate_sub_ = h.subscribe("/odom/by_gps_estimate", 100, gps_estimate_odom_chatterCallback);
	ros::Subscriber gps_sub_          = h.subscribe("/odom/by_gps", 100, gps_odom_chatterCallback);
	ros::Publisher  odom_pub_         = h.advertise<nav_msgs::Odometry>("/odom", 1000);	
	ros::Rate loop_rate(100);
	
	tf::Quaternion quat;
    geometry_msgs::TransformStamped odom_trans;
	nav_msgs::Odometry axisodom;
	ros::Time current_time, last_time;
    current_time = ros::Time::now();
    last_time    = ros::Time::now();

	//初始化
	wheel_odom.pose.pose.orientation.x = 0;
	wheel_odom.pose.pose.orientation.y = 0;
	wheel_odom.pose.pose.orientation.z = 0;
	wheel_odom.pose.pose.orientation.w = 1;
	
	imu_odom.pose.pose.orientation.x = 0;
	imu_odom.pose.pose.orientation.y = 0;
	imu_odom.pose.pose.orientation.z = 0;
	imu_odom.pose.pose.orientation.w = 1;
	
	gps_estimate_odom.pose.pose.orientation.x = 0;
	gps_estimate_odom.pose.pose.orientation.y = 0;
	gps_estimate_odom.pose.pose.orientation.z = 0;
	gps_estimate_odom.pose.pose.orientation.w = 1;
		
	gps_odom.pose.pose.orientation.x = 0;
	gps_odom.pose.pose.orientation.y = 0;
	gps_odom.pose.pose.orientation.z = 0;
	gps_odom.pose.pose.orientation.w = 1;
	
	quat.setRPY( 0, 0, 0 );
	
	for(int i=0;i<35;i++)
	{
		axisodom.pose.covariance[i]=0;
	}

	while(h.ok())
	{
		ros::spinOnce();//check here 
		current_time = ros::Time::now();
		double dt = (current_time - last_time).toSec();
		static tf::TransformBroadcaster odom_broadcaster;   
		flag = 3;	 
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		odom_trans.header.frame_id = "odom";
		odom_trans.child_frame_id  = "base_footprint";
		odom_trans.header.stamp    = current_time;

		axisodom.header.frame_id = "odom";
		axisodom.child_frame_id  = "base_footprint";
		axisodom.header.stamp    = current_time;
			
		tf::quaternionMsgToTF(gps_odom.pose.pose.orientation, quat);
		tf::Matrix3x3(quat).getRPY(Roll, Pitch, Yaw);
		//printf("Yaw : %f\n", Yaw);
		//printf("Pitch : %f\n", Pitch);
		//printf("Roll : %f\n", Roll);

        if(flag == 0)
		{
        odom_trans.header.stamp    = wheel_odom.header.stamp;

		odom_trans.transform.translation.x = wheel_odom.pose.pose.position.x;
		odom_trans.transform.translation.y = wheel_odom.pose.pose.position.y;
		odom_trans.transform.translation.z = wheel_odom.pose.pose.position.z;
		odom_trans.transform.rotation      = wheel_odom.pose.pose.orientation;

		axisodom.header.stamp    = wheel_odom.header.stamp;

		axisodom.pose.pose.position.x  = wheel_odom.pose.pose.position.x;
		axisodom.pose.pose.position.y  = wheel_odom.pose.pose.position.y;
		axisodom.pose.pose.position.z  = wheel_odom.pose.pose.position.z;
		axisodom.pose.pose.orientation = wheel_odom.pose.pose.orientation;

		axisodom.twist.twist.linear.x  = wheel_odom.twist.twist.linear.x;   
		axisodom.twist.twist.linear.y  = wheel_odom.twist.twist.linear.y;   
		axisodom.twist.twist.angular.z = wheel_odom.twist.twist.angular.z;
		}

		else if(flag == 1)
		{
		odom_trans.header.stamp    = imu_odom.header.stamp;

		odom_trans.transform.translation.x = imu_odom.pose.pose.position.x;
		odom_trans.transform.translation.y = imu_odom.pose.pose.position.y;
		odom_trans.transform.translation.z = imu_odom.pose.pose.position.z;
		odom_trans.transform.rotation      = imu_odom.pose.pose.orientation;

		axisodom.header.stamp    = imu_odom.header.stamp;

		axisodom.pose.pose.position.x  = imu_odom.pose.pose.position.x;
		axisodom.pose.pose.position.y  = imu_odom.pose.pose.position.y;
		axisodom.pose.pose.position.z  = imu_odom.pose.pose.position.z;
		axisodom.pose.pose.orientation = imu_odom.pose.pose.orientation;
		
		//set the velocity m/s deg/s 	
		axisodom.twist.twist.linear.x  = imu_odom.twist.twist.linear.x;   
		axisodom.twist.twist.linear.y  = imu_odom.twist.twist.linear.y;   
		axisodom.twist.twist.linear.z  = imu_odom.twist.twist.linear.z;
		
		axisodom.twist.twist.angular.x = imu_odom.twist.twist.angular.x;
		axisodom.twist.twist.angular.y = imu_odom.twist.twist.angular.y;
		axisodom.twist.twist.angular.z = imu_odom.twist.twist.angular.z; 

		flag =0;  
		}

		else if(flag == 2)
		{
		odom_trans.header.stamp    = gps_estimate_odom.header.stamp;
		
		odom_trans.transform.translation.x = gps_estimate_odom.pose.pose.position.x;
		odom_trans.transform.translation.y = gps_estimate_odom.pose.pose.position.y;
		odom_trans.transform.translation.z = gps_estimate_odom.pose.pose.position.z;
		odom_trans.transform.rotation      = gps_estimate_odom.pose.pose.orientation;

		axisodom.header.stamp    = gps_estimate_odom.header.stamp;

		axisodom.pose.pose.position.x  = gps_estimate_odom.pose.pose.position.x;
		axisodom.pose.pose.position.y  = gps_estimate_odom.pose.pose.position.y;
		axisodom.pose.pose.position.z  = gps_estimate_odom.pose.pose.position.z;
		axisodom.pose.pose.orientation = gps_estimate_odom.pose.pose.orientation;

		axisodom.twist.twist.linear.x  = gps_estimate_odom.twist.twist.linear.x;   
		axisodom.twist.twist.linear.y  = gps_estimate_odom.twist.twist.linear.y;   
		axisodom.twist.twist.linear.z  = gps_estimate_odom.twist.twist.linear.z;

		flag =0;		
		}
//&& g_p_s == 1
		else if(flag == 3)
		{
		odom_trans.header.stamp    = gps_odom.header.stamp;
		
		odom_trans.transform.translation.x = gps_odom.pose.pose.position.x;
		odom_trans.transform.translation.y = gps_odom.pose.pose.position.y;
		odom_trans.transform.translation.z = gps_odom.pose.pose.position.z;
		//odom_trans.transform.translation.z = 0;
		odom_trans.transform.rotation      = gps_odom.pose.pose.orientation;

		axisodom.header.stamp    = gps_odom.header.stamp;

		axisodom.pose.pose.position.x  = gps_odom.pose.pose.position.x;
		axisodom.pose.pose.position.y  = gps_odom.pose.pose.position.y;
		axisodom.pose.pose.position.z  = gps_odom.pose.pose.position.z;
		//axisodom.pose.pose.position.z  = 0;
		axisodom.pose.pose.orientation = gps_odom.pose.pose.orientation;

		axisodom.twist.twist.linear.x  = gps_odom.twist.twist.linear.x;   
		axisodom.twist.twist.linear.y  = gps_odom.twist.twist.linear.y;   
		axisodom.twist.twist.linear.z  = gps_odom.twist.twist.linear.z;
		//none angular 
		axisodom.twist.twist.angular.x = imu_odom.twist.twist.angular.x;
		axisodom.twist.twist.angular.y = imu_odom.twist.twist.angular.y;
		axisodom.twist.twist.angular.z = imu_odom.twist.twist.angular.z; 
		
		flag =0;
		g_p_s =0;
		}

		else
		{
    		ROS_INFO("no info!");
		}

		odom_broadcaster.sendTransform(odom_trans);
		odom_pub_.publish(axisodom);
		last_time = current_time;
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ros::spinOnce();
		loop_rate.sleep();
	}
	ros::spin();	
	return 0;
}


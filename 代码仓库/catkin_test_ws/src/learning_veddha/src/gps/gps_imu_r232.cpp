#include <ros/ros.h>
#include <std_msgs/Empty.h> 
#include <std_msgs/String.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "tf/transform_broadcaster.h"

#include <math.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cmath>
#include <string> 
#include <vector>
#include <cstdlib>
#include <sstream>
#include <iomanip>

#include <serial/serial.h>

#include <crss_vdcar/ImuCalib.h>
#include <crss_vdcar/GpsInfo.h>

#define gravity        9.8
#define statics_error

static serial::Serial ser;
static crss_vdcar::ImuCalib calib_data;

using namespace std;

//解析GPS
void RecePro(std::string s, 
			 double& Latitude,              double& Longitude,             double& Altitude, 
			 double& Heading,               double& Pitch,                 double& Roll,
			 double& angular_velocity_x,    double& angular_velocity_y,    double& angular_velocity_z,
			 double& linear_acceleration_x, double& linear_acceleration_y, double& linear_acceleration_z,
			 double& Ve,                    double& Vn,                    double& Vu,                    double& Vv, 
			 double& status)
{
	{/* Example:
	//$GPCHC,2118, 458024.30, 0.00,  0.37,-0.86, 0.08,-0.02,-0.06,0.0149,0.0063,0.9939,0.00000000,0.00000000, -16.13,1.482,-2.114,-1.109,2.581,0, 0,61,0,2*60
	//$GPCHC,2118, 458023.90, 0.00,  0.42,-0.80, 0.12,-0.08, 0.04,0.0139,0.0073,0.9927,0.00000000,0.00000000, -16.13,1.482,-2.114,-1.109,2.581,0, 0,61,0,2*4E
	//$GPCHC,2118, 457937.60, 0.00,  0.39,-0.83, 0.16, 0.00,-0.02,0.0144,0.0068,0.9927,0.00000000,0.00000000, -16.13,1.482,-2.114,-1.109,2.581,0, 0,61,0,2*47
	//$GPCHC,1980,  16897.68, 289.19, 0.42, 0.21, 0.23, 0.07,-0.06,0.0009, 0.0048,1.0037, 38.8594969, 121.5150073,121.51,0.023, 0.011, 0.000,1.500,14, 6, 4,0,2*47
	//$GPCHC,2128, 543938.03,  74.36,-0.18,-0.67, 0.08, 0.12, 0.07,0.0118,-0.0032,0.9995,28.18596230,112.94463517, 21.37,0.009,-0.004, 0.004,0.010,30,27,41,2,2*69
	*/}
	//分割有效数据，存入vector中
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
	pos2 = s.find(",");
	pos1 = 0;

	while (std::string::npos != pos2){
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
		pos2 = s.find(",", pos1);
	}
	if (pos1 != s.length()){
		v.push_back(s.substr(pos1));
	}
	
	{/*
	$GPCHC,    GPSWeek,   GPSTime, 
	0          1          2 
	Heading,   Pitch,     Roll, 
	3          4          5 
	gyro x,    gyro y,    gyro z, 
	6          7          8  
	acc x,     acc y,     acc z, 
	9          10         11
	Latitude, Longitude, Altitude, 
	12         13         14
	Ve,        Vn,        Vu,        VBaseline, 
	15         16         17         18
	NSV1,      NSV2,      Status,
	19         20         21
	Age,       Warming,   Cs,        <CR><LF>
	22         23         24         25
	*/}
	// std::cout << s << "\n" ;
	// std::cout <<"v.size():" << v.size()<< "\n" ;
	if (v.size() >= 24)
	{
		std::cout <<"v.size() >= 24: true " << "\n" ;
		// if (v[21] == "11" || v[21] == "12" || v[21] == "13" ||
			// v[21] == "31" || v[21] == "32" || v[21] == "33" ||
			// v[21] == "41" || v[21] == "42" || v[21] == "43" ||
			// v[21] == "51" || v[21] == "52" || v[21] == "53" ||
			// v[21] == "61" || v[21] == "62" || v[21] == "63" ||
			// v[21] == "81" || v[21] == "82" || v[21] == "83" ||
			// v[21] == "91" || v[21] == "92" || v[21] == "93" )
		if(v[21]!="")
		{
			//纬度 经度 高度  --gps
			if (v[12] != "") Latitude  = std::atof(v[12].c_str());
			if (v[13] != "") Longitude = std::atof(v[13].c_str());
			if (v[14] != "") Altitude  = std::atof(v[14].c_str());
			//std::cout << "Latitude:" << Latitude << "\tLongitude:" << Longitude << "\tAltitude:" << Altitude << std::endl;
			
			//大地坐标系
			if (v[15] != "") Ve = std::atof(v[15].c_str());
			if (v[16] != "") Vn = std::atof(v[16].c_str());
			if (v[17] != "") Vu = std::atof(v[17].c_str());
			if (v[18] != "") Vv = std::atof(v[18].c_str());
			//std::cout << "Ve_east:" << Ve  << "\t\tVn_north:" << Vn << "\t\tVu_vertical:" << Vu << "\t\tVu_vertical:" << Vv << std::endl;

			//偏航角  俯仰角  横滚角 --imu deg  X->Roll Y->Pitch Z->Yaw
			if (v[3] != "") Heading = std::atof(v[3].c_str());
			if (v[4] != "") Pitch   = std::atof(v[4].c_str());
			if (v[5] != "") Roll    = std::atof(v[5].c_str());
			//std::cout << "orien_x:" << Heading << "\torien_y:" << Pitch << "\torien_z:" << Roll << std::endl;
		}
		else
		{
			std::cout<<"there is no gps information, please move to the place that can receive the information about gps."<< std::endl;
		}

		//陀螺 x y z 轴  --imu  deg/s		
		if (v[6] != "") angular_velocity_x = std::atof(v[6].c_str());
		if (v[7] != "") angular_velocity_y = std::atof(v[7].c_str());
		if (v[8] != "") angular_velocity_z = std::atof(v[8].c_str());
		//std::cout << "ang_vel_x :" << angular_velocity_x  << "\tang_vel_y:" <<  angular_velocity_y << "\tang_vel_z:" << angular_velocity_z<< std::endl;

		//加表x  y  z 轴  -imu  g
		if (v[9]  != "") linear_acceleration_x = std::atof(v[9].c_str());
		if (v[10] != "") linear_acceleration_y = std::atof(v[10].c_str());
		if (v[11] != "") linear_acceleration_z = std::atof(v[11].c_str());
		//std::cout << "line_acc_x :" << linear_acceleration_x  << "\tline_acc_y:" << linear_acceleration_y << "\tline_acc_z:" << linear_acceleration_z<< std::endl;

		//状态status
		if (v[21] != "") status = std::atof(v[21].c_str());
		// std::cout <<"v[21]:" << v[21]<< "\n" ;
	}
	else
	{
		std::cout << "information is not whole and error"<< std::endl;
	}
}

void sigint_handler(int sig)
{
	ROS_INFO("shutting down!");
	ser.close();
	ros::shutdown();
}

void imu_gps_calibCallback(const crss_vdcar::ImuCalib msg)
{
	calib_data = msg;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "gps_imu_R232");
	ros::NodeHandle nh;
	ros::Publisher  gps_raw_pub_      = nh.advertise<sensor_msgs::NavSatFix>("/gps/data_raw", 100);
	ros::Publisher  gps_pub_          = nh.advertise<sensor_msgs::NavSatFix>("/gps/data", 100);
	ros::Publisher  imu_raw_pub_      = nh.advertise<sensor_msgs::Imu>("/imu/data_raw", 100);
	ros::Publisher  imu_pub_          = nh.advertise<sensor_msgs::Imu>("/imu/data_own", 100);
	ros::Publisher  gps_info_raw_pub_ = nh.advertise<crss_vdcar::GpsInfo>("/gps/info_raw", 100);
	ros::Publisher  gps_info_pub_     = nh.advertise<crss_vdcar::GpsInfo>("/gps/info", 100);
	ros::Subscriber calib_sub_        = nh.subscribe("/imu/calibrate", 100, imu_gps_calibCallback);
	ros::Rate loop_rate(100); 

	std::string strRece, buff;	
	ros::Time current_time;
	
	//初始化
	calib_data.linearx = 0;
	calib_data.lineary = 0;
	calib_data.linearz = 0;
	calib_data.ThetaX = 0;
	calib_data.ThetaY = 0;
	calib_data.ThetaZ = 0;
	// GPS.position_covariance =  {1,   0,   0,   0,    0,   0, // covariance on gps_x
	// 							   0,   1,   0,   0,    0,   0, // covariance on gps_y 
	// 							   0,   0,   1,   0,    0,   0, // covariance on gps_z 
	// 						       0,   0,   0, 1e3,    0,   0, // large covariance on rot x 
	// 							   0,   0,   0,   0,  1e3,   0, // large covariance on rot y 
	// 							   0,   0,   0,   0,    0, 1e3};// large covariance on rot z
	signal(SIGINT, sigint_handler);

	try{
		ser.setPort("/dev/ttyUSB1");
		ser.setBaudrate(460800);
		serial::Timeout to = serial::Timeout::simpleTimeout(1000);
		ser.setTimeout(to);
		ser.open();
	}
	catch (serial::IOException & e){
		ROS_ERROR_STREAM("Unable to open Serial Port !");
		return -1;
	}
	if (ser.isOpen()){
		ROS_INFO_STREAM("Serial Port initialized");
	}
	else{
		return -1;
	}
	
	while(nh.ok())
	{
		if (ser.available())
		{
			//1.读取串口信息：
			ROS_INFO_STREAM("Reading from serial port");
			//std::cout << ser.available();
			//std::cout << ser.read(ser.available());
			//strRece  += ser.read(ser.available());
			buff.clear();
			buff += ser.read(ser.available());
			ros::Duration(0.0001).sleep();
			strRece = buff;
			buff.clear();
			//std::cout <<"strRece:" << strRece  << std::endl;
			//std::cout <<"strRece.size():" << strRece.size() << std::endl;
			//2.截取数据、解析数据：		        
			//GPS起始标志
			std::string gstart = "$GPCHC";
			//GPS终止标志
			std::string gend = "\r\n";
			int i = 0, start = -1, end = -1;
			while (i < strRece.length())
			{
				//找起始标志
				start = strRece.find(gstart);
				// std::cout <<"start find start"<< std::endl;
				if (start == -1){
					break;
					// if (strRece.length() > 2)
					// {
					// 	// std::cout <<"find start1"<< std::endl;
					// 	strRece = strRece.substr(strRece.length() - 3);
					// 	// std::cout <<"find start2"<< std::endl;
					// 	break;
					// }
				}
				else
				{
					//找终止标志
					end = strRece.find(gend);
					// std::cout <<"start find end"<< std::endl;
					//如果没找到，把起始标志开始的数据留下，前面的数据丢弃，然后跳出循环
					if (end == -1){
						strRece = strRece.substr(start);
						break;
					}
					//如果找到了终止标志，把这段有效的数据剪切给解析的函数，剩下的继续开始寻找
					else
					{
						ros::spinOnce();
						i = end;
						//把有效的数据给解析的函数以获取经纬度
						double Latitude,              Longitude,             Altitude; 
						double Heading,               Pitch,                 Roll;
						double angular_velocity_x,    angular_velocity_y,    angular_velocity_z;
						double linear_acceleration_x, linear_acceleration_y, linear_acceleration_z; 
						double Ve,                    Vn,                    Vu,                    Vv;
						double status;

						RecePro(strRece.substr(start, end + 2 - start),
								Latitude,              Longitude,             Altitude, 
								Heading,               Pitch,                 Roll,
								angular_velocity_x,    angular_velocity_y,    angular_velocity_z,
								linear_acceleration_x, linear_acceleration_y, linear_acceleration_z, 
								Ve,                    Vn,                    Vu,                    Vv,
								status);

						ROS_INFO_STREAM("RecePro initialized");
						current_time = ros::Time::now();

						//发布GPS RAW_DATA
						sensor_msgs::NavSatFix gps_raw_data;
						gps_raw_data.header.stamp    = current_time;
						gps_raw_data.header.frame_id = "gps_link";
						gps_raw_data.status.status = 1;  // 1 with satellite-based augmentation
						gps_raw_data.status.service = 4;
						gps_raw_data.latitude  = Latitude;
						gps_raw_data.longitude = Longitude;
						gps_raw_data.altitude  = Altitude;

						gps_raw_pub_.publish(gps_raw_data);
						
						//发布组合导航数据  GPS信号不一定存在
						crss_vdcar::GpsInfo gps_raw_info;
						gps_raw_info.header.stamp    = current_time;
						gps_raw_info.header.frame_id = "gps_link";
						gps_raw_info.status  = status;
						gps_raw_info.Vx      = Ve;
						gps_raw_info.Vy      = Vn;
						gps_raw_info.Vz      = Vu;
						gps_raw_info.Vv      = Vv;
						gps_raw_info.Heading = Heading; //->X
						gps_raw_info.Pitch   = Pitch;   //->y
						gps_raw_info.Roll    = Roll;    //->Z
						
						gps_info_raw_pub_.publish(gps_raw_info);
						
						//发布IMU
						sensor_msgs::Imu imu_raw_data;
						imu_raw_data.header.stamp    = current_time;
						imu_raw_data.header.frame_id = "imu_link";
						//角速度  rad/s
						imu_raw_data.angular_velocity.x = angular_velocity_x;
						imu_raw_data.angular_velocity.y = angular_velocity_y;
						imu_raw_data.angular_velocity.z = angular_velocity_z;				
						//线加速度 m/s^2
						imu_raw_data.linear_acceleration.x = linear_acceleration_x;
						imu_raw_data.linear_acceleration.y = linear_acceleration_y;
						imu_raw_data.linear_acceleration.z = linear_acceleration_z;
						//四元数位姿
						geometry_msgs::Quaternion catch_orientation;	
						catch_orientation = tf::createQuaternionMsgFromRollPitchYaw(Roll, Pitch, Heading);
						imu_raw_data.orientation.x = catch_orientation.x;
						imu_raw_data.orientation.y = catch_orientation.y;
						imu_raw_data.orientation.z = catch_orientation.z;
						imu_raw_data.orientation.w = catch_orientation.w;

						imu_raw_pub_.publish(imu_raw_data);
						
						//转换右手坐标系  车身方向y轴为主方向
						//发布GPS
						sensor_msgs::NavSatFix gps_data;
						gps_data.header.stamp    = current_time;
						gps_data.header.frame_id = "gps_link";
						gps_data.status.status = 1;  // 1 with satellite-based augmentation
						gps_data.status.service = 4;
						gps_data.latitude  = Latitude;
						gps_data.longitude = Longitude;
						gps_data.altitude  = Altitude;

						gps_pub_.publish(gps_data);
						
						//发布组合导航数据  GPS信号存在
						//if(status == 42 || status == 52)
						double degRoll    = Roll*M_PI/180;              //->X
						double degPitch   = Pitch*M_PI/180;             //->y
						double degHeading = (360-Heading+90)*M_PI/180;  //->Z
						if(degHeading > M_PI)
						{degHeading = degHeading - 2*M_PI;}
						if(degHeading < -M_PI)
						{degHeading = degHeading + 2*M_PI;}
						//if(degHeading + 1.569 > M_PI)
						//	degHeading -= 2* M_PI;
						//if(degHeading + 1.569 > 0)
						//	degHeading += 2* M_PI;
						crss_vdcar::GpsInfo gps_info;
						gps_info.header.stamp    = current_time;
						gps_info.header.frame_id = "gps_link";
						gps_info.status  = status;
						gps_info.Vx      = Ve;
						gps_info.Vy      = Vn;
						gps_info.Vz      = Vu;
						gps_info.Vv      = Vv;
						gps_info.Heading = degHeading;
						gps_info.Pitch   = degPitch;
						gps_info.Roll    = degRoll;

						gps_info_pub_.publish(gps_info);
						
						//发布IMU
						sensor_msgs::Imu imu_data;
						imu_data.header.stamp    = current_time;
						imu_data.header.frame_id = "imu_link";
						//角速度  rad/s
						imu_data.angular_velocity.x =  angular_velocity_x*M_PI/180-(calib_data.ThetaX);
						imu_data.angular_velocity.y =  angular_velocity_y*M_PI/180-(calib_data.ThetaY);
						imu_data.angular_velocity.z =  angular_velocity_z*M_PI/180-(calib_data.ThetaZ);
						//线加速度 m/s^2
						imu_data.linear_acceleration.x =  linear_acceleration_x*gravity-(calib_data.linearx);
						imu_data.linear_acceleration.y =  linear_acceleration_y*gravity-(calib_data.lineary);
						imu_data.linear_acceleration.z =  linear_acceleration_z*gravity-(calib_data.linearz);
						//四元数位姿,初始位姿 -> 获取ＩＭＵ的数据，卡尔曼滤波获得稳定位姿
						//degRoll = degPitch = degHeading = 0;
						catch_orientation = tf::createQuaternionMsgFromRollPitchYaw(degRoll, degPitch, degHeading);
						imu_data.orientation.x = catch_orientation.x;
						imu_data.orientation.y = catch_orientation.y;
						imu_data.orientation.z = catch_orientation.z;
						imu_data.orientation.w = catch_orientation.w;

						imu_pub_.publish(imu_data);
						
						//如果剩下的字符大于等于4，则继续循环寻找有效数据,如果所剩字符小于等于3则跳出循环
						if (i + 5 < strRece.length()){
							strRece = strRece.substr(end + 2);
							// std::cout <<"i+5 < strRece.length()"<< std::endl;
						}
						else{
							strRece = strRece.substr(end + 2);
							// std::cout <<"i+5 > strRece.length()"<< std::endl;
							break;
						}
					}
				}
			}
		}
		else {
			if (!ser.isOpen()) {
				ROS_ERROR("GPS IMU SERIAL OFFLINE, RETRY AFTER 2 SECONDS");
				sleep(2);
				try {
					ROS_INFO("Close and reopen Serial Port");
					ser.close();
					ser.setPort("/dev/ttyUSB1");
					ser.setBaudrate(460800);
					serial::Timeout to = serial::Timeout::simpleTimeout(1000);
					ser.setTimeout(to);
					ser.open();
					if (!ser.isOpen()) {
						ROS_ERROR("Unable to open Serial Port !");
					}
				}
				catch (serial::IOException & e) {
					ROS_ERROR("Unable to open Serial Port !");
				}
			}
		}
		loop_rate.sleep();
	}
	//ser.close();
	//进程挂起等待退出
	ros::spin();
	return 0;
}

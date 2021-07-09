#include<iostream>
#include<ros/ros.h>
#include <serial/serial.h>
#include <queue>
#include <deque>
#include <math.h>

#include <sensor_msgs/Imu.h>
#include <sensor_msgs/NavSatFix.h>

#include "tf/transform_broadcaster.h"
#include <vd_msgs/GpsInfo.h>


using std::endl;
using std::cout;

const double gravity = 9.80665;

static serial::Serial ser;

/**
 * @brief:异或和校验BBC
 * @param:数据帧起始位
 * @return:异或校验码
  */
uint8_t BCCCheckOut(const std::vector<uint8_t>& data)
{
	uint8_t BCCval = 0x00;
	size_t length = data.size();
	for(size_t i = 0;i < length-1;i++)
	{
		BCCval ^= data[i];
	}
	return BCCval;
}

/**
 * @brief: 判断当前数据状态
 * 传入为数据的第39位偏移，bit0:位置 bit1:速度 bit2:姿态 bit3:航向角，以上全为1表示完成初对准
  */
void statusCheckOut(const uint8_t& data,double* status)
{
    bool position_ok = false;
    bool velosity_ok = false;
    bool pose_ok = false;
    bool yaw_ok = false;
    uint8_t position_status = data & 0x01;
    if(position_status == 0x01)
    {
        position_ok = true;
        cout << "[success]位置完成初对准" << endl;
    }
    else
        cout << "[failure]位置未完成初对准" << endl;
    uint8_t velosity_status = data & 0x02;
    if(velosity_status == 0x02)
    {
        velosity_ok = true;
        cout << "[success]速度完成初对准" << endl;
    }
    else
        cout << "[failure]速度未完成初对准" << endl;
    uint8_t pose_status = data & 0x04;
    if(pose_status == 0x04)
    {
        pose_ok = true; 
        cout << "[success]姿态完成初对准" << endl;
    }    
    else
        cout << "[failure]姿态未完成初对准" << endl; 
    uint8_t yaw_status = data & 0x08;
    if(yaw_status == 0x08)
    {
        yaw_ok = true;
        cout << "[success]航向角完成初对准" << endl;
    }
    else
        cout << "[failure]航向角未完成初对准" << endl;    
    if(position_ok && velosity_ok && pose_ok && yaw_ok){
        cout << "[all initialization success]位置、速度、姿态、航向角均已完成初对准！" << endl;
        *status = 1; //这里用double代替bool了
    }
}

void printData(double& roll, double& pitch, double& yaw,
					  double& angular_velocity_x, double& angular_velocity_y, double& angular_velocity_z,
					  double& linear_acceleration_x, double& linear_acceleration_y, double& linear_acceleration_z,
					  double& Latitude, double& Longitude, double& Altitude,
					  double& Vn, double& Ve, double& Vg,
					  double& status, double& time)
{
    cout << "roll = " << roll << endl;
    cout << "pitch = " << pitch << endl;
    cout << "yaw = " << yaw << endl;
    cout << "angular_velocity_x = " << angular_velocity_x << endl;
    cout << "angular_velocity_y = " << angular_velocity_y << endl;
    cout << "angular_velocity_z = " << angular_velocity_z << endl;
    cout << "linear_acceleration_x = " << linear_acceleration_x << endl;
    cout << "linear_acceleration_y = " << linear_acceleration_y << endl;
    cout << "linear_acceleration_z = " << linear_acceleration_z << endl;
    cout << "Latitude = " << Latitude << endl;
    cout << "Longitude = " << Longitude << endl;
    cout << "Altitude = " << Altitude << endl;
    cout << "Vn = " << Vn << endl;
    cout << "Ve = " << Ve << endl;
    cout << "Vg = " << Vg << endl;
    cout << "time = " << time << endl;
}

/**
 * @brief:将LSB_first字节序存储的两个字节解析为实际数据
 * @param:
 * 
  */
void processTwoBytes(const uint8_t& low, const uint8_t& high, const double& factor, double* res)
{
	uint8_t low_ = low;
    uint16_t high_ = high << 8;
    uint16_t res_ = low_ + high_;
    int16_t res_i16 = 0;
    res_i16 = *((int16_t*)(&res_));
    *res = (double) res_i16 * factor;
}

/**
 * @brief:将LSB_first字节序存储的四个字节解析为实际数据
 * @param:
  */
void processFourBytes(const uint8_t& low_low, const uint8_t& low_high, 
                      const uint8_t& high_low, const uint8_t& high_high,
					  const double& factor, double* res)
{
    uint8_t low_low_ = low_low;
    uint16_t low_high_ = low_high << 8;
    uint32_t high_low_ = high_low << 16;
    uint32_t high_high_ = high_high << 24;
    uint32_t res_ = high_high_ + high_low_ + low_high_ + low_low_;
    int32_t res_i32 = 0;
    res_i32 = *((int32_t*)(&res_));
    *res = (double) res_i32 * factor;
}

/**
 * @brief:从队列头中取得一个数据帧进行报文解析
 * @param:一个完整的数据帧，包括三位数据头与尾校验位共58个字节
*/
void processDataFrame(std::vector<uint8_t>& frame,
					  double& roll, double& pitch, double& yaw,
					  double& angular_velocity_x, double& angular_velocity_y, double& angular_velocity_z,
					  double& linear_acceleration_x, double& linear_acceleration_y, double& linear_acceleration_z,
					  double& Latitude, double& Longitude, double& Altitude,
					  double& Vn, double& Ve, double& Vg,
					  double& status, double& time)
{
	//横滚角 deg
	processTwoBytes(frame[3],frame[4],static_cast<double>(360)/32768,&roll);
	//俯仰角 deg
	processTwoBytes(frame[5],frame[6],static_cast<double>(360)/32768,&pitch);
	//航向角 deg
    processTwoBytes(frame[7],frame[8],static_cast<double>(360)/32768,&yaw);
	//x轴角速度 deg/s
	processTwoBytes(frame[9],frame[10],static_cast<double>(360)/32768,&angular_velocity_x);
	//y轴角速度 deg/s
	processTwoBytes(frame[11],frame[12],static_cast<double>(360)/32768,&angular_velocity_y);
	//z轴角速度 deg/s
	processTwoBytes(frame[13],frame[14],static_cast<double>(360)/32768,&angular_velocity_z);
	//x轴加速度 m/s^2
	processTwoBytes(frame[15],frame[16],static_cast<double>(12)/32768*gravity,&linear_acceleration_x);
	//y轴加速度 m/s^2
	processTwoBytes(frame[17],frame[18],static_cast<double>(12)/32768*gravity,&linear_acceleration_y);
	//z轴加速度 m/s^2
	processTwoBytes(frame[19],frame[20],static_cast<double>(12)/32768*gravity,&linear_acceleration_z);
	//latitude维度 deg
    processFourBytes(frame[21],frame[22],frame[23],frame[24],1.00e-07,&Latitude);
	//longitude经度 deg
	processFourBytes(frame[25],frame[26],frame[27],frame[28],1.00e-07,&Longitude);
	//altitude高度 deg
	processFourBytes(frame[29],frame[30],frame[31],frame[32],1.00e-07,&Altitude);
	//Vn北向速度 m/s
	processTwoBytes(frame[33],frame[34],static_cast<double>(1e2)/32768,&Vn);
	//Ve东向速度 m/s
	processTwoBytes(frame[35],frame[36],static_cast<double>(1e2)/32768,&Ve);
	//Vg地向速度 m/s
	processTwoBytes(frame[37],frame[38],static_cast<double>(1e2)/32768,&Vg);
	//time时间 s
	processFourBytes(frame[52],frame[53],frame[54],frame[55],static_cast<double>(2.5e-04),&time);

	//状态位观测
	statusCheckOut(frame[39],&status);
}

int main(int argc,char* argv[])
{
   
    ros::init(argc,argv,"gnss_dy_rs422");
    ros::NodeHandle nh;
	ros::Publisher gps_raw_pub = nh.advertise<sensor_msgs::NavSatFix>("/dy_gnss/gps_data_raw",100);
	ros::Publisher imu_raw_pub = nh.advertise<sensor_msgs::Imu>("/dy_gnss/imu_data_raw", 100);
	ros::Publisher  gps_info_raw_pub = nh.advertise<vd_msgs::GpsInfo>("dy_gnss/gps_info_raw", 100);

    std::string buff;	

    ros::Time current_time;
    ros::Rate rate(40);

    try{
		ser.setPort("/dev/ttyUSB0");
		ser.setBaudrate(230400);
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

	size_t buffer_size = 500;
	std::deque<std::vector<uint8_t>> data_frame_buff(buffer_size); //数据帧缓存区
	size_t passed_count = 0;
	size_t unpassed_count = 0;
    while(nh.ok())
    {
	
		size_t n = ser.available();
	//std::cout  << "data buffer size:" << std::dec << n << std::endl;
        if(ser.available())
        {   
        //*1.从串口读取数据
	    //ROS_INFO_STREAM("Reading from serial port");
		uint8_t buffers[1024];
		n = ser.read(buffers, n);
		/*
		std::cout << "the data of buffers:" << std::endl;
		for(size_t i = 0; i < n; i++){
			std::cout<< std::hex << (buffers[i]&0xff) << " ";
		}
       		std::cout << std::endl;
		*/
		//防止data_frame_buff溢出，删除历史较久的数据,保持buffer里面存储时间最近的100条解析数据
		if(data_frame_buff.size() > buffer_size)
			data_frame_buff.erase(data_frame_buff.begin(),data_frame_buff.begin()+buffer_size-100);

		//*2.放入数据缓存队列
		//导远电子的组合导航一个数据帧有58个偏移，偏移0-2的三个字节为起始标志位，偏移57为异或校验位(BCC校验)
		//数据帧起始标志 bd db b,剩下55个字节为数据位

		size_t i = 0;
		if(n > 58)
		{
			while(i < n)
			{
				std::vector<uint8_t> OneFrame(58,0);
				if(i + 2 <= n-1)
				{
					//找到数据头
					if(buffers[i] == 0xbd && buffers[i+1] == 0xdb && buffers[i+2] == 0x0b)
					{
						//缓存后面一个不完整的数据帧，直接抛弃
						if( i + 57 > n - 1){
							break;
						}
						//如果是一个完整的数据帧
						size_t j = 0;
						while(j < 58)
						{
							OneFrame[j++] = buffers[i++];
						}
						//数据校验
						if(BCCCheckOut(OneFrame) == OneFrame[57])
						{
						data_frame_buff.push_back(OneFrame);
						//ROS_INFO("This packet pass the BCC check out!");
						passed_count++;
						}
						else
						{
							ROS_WARN("This packet loss!");
							unpassed_count++;
							continue;
						}
						

						ROS_INFO("The num of passed data frame is:%d",passed_count);
						ROS_INFO("The num of unpassed data frame is:%d",unpassed_count);
						/*
						std::cout <<"the data of OneFrame is:"<<std::endl;
						for(int k=0;k<OneFrame.size();k++)
						{
						std::cout<< std::hex << (OneFrame[k]&0xff) << " ";}
						std::cout << std::endl;
						*/
						
					}
				}
				i++;
				
			}
					

			memset(buffers,0,n); //buffers数组清零

			std::cout << "data_frame size:" << std::dec << data_frame_buff.size()<<std::endl;
			
			//*3.从队列头中取一个数据帧进行解析
			if(data_frame_buff.empty())
			{
				ROS_ERROR("the gnss data frame buff is empty!");
				continue;
			}
			else
			{
				double roll,pitch,yaw,
					angular_velocity_x,angular_velocity_y,angular_velocity_z,
					linear_acceleration_x,linear_acceleration_y,linear_acceleration_z,
					Latitude,Longitude,Altitude,
					Vn,Ve,Vg,
					status,time;
				
				processDataFrame(data_frame_buff.back(),roll,pitch,yaw,
								angular_velocity_x,angular_velocity_y,angular_velocity_z,
								linear_acceleration_x,linear_acceleration_y,linear_acceleration_z,
								Latitude,Longitude,Altitude,
								Vn,Ve,Vg,
								status,time);

				printData(roll,pitch,yaw,
						angular_velocity_x,angular_velocity_y,angular_velocity_z,
						linear_acceleration_x,linear_acceleration_y,linear_acceleration_z,
						Latitude,Longitude,Altitude,
						Vn,Ve,Vg,
						status,time);
				
				data_frame_buff.pop_front();

				//*4.将解析的信息发布给ros
				current_time = ros::Time::now();
				
				sensor_msgs::NavSatFix gps_raw_data;
				gps_raw_data.header.stamp    = current_time;
				gps_raw_data.header.frame_id = "gps_link";
				gps_raw_data.status.status = 1;  // 1 with satellite-based augmentation
				gps_raw_data.status.service = 4;
				gps_raw_data.latitude  = Latitude;  //纬度
				gps_raw_data.longitude = Longitude; //经度
				gps_raw_data.altitude  = Altitude;  //高程

				gps_raw_pub.publish(gps_raw_data);

				//发布IMU
				sensor_msgs::Imu imu_raw_data;
				imu_raw_data.header.stamp    = current_time;
				imu_raw_data.header.frame_id = "imu_link";
				//角速度  deg/s
				imu_raw_data.angular_velocity.x = angular_velocity_x;
				imu_raw_data.angular_velocity.y = angular_velocity_y;
				imu_raw_data.angular_velocity.z = angular_velocity_z;				
				//线加速度 m/s^2
				imu_raw_data.linear_acceleration.x = linear_acceleration_x;
				imu_raw_data.linear_acceleration.y = linear_acceleration_y;
				imu_raw_data.linear_acceleration.z = linear_acceleration_z;
				//四元数位姿
				geometry_msgs::Quaternion catch_orientation;	
				catch_orientation = tf::createQuaternionMsgFromRollPitchYaw(roll, pitch, yaw);
				imu_raw_data.orientation.x = catch_orientation.x;
				imu_raw_data.orientation.y = catch_orientation.y;
				imu_raw_data.orientation.z = catch_orientation.z;
				imu_raw_data.orientation.w = catch_orientation.w;

				imu_raw_pub.publish(imu_raw_data);

				//发布组合导航数据
				vd_msgs::GpsInfo gps_raw_info;
				gps_raw_info.header.stamp    = current_time;
				gps_raw_info.header.frame_id = "gps_link";
				gps_raw_info.status  = status;
				gps_raw_info.Vx      = Vn;
				gps_raw_info.Vy      = Ve;
				gps_raw_info.Vz      = -Vg;
				gps_raw_info.Vv      = std::sqrt(Vn*Vn+Ve*Ve+Vg*Vg); //合速度
				gps_raw_info.Heading = yaw; //->X
				gps_raw_info.Pitch   = pitch;   //->y
				gps_raw_info.Roll    = roll;    //->Z
				
				gps_info_raw_pub.publish(gps_raw_info);
			}
			

		}

		rate.sleep();
        ros::spinOnce();
	}
    }
	
    return 0;

}


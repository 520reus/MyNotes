#include <queue>
#include <string>
#include <thread>
#include <math.h>
#include <stdio.h>
#include <errno.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ros/ros.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sensor_msgs/Imu.h>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include <sensor_msgs/NavSatFix.h>
#include "tf/transform_broadcaster.h"

#define M_PI 3.14159265358979323846
#include <vdcar/control.h>
#include <vdcar/gps_status.h>
#include <vdcar/AckermannDriveStamped.h>
#include <vdcar/AckermannDrive.h>

#define SERVPORT       8887
#define MAX_BUFF_SIZE  2048
#define SERVER_IP      "192.168.30.50"
#define BACKLOG        10

int acceptfd = 0;
int listenfd = 0;

double  D   = 241.3/1000;
double  lLeftRtight = 602.8/1000;
double  lFrontRear  = 981.215/1000;
double  lturnFR     = 427.0/1000;

int flag_cmd = 0;
int flag_Ack = 0;
int flag_obstacle =0;

//用于控制
//源于路径规划
double  VelFirst = 0;
int16_t VelLast  = 0;
double  AngleFirst1 = 0;//弧度
double  AngleFirst2 = 0;//弧度
int16_t AngleLast   = 0;//角度

using namespace std;

void RecePro(std::string s, 
	         double& Ve,                    double& Vn,                    double& Vu,                    double& VBaseline, 
		     double& status)
{
	//分割有效数据，存入vector中
	std::vector<std::string> v;
	std::string::size_type pos1, pos2;
	pos2 = s.find(",");
	pos1 = 0;

	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
		pos2 = s.find(",", pos1);
	}

	if (pos1 != s.length())
	{
		v.push_back(s.substr(pos1));
	}

	if (v.size() >= 24)
	{
		//状态status
		if (v[21] != "") 
		{
			status = std::atof(v[21].c_str());
		}
	}
	else
	{ 
	    std::cout << "information is not whole and error"<< std::endl;
	}
}

void mySigintHandler(int sig)
{
	ROS_INFO("shutting down!");
	close(acceptfd);
	close(listenfd);
	ros::shutdown();
}

int send_cmd(int fd, int16_t rpm, int16_t angle)
{
	char buff[10] = { 0 };

	buff[0] = 0x05;
	buff[1] = 0x00;
	buff[2] = 0x00;
	buff[3] = 0x00;
	buff[4] = 0x21;

	buff[5] = (rpm >> 8) & 0xFF;
	buff[6] = rpm & 0xFF;

	buff[7] = (angle >> 8) & 0xFF;
	buff[8] = angle & 0xFF;

	//buff[5] = 0x00;
	//buff[6] = 0x10;
	//buff[7] = 0x00;
	//buff[8] = 0x10;

	buff[9] = 0x02;

	// ROS_INFO("%02X %02X %02X %02X %02X %02X %02X %02X %02X", buff[0], buff[1], buff[2], buff[3], buff[4], buff[5], buff[6], buff[7], buff[8]);

	int nRet = send(fd, buff, 10, 0);
	//printf("send_cmd : start %i\n" , rpm);
	return nRet;
}

void cmd_chatterCallback(const geometry_msgs::Twist msg)
{
	VelFirst     = msg.linear.x;                     //m/s
	AngleFirst1  = -msg.angular.z;//move_base angle  //弧度

	if(VelFirst<0)
	{	
		AngleFirst1  = msg.angular.z;
	}
	flag_cmd = 1;

	// ROS_INFO("[cmd_vel] VEL: %f ANGLE: %f", VelFirst, AngleFirst1);
}

void ark_chatterCallback(const  vdcar::AckermannDriveStamped msg)
{
	VelFirst     = msg.drive.speed;                     	    //m/s
	AngleFirst1  = -msg.drive.steering_angle;//move_base angle  //弧度
	flag_Ack = 1;
}

void control_chatterCallback(const  vdcar::control msg)
{
	if(msg.obstacle)
	{
		flag_obstacle = 1;
	}
	if(!msg.obstacle){
		flag_obstacle = 0;
	}
}

int main(int argc, char** argv)
{	
	ros::init(argc, argv, "CAN_TCP_server");
	ros::NodeHandle actsnh;
	
	ros::Subscriber control_sub_ = actsnh.subscribe("/control", 100, control_chatterCallback);
	ros::Subscriber cmd_vel_sub_ = actsnh.subscribe("/cmd_vel", 100, cmd_chatterCallback);// getW  velH   topic cmd_vel 
	ros::Subscriber ark_vel_sub_ = actsnh.subscribe("/navigation", 100, ark_chatterCallback);
	//ros::Publisher own_vel_pub_wheel = actsnh.advertise<geometry_msgs::Twist>("/wheel/data", 100);      
	ros::Rate loop_rate(100);
	
	std::string strRece;
	std::queue<char> receive_buffer;

	ros::Time current_time, last_time;
	current_time = ros::Time::now();
	last_time    = ros::Time::now();

	signal(SIGINT, mySigintHandler);

	//1.创建TCP socket
	if ((listenfd = socket( AF_INET, SOCK_STREAM, 0 )) == -1)
	{
		perror("create socket failed!\n");
		exit(1);
    }
	//2.绑定端口
	int on = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));	// 配置地址重用，防止软件重启后绑定端口失败

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERVPORT);
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);         //inet_addr(SERVER_IP); 
	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) 
	{
		perror("绑定端口 失败");
		exit(1);
	}
	//3.监听端口，监听进程数
	if (listen(listenfd, BACKLOG) == -1) 
	{
		perror("监听端口 失败");
		exit(1);
	}

	while (actsnh.ok())
	{	
		//4.等待客户端的连接accept()，返回用于交互的socket描述符
		struct sockaddr_in  clientaddr;
		socklen_t len = sizeof(clientaddr);
		if ( (acceptfd = accept(listenfd, (struct sockaddr*)&clientaddr, &len) ) == -1)
		{
			cout << "accept 错误\n" << endl;
			exit(-1);
		}
		printf("IP:%s port:%d 连接到服务器. \r\n",inet_ntoa(clientaddr.sin_addr),ntohs(clientaddr.sin_port));
		
		while (actsnh.ok())
		{
			//1.读取串口信息：
			char buff[MAX_BUFF_SIZE] = {0};
			int  nRet = read(acceptfd, buff, sizeof(buff));

			if (nRet == 0)	// 客户端是否断开连接
			{
				close(acceptfd);
				printf("客户端断开连接");
				break;
			}
			else if (nRet == -1)
			{
				if (errno == EAGAIN || errno == EWOULDBLOCK)
				{
					printf("Resource temporarily unavailable\n");
				}
				close(acceptfd);
				printf("客户端连接错误");
				break;
			}
			ros::Duration(0.001).sleep();

			//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//用于控制  右转为正
			if( flag_cmd == 1 )
			{
				if(VelFirst > 0.01 || VelFirst < -0.01)
				{
					// double agtest = AngleFirst1*(lFrontRear)/VelFirst;
					// AngleFirst2 = atan(agtest)*180/M_PI;
					// AngleLast = (int16_t)(AngleFirst2*100);
					AngleLast = (int16_t)(AngleFirst1 * 40);
				}
				else
				{
					AngleLast = 0;
				}
			}
			else if ( flag_Ack == 1 )
			{
				//获得前轮转角  //弧度转角度
				AngleFirst2 = AngleFirst1;
				AngleLast = (int16_t)(AngleFirst2 * 40);
			}
			
			//获得转速n  //获得后轮转速
			// VelLast = VelFirst * 60 / (M_PI * D) / 5;
			//VelLast = VelFirst * 60;
			VelLast = VelFirst * 100;
			
			//VelLast = 10;
			//AngleLast = 10;
			if(AngleLast >= 30)
			{
				AngleLast = 30;
			}
			else if (AngleLast <= -30)
			{
				AngleLast = -30;
			}
else{
AngleLast = AngleLast;
}
			
			//if(flag_obstacle == 1){
			//	AngleLast = 0;
			//	VelLast = 0;
			//}
			//printf("get_v : start %i\n" , AngleLast);
			printf("Angle: %d\tVel: %d\n" , AngleLast, VelLast);
			nRet = send_cmd(acceptfd, VelLast, AngleLast);
			//printf("send_cmd : end %i\n" , AngleLast);
			flag_cmd = 0;
			flag_Ack = 0;

			if (nRet == -1)
			{
				perror("数据发送 失败 \r\n");
				exit(1);
			}
			memset(buff, 0, MAX_BUFF_SIZE);
			ros::spinOnce();
			loop_rate.sleep();
		}
	}
	close(acceptfd);
	close(listenfd);
	return 0;
} 

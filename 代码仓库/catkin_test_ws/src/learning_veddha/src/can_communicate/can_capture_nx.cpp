#include <math.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <net/if.h>
#include <signal.h>
#include <libgen.h>
#include <getopt.h>

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "ros/ros.h"
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

#include "lib.h"
#include "terminal.h"
#include "crss_vdcar/CanCtrlInfo.h"
#include <can_msgs/Frame.h>

#include <linux/can.h>
#include <linux/can/raw.h>

double  D           = 241.3/1000;
double  lLeftRtight = 602.8/1000;
double  lFrontRear  = 981.215/1000;
double  lturnFR     = 427.0/1000;

int16_t velLeft   =  0;
int16_t velRight  =  0;
uint8_t sfRate[8] =  {0};

double  ownVEL1   = 0;
double  ownVEL2   = 0;
double  ownVEL3   = 0;
int16_t ownANGLE  = 0;
double  ownangle  = 0;
double  ownW      = 0;
double  ownW2     = 0;

#define N  30
double  buff[N]   = {0};
int     i         = 0;
double  sum       = 0;
int flag_ackman   = 1;
int k_left        = 1;
int k_right       = 1;

using namespace std;

void mySigintHandler(int sig)
{
	ROS_INFO("shutting down!");
	ros::shutdown();
}

void chatterCallback(const can_msgs::Frame msg)
{
	if(msg.id == 0x31)
	{
		for(int i=0;i<8;i++)
		{
			sfRate[i] = msg.data[i];       
		} 
        /* right -> positive */	
		velLeft = ((sfRate[0] << 8) & 0xff00) | sfRate[1];
		velRight= ((sfRate[2] << 8) & 0xff00) | sfRate[3];
		ownVEL1 = (double)(velLeft+velRight)/2;   
		ownVEL2 = (ownVEL1*M_PI*D)/60.0;        
		//printf("ownvel %f\n",ownVEL2);

	    /* right -> positive for chassis */
		ownANGLE = ( ( (sfRate[4] << 8) & 0xffff ) | sfRate[5]  ) ;
		ownangle = (double)ownANGLE/100;           
		double test = ownangle*M_PI/180.0;          
		//printf("ownANGLE %i\n",ownANGLE);
		//printf("ownangle %f\n",ownangle);

	    /* left -> positive for chassis */
		if (flag_ackman == 1)
		{
			if(test<0)
			{
				double r = lFrontRear/tan(-test);
				double R = r - lLeftRtight/2;
				ownW = ownVEL2/R;           
			};
			if(test>0)
			{
				double r = lFrontRear/tan(test);
				double R = r + lLeftRtight/2;
				ownW = -ownVEL2/R;              
			};
		}
		else
		{
			if(test>0)
			{
				test = test*k_left;
			};
			if(test<0)
			{
				test = test*k_right;
			}; 
			ownW = tan(test)*ownVEL2/lFrontRear;
		}
		ownW2 = (velRight-velLeft)/lLeftRtight;
	}
}

double filter(double vel_get)
{
	sum = 0;
	buff[i++] = vel_get;
	if(i==N) i=0;
	for(int count=0;count<N;count++)
	sum += buff[count];
	return sum/N; 
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "can_cauptre_nx");
	ros::NodeHandle nh;
	ros::Subscriber message_sub_ = nh.subscribe("/received_messages", 100, chatterCallback);
	ros::Publisher  wheel_pub_   = nh.advertise<geometry_msgs::Twist>("/wheel/data", 100);
	ros::Rate loop_rate(100);

	signal(SIGINT, mySigintHandler);

	while(nh.ok())
	{
		ros::spinOnce();
        /* left -> positive */
		geometry_msgs::Twist  velpubwheel;
		ownVEL3 = filter(ownVEL2);             
		velpubwheel.linear.x  = ownVEL2;       
		velpubwheel.linear.y  = ownVEL3; /* filter */       
		velpubwheel.angular.z = ownW;          
		velpubwheel.angular.y = ownW2;   /* differ */         
		wheel_pub_.publish(velpubwheel);
		
		ros::spinOnce();
		loop_rate.sleep();
	}
	ros::spin();	
	return 0;
}

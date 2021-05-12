#include <math.h>
#include <sstream>
#include "ros/ros.h"
#include "tf/transform_broadcaster.h"

#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"

#define M_PI 3.14159265358979323846
#include <crss_vdcar/ChassisCtrl.h>
#include "crss_vdcar/CanCtrlInfo.h"
#include "crss_vdcar/AckermannDrive.h"
#include "crss_vdcar/AckermannDriveStamped.h"

double  D   = 241.3/1000;
double  lLeftRtight = 602.8/1000;
double  lFrontRear  = 981.215/1000;
double  lturnFR     = 427.0/1000;
/* data from odom */
int16_t velLeft  =  0;
int16_t velRight =  0;
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
int flag_cmd      = 0;
int flag_obstacle = 0;
int k_left        = 1;
int k_right       = 1;
/* ctrl from planner */
double  VelFirst = 0;
int16_t VelLast  = 0;
double  AngleFirst1 = 0;
double  AngleFirst2 = 0;
int16_t AngleLast   = 0;

using namespace std;

bool act(crss_vdcar::CanCtrlInfo::Request  &req, crss_vdcar::CanCtrlInfo::Response &res)
{
	for(int i=0;i<8;i++)
	{
		res.sfRate[i] = req.cRate[i];
		res.sControl[i] = 0;
	}
	/* right -> positive */
	velLeft = ((res.sfRate[0] << 8) & 0xff00) | res.sfRate[1];
	velRight = ((res.sfRate[2] << 8) & 0xff00) | res.sfRate[3];
	ownVEL1 = (double)(velLeft+velRight)/2;          //RPM
	ownVEL2 = (ownVEL1*M_PI*D)/60.0;                 //m/s
    /* left -> positive */
	ownANGLE = (((res.sfRate[4] << 8) & 0xff00) | res.sfRate[5]);
	ownangle = (double)ownANGLE/100;                 //degree
	double test = ownangle*M_PI/180.0;               //rad

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

    /* differ left -> positive */
	ownW2 = (velRight-velLeft)/lLeftRtight;

	/* chassis ctrl -> positive */
	if(flag_cmd == 1)
	{
		if(VelFirst > 0.01 || VelFirst < -0.01)
		{
			double agtest = AngleFirst1*(lLeftRtight)/VelFirst;
			AngleFirst2 = atan(agtest)*180/M_PI;
			AngleLast = (int16_t)(AngleFirst2*100);
		}
		else
		{
			AngleLast = 0;
		};
	}
	else
	{
	        /* rad 2 degree */
		AngleFirst2 = AngleFirst1*180.0/M_PI;              
		AngleLast = (int16_t)(AngleFirst2*100);

	}
	/* m/s 2 rpm */
	VelLast = VelFirst*60/(M_PI*D); 

	if(flag_obstacle == 1){
		AngleLast = 0;
		VelLast = 0;
	}
	/* return service */
	res.sControl[0]= ((VelLast >> 8) & 0xff );	 
	res.sControl[1]= (VelLast & 0xff); 
	res.sControl[2]= ((AngleLast >> 8) & 0xff );
	res.sControl[3]= (AngleLast & 0xff);

	return true;
}

void chatterCallback(const geometry_msgs::Twist msg)
{
	VelFirst     = msg.linear.x;                     
	AngleFirst1  = -msg.angular.z;//move_base angle  
}

void control_chatterCallback(const crss_vdcar::ChassisCtrl msg)
{
	if(msg.obstacle){
		flag_obstacle = 1;
	}
	if(!msg.obstacle){
		flag_obstacle = 0;
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
	ros::init(argc, argv, "can_capture_emcb_server");
	ros::NodeHandle actsnh;
	ros::ServiceServer service      = actsnh.advertiseService("emcb_info", act);   
	ros::Subscriber    control_sub_ = actsnh.subscribe("/chassis_ctrl", 100, control_chatterCallback);  
	ros::Subscriber    cmd_vel_sub_ = actsnh.subscribe("/cmd_vel", 100, chatterCallback);// getW  velH  topic cmd_vel 
	ros::Publisher     wheel_pub_   = actsnh.advertise<geometry_msgs::Twist>("/wheel/data", 100);      
	ros::Rate loop_rate(100);

	while(actsnh.ok())
	{
		ros::spinOnce();
		geometry_msgs::Twist  velpubwheel;
		/* left -> positive */
		ownVEL3 = filter(ownVEL2);             //m/s
		velpubwheel.linear.x  = ownVEL2;       //m/s
		velpubwheel.linear.y  = ownVEL3;       /* filter */		
		velpubwheel.angular.z = ownW;          //rad/s
		velpubwheel.angular.y = ownW2;         /* differ */
		
		wheel_pub_.publish(velpubwheel);
		
		ros::spinOnce();
		loop_rate.sleep();
	}
	ROS_INFO("Ready to act motor raid.");
	ros::spin();   
	return 0;
}

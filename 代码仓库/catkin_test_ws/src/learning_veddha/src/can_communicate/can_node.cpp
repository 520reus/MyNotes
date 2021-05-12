#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <math.h>
#include <vector>

#include "controlcan.h"

#include "crss_vdcar/ChassisCtrl.h"
#include "crss_vdcar/CanCtrlInfo.h"
#include "crss_vdcar/AckermannDrive.h"
#include "crss_vdcar/AckermannDriveStamped.h"

#define msleep(ms)  usleep((ms)*1000)
#define min(a,b)  (((a) < (b)) ? (a) : (b))

#define MAX_CHANNELS        2   // can bus channel number
#define CHANNEL_CHASSIS     0   // can channel for chassis control
#define CHANNEL_USER        1   // can channel for user's function

#define RX_WAIT_TIME        100     // max waitting time when wait for receiving can packet
#define RX_BUFF_SIZE        100     // max can packet receiving buffer

const double  D           = 241.300 / 1000; // wheel diameter, unit m
const double  lLeftRtight = 602.800 / 1000; // wheel base left to right, unit m
const double  lFrontRear  = 981.215 / 1000; // wheel base front to rear, unit m
const double  lturnFR     = 427.000 / 1000; // turning radius, unit m

/* data from odom */
double  g_vel_cs_ms     = 0;    // current chassis vel m/s
double  g_vel_cs_ms_flt = 0;    // filtered chassis vel m/s
double  g_ang_vel       = 0;    // current chassis angular velocity
double  g_ang_vel_diff  = 0;    // current diff-wheel chassis angular velocity

const unsigned int FRAME_ID_PRECEP  = 0x31; // chassis status
const unsigned int FRAME_ID_CTRL    = 0x21; // chassis control

static int g_flag_diff = 0;
static int g_flag_ack  = 1;
static int g_flag_obstacle = 0;

#define VEL_FILT_BUFF_NUM  30
const int k_left  = 1;
const int k_right = 1;

/* ctrl from planner */
static double  g_vel_planner = 0;
static double  g_ang_planner = 0;

typedef struct {
    unsigned channel;   // channel index, [0,1]
    unsigned stop;      // stop RX-thread
    unsigned total;     // total received
    unsigned error;     // error(s) detected
} RX_CTX;

const unsigned g_dev_type = 4;
const unsigned g_dev_idx  = 0;
const unsigned g_ch_mask  = 3;
const unsigned g_baud     = 0x1400;
const unsigned g_tx_type  = 3;

RX_CTX      g_rx_ctx[MAX_CHANNELS];
pthread_t   g_rx_threads[MAX_CHANNELS];
unsigned    g_tx_frame_id[MAX_CHANNELS] = { 0 };

void generate_frame(VCI_CAN_OBJ *can, unsigned int frame_id, char* data, BYTE len)
{
    memset(can, 0, sizeof(VCI_CAN_OBJ));
    can->SendType = g_tx_type;
    can->DataLen = len;

    for (unsigned int i = 0; i < can->DataLen; i++) {
        can->Data[i] = data[i] & 0xff;
    }
    
    can->ID = frame_id;
    can->ExternFlag = 0;
    can->RemoteFlag = 0;
    
    if (!can->ExternFlag) {
        return;
    }
    can->ID |= can->ID << 11; // id: bit11~bit21 == bit0~bit10
    can->ID |= can->ID << 11; // id: bit22~bit28 == bit0~bit7
}

int  can_transmit(int channel, unsigned int frame_id, char* data, BYTE len)
{
    VCI_CAN_OBJ *buff = (VCI_CAN_OBJ *)malloc(sizeof(VCI_CAN_OBJ));

    if ((g_ch_mask & (1 << channel)) == 0) {
        ROS_WARN("chanel %d is masked", channel);
        free(buff);
        return -1;
    }

    if (!len==0) {
        generate_frame(buff, frame_id, data, len);
    }

    char msg[1024];
    char tmp[56];
    sprintf(msg, "[TX] CAN%d ID: 0x%04X ", channel, buff->ID);
    for (int n = 0; n < buff->DataLen; ++n) {
        sprintf(tmp, "%02X ", buff->Data[n]);
        strcat(msg, tmp);
    }
    ROS_INFO("%s", msg);

    int cnt = VCI_Transmit(g_dev_type, g_dev_idx, channel, buff, 1);
    if (cnt != 1) {
        ROS_ERROR("CAN%d TX failed: ID=%08x CNT=%d", channel, buff->ID, cnt);
        free(buff);
        return -1;
    }

    // !!! record frame_id for receiving filter
    g_tx_frame_id[channel] = frame_id;

    free(buff);
    return 0;
}

void* rx_thread(void *data)
{
    RX_CTX *ctx = (RX_CTX *)data;
    VCI_CAN_OBJ can[RX_BUFF_SIZE]; // can message receiving buffer

    int16_t vel_rl   = 0;   // rear-left-wheel vel rpm   
    int16_t vel_rr   = 0;   // rear-right-wheel vel rpm
    double  vel_cs   = 0;   // chassis vel rpm
    int16_t steer_angle_deg_100 = 0;    // steering angle degree x 100
    double  steer_angle_deg = 0;        // steering angle degree
    double  steer_angle_rad = 0;        // steering angle rad
    double  r, R;

    while(!ctx->stop && !ctx->error)
    {
        int cnt = VCI_Receive(g_dev_type, g_dev_idx, ctx->channel, can, RX_BUFF_SIZE, RX_WAIT_TIME);
        if (cnt == 0) { // no data received
            continue;
        }

        for (int i = 0; i < cnt; i++) {
            // filter out frame sent by current channel
            if (can[i].ID == g_tx_frame_id[ctx->channel]) {
                continue;
            }

            #if 0   // out packet detail info for debuging
            char msg[1024];
            char tmp[56];
            sprintf(msg, "[RX] CAN%d ID: 0x%04X ", ctx->channel, can[i].ID);
            for (int n = 0; n < can[i].DataLen; ++n) {
                sprintf(tmp, "%02X ", can[i].Data[n]);
                strcat(msg, tmp);
            }
            ROS_INFO("%s", msg);
            #endif  // out packet detail info for debuging

            if (can[i].ID == FRAME_ID_PRECEP) {
                /* right -> positive */
                vel_rl  = ((can[i].Data[0] << 8) & 0xff00) | can[i].Data[1];   // rear-left-wheel vel rpm
                vel_rr  = ((can[i].Data[2] << 8) & 0xff00) | can[i].Data[3];   // rear-right-wheel vel rpm
                vel_cs  = (double)(vel_rl + vel_rr) / 2;            // chassis vel rpm
                g_vel_cs_ms  = (vel_cs * M_PI * D) / 60.0;          // chassis vel m/s
                /* right -> positive */
                steer_angle_deg_100 = (((can[i].Data[4] << 8) & 0xff00) | can[i].Data[5]);  // steering angle degree x 100
                steer_angle_deg = (double)steer_angle_deg_100 / 100;    // steering angle degree
                steer_angle_rad = steer_angle_deg * M_PI / 180.0;       // steering angle rad
                
                if (g_flag_ack == 1) {  // ackermann model
                    if (steer_angle_rad < 0) {
                        r = lFrontRear / tan(-steer_angle_rad);
                        R = r - lLeftRtight / 2;
                        g_ang_vel = g_vel_cs_ms / R;
                    }
                    if (steer_angle_rad > 0) {
                        r = lFrontRear / tan(steer_angle_rad);
                        R = r + lLeftRtight / 2;
                        g_ang_vel = -g_vel_cs_ms / R;
                    }
                }
                else {
                    if (steer_angle_rad > 0) {
                        steer_angle_rad = steer_angle_rad * k_left;
                    }
                    if (steer_angle_rad < 0) {
                        steer_angle_rad = steer_angle_rad * k_right;
                    }
                    g_ang_vel = tan(steer_angle_rad) * g_vel_cs_ms / lFrontRear;
                }
            }

            /* differ right -> positive */
            g_ang_vel_diff = (vel_rr - vel_rl) / lLeftRtight;
        }
 
        // statistic information
        ctx->total += cnt;
    }

    printf("CAN%d RX thread terminated, %d frames received & verified: %s\n",
           ctx->channel, ctx->total, ctx->error ? "error(s) detected" : "no error");

    pthread_exit(0);
}

int  init_device()
{
    int res = 0;

    ROS_INFO("CAN Param: DevType=%d, DevIdx=%d, ChMask=0x%x, Baud=0x%04x, TxType=%d", 
             g_dev_type, g_dev_idx, g_ch_mask, g_baud, g_tx_type);
    
    if (!VCI_OpenDevice(g_dev_type, g_dev_idx, 0)) {
        ROS_ERROR("VCI_OpenDevice failed");
        return -1;
    }
    ROS_INFO("VCI_OpenDevice succeeded");

    VCI_INIT_CONFIG config;
    config.AccCode = 0;
    config.AccMask = 0xffffffff;
    config.Filter  = 1;
    config.Mode    = 0;
    config.Timing0 = g_baud & 0xff;
    config.Timing1 = g_baud >> 8;

    int i, j;
    for (i = 0; i < MAX_CHANNELS; i++)
    {
        if ((g_ch_mask & (1 << i)) == 0) {
            continue;
        }
        if (!VCI_InitCAN(g_dev_type, g_dev_idx, i, &config))
        {
            ROS_ERROR("VCI_InitCAN(%d) failed", i);
            return -1;
        }
        ROS_INFO("VCI_InitCAN(%d) succeeded", i);

        if (!VCI_StartCAN(g_dev_type, g_dev_idx, i))
        {
            ROS_ERROR("VCI_StartCAN(%d) failed", i);
            return -1;
        }
        ROS_INFO("VCI_StartCAN(%d) succeeded", i);
    }

    for (i = 0; i < MAX_CHANNELS; i++)
    {
        if ((g_ch_mask & (1 << i)) == 0) continue;

        g_rx_ctx[i].channel = i;
        g_rx_ctx[i].stop = 0;
        g_rx_ctx[i].total = 0;
        g_rx_ctx[i].error = 0;

        res = pthread_create(&g_rx_threads[i], NULL, rx_thread, &g_rx_ctx[i]);
        if (res != 0) {
            ROS_ERROR("failed to create rx thread [%d] with error code %d", i, res);
            return -1;
        }
    }

    return 0;
}

int  uninit_device()
{
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if ((g_ch_mask & (1 << i)) == 0) continue;

        g_rx_ctx[i].stop = 1;
        pthread_join(g_rx_threads[i], NULL);
        ROS_ERROR("rx thread[%d] exited", i);
        if (g_rx_ctx[i].error) {
            ROS_ERROR("can error occure when receiving packets");
        }
    }

    VCI_CloseDevice(g_dev_type, g_dev_idx);
    printf("VCI_CloseDevice\n");
    return 0;
}

void sigint_handler(int sig)
{
    ROS_INFO("shutting down!");
    uninit_device();
    ros::shutdown();
}

void diff_ctrl_callback(const geometry_msgs::Twist msg)
{
	g_vel_planner   =  msg.linear.x;                    //m/s
	g_ang_planner   = -msg.angular.z;//move_base angle  //rad
	g_flag_diff = 1;

	ROS_INFO("[cmd_vel] VEL: %f ANGLE: %f", g_vel_planner, g_ang_planner);
}

void ack_ctrl_callback(const geometry_msgs::Twist msg)
{
	g_vel_planner   =  msg.linear.x;                    //m/s
	g_ang_planner   = -msg.angular.z;//move_base angle  //rad
	g_flag_ack = 1;

	ROS_INFO("[cmd_vel] VEL: %f ANGLE: %f", g_vel_planner, g_ang_planner);
}
// void ack_ctrl_callback(const  crss_vdcar::AckermannDriveStamped msg)
// {
// 	g_vel_planner   =  msg.drive.speed;                             //m/s
// 	g_ang_planner   =  msg.drive.steering_angle;//move_base angle   //rad
// 	g_flag_ack = 1;
// }

void chassis_ctrl_callback(const  crss_vdcar::ChassisCtrl msg)
{
    if (msg.obstacle) {
        g_flag_obstacle = 1;
    }
    else {
        g_flag_obstacle = 0;
    }
}

void assemble_msg_data(int16_t rpm, int16_t angle, char* msg)
{
    msg[0] = (rpm >> 8) & 0xFF;
    msg[1] = (rpm) & 0xFF;
    msg[2] = (angle >> 8) & 0xFF;
    msg[3] = (angle) & 0xFF;

    msg[4] = 0;
    msg[5] = 0;
    msg[6] = 0;
    msg[7] = 0;
}

double filter_velocity(double cur_vel)
{
    double sum = 0;
    static std::vector<double> vec_vel;

    vec_vel.push_back(cur_vel);
    while (vec_vel.size() > VEL_FILT_BUFF_NUM) {
        vec_vel.erase(std::begin(vec_vel));
    }

    for (int i = 0; i < vec_vel.size(); ++i) {
        sum += vec_vel[i];
    }

	return sum / vec_vel.size();
}

void publish_chassis_info(ros::Publisher &pub)
{
    geometry_msgs::Twist velpubwheel;
    /* left -> positive */
    g_vel_cs_ms_flt = filter_velocity(g_vel_cs_ms);      // get filtered chassis velocity

    velpubwheel.linear.x  = g_vel_cs_ms;        // current chassis velocity m/s
    velpubwheel.linear.y  = g_vel_cs_ms_flt;    // filtered chassis velocity m/s
    /* left -> positive */
    velpubwheel.angular.z = -g_ang_vel;          // current chassis angular velocity m/s
    //velpubwheel.angular.y = -g_ang_vel_diff;     // diff-wheel chassis angular velocity m/s
    
    pub.publish(velpubwheel);

    g_vel_cs_ms = 0;
    g_vel_cs_ms_flt = 0;
    g_ang_vel = 0;
    g_ang_vel_diff = 0;
}

int16_t update_steering_angle()
{
    double angle = 0;

    /* right -> positive */
    if ( g_flag_diff == 1 ) {
        angle = g_ang_planner * 180.0 / M_PI;
    }
    else if ( g_flag_ack == 1 ) {
        angle = g_ang_planner * 180.0 / M_PI / 5;
    }
    angle = angle * 100;

    if (angle >= 3000) {
        angle = 3000;
    }
    else if (angle <= -3000) {
        angle = -3000;
    }

    return round(angle);
}

int16_t update_velocity()
{
    double vel = g_vel_planner * 60 / (M_PI * D);   /* m/s 2 rpm */

    return round(vel);
}

void update_obstacle(int16_t &vel, int16_t &angle)
{
    /* detected obstacle */
    if (g_flag_obstacle == 1) {
        vel = 0;
        angle = 0;
    }
}

void reset_flag()
{
    g_flag_diff = 0;
    g_flag_ack  = 0;
    g_flag_obstacle = 0;
}

int main(int argc, char** argv)
{
    int res = 0;

    ros::init(argc, argv, "can_node");
    ROS_INFO("can node start...");

    ros::NodeHandle nh;
    ros::Subscriber control_sub = nh.subscribe("/chassis_ctrl", 100,   chassis_ctrl_callback);  // obstacle detection
    ros::Subscriber cmd_vel_sub = nh.subscribe("/cmd_vel", 100,        diff_ctrl_callback);     // diff-wheel model planner
    ros::Subscriber ack_vel_sub = nh.subscribe("/navigation", 100,     ack_ctrl_callback);      // ackermann model planner
    ros::Publisher  wheel_pub   = nh.advertise<geometry_msgs::Twist>("/wheel/data", 100);       // chassis motion info publisher
    
    ros::Rate loop_rate(2);

    // register handler on exit 
	signal(SIGINT, sigint_handler);

    // init can device
    res = init_device();
    if (res) {
        ROS_INFO("failed to init can device...");
        uninit_device();
        return -1;
    }

    char msg[8] = { 0 };    // can message buffer
    int16_t chassis_vel     = 0;
    int16_t chassis_angle   = 0;

    while (ros::ok()) {
        ros::spinOnce();

        // publish chassis information
        publish_chassis_info(wheel_pub);

        // update steering angle
        chassis_angle = update_steering_angle();
        
        // update velocity
        chassis_vel = update_velocity();

        // update obstacle
        update_obstacle(chassis_vel, chassis_angle);

        // send chassis ctrl message by can
        assemble_msg_data(chassis_vel, chassis_angle, msg);
        res = can_transmit(CHANNEL_CHASSIS, FRAME_ID_CTRL, msg, sizeof(msg));
        if (res) {
            ROS_ERROR("failed to send can packet");
        }

        // reset flag
        reset_flag();
		loop_rate.sleep();
    }

    ROS_INFO("can node shutdown...");
    //uninit_device();
    ros::shutdown();

    return 0;
}

#include <math.h>
#include <iostream>
#include <ros/ros.h>

#include <sensor_msgs/Imu.h>
#include "std_msgs/String.h"
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "tf/transform_broadcaster.h"

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>

using std::cout;
using std::endl;


/* class OdomPub
{
    public:
        OdomPub() = default;
        virtual ~OdomPub() = default;
        virtual void Run();

    protected:    
        virtual void sensorDataCB() = 0;
        virtual void processData() = 0;
        virtual void publishOdom() = 0;
        
        nav_msgs::Odometry odom_;

};  */


class ImuOdom
{

public:
    ImuOdom(ros::NodeHandle& nh)
    {
        imu_sub_ = nh.subscribe<sensor_msgs::Imu>("/imu/data",100,boost::bind(&ImuOdom::imuCB,this,_1));
        odom_pub_ = nh.advertise<nav_msgs::Odometry>("/odom/by_imu",100);
    }

    void Run()
    {
            //处理上一帧与当前帧的IMU数据
            processImuData();
            //发布处理好的odom
            publishOdom();
    }

private:
    ros::Subscriber imu_sub_;
    ros::Publisher odom_pub_;

    sensor_msgs::Imu curr_imu_data_;
    sensor_msgs::Imu pre_imu_data_;
    nav_msgs::Odometry odom_;
    Eigen::Matrix4f initial_pose_ = Eigen::Matrix4f::Identity();

    Eigen::Vector3d g_w = Eigen::Vector3d(0,0,-9.81); // 重力加速度
    double dt_ =  0.01;//积分间隔dt


    Eigen::Vector3d P_ = Eigen::Vector3d::Zero();
    Eigen::Vector3d V_ = Eigen::Vector3d::Zero();
    Eigen::Quaterniond Qwb_ = Eigen::Quaterniond::Identity();
    


    void imuCB(const sensor_msgs::ImuConstPtr& msg_ptr)
    {
        curr_imu_data_ = *msg_ptr;
    }

    //处理上一帧与当前帧的IMU数据，采用中值积分法推算里程计信息
    bool processImuData()
    {
        static bool is_first_data = true;
        if(is_first_data)
        {
            pre_imu_data_ = curr_imu_data_;
            is_first_data = false;
        }

        double wx = (pre_imu_data_.angular_velocity.x + curr_imu_data_.angular_velocity.x)/2;
        double wy = (pre_imu_data_.angular_velocity.y + curr_imu_data_.angular_velocity.y)/2;
        double wz = (pre_imu_data_.angular_velocity.z + curr_imu_data_.angular_velocity.z)/2;
        Eigen::Vector3d ea(wx, wy, wz);
        Eigen::Quaterniond dq;
        Eigen::Vector3d half_theta = ea * 0.5 * dt_;
        dq.w() = 1.0;
        dq.x() = half_theta.x();
        dq.y() = half_theta.y();
        dq.z() = half_theta.z();
        dq.normalize(); // 归一化

        Eigen::Quaterniond Qwb_last = Qwb_; // q_k
        // 更新q
        Qwb_ *= dq; // q_k+1

        // 先算a_k+1
        Eigen::Vector3d last_acc(pre_imu_data_.linear_acceleration.x,
                                 pre_imu_data_.linear_acceleration.y,
                                 pre_imu_data_.linear_acceleration.z);
        
        Eigen::Vector3d curr_acc(curr_imu_data_.linear_acceleration.x,
                                 curr_imu_data_.linear_acceleration.y,
                                 curr_imu_data_.linear_acceleration.z);

        Eigen::Vector3d a_w = (Qwb_last * last_acc - g_w + Qwb_ * curr_acc - g_w) * 0.5 ;

        P_ = P_ +  V_ * dt_ + a_w * dt_ * dt_ * 0.5;

        V_ += a_w * dt_;

        pre_imu_data_ = curr_imu_data_;
    }


    void publishOdom()
    {
        odom_.header.stamp = curr_imu_data_.header.stamp;
        odom_.header.frame_id = "imu_odom";
        odom_.child_frame_id = "imu_link";

        odom_.pose.pose.position.x = P_.x();
        odom_.pose.pose.position.y = P_.y();
        odom_.pose.pose.position.z = P_.z();


        odom_.pose.pose.orientation.x = Qwb_.x();
        odom_.pose.pose.orientation.y = Qwb_.y();
        odom_.pose.pose.orientation.z = Qwb_.z();
        odom_.pose.pose.orientation.w = Qwb_.w();

        odom_.twist.twist.linear.x = V_.x();
        odom_.twist.twist.linear.y = V_.y();
        odom_.twist.twist.linear.z = V_.z();

        odom_.twist.twist.angular.x = curr_imu_data_.angular_velocity.x;
        odom_.twist.twist.angular.y = curr_imu_data_.angular_velocity.y;
        odom_.twist.twist.angular.z = curr_imu_data_.angular_velocity.z;

        odom_pub_.publish(odom_);
    }
};

int main(int argc,char** argv)
{
    ros::init(argc,argv,"odom_imu");
    ros::NodeHandle n;

    std::unique_ptr<ImuOdom> imu_odom  = std::make_unique<ImuOdom>(n);

    ros::Rate rate(100);
    while(ros::ok())
    {
        imu_odom->Run();
        ros::spinOnce();
        rate.sleep();
    }
    return 0;
}
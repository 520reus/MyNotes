#include <deque>
#include <mutex>

#include "GeographicLib/LocalCartesian.hpp"

#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_datatypes.h>

#include <Eigen/Core>

#include <crss_vdcar/GpsInfo.h>

class GeoTransformer
{
public:
  GeoTransformer(ros::NodeHandle& nh, 
                                  std::string gps_info_topic, 
                                  std::string gps_data_topic,
                                  std::string imu_data_topic);

  bool run();
  void setOriginPos(const double& lati, const double& longi, const double& alti);

private:
  // ros type member
  ros::NodeHandle nh_;
  ros::Subscriber gps_info_sub_;
  ros::Subscriber gps_data_sub_;
  ros::Subscriber imu_data_sub_;
  ros::Publisher odom_pub_;
  // lock
  std::mutex buff_mutex_;
  // tf::TransformBroadcaster gps_map_odom_tf_;
  // data_buff
  std::deque<crss_vdcar::GpsInfo> gps_info_buff_;
  std::deque<sensor_msgs::NavSatFix> gnss_data_buff_;
  std::deque<sensor_msgs::Imu> imu_data_buff_;
  // current data
  sensor_msgs::NavSatFix current_gnss_data_;
  crss_vdcar::GpsInfo current_gps_info_;
  nav_msgs::Odometry current_odom_;
  sensor_msgs::Imu current_imu_data_;
  // init info
  bool origin_position_inited_ = false;

  double origin_latitude_ = 0.0;
  double origin_longitude_ = 0.0;
  double origin_altitude_ = 0.0;

  GeographicLib::LocalCartesian geo_converter;

private:
  void updateOdom();
  void publishOdom();

  bool hasData();
  bool vaildData();
  bool initGNSS();
  void initOriginPosWithGNSS(const sensor_msgs::NavSatFix& gps_data);

  void gps_info_callback(const crss_vdcar::GpsInfoConstPtr& gps_info_msg);
  void gps_data_callback(const sensor_msgs::NavSatFixConstPtr& gnss_data_msg);
  void imu_data_callback(const sensor_msgs::ImuConstPtr & imu_data_msg);
};

GeoTransformer::GeoTransformer(ros::NodeHandle& nh, 
                                  std::string gps_info_topic, 
                                  std::string gps_data_topic,
                                  std::string imu_data_topic)
: nh_(nh)
{
  gps_info_sub_= nh_.subscribe(gps_info_topic, 100, &GeoTransformer::gps_info_callback, this);
  gps_data_sub_ = nh_.subscribe(gps_data_topic, 100, &GeoTransformer::gps_data_callback, this);
    imu_data_sub_ = nh.subscribe(imu_data_topic, 100, &GeoTransformer::imu_data_callback, this);

  odom_pub_ = nh.advertise<nav_msgs::Odometry>("/odom/by_gps", 100);
}

  void GeoTransformer::setOriginPos(const double& lati0, const double& longi0, const double& alti0)
{
    geo_converter.Reset(lati0, longi0, alti0);
    origin_latitude_ = lati0;
    origin_longitude_ = longi0;
    origin_altitude_ = alti0;
    std::cout << "GeoConverter inited." << std::endl;
    std::cout.precision(8);
    std::cout << origin_latitude_ << "\t" << origin_longitude_ << "\t" << origin_altitude_ << std::endl;
    origin_position_inited_ = true;
}
// 判断是否有数据
bool GeoTransformer::hasData()
{
  if(gnss_data_buff_.size() == 0)
    return false;
  if(gps_info_buff_.size() == 0)
    return false;
    if(imu_data_buff_.size() == 0)
        return false;

  return true;
}

// 判断数据的有效性
bool GeoTransformer::vaildData()
{
  current_gnss_data_ = gnss_data_buff_.front();
  current_gps_info_ = gps_info_buff_.front();
  current_imu_data_ = imu_data_buff_.front();

  double diff_info_time = current_gnss_data_.header.stamp.toSec() - current_gps_info_.header.stamp.toSec();
  double diff_imu_time = current_gnss_data_.header.stamp.toSec() - current_imu_data_.header.stamp.toSec();


  if(diff_info_time < -0.01 || diff_imu_time < -0.01)
  {
    gnss_data_buff_.pop_front();
    return false;
  }
  if(diff_info_time > 0.01)
  {
    gps_info_buff_.pop_front();
    return false;
  }
    if(diff_imu_time > 0.01)
    {
        imu_data_buff_.pop_front();
        return false;
    }

  buff_mutex_.lock();
  gnss_data_buff_.pop_front();
  gps_info_buff_.pop_front();
  imu_data_buff_.pop_front();
  buff_mutex_.unlock();

  return true;
}

void GeoTransformer::initOriginPosWithGNSS(const sensor_msgs::NavSatFix& gps_data)
{
  setOriginPos(gps_data.latitude, gps_data.longitude, gps_data.altitude);
}

bool GeoTransformer::initGNSS()
{
  if(!origin_position_inited_)
  {
    if(gnss_data_buff_.size() != 0)
    {
      current_gnss_data_ = gnss_data_buff_.front();
      initOriginPosWithGNSS(current_gnss_data_);
      ROS_INFO("Init GeoConverter with first GPS data.");
    }
  }
  return origin_position_inited_;
}

// gps_info_callback
void GeoTransformer::gps_info_callback(const crss_vdcar::GpsInfoConstPtr& gps_info_msg)
{
  crss_vdcar::GpsInfo new_data = *gps_info_msg;
  buff_mutex_.lock();
  gps_info_buff_.push_back(new_data);
  buff_mutex_.unlock();
}

// gps_data_callback
void GeoTransformer::gps_data_callback(const sensor_msgs::NavSatFixConstPtr& gnss_data_msg)
{
  sensor_msgs::NavSatFix new_data = *gnss_data_msg;
  buff_mutex_.lock();
  gnss_data_buff_.push_back(new_data);
  buff_mutex_.unlock();
}

  void GeoTransformer::imu_data_callback(const sensor_msgs::ImuConstPtr & imu_data_msg)
{
  sensor_msgs::Imu new_data = *imu_data_msg;
  buff_mutex_.lock();
  imu_data_buff_.push_back(new_data);
  buff_mutex_.unlock();
}

void GeoTransformer::updateOdom()
{
  // 将gps info 和 gps data融合成odom信息
  geometry_msgs::Point local_pos; // x,y,z分别对应东北天（ENU）
  geo_converter.Forward(
                current_gnss_data_.latitude, current_gnss_data_.longitude,current_gnss_data_.altitude, 
                local_pos.x, local_pos.y, local_pos.z
              );
  local_pos.z = 0.0;

  geometry_msgs::Quaternion odom_quat = 
          tf::createQuaternionMsgFromRollPitchYaw(
              current_gps_info_.Roll, current_gps_info_.Pitch, current_gps_info_.Heading
          );

  geometry_msgs::Twist local_twist;
  local_twist.linear.x = current_gps_info_.Vv;
  local_twist.linear.y = 0.0;
  local_twist.linear.z = 0.0;
  // 目前gps信息没有加入角速度
  local_twist.angular.x = current_imu_data_.angular_velocity.x;
  local_twist.angular.y = current_imu_data_.angular_velocity.y;
  local_twist.angular.z = current_imu_data_.angular_velocity.z;

  current_odom_.header.stamp = current_gnss_data_.header.stamp;
  current_odom_.header.frame_id = "gps_odom";
  current_odom_.child_frame_id = "gps_link";

  current_odom_.pose.pose.orientation = odom_quat;
  current_odom_.pose.pose.position = local_pos;
  current_odom_.twist.twist = local_twist;
}

void GeoTransformer::publishOdom()
{
  odom_pub_.publish(current_odom_);

  // tf::StampedTransform ttff;
  // ttff.stamp_ = current_odom_.header.stamp;
  // ttff.frame_id_ = "map";
  // ttff.child_frame_id_ = "gps_odom";
  // geometry_msgs::Quaternion q = current_odom_.pose.pose.orientation;
  // geometry_msgs::Point p = current_odom_.pose.pose.position;
  // ttff.setRotation(tf::Quaternion(q.x, q.y, q.z, q.w));
  // ttff.setOrigin(tf::Vector3(p.x, p.y, p.z));
  // gps_map_odom_tf_.sendTransform(ttff);
}

bool GeoTransformer::run()
{
  if(!initGNSS())
    return false;

  while(hasData())
  {

    if(!vaildData())
      continue;

    updateOdom();
    publishOdom();
  }

}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "odom_gps_new");
  ros::NodeHandle nh;
  
  std::string gps_info_topic = "/gps/info";
  std::string gps_data_topic = "/gps/data";
  std::string imu_data_topic = "/imu/data_own";

  std::unique_ptr<GeoTransformer> geo_transformer_ptr;
  geo_transformer_ptr = std::make_unique<GeoTransformer>(nh, gps_info_topic, gps_data_topic, imu_data_topic);

  double X0 = 30.76216941 ;
  double Y0 = 121.1487512;
  double Z0 = 14.53;
  geo_transformer_ptr->setOriginPos(X0, Y0, Z0);
  // TODO:需要加一个service，用来设定初始的gnss坐标
  ros::Rate loop_rate(100);
  while (nh.ok())
  {
    ros::spinOnce();
    geo_transformer_ptr->run();
    loop_rate.sleep();
  }
  
  return 0;
}

#include<iostream>
#include<ros/ros.h>
#include <serial/serial.h>
#include <queue>
#include <deque>

using std::endl;
using std::cout;

const double gravity = 9.80665;



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
	processTwoBytes(frame[3],frame[4],(double)360/32768,&roll);
	//俯仰角 deg
	processTwoBytes(frame[5],frame[6],360/32768,&pitch);
	//航向角 deg
    processTwoBytes(frame[7],frame[8],360/32768,&yaw);
	//x轴角速度 deg/s
	processTwoBytes(frame[9],frame[10],360/32768,&angular_velocity_x);
	//y轴角速度 deg/s
	processTwoBytes(frame[11],frame[12],360/32768,&angular_velocity_y);
	//z轴角速度 deg/s
	processTwoBytes(frame[13],frame[14],360/32768,&angular_velocity_z);
	//x轴加速度 m/s^2
	processTwoBytes(frame[15],frame[16],12/32768*gravity,&linear_acceleration_x);
	//y轴加速度 m/s^2
	processTwoBytes(frame[17],frame[18],12/32768*gravity,&linear_acceleration_y);
	//z轴加速度 m/s^2
	processTwoBytes(frame[19],frame[20],12/32768*gravity,&linear_acceleration_z);
	//latitude维度 deg
    processFourBytes(frame[21],frame[22],frame[23],frame[24],1.00e-07,&Latitude);
	//longitude经度 deg
	processFourBytes(frame[25],frame[26],frame[27],frame[28],1.00e-07,&Longitude);
	//altitude高度 deg
	processFourBytes(frame[29],frame[30],frame[31],frame[32],1.00e-07,&Altitude);
	//Vn北向速度 m/s
	processTwoBytes(frame[33],frame[34],1e2/32768,&Vn);
	//Ve东向速度 m/s
	processTwoBytes(frame[35],frame[36],1e2/32768,&Ve);
	//Vg地向速度 m/s
	processTwoBytes(frame[37],frame[38],1e2/32768,&Vg);
	//time时间 s
	processTwoBytes(frame[52],frame[53],2.5e-04,&time);

	//状态位观测
	statusCheckOut(frame[39],&status);
}

int main(int argc,char* argv[])
{
   

    ros::init(argc,argv,"gnss_dy_rs422");
    ros::NodeHandle nh;

    std::deque<int> p = {1,2,3,4,5};
    p.erase(p.begin(),p.begin()+3);
    for(auto x:p)
        cout << x << " ";
    cout<<endl;

    std::vector<uint8_t> data = {0xbd,0xdb,0xb,0x43,0x0,0x4a,0x0,0x79,0x17,0x0,0x0,0xff,0xff,0x9,0x0,0x24,0x0,0xdb,0xff,0x53,0xf5,0xd9,0xd3,0xcc, 
                                0x10,0xac,0xf1,0x51,0x43,0x46,0x96,0x0,0x0,0xf9,0xff,0xf0,0xff,0x0,0x0,0x1f,0x43,0x2,0x0,0x51,0x2,0x0,0x0,0x0,0xfc, 
                                0xff,0x0,0x0,0x30,0x39,0xfd,0x54,0x9,0x42,0x72,0x8,0x0,0x0,0x7a};

    
    double roll,pitch,yaw,
        angular_velocity_x,angular_velocity_y,angular_velocity_z,
        linear_acceleration_x,linear_acceleration_y,linear_acceleration_z,
        Latitude,Longitude,Altitude,
        Vn,Ve,Vg,
        status,time;
    
    processDataFrame(data,roll,pitch,yaw,
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
				

    return 0;

}
 
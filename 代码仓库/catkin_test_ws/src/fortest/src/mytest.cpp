#include<iostream>
#include<ros/ros.h>
#include<ros/time.h>

using std::endl;
using std::cout;

class TEST
{
    public:
        void test(int num)
        {
            cout<<"num="<<num<<endl;
        }

        void mybind()
        {
            boost::bind(&TEST::test,this,_1);
        }
};

void timeCB(const ros::TimerEvent&)
{
    ROS_INFO("whz");
}


int main(int argc,char* argv[])
{
    TEST t;
    t.mybind();

    ros::init(argc,argv,"time");
    ros::NodeHandle nh;

    ros::Time cur_time,last_time;
    cur_time = ros::Time::now();
    last_time = ros::Time::now();
    ros::Rate rate(1);
    const double duration = 5;
    double time_count = 0;
    while(nh.ok())
    {
        if(time_count > duration){
            nh.shutdown();
        }
        cur_time = ros::Time::now();
        double dt = (cur_time-last_time).toSec();
        time_count += dt;
        ROS_INFO("time duration is:%f",dt);
        
        //last_time = ros::Time::now();
        last_time = ros::Time::now();
        rate.sleep();
    }
    

    ros::Timer timer = nh.createTimer(ros::Duration(0.1),&timeCB);


    ros::spin();

    return 0;
}

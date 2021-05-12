#include<iostream>
#include<thread>
#include<ros/ros.h>

using std::endl;
using std::cout;


void print_t(int a)
{
    cout<<"sub thread start."<<endl;
    cout<<"a="<<a<<endl;
    cout<<"sub thread end."<<endl;
}

class TA
{
    public:
        TA(int a):m_a(a){}
        void operator()()
        {
            cout<<"sub thread start."<<endl;
            cout<<"a="<<m_a<<endl;
            cout<<"sub thread end."<<endl;
        }
        void print_y(int num)
        {
            cout<<"num="<<num<<endl;
        }
    private:
        int m_a;
};

int main(int argc,char** argv)
{

    //1
    std::thread th1(print_t,5);
    //2
    TA ta(6);
    std::thread th2(ta);
    //3
    std::thread* th3_ptr = new std::thread(&TA::print_y,ta,16);
    

    th1.join();
    th2.join();
    th3_ptr->join();
    //th1.detach();
    cout<<"main thread end."<<endl;
    return 0;
}
#include<iostream>
#include<thread>
#include<vector>
#include<ros/ros.h>

using std::endl;
using std::cout;
//? 读的时候不能写，写的时候不能读，也不能同时读与同时写
//只读的数据是安全稳定的，不需要特别的手段，直接读就可以
std::vector<int> global_v = {1,2,3}; //共享数据 

void myprint(int num_id)
{
    //do something
    cout<<"id为"<<std::this_thread::get_id()<<"的线程打印global_v："<<global_v[0]<<global_v[1]<<global_v[2]<<endl;
    cout<<"子线程运行结束，线程编号为："<<num_id<<endl;
}

void set_globalv(int index,int a)
{
    global_v[index] = a;
}

int main(int argc,char** argv)
{
    std::vector<std::unique_ptr<std::thread>> thread_ptrs;
    //创建10个线程读共享数据
     for(int i=0;i<10;i++)
    {
        thread_ptrs.push_back(std::make_unique<std::thread>(std::thread(myprint,i)));
    } 
    //创建两个线程写共享数据
    std::thread thread_w1(set_globalv,2,5);
    std::thread thread_w2(set_globalv,2,7);

    //子线程与主线程汇合，主线程等待子线程全部执行完再统一结束整个进程
    for(int i=0;i<10;i++)
    {
        thread_ptrs[i]->join();
    }
    thread_w1.join();
    thread_w2.join();

    cout<<"globalv="<<global_v[0]<<global_v[1]<<global_v[2]<<endl;
    cout<<"main thread end.";

    return 0;
}
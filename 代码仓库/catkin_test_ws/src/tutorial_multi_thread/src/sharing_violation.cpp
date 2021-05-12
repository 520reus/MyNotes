#include<iostream>
#include<ros/ros.h>
#include<thread>
#include<vector>
#include<deque>
#include<mutex>

using std::endl;
using std::cout;

// 类内定义一个队列，然后分别创建一个读线程与一个写线程，这样会发生冲突
// 安全的操作共享数据资源：线程间分别加锁，锁住的时候该线程独占该资源，处理完成后解锁，其他阻塞的线程可以
// 锁住共享资源，处理完成后再解锁
// 实现上述功能即用：互斥量mutex
class MSG
{
    public:
        void reciveData()
        {
            
            for(int i=0;i<20;i++)
            {
                mutex_.lock();
                container_.push_back(i);     
                mutex_.unlock();
                cout<<"插入的数据："<<i<<endl;
            }
        }

        void getfromData()
        {
            for(int i=0;i<20;i++)
            {
                mutex_.lock();
                if(!container_.empty())
                {
                    int data = container_.front();
                    cout<<"取出的数据为:"<<i<<endl;
                    container_.pop_front();
                }
                else
                    cout<<"container is empty now!"<<endl;
                mutex_.unlock();
            }
        }
    private:
        std::deque<int> container_;
        std::mutex mutex_;
};

int main(int argc,char** argv)
{
    MSG msg;
    std::thread thread_1(&MSG::reciveData,&msg);
    std::thread thread_2(&MSG::getfromData,&msg);
    thread_1.join();
    thread_2.join();

    return 0;
}
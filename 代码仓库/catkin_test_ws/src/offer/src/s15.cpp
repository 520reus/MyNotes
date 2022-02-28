#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

//* 1.可能引起死循环的解法
int NumberOf1(int n)
{
    int count = 0;
    while(n)
    {
        if(n & 1)
            count++;
        n = n >> 1;
    }
    return count;
}

//* 2.常规解法 ,循环的次数等于整数二进制的位数
int Numberof1_normal(int n)
{
    int count = 0;
    unsigned int flag = 1;
    while(flag)
    {
        if(n & flag)
            count++;
        flag = flag << 1;
    }
    return count;
}

//* 3.让面试官惊喜的解法
int NumberOf1_surprise(int n)
{
    int count = 0;
    while(n)
    {
        count++;
        n = (n-1) & n;
    }
    return count;
}

int main(int argc, char** argv)
{
    int num = 0x80000000;  //一个负数
    //cout << NumberOf1(num) << endl; ！死循环
    cout << Numberof1_normal(num) << endl;
    cout << NumberOf1_surprise(num) << endl;
    return 0;
}
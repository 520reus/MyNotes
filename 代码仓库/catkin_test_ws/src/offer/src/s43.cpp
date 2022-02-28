#include <iostream>
#include <ros/ros.h>
#include <algorithm>
#include <vector>


using std::cout; using std::endl;

int NumberOf1(unsigned int n)
{
    int num = 0;
    while(n)
    {
        if(n % 10 == 1)
            num++;
        
        n = n/10;
    }
    return num;
}

int NumberOf1Between1AndN(unsigned int n)
{
    int num = 0;
    for(int i = 1; i <= n; i++)
    {
        num += NumberOf1(i);
    }
    return num;
}

int main(int argc, char** argv)
{
    int n = 12;
    cout << NumberOf1Between1AndN(n) << endl;
    return 0;
}
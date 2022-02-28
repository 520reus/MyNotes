#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

//* 1.动态规划
int maxProductAfterCutting_DynamicProgramming(int length)
{
    if(length < 2)
        return 0;
    if(length == 2)
        return 1;
    if(length == 3)
        return 2;

    int* product = new int[length+1];
    product[0] = 0;
    product[1] = 1;
    product[2] = 2;
    product[3] = 3;

    for(int i = 4;i <= length;i++)
    {
        int max = 0;
        for(int j = 1; j <= i/2; j++)
        {
            int res = product[j]*product[i-j];
            if(res > max)
                max = res;
        }
        product[i] = max;
    }

    delete[] product;
    return product[length];

}

//* 2. 贪婪算法
int maxProductAfterCutting_greedy(int length)
{
    if(length < 2)
        return 0;
    if(length == 2)
        return 1;
    if(length == 3)
        return 2;
    
    //尽可能多地剪去长度为3的绳子段
    int timesOf3 = length / 3;

    // 当绳子最后剩下的长度为4的时候，不能再剪去长度为3的绳子段
    // 此时更好的方法是把绳子剪成长度为2的两段，因为 2*2 > 3*1
    if(length - timesOf3*3 == 1)
        timesOf3 -= 1;
    
    int timesOf2 = (length - timesOf3*3) / 2;

    return (int)(pow(3,timesOf3)) * (int)(pow(2,timesOf2)); 

}

int main(int argc, char** argv)
{
    int length = 8;
    cout << maxProductAfterCutting_DynamicProgramming(length) << endl;
    cout << maxProductAfterCutting_greedy(length) << endl;

    return 0;
}
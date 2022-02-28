#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>

using std::cout; using std::endl;

//和为s的数字
//*1. 和为s的两个数字
bool TwoSum(int* data, int length, int s)
{
    if(data == nullptr || length <= 0)
        return false;
    std::unordered_map<int,int> vmap;
    for(int i = 0; i < length; i++)
    {
        vmap[s - data[i]] = data[i];
    }
    for(int i = 0; i < length; i++)
    {
        if(vmap.find(data[i]) != vmap.end())
        {
            cout << data[i] << " " << s - data[i] << endl;
            return true;
        }
    }
    return false;
}

bool FindNUmbersWithSum(int data[], int length, int sum, int*num1, int* num2)
{
    bool found = false;
    if(data == nullptr || length <= 0 || num1 == nullptr || num2 == nullptr)
        return found;

    int start = 0;
    int end = length - 1;
    int s = 0;
    while(start < end)
    {
        s = data[start] + data[end];
        if(s > sum)
            end--;
        else if(s < sum)
            start++;
        else
        {
            *num1 = data[start];
            *num2 = data[end];
            found = true;
            break;
        }
    }

    return found;
}

//*2. 和为s的连续正数序列
void PrintContinuousSequence(int small, int big)
{
    for(int i = small; i <= big; i++)
        cout << i << " ";
    cout << endl;
}

void FindContinuousSequence(int sum)
{
    if(sum < 3)
        return;
    
    int small = 1;
    int big = 2;
    int middle = (1 + sum) / 2;
    int curSum = small + big;

    while(small < middle)
    {
        if(curSum == sum)
            PrintContinuousSequence(small,big);
        
        while(curSum > sum && small < middle)
        {
            curSum -= small;
            small++;

            if(curSum == sum)
                PrintContinuousSequence(small,big);
        }
        big++;
        curSum += big;
    }
}


int main(int argc, char** argv)
{
    int arr[6] = {1,2,4,7,11,15};
    int s = 15;
    //cout << TwoSum(arr,6,s) << endl;
    int a,b;
    FindNUmbersWithSum(arr,6,s,&a,&b);
    cout << a << " " << b << endl;

    FindContinuousSequence(s);
    return 0;
}
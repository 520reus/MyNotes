#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>

using std::cout; using std::endl;

//* 1.数字在排序数组中出现的次数
int GetFirstK(int* data, int length, int k, int start, int end)
{
    if(start > end)
        return -1;
    
    int middleIndex = (start + end) / 2;
    if(data[middleIndex] == k)
    {
        if(middleIndex == 0 || data[middleIndex - 1] != k)
            return middleIndex;
        
        else
            end = middleIndex - 1;
    }
    else if(data[middleIndex] > k)
    {
        end = middleIndex - 1;
    }
    else
    {
        start = middleIndex + 1;
    }

    return GetFirstK(data,length,k,start,end);
}

int GetLastK(int* data, int length, int k, int start, int end)
{
    if(start > end)
        return -1;
    
    int middleIndex = (start + end) / 2;
    if(data[middleIndex] == k)
    {
        if(middleIndex == length - 1 || data[middleIndex + 1] != k)
            return middleIndex;
        else
            start = middleIndex + 1;
    }
    else if(data[middleIndex] > k)
    {
        end = middleIndex - 1;
    }
    else
    {
        start = middleIndex + 1;
    }

    return GetLastK(data,length,k,start,end);
}

int getNumberOfK(int* data, int length, int k)
{
    int number = 0;
    if(data != nullptr && length > 0)
    {
        int first = GetFirstK(data,length,k,0,length-1);
        int last = GetLastK(data,length,k,0,length-1);
        if(first > -1 && last > -1)
            number = last - first + 1;
    }

    return number;
}

//*2. 0~n-1中缺失的数字
int GetMissingNumber(int* data, int length)
{
    if(data == nullptr || length <= 0)
        return -1;
    
    int start = 0;
    int end = length - 1;
    while(start <= end)
    {
        int middle = (start + end) >> 1;
        if(data[middle] != middle)
        {
            if(middle == 0 || data[middle - 1] == middle - 1)
                return middle;
            end = middle - 1;
        }
        else
            start = middle + 1;
    }
        
    if(start == length)
        return length;

    //无效的输入，比如数组不是按要求排序的，或者有数字不在0-n-1范围内
    return -1;
}

//*3. 数组中数值和下标相等的元素
int GetNumberSameWithIndex(int* data, int length)
{
    if(data == nullptr || length <= 0)
        return -1;

    int start = 0;
    int end = length - 1;
    while(start <= end)
    {
        int middle = start + ((end - start) >> 1);
        if(data[middle] == middle)
            return middle;
        else if(data[middle] < middle)
            start = middle + 1;
        else
            end = middle - 1;
    }
    return -1;
}

int main(int argc, char** argv)
{
    int length = 10;
    int arr[length] = {1,2,3,3,3,3,4,5,7,7}; 
    cout << getNumberOfK(arr,length,3) << endl;
    int arr2[4] = {0,1,2,4};
    cout << GetMissingNumber(arr2,4) << endl;
    int arr3[5] = {-3,-1,1,3,5};
    cout << GetNumberSameWithIndex(arr3,5) << endl;
    return 0;
}
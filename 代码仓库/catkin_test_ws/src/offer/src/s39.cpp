#include <iostream>
#include <ros/ros.h>
#include <algorithm>
#include <vector>
using std::cout; using std::endl;

void Swap(int& a, int& b)
{
    if(a == b)
        return;
    
    int temp = a;
    a = b;
    b = temp;
}

int RandomChoose(int start, int end)
{
    return (rand() % (end - start + 1)) + start;
}

int Partition(int* data, int length, int start, int end)
{
    int index = RandomChoose(start, end);
    Swap(data[index], data[end]);

    int small = start;
    for(int i = start; i < end; i++)
    {
        if(data[i] < data[end])
        {
            Swap(data[i], data[small++]);
        }
    }
    ++small;
    Swap(data[end], data[small]);

    return small;
}

bool CheckInputValid(int* numbers, int length)
{
    bool valid = true;
    if(numbers == nullptr || length <= 0)
        valid = false;
    
    return valid;
}

bool CheckMoreThanHalf(int* numbers, int length, int number)
{
    int count = 0;
    int IsMoreThanHalf = false;
    for(int i = 0; i < length-1; i++)
    {
        if(numbers[i] == number)
            count++;
    }

    if(count*2 > length)
    {
        IsMoreThanHalf = true;
    }

    return IsMoreThanHalf;
}

//* 1.先排序(O(nlogn))，再直接返回排序数组中间的数即可
int TimesOfNumberExceedHalf(std::vector<int> vec)
{
    std::sort(vec.begin(),vec.end());
    return vec[vec.size()/2];
}

//* 2.利用随机快速排序的方法(O(n))，该法会改变输入的数组
int MoreThanHalfNumPartition(int* numbers, int length)
{
    if(!CheckInputValid(numbers,length))
        return 0;

    int start = 0;
    int end = length - 1;
    int middle = length >> 1;
    int index = Partition(numbers,length,start,end);
    while(index != middle)
    {
        if(index > middle)
        {
            end = index - 1;
            index = Partition(numbers,length,start,end);
        }
        else
        {
            start = index + 1;
            index = Partition(numbers,length,start,end); 
        
    }

    int result = numbers[middle];
    
    if(!CheckMoreThanHalf(numbers,length,result))
        return 0;
    
    return result;
    }   
}

//* 3.利用数组特点(O(n)),该法不会改变数组
int MoreThanHalfNum(int* numbers, int length)
{
    if(CheckInputValid(numbers,length))
        return 0;
    
    int result = numbers[0];
    int times = 1;
    for(int i = 1; i < length; i++)
    {
        if(times == 0)
        {
            result = numbers[i];
            times = 1;
        }
        else if(numbers[i] == result)
            times++;
        else
            times--;
    }
    if(!CheckMoreThanHalf(numbers,length,result))
        return 0;
    
    return result;
}

int main(int argc, char** argv)
{
    std::vector<int> v = {1,2,3,2,2,2,2,5,7};
    cout << TimesOfNumberExceedHalf(v) << endl;
    return 0;
}
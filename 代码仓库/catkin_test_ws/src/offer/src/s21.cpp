#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

void ReorderOddEven(int* arr, int length)
{
    if(arr == nullptr || length <= 0)
        return;

    int* left = arr;
    int* right = arr + length - 1;
    while(left < right)
    {
        while(left < right && (*left & 0x1) != 0)
        {
            left++;
        }

        while(left < right && (*right & 0x1) == 0)
        {
            right--;
        }

        if(left < right)
        {
            int temp = *left;
            *left = *right;
            *right = temp;
        }
    }
}

//* 解耦，提高扩展性

// 这里放特殊功能函数
bool isOdd(int a)
{
    return a & 0x1;
}

bool is3Times(int a)
{
    return a % 3;
}

//这里为标准功能函数
void Reorder(int* arr, int length, bool (*func)(int))
{
    if(arr == nullptr || length <= 0)
        return;

    int* left = arr;
    int* right = arr + length - 1;
    while(left < right)
    {
        while(left < right && func(*left))
        {
            left++;
        }

        while(left < right && !func(*right))
        {
            right--;
        }

        if(left < right)
        {
            int temp = *left;
            *left = *right;
            *right = temp;
        }
    }
}

void ReorderOddEvenExtend(int* arr, int length)
{
    Reorder(arr,length,isOdd);
}


int main(int argc, char** argv)
{
    int length = 10;
    int arr[length] = {2,1,5,4,6,3,8,7,10,0};
    //ReorderOddEven(arr,length);
    ReorderOddEvenExtend(arr,length);
    for(int i = 0;i < length; i++)
        cout << arr[i] << " ";
    cout << endl;
    return 0;
}
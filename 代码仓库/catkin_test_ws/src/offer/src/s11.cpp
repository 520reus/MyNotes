#include <iostream>
#include <stdexcept>
#include <ros/ros.h>

using std::cout; using std::endl;

//* 1. 快速排序
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
    return (rand() % (end-start+1))+ start;
}

int Partition(int* data, int length, int start, int end)
{
    if(data == nullptr || length < 1 || start < 0 || end > length-1)
    {
        std::logic_error e("Invalid parameters");
        throw new std::exception(e);
    }
    
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

void QuickSort(int* data, int length, int start, int end)
{
    if(start == end)
        return;
    
    int index = Partition(data, length, start, end);
    if(index > start)
        QuickSort(data, length, start, index-1);
    if(index < end)
        QuickSort(data, length, index+1, end);   
}

//* 2. 面试题11 旋转数组的最小数字
int MinInOrder(int* numbers, int left, int right)
{   
    int res = INT_MAX;
    while(left < right)
    {
        if(res > numbers[left])
            res = numbers[left];
        left++;
    }
    return res;
}

int FindMin(int* numbers, int length)
{
    if(numbers == nullptr || length <= 0)
        return -1;
    
    int left = 0;
    int right = length - 1;

    int mid = left; //把mid初始化为left，一旦发现数组中第一个数字小于最后一个数字，表明该数组是排序的，即可直接返回
    while(numbers[left] >= numbers[right])
    {
        if(right - left == 1)
        {
            mid = right;
            break;
        }

        mid = left + (right - left) / 2;

        if(numbers[mid] == numbers[left] == numbers[right])
            return MinInOrder(numbers,left,right);

        if(numbers[mid] >= numbers[left])
            left = mid;
        else if(numbers[mid] <= numbers[right])
            right = mid;
    }
    return numbers[right];

}

int main(int argc, char** argv)
{
    int input[10] = {2, 4, 6, 5, 3, 4, 1, 110, 22, 45};
    //QuickSort(input,10,0,9);

    int arr[5] = {1,0,1,1,1};
    cout << FindMin(arr,5) << endl;
    return 0;
}
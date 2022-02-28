#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <stack>
#include <deque>

using std::cout; using std::endl; 
using std::vector;
using std::deque;

int MaxDiff(const int* numbers, unsigned int length)
{
    if(numbers == nullptr || length < 2)
        return 0;
    
    int min = numbers[0];
    int max_diff = numbers[1] - min;
    for(int i = 1; i < length; i++)
    {
        if(numbers[i] < numbers[i-1])
            min = numbers[i];
        
        int current_diff = numbers[i] - min;
        if(current_diff > max_diff)
            max_diff = current_diff;
    }
    return max_diff;
}

int main(int argc, char** argv)
{
    int length = 8;
    int arr[length] = {9,11,8,5,7,12,16,14};
    cout << MaxDiff(arr,length) << endl;
    return 0;
}
#include <iostream>
#include <ros/ros.h>
#include <algorithm>
#include <vector>


using std::cout; using std::endl;

int countOfIntegers(int digits)
{
    if(digits == 1)
        return 10;
    else
        return 9 * (int)std::pow(10,digits - 1);
}

int DigitsBeginNumber(int digits)
{
    if(digits == 1)
        return 0;
    else
        return (int)std::pow(10,digits-1);
}

int digitAtIndex(int index, int digits)
{
    int integer = index / digits;
    int number = DigitsBeginNumber(digits) + integer;
    int times = digits - index % digits;
    for(int i = 1; i < times; i++)
    {
        number /= 10;
    } 
    return number % 10;
}

int digitAtIndex(int index)
{
    if(index < 0)
        return -1;
    
    int digits = 1;
    while(true)
    {
        int numbers = countOfIntegers(digits); //计算n位整数有多少个数，比如digits=3，则输出900，即100-999
        if(index < numbers * digits) 
            return digitAtIndex(index,digits); //找到该数的所在位数，然后再找到最终的第n位对应数字 
        index -= numbers * digits;
        digits++;
    }
    return -1;
}


int main(int argc, char** argv)
{
    int number = 1001;
    cout << digitAtIndex(number) << endl; 
    return 0;
}
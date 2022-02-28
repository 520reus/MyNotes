#include <iostream>
#include <ros/ros.h>

using std::cout; using std::endl;

int addToN_iterative(const int& n)
{
    int sum = 0;
    for(int i = 1; i <= n; i++)
        sum += i;
    return sum;
}

int addToN_recursive(const int& n)
{
    if(n == 0)
        return 0;
    return n + addToN_recursive(n-1);
}

//* 1.递归，效率低
int Fibonacci_1(const int& n)
{
    if(n == 0 || n == 1)
        return n;
    return Fibonacci_1(n-1) + Fibonacci_1(n-2);
}

//* 2.用递归的思路分析，用自下而上的循环写代码
int Fibonacci_2(const int& n)
{
    if(n == 0 || n == 1)
        return n;
    
    int first = 1;
    int second = 0;
    int fibN = 0;
    for(int i = 2; i <= n; i++)
    {
        fibN = first + second;
        second = first;
        first = fibN;
    }
    return fibN;
}


int main(int argc, char** argv)
{
    cout << addToN_iterative(12) << endl;
    cout << addToN_recursive(12) << endl;
    cout << Fibonacci_1(12) << endl;
    cout << Fibonacci_2(12) << endl;
    return 0;
}
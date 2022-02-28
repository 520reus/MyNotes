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

int Add(int num1, int num2)
{
    int sum,carry;
    do
    {
        sum = num1 ^ num2;
        carry = (num1 & num2) << 1;
        num1 = sum;
        num2 = carry;
    } while (num2 != 0);

    return num1;
    
}

int main(int argc, char** argv)
{
    int a = 5;
    int b = 7;
    cout << Add(a,b) << endl;
    return 0;
}
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

void multiply(const vector<double>& array1, vector<double>& array2)
{
    int length1 = array1.size();
    int length2 = array2.size();

    if(length1 == length2 && length2 > 1)
    {
        array2[0] = 1;
        for(int i = 1; i < length1; i++)
        {
            array2[i] = array2[i-1] * array1[i-1];
        }

        double temp = 1;
        for(int i = length1 - 2; i >= 0; i--)
        {
            temp *= array1[i+1];
            array2[i] *= temp;
        }
    }
}

int main(int argc, char** argv)
{
    vector<double> a = {1,2,3,4,5};
    vector<double> b(5);
    multiply(a,b);
    for(auto x:b)
        cout << x << " ";
    cout << endl;
    return 0;
}
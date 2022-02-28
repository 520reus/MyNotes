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


bool IsContinuous(vector<int> numbers)
{
    int length = numbers.size();
    std::sort(numbers.begin(),numbers.end());
    int numOfZero = 0;
    int numOfGap = 0;
    
    for(auto x:numbers)
    {
        if(x == 0)
            numOfZero++;
    }

    int small = numOfZero;
    int big = small + 1;
    while(big < length)
    {
        if(numbers[small] == numbers[big])
            return false;
        
        numOfGap += numbers[big] - numbers[small] - 1;
        small = big;
        big++;
    }

    return  numOfGap > numOfZero ? false : true;

}

int main(int argc, char** argv)
{
    vector<int> arr = {5,4,3,0,7};
    cout << IsContinuous(arr) << endl;
    return 0;
}
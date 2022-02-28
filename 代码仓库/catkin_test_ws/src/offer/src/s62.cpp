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

int LastRemaining(unsigned int n, unsigned int m)
{
    if(n < 1 || m < 1)
        return -1;
    
    unsigned int i = 0;
    std::list<int> numbers;
    for(i = 0; i < n; i++)
        numbers.emplace_back(i);

    std::list<int>::iterator current = numbers.begin();
    while(numbers.size() > 1)
    {
        for(int j = 1; j < m; j++)
        {
            current++;
            if(current == numbers.end())
                current = numbers.begin();
                
        }

        std::list<int>::iterator next = ++current;
        if(next == numbers.end())
            next = numbers.begin();
        
        --current;
        numbers.erase(current);
        current = next;

    }
    return *(current);
}

//TODO 数学推导好复杂
int LastRemaining_math(unsigned int n, unsigned int m)
{
    if(n < 1 || m < 1)
        return -1;
    
    int last = 0;
    for(int i = 2; i <= n; i++)
        last = (last + m) % i;
    
    return last;
}

int main(int argc, char** argv)
{
    unsigned int n = 5;
    unsigned int m = 3;
    cout << LastRemaining(n,m) << endl;
    cout << LastRemaining_math(n,m) << endl;
    return 0;
}
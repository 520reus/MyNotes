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

//? 队列的最大值

//* 1.滑动窗口的最大值
vector<int> maxInWindows(const vector<int>& num, unsigned int size)
{
    vector<int> maxInWindows;
    if(num.size() && size >= 1)
    {
        deque<int> index;

        for(unsigned int i = 0; i < size; i++)
        {
            while(!index.empty() && num[i] > num[index.back()])
                index.pop_back();
            
            index.push_back(i);
        }

        for(unsigned int i = size; i < num.size(); i++)
        {
            maxInWindows.push_back(num[index.front()]);

            while(!index.empty() && num[i] >= num[index.back()])
                index.pop_back();
            
            if(!index.empty() && index.front() <= (int)(i-size))
                index.pop_front();
            
            index.push_back(i);
        }
        maxInWindows.push_back(num[index.front()]);
    }

    return maxInWindows;
}

//*2.队列的最大值
template<typename T> class QueueWithMax
{
    public:
        QueueWithMax():currentIndex(0){}

        void push_back(T number)
        {
            while(!maximums.empty() && number > maximums.back().number)
                maximums.pop_back();
            
            InternalData internalData = {number,currentIndex};
            data.push_back(internalData);
            maximums.push_back(internalData);
            ++currentIndex;
        }

        void pop_front()
        {
            if(maximums.empty())
            {
                //throw new exception("queue is empty.");
            }
            if(maximums.front().index == data.front().index)
                maximums.pop_front();
            
            data.pop_front();
        }

        T max() const
        {
            if(maximums.empty())
            {  
                //throw new exception("queue is empty.");
            }

            return maximums.front().number;
        }

    private:
        struct InternalData
        {
            T number;
            int index;
        };

        deque<InternalData> data;
        deque<InternalData> maximums;
        int currentIndex; 
};

int main(int argc, char** argv)
{
    vector<int> arr = {2,3,4,2,6,2,5,1};
    int size = 3;
    vector<int> maxVec = maxInWindows(arr,size);
    vector<int>::iterator it = maxVec.begin();
    for(; it != maxVec.end(); it++)
    {
        cout << *it << " ";
    }
    cout << endl;
    return 0;
}
#include <iostream>
#include <ros/ros.h>
#include <algorithm>
#include <vector>
#include <set>


using std::cout; using std::endl;

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
    return (rand() % (end - start + 1)) + start;
}

int Partition(int* data, int length, int start, int end)
{
    int index = RandomChoose(start, end);
    Swap(data[index], data[end]);

    int small = start - 1;
    for(int i = start; i < end; i++)
    {
        if(data[i] < data[end])
        {
            ++small;
            if(small != i)
                Swap(data[i], data[small]);
        }
    }
    ++small;
    Swap(data[end], data[small]);

    return small;
}

//* 1. 利用Partion操作，改变输入数组，时间复杂度为O(n)
void getLeastNumber(int*const numbers, const int& n, const int& k)
{
    int start = 0;
    int end = n - 1;
    int index = Partition(numbers,n,start,end);
    while(index != k - 1)
    {
        if(index < k - 1)
        {
            start = index + 1;
            index = Partition(numbers,n,start,end);
        }
        else
        {
            end = index - 1;
            index = Partition(numbers,n,start,end);
        }
    }
    
    for(int i = 0; i < k; i++)
    {
        cout << numbers[i] << " ";
    }
    cout << endl;
}

//* 2. 利用堆或红黑树，不改变输入数组，时间复杂度为O(nlogk),特别适合处理海量数据
typedef std::multiset<int,std::greater<int>> intSet;
typedef std::multiset<int,std::greater<int>>::iterator setIterator;

void getLeastNumber(const std::vector<int>& data, intSet& leastNumbers,int k)
{
    leastNumbers.clear();
    if(k < 1 || k > data.size())
        return;

    std::vector<int>::const_iterator iter = data.begin();
    for(; iter != data.end(); iter++)
    {
        if(leastNumbers.size() < k)
            leastNumbers.insert(*iter);

        else
        {
            setIterator iterGreatest = leastNumbers.begin();
            if(*iterGreatest > *iter)
            {
                leastNumbers.erase(*iterGreatest);
                leastNumbers.insert(*iter);
            }
        }
    }
}

int main(int argc, char** argv)
{
    int length = 7;
    int nums[length] = {5,4,7,9,3,2,8};
    int k = 5;
    getLeastNumber(nums,length,k);
    std::vector<int> numbers = {5,4,7,9,3,2,8};
    intSet set;
    getLeastNumber(numbers,set,k);
    setIterator iter = set.begin();
    for(; iter != set.end(); iter++)
    {
        cout << *iter << " ";
    }
    cout << endl;
    return 0;
}
#include <ros/ros.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using std::cout; using std::endl;
using std::vector;using std::unordered_map;

//*面试题3 题目一：找出数组中重复的数字

//?解法一：先排序，再遍历查找，时间复杂度O(nlogn)，空间复杂度O(n)
bool duplicate_sort(const vector<int>& arr,int& result)
{
    vector<int> arr_sorted = arr;
    std::sort(arr_sorted.begin(),arr_sorted.end());
    for(int i = 0; i < arr_sorted.size(); i++)
    {
        if(i != arr_sorted[i])
        {
            result = arr_sorted[i];
            return true;
        }
    }
    return false;
}

//?解法二：利用哈希表，时间复杂度O(n)，空间复杂度O(n)
bool duplicate_hash(const vector<int>& arr,int& result)
{
    unordered_map<int,int> map;

    for(auto x:arr)
    {
        map[x]++;
    }

    for(auto x:map)
    {
        if(x.second > 1)
        {
            result = x.first;
            return true;
        } 
    }

    return false;
}

//?解法三（最优解）：利用哈希表，并优化空间，时间复杂度O(n)，空间复杂度O(1)
bool duplicate_opt_solution(vector<int>& arr,int& result)
{
    if(arr.empty())
        return false;
    
    for(auto x:arr)
    {
        if(x < 0 || x > arr.size()-1)
            return false;
    }

    for(int i = 0; i < arr.size(); i++)
    {
        while(i != arr[i])
        {
            if(arr[i] == arr[arr[i]])
            {
                result = arr[i];
                return true;
            }

            int temp = arr[i];
            arr[i] = arr[temp];
            arr[temp] = temp;
        }
    }
    return false;
}

//*题目二：不修改数组找出数组中重复的数字
//?解法一：利用辅助数组，时间复杂度O(n)，空间复杂度O(n)
bool dulplicate_non_modify_assist_arr(const vector<int>& arr,int& result)
{
    if(arr.empty())
        return false;
    
    for(auto x:arr)
    {
        if(x < 0 || x > arr.size()-1)
            return false;
    }

    vector<int> assist_arr(arr.size());

    for(int i = 0;i < arr.size();i++)
    {
        if(assist_arr[arr[i]] == arr[i])
        {
            result = arr[i];
            return true;
        }
        assist_arr[arr[i]] = arr[i];
    }
    return false;
}

//?解法二（最优解）：利用二分查找，时间复杂度O(nlogn)，空间复杂度O(1) (用时间换空间)
int countRange(const vector<int>& arr,int start,int end);
int dulplicate_non_modify_binary_search(const vector<int>& arr)
{
    if(arr.empty())
        return -1;
    
    for(auto x:arr)
    { 
        if(x <= 0 || x > arr.size() - 1)
            return -1;
    }

    int start = 1;
    int end = arr.size() - 1;
    while(end >= start)
    {
        int middle = ((end - start) >> 1) + start;
        int count = countRange(arr,start,middle);
        if(end == start)
        {
            if(count > 1)
                return start;
            else
                break;
        }

        if(count > (middle - start +1))
            end = middle;
        else
            start = middle + 1;
    
    }
    return -1;
}

int countRange(const vector<int>& arr,int start,int end)
{
    int count = 0;
    for(int i = 0;i < arr.size();i++)
    {
        if(arr[i] >= start && arr[i] <= end)
            count++;
    }
    return count;
}


int main(int argc,char** argv)
{
    //test1
    vector<int> arr = {2,3,1,0,2,5,3};
    int result = INT_MAX;    
    if(duplicate_sort(arr,result))
        cout << "Test1:(1.Sort) The dulplicate num is:" << result << endl;

    if(duplicate_hash(arr,result))
        cout << "Test1:(2.Hash) The dulplicate num is:" << result << endl;

    if(duplicate_opt_solution(arr,result))
        cout << "Test1:(3.Optimal solution) The dulplicate num is:" << result << endl;

    //test2
    vector<int> arr2 = {2,3,5,4,3,2,6,7};
    if(dulplicate_non_modify_assist_arr(arr2,result))
        cout<< "Test2:(1.Assist arr) The dulplicate num is:" << result << endl;

    int res = dulplicate_non_modify_binary_search(arr2);
    if(res != -1)
        cout << "Test2:(2.Binary search) The dulplicate num is:" << res << endl;

    return 0;
}
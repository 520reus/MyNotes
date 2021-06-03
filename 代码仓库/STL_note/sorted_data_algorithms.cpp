#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

/* 
* 本文介绍模板库的对于已排好序的数据的算法应用
* algoritms needs data being pre-sorted.
* -binary search,merge,set operation
 */
int main()
{
    vector<int> vec = {8,9,9,9,45,87,90}; //7 items
    //? 1.binary search
    //search elements
    bool found = binary_search(vec.begin(),vec.end(),8); //check if 8 is in vec

    vector<int> s = {9,45,66};
    bool found2 = includes(vec.begin(),vec.end(), //range #1
                          s.begin(),s.end());     //range #2
    // return true if all elements of s is included in vec
    // both vec and s must be sorted!

    //search position
    vector<int>::iterator it;
    it = lower_bound(vec.begin(),vec.end(),9); //vec[1]
    // find the first position where 9 could be inserted and still keep the sorting.
    it = upper_bound(vec.begin(),vec.end(),9); //vec[4]
    // find the last position where 9 could be inserted and still keep the sorting.
    pair<vector<int>::iterator,vector<int>::iterator> pair_of_itr;
    pair_of_itr = equal_range(vec.begin(),vec.end(),9);
    // return both first and last position

    //? 2.merge
    vector<int> vec2 = {8,9,9,10};
    vector<int> vec3 = {7,9,10};
    vector<int> vec_out(vec2.size()+vec3.size(),0);
    merge(vec2.begin(),vec2.end(), //Input range #1
          vec3.begin(),vec3.end(), //Input range #2
          vec_out.begin());        //output
    //both vec2 and vec3 should be sorted(same for the set operation)
    //vec_out:{7,8,8,9,9,9,10,10}
    vector<int> vec4 = {1,2,3,4,1,2,3,4,5}; //both part of vec4 are already sorted
    inplace_merge(vec4.begin(),vec4.begin()+4,vec4.end());
    // vec4:{1,1,2,2,3,3,4,4,5}  -One step of merge sort

    //? 3.set operations
    // -Both should be sorted
    // -The resulted data is also sorted
    set_union(vec2.begin(),vec2.end(), //Input range #1
              vec3.begin(),vec3.end(), //Input range #2
              vec_out.begin());        //output
    // if x in both vec2 and vec3,only one X is kept in vec_out
    // vec_out:{7,8,9,9,10}
    
    set_intersection(vec2.begin(),vec2.end(), //Input range #1
                     vec3.begin(),vec3.end(), //Input range #2
                     vec_out.begin());        //output
    //only the items that are in both vec2 and vec3 are saved in vec_out
    //vec_out:{9,10,0,0,0}
    

    system("pause");
    return 0;
}

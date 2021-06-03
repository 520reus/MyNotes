#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;
bool lsb_less(int x,int y);
bool lessThan10(int i);
/* 
* 本文介绍模板库的排序算法
* sorting algorithm requires random access iterators:
* -vector,deque,container array,native array
 */
int main()
{
    vector<int> vec = {9,1,10,2,45,3,90,4,9,5,8};

    sort(vec.begin(),vec.end()); //sort with operator <
    //vec:{1,2,3,4,5,8,9,9,10,45,90}

    sort(vec.begin(),vec.end(),lsb_less);//sort with lsb_less()
    //vec:{10,90,1,2,3,4,45,5,8,9,9}

    //sometimes we don't need complete sorting.

    //? Problem #1:Finding top 5 students according to their test scores.
    // using partial sort
    vector<int> vec2={9,60,70,8,45,87,90,69,69,55,7};
    // 只对vec中的最大的5个数进行排序再放到vector头部
    partial_sort(vec.begin(),vec.begin()+5,vec.end(),greater<int>());
    // vec:{90,87,70,69,69,8,9,45,60,55,7}
    partial_sort(vec.begin(),vec.begin()+5,vec.end());
    // vec:{7,8,9,45,55,90,60,87,70,69,69}

    //? problem #2:Finding top 5 students accroding to their score,but I don't care their order
    vector<int> vec3={9,60,70,8,45,87,90,69,69,55,7};
    nth_element(vec3.begin(),vec3.begin()+5,vec.end(),greater<int>());
    // vec: 69 87 70 90 69 60 55 45 9 8 7

    //? problem #3: move the students whose score is less than 10 to the front
    vector<int> vec4 = {9,60,70,8,45,87,90,69,69,55,7};

    partition(vec4.begin(),vec4.end(),lessThan10);
    //vec:{8,7,9,90,69,60,55,45,70,87,69}

    //To preserve the original order within each partition
    stable_partition(vec4.begin(),vec4.end(),lessThan10);
    // vec:{9,8,7,60,70,45,87,90,69,69,55}

    //? Heap Algorithm
    /* 
    * Heap:
    * 1.First element is always the largest
    * 2.add/remove takes O(log(n)) time
     */
    vector<int> vec5 = {9,1,10,2,45,3,90,4,9,5,8};
    //构造大顶堆
    make_heap(vec5.begin(),vec5.end());
    // vec5:{90,45,10,9,8,3,9,4,2,5,1}
    // remove the largest element
    pop_heap(vec5.begin(),vec5.end()); //1.swap vec5[0] with last item vec5[size-1]
                                     //2.Heapify [vec5.begin(),vec5.end()-1]
    //vec5:{45,9,10,4,8,3,9,1,2,5,90}    
    vec5.pop_back(); //remove the last item(the largest one)
    // vec:{45,9,10,4,8,3,9,1,2,5}

    // add a new element:
    vec5.push_back(100);
    push_heap(vec5.begin(),vec5.end()); //heapify the last item in vec5
    // vec5: {100,45,10,4,9,3,9,1,2,5,8}

    //Heap sorting
    vector<int> vec6 = {9,1,10,2,45,3,90,4,9,5,8};
    make_heap(vec6.begin(),vec6.end());
    
    sort_heap(vec6.begin(),vec6.end());
    // vec6:{1 2 3 4 5 8 9 9 10 45 100}
    //Note: sort_heap can only work on a heap
    system("pause");
    return 0;
}

bool lsb_less(int x,int y){
    return (x%10)<(y%10);
}

bool lessThan10(int i){
    return (i<10);
}
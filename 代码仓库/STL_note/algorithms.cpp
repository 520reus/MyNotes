#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
bool isOdd(int i);
void add2(int& i);
/* 
* Algorithms
* —— mostly loops
 */
int main()
{
    vector<int> vec = {4,2,5,1,3,9};
    vector<int>::iterator itr = min_element(vec.begin(),vec.end()); //itr->1
    //* Note1: Algorithm always process ranges in a half-open way:[begin,end)
    sort(vec.begin(),itr); //vec:{2,4,5,1,3,9}; 即只对[begin,itr)间的元素进行排序
    
    reverse(itr,vec.end()); //vec:{2,4,5,9,3,1};同样的只对[itr,end)间的元素进行逆序翻转，并且使itr->9
    
    //* Note2: 
    vector<int> vec2(3);
    copy(itr,vec.end(), //source
    vec2.begin());      //destination
                        //vec2 needs to have at least space for 3 elements
    
    //* Note3:为了解决以上安全问题，STL给出了back_inserter()的方法
    //*       即使vec3是空的也是可以的，因为它会在vec3的后面逐个插入source中的元素
    vector<int> vec3;
    copy(itr,vec.end(),back_inserter(vec3));//Inserting instead of overwriting 
                       //back_insert_iterator   not efficient
    //* 最推荐的用法
    vec3.insert(vec3.end(),itr,vec.end()); //efficient and safe
    
    //* Note4: algorithm with function
    vector<int> vec4 = {2,4,6,8,9};
    //若函数返回1则将该元素的位置赋值给迭代器
    vector<int>::iterator itr2 = find_if(vec4.begin(),vec4.end(),isOdd); //itr2->9
    //若函数返回0贼将该元素的位置赋值给迭代器
    vector<int>::iterator itr3 = find_if_not(vec4.begin(),vec4.end(),isOdd); //itr2->2
    // 使得vector的每一个元素都调用同一个函数
    for_each(vec4.begin(),vec4.end(),add2); //vec4:{4,6,8,10,11}
    
    //* Note5: Algorithm with native c++ array
    int arr[4] = {6,3,7,4}; //arr:{3,4,6,7}
    

    system("pause");
    return 0;
}
// 奇数返回1，否则返回0
bool isOdd(int i)
{
    return i%2;
}

void add2(int& i)
{
    i+=2;
}
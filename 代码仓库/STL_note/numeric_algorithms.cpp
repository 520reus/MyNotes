#include<iostream>
#include<numeric>
#include<vector>

using namespace std;

/* 
* 本文介绍模板库中的Numeric algorithms
* -accumulate,inner product,partial sum,adjacent difference
 */
int main()
{
    vector<int> vec = {8,9,9,9,45,87,90}; //7 items
    //? 1.accumulate 求和/积
    int x = accumulate(vec.begin(),vec.end(),10);
    // 10+vec[0]+vec[1]+...
    int x2 = accumulate(vec.begin(),vec.end(),10,multiplies<int>());
    // 10*vec[0]*vec[1]*...

    //? 2.inner product 容器范围内的求和/积
    int x3 = inner_product(vec.begin(),vec.begin()+3, // range #1
                           vec.end()-3,               // range #2
                           10);                       // init value
    // 10+vec[0]*vec[4]+vec[1]*vec[5]+vec[2]*vec[6]

    int x4 = inner_product(vec.begin(),vec.begin()+3, // range #1
                           vec.end()-3,               // range #2
                           10,                      // init value
                           multiplies<int>(),       
                           plus<int>());
    //10*(vec[0]+vec[4])*(vec[1]*vec[5])*(vec[2]*vec[6])

    //? 3.partial sum 累加
    vector<int> vec2(vec);
    partial_sum(vec.begin(),vec.end(),vec2.begin());
    /* 
    vec2[0] = vec[0]
    vec2[1] = vec[0] + vec[1]
    vec2[2] = vec[0] + vec[1]+vec[2]
     */
    partial_sum(vec.begin(),vec.end(),vec2.begin(),multiplies<int>());

    //? 4.adjacent difference
    adjacent_difference(vec.begin(),vec.end(),vec2.begin());
    /* 
    vec2[0] = vec[0]
    vec2[1] = vec[1] - vec[0]
    vec2[2] = vec[2] - vec[1]
     */
    adjacent_difference(vec.begin(),vec.end(),vec2.begin(),plus<int>());
    /* 
    vec2[0] = vec[0]
    vec2[1] = vec[1] + vec[0]
    vec2[2] = vec[2] + vec[1]
     */

    system("pause");
    return 0;
}

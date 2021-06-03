#include<iostream>
#include<queue>
#include<stack>

using namespace std;
/* 
* 容器适配器 container adaptor
* 1.stack: LIFO,push(),pop(),top()
* 2.queue: FIFO,push(),pop(),front(),back()
* 3.priority queue: first item always has the greatest priority
*                   push(),pop(),top()
 */

/* 
* another way of categorizing containers
* 1.array based containers:vector,deque
* 2.Node base containers:list+associative containers+unordered containers
* array based containers 存在 指针无效的问题

 */
int main()
{
    
        
    vector<int> vec = {1,2,3,4};
    //int *p = &vec[2]; // p points to 3
    vec.insert(vec.begin(),0);
    //!数组型的容器存在指针失效问题，p此时指向一个未知的空间，因此不用这种方式
    //cout<<*p<<endl; //2, or ???
    
    system("pause");
    return 0;
}
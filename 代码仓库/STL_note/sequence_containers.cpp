#include<iostream>
//STL Headers
#include<vector>
#include<deque>
#include<list>
#include<set> //set and multiset
#include<map> //map and multimap
#include<unordered_map> // unordered set/multiset
#include<unordered_set> // unordered map/multimap
#include<iterator>
#include<algorithm>
#include<numeric> // some numeric algorithm
#include<functional>
using namespace std;

/* 
? 通用容器的分类
* STL 对定义的通用容器分三类：顺序性容器、关联式容器和容器适配器。
* 1、顺序性容器是一种各元素之间有顺序关系的线性表，是一种线性结构的可序群集。顺序性容器中的每个元素均有固定的位置，
*    除非用删除或插入的操作改变这个位置。顺序性容器不会根据元素的特点排序而是直接保存了元素操作时的逻辑顺序。
* 2、关联式容器是非线性的树结构，更准确的说是二叉树结构。各元素之间没有严格的物理上的顺序关系，
*    也就是说元素在容器中并没有保存元素置入容器时的逻辑顺序。但是关联式容器提供了另一种根据元素特点排序的功能，
*    这样迭代器就能根据元素的特点“顺序地”获取元素。
*    关联式容器另一个显著的特点是它是以键值的方式来保存数据，就是说它能把关键字和值关联起来保存，
* 而顺序性容器只能保存一种（可以认为它只保存关键字，也可以认为它只保存值）。
* 容器适配器是一个比较抽象的概念,通过容器接口实现队列、栈、优先级队列等数据结构
? 具体分类
 顺序性容器(sequence containers)(用数组和链表实现)
 1.vector  从后面快速的插入与删除，直接访问任何元素
 2.deque   从前面或后面快速的插入与删除，直接访问任何元素
 3.list    双链表，从任何地方快速插入与删除

 关联容器(assotiative containers)(用二叉树实现)
 !关联容器总是有顺序的
 1.set      快速查找，不允许重复值
 2.multiset 快速查找，允许重复值
 3.map      一对多映射，基于关键字快速查找，不允许重复值
 4.multimap 一对多映射，基于关键字快速查找，允许重复值

 无序关联容器(unordered associative containers)(用哈希表实现)
 !其与关联容器的区别是
 1.unordered set/multiset;
 2.unordered map/multimap;

 容器适配器(container adaptor)
 1.stack            后进先出
 2.queue            先进先出
 3.priority_queue   最高优先级元素总是第一个出列
 */
int main()
{
    //? 一、 vector  （向量是一个能够存放任意类型的动态数组,仅可以在队尾增删元素）
    //初始化
    vector<int> vec; //vec.size() == 0
    vector<int> p(vec); //复制vec到p中
    vector<int> r = {4,2,3}; //类似于数组的初始化
    vector<int> v(5,0); // 初始化5个元素且初值为0
    //在队尾添加元素
    vec.push_back(4);
    vec.push_back(1);
    vec.push_back(8); // vec:{4,1,8} vec.size() == 3
    //vector的特殊操作
    cout<<vec[2];  // 8 (不会进行范围检查)可以使用下标索引
    cout<<vec.at(2); // 8 (会抛出溢出异常)
    //vector的遍历
    for(int i=0;i<vec.size();i++){
        cout<<vec[i]<<" ";
    }
    // 相比第一种遍历方式更推荐这种，因为它利用了迭代器，即该法可以用来遍历任意容器的元素
    for(vector<int>::iterator it=vec.begin();it!=vec.end();it++)
    {
        cout<<*it<<" ";
    }
    // 该法就更简单了
    for(int i:vec){
        cout<<i<<" ";
    }
    
    //* 对于所有容器都通用的成员函数
    // vec:{4,1,8}
    if(vec.empty()){cout<<"Not possible.";}

    cout<<vec.size(); //3

    vector<int> vec2(vec); //复制构造函数,vec2:{4,1,8}

    vec.clear(); //删除vec中所有元素

    vec2.swap(vec); //交换vec与vec2

    /* 
    * vector的特性
    * 1.fast insert/remove at the end:O(1)
    * 2.slow insert/remove ate the begining or in the middle:O(n)
    * 3.slow search:O(n)
    * 4.指定一块如同数组一样的连续存储，但空间可以动态扩展。即它可以像数组一样操作，并且可以进行动态操作
    * 5.随机访问方便，它像数组一样被访问，即支持[]操作符和vector.at()
    * 6.节省空间，因为它是连续存储，在存储数据的区域都是没有被浪费的，但是要明确一点vector大多情况下并不是满存的，在未存储的区域实际是浪费的
    * 7.在内部进行插入、删除操作效率非常低，这样的操作基本上是被禁止的
    * 8.当动态添加的数据超过vector 默认分配的大小时要进行内存的重新分配、拷贝与释放，这个操作非常消耗性能。 
    * 所以要vector 达到最优的性能，最好在创建vector时就指定其空间大小。
     */

    //? 二、 list 双向链表
    list<int> mylist = {5,2,9};
    mylist.push_back(6); //mylist:{5,2,9,6}
    mylist.push_front(4);//mylist:{4,5,2,9,6}
    
    list<int>::iterator itr = find(mylist.begin(),mylist.end(),2); //itr->2
    mylist.insert(itr,8); //mylist:{4,5,8,2,9,6}; O(1),faster than vector and deque
    ++itr; // itr->9
    mylist.erase(itr); //删除itr所指向的元素，mylist:{4,5,8,2,6} O(1)
    //! list有一个其他容器都没有的牛逼方法，它可以仅耗费O(1)的时间不需要拷贝的将两个list合并到一个中取
    list<int> mylist2 = {3,4,5};
    mylist.splice(mylist.begin(),mylist2);
    /* 
    * list的特性
    * 1.fast insert/remove at any place:O(1)
    * 2.slow search:O(n)
    * 3.不使用连续的内存空间这样可以随意地进行动态操作
    * 4.可以在内部任何位置快速地插入或删除，当然也可以在两端进行push和pop
    * 5.不能进行内部的随机访问，即不支持[]操作符和vector.at()
    
     */
    //? 三、 deque 双向队列(可以在队尾和队头增删元素)
    deque<int> deq = {4,6,7};
    deq.push_back(3);  // deq:{4,6,7,3}
    deq.push_front(2); // deq:{2,4,6,7,3}
    

    //deque has similar interface with vector
    cout<<deq[1];  // 4
    /* 
    * deque的特性
    * 1.fast insert/remove at the begining and the end
    * 2.slow insert/remove in the middle:O(n)
    * 3.slow search:O(n)
    * 4. 随机访问方便，即支持[]操作符和vector.at() ，但性能没有vector好
    * 5.可以在内部进行插入和删除操作，但性能不及list
    * 6.可以在两端进行push、pop ；
     */

    //? 三者的比较
    /* 
    * vector是一段连续的内存块，而deque是多个连续的内存块,list 是所有数据元素分开保存，可以是任何两个元素没有连续。
    * vector的查询性能最好，并且在末端增加数据也很好，除非它重新申请内存段；适合高效地随机存储。
    * list是一个链表，任何一个元素都可以是不连续的，但它都有两个指向上一元素和下一元素的指针。
    * 所以它对插入、删除元素性能是最好的，而查询性能非常差；适合大量地插入和删除操作而不关心随机存取的需求。
    * deque是介于两者之间，它兼顾了数组和链表的优点，它是分块的链表和多个数组的联合。
    * 所以它有比list好的查询性能，有比vector好的插入、删除性能。如果你需要随即存取又关心两端数据的插入和删除，那么deque 是最佳之选。
     */
    

    system("pause");
    return 0;
    
}
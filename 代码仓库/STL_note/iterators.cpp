#include<iostream>
#include<vector>
#include<list>
#include<forward_list>
#include<set>
#include<map>
using namespace std;
/* 
* Iterators
 */
int main()
{
    //? 各容器允许的迭代器操作
    //*1.Random Access Iterator:vector,deque,array
    vector<int> p={1,2,3,4,5,6};
    vector<int>::iterator itr=p.begin();
    itr = itr + 5; //advance itr by 5
    cout<<*itr<<endl;  // 6
    itr = itr - 4;
    cout<<*itr<<endl; //2

    //*2.Bidirectional Iterator:list,set/multiset,map/multimap
    list<int> p2 = {1,2,3};
    list<int>::iterator itr2 = p2.begin();
    ++itr2; 
    cout<<*itr2<<endl; //2
    --itr2;
    cout<<*itr2<<endl; //1

    //*3.Forward Iterator:forward_list
    forward_list<int> p3 = {1,2,3};
    auto itr3 = p3.begin();
    ++itr3;
    cout<<*itr3<<endl; //2
    //--itr3; error!

    //? Every containers has a iterator and a const iterator
    set<int>::iterator itr4;
    set<int>::const_iterator citr4; //ready_only access to container elements

    set<int> set1 = {2,4,5,1,9};
    for(citr4=set1.begin();citr4!=set1.end();++citr4)
        cout<<*citr4<<" ";
    // *citr4 = 3; const iterator无法修改！
    
    //? Iterator functions:
    advance(itr3,1); //  itr3+=1
    cout<<endl;
    cout<<*itr3<<endl; //3
    cout<<distance(itr3,itr3)<<endl; //计算两个迭代器之间的距离

    //? 1.Insert Iterator
    //TODO 不知道为什么find在这里会有问题，但是放到别的文件里允许是可以的
    /* vector<int> vec1 = {4,5}; 
    vector<int> vec2 = {12,14,16,18};
    vector<int>::iterator it = find(vec2.begin(),vec2.end(),16);
    insert_iterator<vector<int>> i_itr(vec2,it);
    copy(vec1.begin(),vec1.end(), //source
    i_itr); */                       //destination
                                  //vec2:{12,14,4,5,16,18}
    //other insert iterators:back_insert_iterator,front_insert_iterator

    //?2.Reverse Iterator
    vector<int> vec3 = {4,5,6,7};
    reverse_iterator<vector<int>::iterator> ritr;
    for(ritr=vec3.rbegin();ritr!=vec3.rend();++ritr)
        cout<<*ritr<<" "; // 7 6 5 4

    system("pause");
    return 0;
}
#include<iostream>
#include<unordered_set>
#include<unordered_map>
#include<vector>

using namespace std;
void foo(const unordered_map<char,string>& m);
/* 
*无序关联容器与关联容器的最大区别在于实现方式为哈希表
*使用哈希表会使得查找速度很快，O(1),(在避免冲突的情况下)
 */

int main()
{
    unordered_set<string> set1 = {"red","yellow","blue"};
    unordered_set<string>::iterator it = set1.find("green"); //O(1)
    if(it!=set1.end()) //若查找不成功则会让迭代器指向容器外的地址，因此检查很有必要！
        cout<<*it<<endl;
    set1.insert("yellow"); //O(1)
    vector<string> vec = {"purple","pink"};
    set1.insert(vec.begin(),vec.end());
    // Hash table specific APIs:
    cout<<"load_factor="<<set1.load_factor()<<endl;//装填因子=填入表中的记录数/散列表长度
                                                   //装填因子越大，发生冲突的可能性就越大
    string x = "red";
    cout<<x<<"is in bucket #"<<set1.bucket(x)<<endl;
    cout<<"Total bucket #"<<set1.bucket_count()<<endl;
    
    /* 
    unordered multiset:unordered set that allows duplicated elements
    unordered map:unordered set of pairs
    unordered multimap:unordered map that allows duplicated keys
    hash collision -> performance degrade
     */
    /* 
    *properties of unordered containers
    * 1.fastest search/insert at any place:O(1)
    *   associative container takes O(log(n))
    *   vector deque takes O(n)
    *   list takes O(1) to insert,O(n) to search
    * 2.unordered set/multiset:element value cannot be changed
    *   unordered map/multimap:element key cannot be changed
    
     */

    /* 
    实现关联数组associative array(实现方式：map 或 unordered map)
     */
    unordered_map<char,string> day = {{'S',"Sunday"},{'M',"Monday"}};

    cout<<day['S']<<endl; //no range check
    cout<<day.at('S')<<endl; //has range check

    vector<int> vec2 = {1,2,3};
    // vec2[5] = 5; compile error
    day['W'] = "wednesday"; //Inserting{'W',"Wednesday"}
    day.insert(make_pair('F',"Friday")); //Inserting{'F',"Friday"}

    day.insert(make_pair('M',"MONDAY")); //fail to modify,because 'M' has existed
    //cout<<day['M']<<endl;
    day['M'] = "MONDAY"; // succeed to modify
    //cout<<day['M']<<endl;
    
    /* 
    Notes：
    1.search time:unordered_map,O(1);map:O(log(n))
    2.unordered_map may degrade to O(n)
    3.cannot use multimap and unordered_multimap,they donot have[]operator
     */
    

    system("pause");
    return 0;
}
/* 
void foo(const unordered_map<char,string>& m)
    {
        // m['S'] = "SUNDAY" 是错误的，因为const声明m是不可改变的
        // cout<<m['S']<<endl 这种打印方式也是错误的，需要改为下面的方式
        auto itr = m.find('S');
        if(itr != m.end())
            cout << *itr << endl;
    } */
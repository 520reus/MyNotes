#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;
/* 
* STL Algorithm walkthrough
* Non-modifying Algorithms
*--- count,min,max,compare,linear search,attribute
 */
int main()
{
    vector<int> vec = {4,2,5,1,3,9};
    // count_if()用来统计容器中满足条件的元素个数
    // 对于简单的函数可以用匿名函数：lambda function
    int num = count_if(vec.begin(),vec.end(),[](int x){return x<5;}); //4
    
    vector<int> vec2 = {9,60,90,8,45,87,90,69,69,55,7};
    vector<int>::iterator itr,itr2;
    pair<vector<int>::iterator,vector<int>::iterator> pair_of_itr;

    //? 1.counting 统计满足条件的元素个数
    // Algorithm       Data          Operation
    int n = count(vec2.begin(),vec2.end(),69); //与69相等的元素有2个
    int m = count_if(vec2.begin(),vec2.end(),[](int x){return x>5;}); //大于5的元素有11个

    //? 2.min and max 
    itr = max_element(vec2.begin(),vec2.end()); //返回最大的数的位置，若有多个最大值，返回第一个位置
    itr = max_element(vec2.begin(),vec2.end(),[](int x,int y){return (x%10)<(y%10);}); //返回满足条件的最大值的位置，9
    itr = min_element(vec2.begin(),vec2.end()); //7
    // return a pair,which contains first of min and last of max
    pair_of_itr = minmax_element(vec2.begin(),vec2.end(),[](int x,int y){return (x%10)<(y%10);});
    cout<<*pair_of_itr.first<<endl; 
    cout<<*pair_of_itr.second<<endl;

    //? linear searching(used when data is not sorted,else use BST)
    //* returns the first match position
    //返回第一个等于55的迭代器，用*运算符得到该值
    itr = find(vec2.begin(),vec2.end(),55);
    
    //返回第一个大于80的迭代器
    itr = find_if(vec2.begin(),vec2.end(),[](int x){return x>80;});
    cout<<*itr<<endl;
    //返回第一个不大于80的迭代器
    itr = find_if_not(vec2.begin(),vec2.end(),[](int x){return x>80;});
    //返回第一个连续2个都是69的迭代器
    itr = search_n(vec2.begin(),vec2.end(),2,69);

    //? search subrange 找到第一个匹配子序列的位置
    vector<int> sub = {45,87,90};
    // search fist subrange
    itr = search(vec2.begin(),vec2.end(),sub.begin(),sub.end());
    // search last subrange
    itr = find_end(vec2.begin(),vec2.end(),sub.begin(),sub.end());

    //? search any_of 返回找到第一个items中任意元素的位置，由于87在69前面，故返回87的位置
    vector<int> items = {36,87};
    itr = find_first_of(vec2.begin(),vec2.end(),items.begin(),items.end());
    // search any one of the item in items that satisfy:x==y*4
    //itr = find_first_of(vec2.begin(),vec2.end(),items.begin(),items.end(),[](int x,int y){return x==y*4;});
    
    //? search adjacent 
    itr = adjacent_find(vec2.begin(),vec2.end()); // find two adjacent items that are same
    //itr = adjacent_find(vec2.begin(),vec2.end()),[](int x,int y){return x==y*4;};
    
   //?4.comparing ranges
   if(equal(vec2.begin(),vec2.end(),vec.begin())){
       cout<<"vec and vec2 are same.\n";
   }

   if(is_permutation(vec.begin(),vec.end(),vec2.begin())){
       cout<<"vec and vec2 have same items,but in diffrent order.\n";
   }

    pair_of_itr = mismatch(vec.begin(),vec.end(),vec2.begin());
    // find first difference
    // pair_of_itr.fist is an iterator of vec
    // pair_of_itr.second is an iterator of vec2

    //Lexicographical comparison:one-by-one comparison with"less than"
    lexicographical_compare(vec.begin(),vec.end(),vec2.begin(),vec2.end());
    //{1,2,3,5} < {1,2,4,5}
    //{1,2} < {1,2,3}

    //? check attributes
    //check if vec is sorted
    is_sorted(vec.begin(),vec.end()); 
    // itr points to first place to where elements are no longer sorted
    itr = is_sorted_until(vec.begin(),vec.end());
    // check if vec is partitioned according to the condition of(x>80)
    is_partitioned(vec.begin(),vec.end(),[](int x){return x>80;});
    // check if vec is heap
    is_heap(vec.begin(),vec.end());
    // find the first place where it is no longer a heap
    itr = is_heap_until(vec.begin(),vec.end());

    //? All,any,none
    // if all of vec is bigger than 5
    all_of(vec.begin(),vec.end(),[](int x){return x>5;});
    // if any of vec is bigger than 5
    any_of(vec.begin(),vec.end(),[](int x){return x>5;});
    // if none of vec is bigger than 5
    none_of(vec.begin(),vec.end(),[](int x){return x>5;});



   

    system("pause");
    return 0;
}


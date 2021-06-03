
#include<iostream>
#include<set>
#include<map>


using namespace std;
/* 
* set, multiset, map, multimap 是一种非线性的树结构，具体的说采用的是一种比较高效的特殊的平衡检索二叉树——红黑树结构。
* 因为关联容器的这四种容器类都使用同一原理，所以他们核心的算法是一致的，但是它们在应用上又有一些差别
* set
* 又称集合，实际上就是一组元素的集合，但其中所包含的元素的值是唯一的，且是按一定顺序排列的，集合中的每个元素被称作
* 集合中的实例。因为其内部是通过链表的方式来组织，所以在插入的时候比vector快，但在查找和末尾添加上比vector慢。
* multiset
* 是多重集合，其实现方式和set 是相似的，只是它不要求集合中的元素是唯一的，也就是说集合中的同一个元素可以出现多次。
* map
* 提供一种“键- 值”关系的一对一的数据存储能力。其“键”在容器中不可重复，且按一定顺序排列
* 由于其是按链表的方式存储，它也继承了链表的优缺点。
* multimap 
* 和map的原理基本相似，它允许“键”在容器中可以不唯一。

? 关联容器的特点是明显的，相对于顺序容器，有以下几个主要特点：
*1， 其内部实现是采用非线性的二叉树结构，具体的说是红黑树的结构原理实现的；
*2， set 和map 保证了元素的唯一性，mulset 和mulmap 扩展了这一属性，可以允许元素不唯一；
*3， 元素是有序的集合，默认在插入的时候按升序排列。

? 基于以上特点，
*1， 关联容器对元素的插入和删除操作比vector 要快，因为vector 是顺序存储，而关联容器是链式存储；比list 要慢，
*是因为即使它们同是链式结构，但list 是线性的，而关联容器是二叉树结构，其改变一个元素涉及到其它元素的变动比list 要多，
*并且它是排序的，每次插入和删除都需要对元素重新排序；
*2， 关联容器对元素的检索操作比vector 慢，但是比list 要快很多。vector 是顺序的连续存储，当然是比不上的，
*但相对链式的list 要快很多是因为list 是逐个搜索，它搜索的时间是跟容器的大小成正比，而关联容器 查找的复杂度基本是Log(N) ，
*比如如果有1000 个记录，最多查找10 次，1,000,000 个记录，最多查找20 次。容器越大，关联容器相对list 的优越性就越能体现；
*3， 在使用上set 区别于vector,deque,list 的最大特点就是set 是内部排序的，这在查询上虽然逊色于vector ，但是却大大的强于list 。
*4， 在使用上map 的功能是不可取代的，它保存了“键- 值”关系的数据，而这种键值关系采用了类数组的方式。
*数组是用数字类型的下标来索引元素的位置，而map 是用字符型关键字来索引元素的位置。在使用上map 也提供了一种类数组操作的方式，
*即它可以通过下标来检索数据，这是其他容器做不到的，当然也包括set 。
*（STL 中只有vector 和map,array 可以通过类数组的方式操作元素，即如同ele[1] 方式）
 */

int main()
{
    /* 
    associative container
    always sorted,default criteria is <
    No push_back(),push_front()
    element value can not be changed because this will break the "sorted"
     */
    //? 1.set
    set<int> set1;
    set1.insert(3); //set1:{3}
    set1.insert(1); //set1:{1,3}
    set1.insert(7); //set1:{1,3,7},树的插入速度很快，O(log(n))

    set<int>::iterator it;
    it = set1.find(7); // 查找的速度也是O(log(n))，it points to 7
                       // 顺序式容器甚至没有find()方法，因为查找的速度远不如关系型容器
    pair<set<int>::iterator,bool> ret;
    ret = set1.insert(3); //set已经有3,因此插入不成功，ret.second==false
    if(ret.second == false)
        it = ret.first;    // it now points to element 3
    set1.insert(it,9); //!这里it参数不是告诉在哪个位置插入9这个元素，因为这会破坏有序型。而是为了更快的找到插入位置(hint)
                       //!这样传参的好处是提升插入速度：O(log(n)->O(1)
                       //set1:{1,3,7,9},it points to 3;
    set1.erase(it);  //set1:{1,7,9} 
                     //*这种以迭代器的删除方法与顺序性容器相同
    set1.erase(7);   //set1:{1,9}  
          lessThan10           //*但是这种以value的方式删除就是set独有的
    //? 2.multiset
    //* 与set唯一不同的一点是multiset允许出现重复的元素
    multiset<int>set2;
    //* set/multiset: value of the elements cannot be modified
    // *it = 10; error! *it is read-only
    /* 
    * set/multiset特性
    * 1.Fast search:O(log(n))
    * 2.Traversing is slow(compare to vector & deque,树的结构更复杂)
    * 3.No random access,no [] operator
     */
    //? 3.map (No duplicated key)
    map<char,int> map1;
    map1.insert(pair<char,int>('a',100));
    map1.insert(pair<char,int>('z',200));
    typedef pair<char,int> p; //pair因为太长一般与typedef联用
    map<char,int>::iterator itr = map1.begin();
    map1.insert(itr,p('b',300)); //!和set一样，map中的insert方法传入迭代器参数是为了更快的插入

    itr = map1.find('z'); //O(log(n))
    // showing contents:
    for(itr=map1.begin();itr!=map1.end();itr++)
    {
        cout<<(*itr).first<<"->"<<(*itr).second<<endl;
    }
    //? 4.multimap(allows duplicated keys)
    multimap<char,int> map2;
    //*与set/multiset一样， map/multimap的元素值也不能改变，一样会破坏有序性
    
    system("pause");
    return 0;
}



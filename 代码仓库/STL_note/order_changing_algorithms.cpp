#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;
/* 
* 本文介绍会改变元素顺序的模板库算法
* order-changing algorithm - change the element's order
* -reverse,rotate,permute,shuffle
 */
int main()
{
    vector<int> vec = {9,60,70,8,45,87,90}; //7 items
    vector<int>vec2(7,0);  // 7 items

    //? 1.reverse
    // 反转给定范围的元素顺序
    reverse(vec.begin()+1,vec.end()-1); //vec:{9,87,45,8,70,60,90}
    // 先反转vec再复制到vec2，但vec不变
    reverse_copy(vec.begin(),vec.end(),vec2.begin());//vec2:{87,45,8,70,60,0,0}
    
    //? 2.rotate
    // 将vec.begin()+3开始的元素与它前面的元素换位
    rotate(vec.begin(),vec.begin()+3,vec.end()); //vec:{8,45,87,90,9,60,70}
    //先翻转再复制
    rotate_copy(vec.begin(),vec.begin()+3,vec.end(), //source
                vec2.begin());                       //destination
    //? 3.permute

    //? 4.shuffle(洗牌)
    // 重新任意排列元素
    random_shuffle(vec.begin(),vec.end());
    
    system("pause");
    return 0;
}
#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;
/* 
* 本文介绍会改变元素值的标准模板库算法
* value-changing algorithm - change the element values
* copy,move,transform,swap,fill,replace,remove
 */
int main()
{
    vector<int> vec = {9,60,70,8,45,87,90}; //7 items
    vector<int> vec2(11,0); // 11 items
    vector<int>::iterator itr,itr2;
    pair<vector<int>::iterator,vector<int>::iterator> pair_of_itr;

    //? 1.copy
    //将vec中的所有元素复制到vec2中。copy共接收3个参数，前两个参数为数值范围，第三个参数指明复制到的位置
    copy(vec.begin(),vec.end(), //source
         vec2.begin());         //destination
    //将vec中满足条件的值复制到vec2中
    copy_if(vec.begin(),vec.end(), //source
            vec2.begin(),          //destination
            [](int x){return x>80;}); //condition
    //将vec的前4个数复制到vec2中
    copy_n(vec.begin(),4,vec2.begin());
    //将vec中的值复制到vec2的后面
    copy_backward(vec.begin(),vec.end(), //source
                  vec2.begin());         //destinaton
    //vec2:{0,0,0,0,9,60,70,8,45,87,90}

    //? 2.move
    vector<string> vec3 = {"apple","orange","pear","grape"};//4 items
    vector<string> vec4(6,""); // 6 items

    move(vec3.begin(),vec3.end(),vec4.begin());
    //vec3:{"","","",""}
    //vec4:{"apple","orange","pear","grape","",""};

    move_backward(vec3.begin(),vec3.end(),vec4.end());
    //vec4:{"","","apple","orange","pear","grape"}

    //? 3.transform
    vector<int> vec5(vec); //{9,60,70,8,45,87,90}; 7 items
    vector<int> vec6(vec); //{9,60,70,8,45,87,90}; 7 items
    vector<int> vec7(11,0);// 11 items
    //将vec5中的每个元素都减1再复制到vec7中，vec5的元素不变！
    transform(vec5.begin(),vec5.end(), //source
              vec7.begin(),          //destination
              [](int x){return x-1;}); //operation

    transform(vec5.begin(),vec5.end(), //source1
              vec6.begin(),            //source2
              vec7.begin(),            //destination
              [](int x,int y){return x+y;}); //operation
    // add items from vec5 and vec6 and save in vec7
    // vec7[0] = vec5[0] + vec6[0];
    // vec7[1] = vec5[1] + vec6[1];...

    //? 4.swap 交换两个容器
    swap_ranges(vec5.begin(),vec5.end(),vec2.begin());

    //? 5.fill 填充值
    vector<int> vec8(5,0);

    fill(vec8.begin(),vec8.end(),9); //vec8:{9,9,9,9,9}

    fill_n(vec8.begin(),3,9); //vec8:{9,9,9,0,0}
    
    generate(vec8.begin(),vec8.end(),rand);

    generate_n(vec8.begin(),3,rand);

    //? 6.replace
    // 把vec中的所有6都替换成9
    replace(vec.begin(),vec.end(), //data range
            6,                     //old value condition
            9);                    //new value
    //把vec中大于80的值都替换成9
    replace_if(vec.begin(),vec.end(), //data range
               [](int x){return x>80;}, //old value condition
               9);                                  //new value
    //把vec中的等于9的元素都替换成6再复制到vec2中，vec中的元素不变！
    replace_copy(vec.begin(),vec.end(), //source
                 vec2.begin(),          //destination
                 9,                     //old value condition
                 6);                    //new value
    
    //? 7. remove
    // 把vec中所有等于3的值全部删掉
    remove(vec.begin(),vec.end(),3);
    // 把vec中大于80的值全部删掉
    remove_if(vec.begin(),vec.end(),[](int x){return x>80;});
    // 把vec中等于6的值全部删掉，再把剩余的值复制到vec2中，且vec中的值保持不变！
    remove_copy(vec.begin(),vec.end(), //source
                vec2.begin(),          //destination
                6);                    //condition
    
    system("pause");
    return 0;
}

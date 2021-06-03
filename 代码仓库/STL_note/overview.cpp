
#include<iostream>
using namespace std;
/* 
* STL：Standard Template Libarary
* ————Data Structure and Algorithms

* 标准模板库中包括：容器(containers)、迭代器(Iterator)、算法(Algorithm)
* 其中迭代器连接着容器与算法，容器即数据结构。为了实现每个容器都能利用相同的算法，需要迭代器(类似指针）作为中介。

* 模板是泛型编程的基础，泛型编程即以一种独立于任何特定类型的方式编写代码。
* 模板是创建泛型类或函数的蓝图或公式。库容器，比如迭代器和算法，都是泛型编程的例子，它们都使用了模板的概念。
* 每个容器都有一个单一的定义，比如向量，我们可以定义许多不同类型的向量，比如 vector <int> 或 vector <string>。

*要访问顺序容器和关联容器中的元素，需要通过“迭代器（iterator）”进行。迭代器是一个变量，相当于容器和操纵容器的算法之间的中介。
*迭代器可以指向容器中的某个元素，通过迭代器就可以读写它指向的元素。从这一点上看，迭代器和指针类似。
* 您可以使用模板来定义函数和类，接下来让我们一起来看看如何使用。
 */
template<typename T>
T  square(T x)
{
    return x*x;
}
template<typename T>
class BoVector{
    private:
        T arr[1000];
        int size;
    public:
        BoVector():size(0){} //初始化size
        void push(T x)   //增
        {
            arr[size] = x;
            size++;
        }
        T get(int i)    //查数据
        {
            return arr[i];
        }
        int getSize()    //查大小
        {
            return size;
        }
        void print()    //打印
        {
            for(int i=0;i<size;i++)
            {
                cout<<arr[i]<<" ";
            }
        }
};
// 重载*操作符，用于两个类对象之间数据的乘法运算
template<typename T>
BoVector<T> operator*(BoVector<T>& r1,BoVector<T>& r2)
{
    BoVector<T> ret;
    for(int i=0;i<r1.getSize();i++)
        ret.push(r1.get(i)*r2.get(i));
    return ret;
}
template<typename T>
bool operator==(BoVector<T>& r1,BoVector<T>& r2)
{
    if(r1.getSize() != r2.getSize())
        return false;
    for(int i=0;i<r1.getSize();i++)
    {
        if(r1.get(i) != r2.get(i))
            return false;
    }
    return true;
}
template<typename T>
bool operator<(BoVector<T>& r1,BoVector<T>& r2)
{
    if(r1.getSize()<r2.getSize())
        return true;
    return false;
}
template<typename T>
bool operator>(BoVector<T>& r1,BoVector<T>& r2)
{
    if(r1.getSize()>r2.getSize())
        return true;
    return false;
}

template<typename T>
void compare(BoVector<T>& r1,BoVector<T>& r2)
{
    if(r1>r2)
        cout<<"第一个比第二个大"<<endl;
    if(r1<r2)
        cout<<"第一个比第二个小"<<endl;
    if(r1==r2)
        cout<<"第一个与第二个相等"<<endl;
}



int main()
{
    //cout<<square(5)<<endl; //*函数模板可以根据从参数类型推断T的类型
    //cout<<square(1.5)<<endl;
    BoVector<int> bv;      //*但是类必须先声明参数类型
    bv.push(2);
    bv.push(3);
    bv.push(5);
    bv.push(4);
    BoVector<int> bv2;
    bv2.push(2);
    bv2.push(3);
    bv2.push(2);
    //bv.print();
    bv = square(bv);
    bv.print();
    compare(bv,bv2);
    system("pause");
    return 0;
}

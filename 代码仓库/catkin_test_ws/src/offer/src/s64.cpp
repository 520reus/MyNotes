#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <stack>
#include <deque>

using std::cout; using std::endl; 
using std::vector;
using std::deque;

//求1+2+...n，要求不能使用乘除法，for、while、if、else、switch、case等关键字及条件判断语句（A?B:C）

//*1.利用构造函数求解
class Temp
{
    public:
        Temp()
        {
            N++;
            Sum += N;
        }

        static unsigned int GetSum(){return Sum;}

        static void Reset(){N = 0; Sum = 0;}

    private:
        static unsigned int N;
        static unsigned int Sum;
};

unsigned int Temp::N = 0;
unsigned int Temp::Sum = 0;

unsigned int Sum_Solution1(unsigned int n)
{
    Temp::Reset();
    Temp* t = new Temp[n];

    if(t != nullptr)
    {
        delete[] t;
        t = nullptr;
    }

    return Temp::GetSum();
        
}

//*2.利用虚函数求解
class A;
A* Array[2];

class A
{
    public:
        virtual unsigned int sum(unsigned int n)
        {
            return 0;
        }

    private:
        unsigned int n;
};

class B : public A
{
    public:
        unsigned int sum(unsigned int n)
        {
            return Array[!!n]->sum(n-1) + n;
        }
};
 
unsigned int Sum_Solution2(unsigned int n)
{
     A a;
     B b;
     Array[0] = &a;
     Array[1] = &b;

     unsigned int value = Array[1]->sum(n);
     return value;

}

//*3. 利用函数指针求解
typedef unsigned int (*fun)(unsigned int);

unsigned int Solution3_Terminator(unsigned int n)
{
    return 0; 
}

unsigned int Sum_Solution3(unsigned int n)
{
    static fun f[2] = {Solution3_Terminator,Sum_Solution3};
    return n + f[!!n](n-1);
}



int main(int argc, char** argv)
{
    unsigned int n = 10;
    cout << "sum of " << n << " = " << Sum_Solution1(10) << endl;
    cout << "sum of " << n << " = " << Sum_Solution2(10) << endl;
    cout << "sum of " << n << " = " << Sum_Solution3(10) << endl;
    return 0;
}
#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

//搜索小数部分是否有数字
bool scanUnsignedInteger(const char** str)
{
    const char* before = *str;
    while(**str != '\0' && **str >= '0' && **str <= '9')
    {
        ++(*str);
    }
    return *str > before;
}

//搜索整数以及指数部分是否有数字
bool scanInteger(const char** str)
{
    if(**str == '+' || **str == '-')
        ++(*str);
    
    return scanUnsignedInteger(str);
}


bool numeric(const char* str)
{
    if(str == nullptr)
        return false;
    
    bool numeric = scanInteger(&str);

    //若存在小数
    if(*str == '.')
    {
        ++str;

        //* 这里用 || 的原因
        // 1.可以没有整数 ，即前者为true，后者为false
        // 2.可以没有小数，即前者为false，后者为true
        // 3.即有整数部分，又有小数部分，即两者都为true
        numeric = scanUnsignedInteger(&str) || numeric;
    }

    if(*str == 'e' || *str == 'E')
    {
        ++str;
        //* 这里用 && 的原因
        // 1.当e或E前面没有数字时，整个字符串不能表示数字，如.e1,e1
        // 2.当e后E后面没有整数时，整个字符串不能表示数字，如12e，12e+5.4
        numeric = scanInteger(&str) && numeric;
    }

    return numeric && *str == '\0';
}

//这里传入指针，形参赋值出一个新的指针，并且指向与传入指针指向相同，故函数体内可以对其操作，函数执行结束该指针被销毁。
void test(char* p)
{
    p++;
    cout << *p << endl;
}

int main(int argc, char** argv)
{
    //test
    char x[3] = {'0','1'};
    char* p = x;
    test(p);
    cout << *p << endl;

    //main
    char str[9] = {'-','1','2','.','5','E','+','8'};
    cout << numeric(str) << endl;
    return 0;
}
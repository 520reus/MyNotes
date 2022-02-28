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

//字符串转整型,自己实现atoi，需考虑空指针nullptr、空字符串"",正负号、溢出等问题

enum Status {kValid,kInvalid};
int g_nStatus = kValid;

long long StrToIntCore(const char* str, bool minus)
{
    long long num = 0;

    while(*str != '\0')
    {
        if(*str >= '0' && *str <= '9')
        {
            int flag = minus ? -1 : 1;
            num = num * 10 + flag * (*str - '0');

            if((!minus && num > 0x7FFFFFFF) || (minus && num < (signed int)0x80000000))
            {
                num = 0;
                break;
            } 
            str++;
        }
        else
        {
            num = 0;
            break;   
        }
    }

    if(*str == '\0')
    {
        g_nStatus = kValid;
    }

    return num;
}

int StrToInt(const char* str)
{
    g_nStatus = kInvalid;
    long long num = 0;

    if(str != nullptr && *str != '\0')
    {
        bool minus = false;
        if(*str == '+')
        {
            str++;
        }
        if(*str == '-')
        {
            minus = true;
            str++;
        }
        if(*str != '\0')
        {
            num = StrToIntCore(str,minus);
        }
    }

    return (int) num;
    
}


int main(int argc, char** argv)
{
    char str[4] = {'1','5','7'};
    int a = StrToInt(str);
    cout << a << endl;
    return 0;
}
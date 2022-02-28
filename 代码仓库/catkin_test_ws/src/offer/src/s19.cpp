#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

bool matchCore(char* str, char* pattern)
{
    if(*str == '\0' && *pattern == '\0')
        return true;
    
    if(*str != '\0' && *pattern == '\0')
        return false;
    
    if(*(pattern+1) == '*')
    {
        //在字符匹配的前提下
        if(*pattern == *str || (*pattern == '.' && *str!='\0'))
            return matchCore(str+1,pattern+2) //a出现一次，比如 pattern = a*b, str = ab
                || matchCore(str+1,pattern)   //a出现多次,比如 pattern = a*b, str = aaaaab
                || matchCore(str,pattern+2);  // / *前字符出现0次，比如 pattern = a*ab, str = ab
        
        else
            // 当当前字符不匹配且匹配的下一个字符是*时，忽略*前的字符，继续往后移动与当前字符匹配
            // 比如 pattern = b*ab, str = ab
            return matchCore(str,pattern+2);
    }
    
    if(*str == *pattern || (*pattern == '.' && *str != '\0'))
        return  matchCore(str+1,pattern+1);
    
    return false;
}

bool match(char* str, char* pattern)
{
    if(str == nullptr || pattern == nullptr)
        return false;
    
    return matchCore(str,pattern);
}

int main(int argc, char** argv)
{

    char str[4] = {'a','b','c'};
    char pattern[4] = {'a','.','c'};
    cout << match(str,pattern) << endl;
    return 0;
}
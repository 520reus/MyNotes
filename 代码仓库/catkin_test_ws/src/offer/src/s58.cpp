#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <stack>

using std::cout; using std::endl;

//翻转字符串
//*1.翻转单词顺序
void Reverse(char* pBegin, char* pEnd)
{
    if(pBegin == nullptr || pEnd == nullptr)
        return;
    
    while(pBegin < pEnd)
    {
        char temp = *pBegin;
        *pBegin = *pEnd;
        *pEnd = temp;
        pBegin++;
        pEnd--;
    }
}

char* ReverseSentence(char* pData)
{
    if(pData == nullptr)
        return nullptr;
    
    char* pBegin = pData;
    char* pEnd = pData;

    while(*pEnd != '\0')
        pEnd++;
    pEnd--;

    //翻转整个句子
    Reverse(pBegin,pEnd);

    //翻转句子中的每个单词
    pBegin = pEnd = pData;
    while(*pBegin != '\0')
    {
        if(*pBegin == ' ')
        {
            pBegin++;
            pEnd++;
        }
        else if(*pEnd == ' ' || *pEnd == '\0')
        {
            Reverse(pBegin,--pEnd);
            pBegin = ++pEnd;
        }
        else
            pEnd++;
    }
    return pData;
}

//*2.左旋转字符串
char* LeftRotateString(char* pStr, int n)
{
    if(pStr == nullptr)
        return nullptr;
    
    int length = static_cast<int>(strlen(pStr));
    if(length > 0 && n > 0 && n < length)
    {
        char* pFirstStart = pStr;
        char* pFirstEnd = pStr + n - 1;
        char* pSecondStart = pStr + n;
        char* pSecondEnd = pStr + length - 1;

        //翻转字符串的前n个字符
        Reverse(pFirstStart,pFirstEnd);
        //翻转字符串的后面部分
        Reverse(pSecondStart,pSecondEnd);
        //翻转整个字符串
        Reverse(pFirstStart,pSecondEnd);
    }
    return pStr;
}

int main(int argc, char** argv)
{
    char str[9] = {'w','h','z','.',' ','d','s','g'};
    ReverseSentence(str);
    cout << str << endl;
    LeftRotateString(str,2);
    cout << str << endl;
    return 0;
}
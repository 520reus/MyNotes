#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

void Permutation(char* pStr, char* pBegin)
{
    if(*pBegin == '\0')
        cout << pStr << endl;
    
    else
    {
        char* pCh = pBegin;
        for(; *pCh != '\0'; pCh++)
        {
            char temp = *pBegin;
            *pBegin = *pCh;
            *pCh = temp;

            Permutation(pStr, pBegin + 1);
        
            temp = *pBegin;
            *pBegin = *pCh;
            *pCh = temp;
        }
    }
}

void Permutation(char* pStr)
{
    if(pStr == nullptr)
        return;
    
    Permutation(pStr,pStr);
}

int main(int argc, char** argv)
{
    char str[4] = {'a','b','c'};   
    Permutation(str);
    return 0;
}
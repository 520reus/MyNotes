#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>

using std::cout; using std::endl;

//*1.使用STL库的unordered_map实现哈希表，时间复杂度O(n)
char FindOnlyOnce(const std::string& str)
{
    std::unordered_map<char,int> charCounts;
    for(int i = 0; i < str.length(); i++)
    {
        charCounts[str[i]]++;
    }
    for(int i = 0; i < str.length(); i++)
    {
        if(charCounts[str[i]] == 1)
            return str[i];
    }
}

//* 2.自己实现简易哈希表
char FindOnlyOnce_solution2(char* pString)
{
    if(pString == nullptr)
        return '\0';
    
    const int tableSize = 256;
    unsigned int hashTable[tableSize];
    for(unsigned int i = 0; i < tableSize; i++)
        hashTable[i] = 0;

    char* pHashKey = pString;
    while(*(pHashKey) != '\0')
        hashTable[*(pHashKey)++]++;
    pHashKey = pString;
    while(*pHashKey != '\0')
    {
        if(hashTable[*pHashKey] == 1)
            return *pHashKey;
        
        pHashKey++;
    } 
    return '\0';
}

int main(int argc, char** argv)
{
    std::string str = "abaccdeff";
    cout << FindOnlyOnce(str) << endl;
    return 0;
}
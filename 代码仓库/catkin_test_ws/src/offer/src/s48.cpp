#include <iostream>
#include <ros/ros.h>
#include <string>
#include <algorithm>
#include <vector>

using std::cout; using std::endl;

int LongestSubstringWithoutDuplication(const std::string& str)
{
    int curLength = 0;
    int maxLength = 0;
    int* position = new int[26];
    for(int i = 0; i < 26; i++)
        position[i] = -1;

    for(int i = 0; i < str.length(); i++)
    {
        int preIndex = position[str[i] - 'a'];
        if(preIndex < 0 || i - preIndex > curLength)
            curLength++;

        else
        {
            if(curLength > maxLength)
                maxLength = curLength;
            
            curLength = i - preIndex;
        }

        position[str[i] - 'a'] = i;
    }
    if(curLength > maxLength)
        maxLength = curLength;

    delete[] position;
    return maxLength;
}

int main(int argc, char** argv)
{
    std::string str = "arabcacfr";
    cout << LongestSubstringWithoutDuplication(str) << endl;
    return 0;
}
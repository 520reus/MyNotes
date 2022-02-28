#include <iostream>
#include <ros/ros.h>
#include <algorithm>
#include <vector>


using std::cout; using std::endl;

bool g_InvalidInput = false;
int FindGreatestSumOfSubArray(int* pData, int nLength)
{
    if(pData == nullptr || nLength < 0)
    {
        g_InvalidInput = true;
        return 0;
    }

    g_InvalidInput = false;

    int nCurSum = 0;
    int nGreatestSum = 0;
    for(int i = 0; i < nLength; i++)
    {
        if(nCurSum <= 0)
            nCurSum = pData[i];
        
        else
            nCurSum += pData[i];
        
        if(nCurSum > nGreatestSum)
            nGreatestSum = nCurSum;

    }
    return nGreatestSum;
}

int main(int argc, char** argv)
{
    int length = 8;
    int arr[length] = {1,-2,3,10,-4,7,2,-5};
    cout << FindGreatestSumOfSubArray(arr,length) << endl;

    return 0;
}
#include <iostream>
#include <ros/ros.h>
#include <string>
#include <algorithm>
#include <vector>

using std::cout; using std::endl;

int getMaxValue_solution1(const int* values, int rows, int cols)
{
    if(values == nullptr || rows <= 0 || cols <=0)
        return 0;

    int** maxValues = new int*[rows]; 
    for(int i = 0; i < rows; i++)
    {
        maxValues[i] = new int[cols]; 
    }

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            int up = 0;
            int left = 0;
            if(i > 0)
                up = maxValues[i-1][j];
            if(j > 0)
                left = maxValues[i][j-1];

            maxValues[i][j] = std::max(up,left) + values[i * cols + j];
        }
    }

    int maxValue = maxValues[rows-1][cols-1];
    for(int i = 0; i < rows; i++)
    {
        delete[] maxValues[i]; 
    }
    delete[] maxValues;

    return maxValue;

}

//* 2.用一维数组，大小为列数，进一步优化空间效率
int getMaxValue_solution2(const int* values, int rows, int cols)
{
    if(values == nullptr || rows <= 0 || cols <=0)
        return 0;

    int* maxValues = new int[cols]; 

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            int up = 0;
            int left = 0;
            if(i > 0)
                up = maxValues[j];
            if(j > 0)
                left = maxValues[j-1];

            maxValues[j]= std::max(up,left) + values[i * cols + j];
        }
    }

    int maxValue = maxValues[cols-1];
    delete[] maxValues;

    return maxValue;

}


int main(int argc, char** argv)
{
    int arr[16] = {1,10,3,8,12,2,9,6,5,7,4,11,3,7,16,5};
    cout << getMaxValue_solution1(arr,4,4) << endl;
    cout << getMaxValue_solution2(arr,4,4) << endl;

    return 0;
}
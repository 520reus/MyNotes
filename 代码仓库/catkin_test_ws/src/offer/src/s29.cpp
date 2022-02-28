#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

void PrintNumber(int i)
{
    cout << i << " ";
}

void PrintMatrixInCircle(int** numbers, int colomns, int rows, int start)
{
    int EndX = colomns - 1 - start;
    int EndY = rows - 1 - start;

    //从左到右打印一行
    for(int i = start; i <= EndX; i++)
    {
        int number = numbers[start][i];
        PrintNumber(number);
    }
    //从上到下打印一行
    if(EndY > start)
    {
        for(int i = start+1; i <= EndY; i++)
        {
            int number = numbers[i][EndX];
            PrintNumber(number);
        }
    }
    //从右到左打印一行
    if(EndX > start && EndY > start)
    {
        for(int i = EndX - 1; i >= start; i--)
        {
            int number = numbers[EndY][i];
            PrintNumber(number);
        }
    }
    //从下到上打印一行
    if(EndX > start && EndY - 1 > start)
    {
        for(int i = EndY - 1; i >= start; i--)
        {
            int number = numbers[i][start];
            PrintNumber(number);
        }
    }
}

void PrintMatrixClockWise(int** numbers, int colomns, int rows)
{
    if(numbers == nullptr || colomns <= 0 || rows <= 0)
        return;
    
    int start = 0;
    while(rows > start*2 && colomns > start*2)
    {
        PrintMatrixInCircle(numbers,colomns,rows,start);
        start++;
    }
}

int main(int argc, char** argv)
{
    //申请内存存放二维数组
    int rows = 4;
    int colomns = 4;
    //int matrix[rows][colomns] = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};
    int** matrix = new int*[rows];
    for(int i = 0; i < rows; i++)
    {
        matrix[i] = new int[colomns];
    }
    int num = 1;
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < colomns; j++)
        {
            matrix[i][j] = num;
            num++;
        }

    PrintMatrixClockWise(matrix,rows,colomns);
    //删除内存
    for(int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;

    return 0;
}
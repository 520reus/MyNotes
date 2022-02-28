#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

int getDigitSum(int num)
{
    int sum = 0;
    while(num > 0)
    {
        sum += num % 10;
        num /= 10;
    }
    return sum;
}

bool Check(int threshold, int rows, int cols, int row, int col,bool* visited)
{
    if(row >= 0 && row < rows && col >= 0 && col < cols && 
       !visited[row*cols+col] && getDigitSum(row) + getDigitSum(col) <= threshold)
        return true;
    
    return false;
}

int Search(int threshold, int rows, int cols, int row, int col, bool* visited)
{
    int count = 0;
    if(Check(threshold,rows,cols,row,col,visited))
    {
        visited[row*cols + col] = true;
        count = 1 + Search(threshold,rows,cols,row,col-1,visited) + 
                    Search(threshold,rows,cols,row,col+1,visited) + 
                    Search(threshold,rows,cols,row-1,col,visited) + 
                    Search(threshold,rows,cols,row+1,col,visited);
    }

    return count;
}

int movingCount(int threshold, int rows, int cols)
{
    if(rows < 0 || cols < 0 || threshold < 0)
        return 0;
    
    bool* visited = new bool[rows * cols];
    memset(visited,0,rows * cols);

    int count = Search(threshold,rows,cols,0,0,visited);

    delete[] visited;
    return count;
}



int main(int argc, char** argv)
{
    int threshold = 2;
    int rows = 40;
    int cols = 40;
    cout << movingCount(threshold,rows,cols) << endl;
    return 0;
}
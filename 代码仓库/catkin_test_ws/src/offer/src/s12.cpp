#include <iostream>
#include <stdexcept>
#include <ros/ros.h>
using std::cout; using std::endl;


bool hasPathCore(char* matrix, int rows, int cols, char* str,
                 int& length, int row, int col, bool* visited);
bool hasPath(char* matrix, int rows, int cols, char* str)
{
    if(matrix == nullptr || rows <= 0 || cols <=0 || str == nullptr)
        return false;
    
    int pathLength = 0;
    bool* visited = new bool[rows * cols];
    memset(visited,0,rows * cols);

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
        {
            if(hasPathCore(matrix,rows,cols,str,pathLength,i,j,visited))
                return true;
        }
    
    delete[] visited;
    return false;
}

bool hasPathCore(char* matrix, int rows, int cols, char* str,
                 int& length, int row, int col, bool* visited)
{
    if(str[length] == '\0')
        return true;
    
    bool haspath = false;
    if(row < rows && row >= 0 && col < cols && col >= 0 && 
       matrix[row * cols + col] == str[length] && visited[row * cols + col] == false)
    {
        length++;
        visited[row * cols + col] = true;

        haspath = hasPathCore(matrix,rows,cols,str,length,row-1,col,visited) ||
                  hasPathCore(matrix,rows,cols,str,length,row+1,col,visited) ||
                  hasPathCore(matrix,rows,cols,str,length,row,col-1,visited) ||
                  hasPathCore(matrix,rows,cols,str,length,row,col+1,visited);
                    
        if(!haspath)
        {
            length--;
            visited[row * cols + col] = false;
        }
    }

    return haspath;

}

int main(int argc, char** argv)
{
    //注意字符数组容量要+1！，因为'\0'的存在
    char matrix[13] =  {'a','b','c','d','e','f','g','h','i','j','k','l'}; 
    int rows = 3;
    int cols = 4;
    char str[7] = {'c','g','f','j'};
    cout << hasPath(matrix,rows,cols,str) << endl;
    return 0;
}
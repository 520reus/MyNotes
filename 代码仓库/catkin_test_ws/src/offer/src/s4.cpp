#include <ros/ros.h>
#include <iostream>
#include <vector>
using std::cout; using std::endl;
using std::vector;

//* 1.vector二维实现
bool FindNumber(const vector<vector<int>>& arr,const int& req)
{
    if(arr.empty()){return false;}
    int row = arr.size();
    int colomn = arr[0].size();
    int i = 0;
    int j = colomn - 1;
    while(i < row && j >= 0)
    {
        int choose_num = arr[i][j];
        if(req == choose_num)
            return true;
        else if(req >= choose_num)
            i++;
        else
            j--;
    }
    return false;
}

//* 2.一维数组存储二维数据的实现
bool FindNumber2(int* arr, const int& rows, const int& colomns, const int& req)
{
    bool found = false;
    if(rows > 0 && colomns > 0 && arr != nullptr)
    {
        int row = 0;
        int colomn = colomns - 1;
        while(row < rows && colomns >= 0)
        {
            int choose_num = arr[row*colomns + colomn];
            if(req == choose_num)
            {
                found = true;
                break;
            }
            else if(req > choose_num)
                row++;
            else
                colomn--;
        }
    }
    return found;
}

//* 2.二维数组的实现
bool FindNumber3(int(*arr)[4], const int& rows, const int& colomns, const int& req)
{
    bool found = false;
    if(rows > 0 && colomns > 0 && arr != nullptr)
    {
        int row = 0;
        int colomn = colomns - 1;
        while(row < rows && colomns >= 0)
        {
            int choose_num = arr[row][colomn];
            if(req == choose_num)
            {
                found = true;
                break;
            }
            else if(req > choose_num)
                row++;
            else
                colomn--;
        }
    }
    return found;
}

int main(int argc,char* argv)
{
    // test1
    vector<vector<int>> arr2D = {{1,2,8,9},{2,4,9,12},{4,7,10,13},{6,8,11,15}};
    double number = 14;
    if((int)number == number)
    {
        if(FindNumber(arr2D,number))
            cout << "Find number!" << endl;
        else
            cout << "Number is not in this arr!" << endl;
    }
    else
        cout <<"Number is not int!" << endl;
    

    // test2
    int arr[16] = {1,2,8,9,2,4,9,12,4,7,10,13,6,8,11,15};
    double num = 14;
    if((int)num == num)
    {
        if(FindNumber2(arr,4,4,num))
            cout << "Find num!" << endl;
        else
            cout << "Num is not in this arr!" << endl;
    }
    else
        cout <<"Num is not int!" << endl;


    // test3
    int arr2[4][4] = {{1,2,8,9},{2,4,9,12},{4,7,10,13},{6,8,11,15}};;
    double num2 = 14;
    if((int)num2 == num2)
    {
        if(FindNumber3(arr2,4,4,num2))
            cout << "Find num2!" << endl;
        else
            cout << "Num2 is not in this arr!" << endl;
    }
    else
        cout <<"Num2 is not int!" << endl;

    /* int ats[5] = {1,2,3,4,5};
    int* x = ats;
    int** p = &x;
    p++;
    std::cout << *p << endl; */
    char* s = " ";
    cout << (int)*s << endl;

    return 0;
}
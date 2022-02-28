#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

//* 1.未考虑大数问题，用整型(int)或长整型(long int)都会溢出
void PrintToMaxOfDigits_1(const int& n)
{
    int maxNum = pow(10,n) - 1;
    int res = 0;
    for(int i = 1;i <= maxNum;i++)
    {
        cout << i << endl;
    }
}

//* 2.用字符串表示大数，在字符串上模拟数字加法的解法
bool Increment(char* number)
{
    bool isOverflow = false;
    int nTakeOver = 0;
    int nLength = strlen(number);
    for(int i = nLength - 1;i >= 0;i--)
    {
        int nSum = number[i] - '0' + nTakeOver;
        if(i == nLength-1)
            nSum++;
        if(nSum >= 10)
        {
            if(i == 0)
                isOverflow = true;
            else
            {
                nSum -= 10;
                nTakeOver = 1;
                number[i] = '0' + nSum;
            }
        }
        else{
            number[i] = '0' + nSum;
            break;
        }
    }
    return isOverflow;
}

void PrintNumber(char* number)
{
    int nLength = strlen(number);
    bool isBeginningZero = true;
    for(int i = 0;i < nLength;i++)
    {
        if(isBeginningZero && number[i] != '0')
            isBeginningZero = false;

        if(!isBeginningZero)
        {
            cout << number[i];
        }
    }
    cout << endl;
}

void PrintToMaxOfDigits_2(const int& n)
{
    if(n <= 0)
        return;
    
    char* number = new char[n+1];
    memset(number,'0',n);
    number[n] = '\0';

    while(!Increment(number))
        PrintNumber(number);


    delete[] number;
}

//* 3.把问题转换为数字排列的解法，递归让代码更简洁

void PrintToMaxOfDigitsRecursively(char* number, const int& length, const int& index)
{
    if(index == length - 1)
    {
        PrintNumber(number);
        return;
    }

    for(int i = 0; i < 10;i++)
    {
        number[index+1] = i + '0';
        PrintToMaxOfDigitsRecursively(number,length,index+1);
    }
}

void PrintToMaxOfDigits_3(const int& n)
{
    if(n <= 0)
        return;
    
    char* number = new char[n+1];
    number[n] = '\0';

    for(int i = 0;i < 10;i++)
    {
        number[0] = i + '0';
        PrintToMaxOfDigitsRecursively(number,n,0);
    }

    delete[] number;
}


int main(int argc, char** argv)
{
    int n = 3;
    //PrintToMaxOfDigits_1(n);
    //PrintToMaxOfDigits_2(n);
    PrintToMaxOfDigits_3(n);
    return 0;
}
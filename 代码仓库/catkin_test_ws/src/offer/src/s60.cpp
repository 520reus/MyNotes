#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <stack>
#include <deque>

using std::cout; using std::endl; 
using std::vector;
using std::deque;

int g_maxVaule = 6; 

//*解法1：基于递归，代码简单，重复计算较多，时间性能差
void Probability(int original, int current, int sum, int* pProbabilities)
{
    if(current == 1)
        pProbabilities[sum - original]++;
    else
    {
        for(int i = 1; i < g_maxVaule; i++)
            Probability(original,current-1,i+sum,pProbabilities);
    }
}

void Probability(int number, int* pProbabilities)
{
    for(int i = 1; i <= g_maxVaule; i++)
        Probability(number,number,i,pProbabilities);
}

void PrintProbability_recrusive(int number)
{
    if(number < 1)
        return;
    
    int maxSum = number * g_maxVaule;
    int* pProbilities = new int[maxSum - number + 1];
    for(int i = number; i <= maxSum; i++)
        pProbilities[i - number] = 0;
    
    Probability(number,pProbilities);

    int total = pow((double)g_maxVaule,number);
    for(int i = number; i <= maxSum; i++)
    {
        double ratio = (double)pProbilities[i-number] / total;
        cout << i << ":" << ratio << endl;
    }
    delete[] pProbilities;
}

//*解法2：基于循环，代码复杂，无重复计算，时间性能高
void PrintProbability(int number)
{
    if(number < 1)
        return;
    
    int* pPobabilities[2];
    pPobabilities[0] = new int[number * g_maxVaule + 1]; //第一个数组中的第n个数表示骰子和为n出现的次数
    pPobabilities[1] = new int[number * g_maxVaule + 1]; //第二个数组第n个数字为第n-1...n-6的数字之和
    for(int i = 0; i < number*g_maxVaule; i++)
    {
        pPobabilities[0][i] = 0;
        pPobabilities[1][i] = 0;
    }

    int flag = 0;

    for(int i = 1; i <= g_maxVaule; i++)
        pPobabilities[flag][i] = 1; //第一个骰子，前6个数自然均为1

    for(int k = 2; k < number; k++) //第几个骰子
    {
        for(int i = 0; i < k; i++) //
            pPobabilities[1 - flag][i] = 0;
        
        for(int i = k; i <= g_maxVaule*k; i++)//第几个骰子对应的数
        {
             pPobabilities[1-flag][i] = 0;
             for(int j = 1; j <= i && j <= g_maxVaule; j++)
                pPobabilities[1-flag][i] += pPobabilities[flag][i-j];
        }
        flag = 1 - flag;
    }

    double total = pow((double)g_maxVaule,number);
    for(int i = number; i <= g_maxVaule*number;i++)
    {
        double ratio = (double)pPobabilities[flag][i] / total;
        cout << i << ":" << ratio << endl;
    }

    delete[] pPobabilities[0];
    delete[] pPobabilities[1];
}

int main(int argc, char** argv)
{
    PrintProbability_recrusive(2);
    PrintProbability(2);
    return 0;
}
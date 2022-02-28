#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

//* 1.全面但不够高效的解法
bool g_InvalidInput = false;

double PowerWithUnsignedExponent(const int& base, const unsigned int& exponent)
{
    double result = 1;
    for(int i = 1; i <= exponent; i++)
        result *= base;
    return result;
}   

double Power(const int& base, const int& exponent)
{
    g_InvalidInput = false;
    if(base == 0 && exponent < 0)
    {
        g_InvalidInput = true;
        return 0.0;
    }

    unsigned int absExponent = (unsigned int)(exponent);
    if(exponent < 0)
        absExponent = (unsigned int)(-exponent);

    double result = PowerWithUnsignedExponent(base,absExponent);

    if(exponent < 0)
        result = 1.0 / result;
    
    return result;
}

//* 2.即全面又高效的解法,时间复杂度O(logn)
bool g_InvalidInput2 = false;

double PowerWithUnsignedExponent2(const int& base, const unsigned int& exponent)
{
    if(exponent == 0)
        return 1;
    if(exponent == 1)
        return base;

    double result = PowerWithUnsignedExponent2(base,exponent>>1);
    result *= result;

    if(exponent & 0x1 == 1)
        result  *= base;

    return result;    
}   

double Power2(const int& base, const int& exponent)
{
    g_InvalidInput2 = false;
    if(base == 0 && exponent < 0)
    {
        g_InvalidInput2 = true;
        return 0.0;
    }

    unsigned int absExponent = (unsigned int)(exponent);
    if(exponent < 0)
        absExponent = (unsigned int)(-exponent);

    double result = PowerWithUnsignedExponent2(base,absExponent);

    if(exponent < 0)
        result = 1.0 / result;
    
    return result;
}

int main(int argc, char** argv)
{
    int base = 5;
    int exponent = 7;
    cout << Power(base,exponent) << endl;
    cout << Power2(base,exponent) << endl;
    return 0;
}
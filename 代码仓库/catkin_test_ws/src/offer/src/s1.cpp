
#include <ros/ros.h>
#include <iostream>
using std::cout; using std::endl;

class CMyString 
{
    public:
        CMyString(char* pData = nullptr)
        {
            pData_ = new char[strlen(pData)+1];
            strcpy(pData_,pData);
        }

        CMyString(const CMyString& str)
        {
            delete[] pData_;
            pData_ = nullptr;

            pData_ = new char[strlen(str.pData_)+1];
            strcpy(pData_,str.pData_);
        }

        ~CMyString(){};

        
        /* //* 赋值构造函数(初级版)
        CMyString& operator=(const CMyString& str)
        {
            //自己给自己赋值，直接返回自己
            if(this == &str)
                return *this;

            delete[] pData_;
            pData_ = nullptr;

            pData_ = new char[strlen(str.pData_)+1];
            strcpy(pData_,str.pData_);

            return *this;
        } */

        //* 赋值构造函数(高级版)
        CMyString& operator=(const CMyString& str)
        {
            if(this != &str)
            {
                CMyString strTemp(str);

                char* pTemp = strTemp.pData_;
                strTemp.pData_ = pData_;
                pData_ = pTemp;
            }

            return *this;
        }

    private:
        char* pData_;
};



int main(int argc,char** argv)

{
    char p[10] = {};
    p[0] = 'w';
    p[1] = 'h';
    p[2] = 'z';
    CMyString s1(p);
    char x[2] = {};
    x[0] = 't';
    CMyString s2(x);
    char x2[2] = {};
    x2[0] = 'h';
    CMyString s3(x2);
    s3 = s2 = s1;
    ROS_INFO("1");
    
    return 0;
}
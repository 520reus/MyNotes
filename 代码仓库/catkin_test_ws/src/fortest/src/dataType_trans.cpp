#include <iostream>
#include <sstream>
#include <fstream>

using std::cout; using std::endl;

int main()
{
    //*1. int/double -> string
    std::stringstream sstream;
    std::string strResult;
    double nvalue = 1000.012;
    sstream << nvalue;
    sstream >> strResult;
    cout<<strResult<<endl;

    //*2. string -> int/double
    std::string num = "623.14";
    double num_ = atof(num.c_str());
    cout<<"string to double:"<<num_<<endl;

    //*3. string -> c_string
    std::string name = "whz";
    const char* name_ = name.c_str();
    cout<<name_<<endl;
    
    //*4. double -> int
    double x = 5.35;
    int x_ = int(x);
    cout<<x_<<endl;

    int x__ = static_cast<int>(x);
    cout<<x__<<endl;


    return 0;
}

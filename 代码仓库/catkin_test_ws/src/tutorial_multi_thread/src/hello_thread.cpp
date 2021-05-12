#include  <iostream>
#include <thread>

using namespace std;

void hello(const string& name)
{
    cout<<name<<":hello world from new thread"<<endl;
}


int main(int argc,char** argv)
{
    thread t(hello,"whz");
    thread t2([]{cout<<"hello world from lambda thread"<<endl;});
    t.join();
    t2.join();
    return 0;
}
#include <iostream>
#include <ros/ros.h>
#include <stack>
using std::cout; using std::endl;

template<typename T>
class StackWithMin
{
    public:
        StackWithMin(){}
        ~StackWithMin(){}
        void push(const T& value);
        void pop();
        T min();
    private:
        std::stack<T> dataStack_;
        std::stack<T> minStack_;
        
};

template<typename T>
void StackWithMin<T>::push(const T& value)
{
    dataStack_.push(value);

    if(minStack_.empty() || value < minStack_.top())
        minStack_.push(value);
    
    else
        minStack_.push(min());
}

template<typename T>
void StackWithMin<T>::pop()
{
    assert(!dataStack_.empty() && !minStack_.empty());

    dataStack_.pop();
    minStack_.pop();
    
}

template<typename T>
T StackWithMin<T>::min()
{
    assert(!dataStack_.empty() && !minStack_.empty());

    return minStack_.top();
}

int main(int argc, char** argv)
{
   StackWithMin<int> stack;
   stack.push(5);
   stack.push(4);
   stack.push(2);
   stack.push(1);
   stack.push(3);
   cout << stack.min() << endl;
   stack.pop();
   cout << stack.min() << endl;
   stack.pop();
   cout << stack.min() << endl;
}
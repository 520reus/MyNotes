#include <iostream>
#include <stack>
#include <queue>
#include <ros/ros.h>
#include <stdexcept>
using std::cout; using std::endl;

//*1. 两个栈实现一个队列
template<typename T> 
class CQueue
{   
    public:
        CQueue(){};
        ~CQueue(){};
        void AddTail(const T& node);
        T DeleteHead();

    private:
        std::stack<T> stack1;
        std::stack<T> stack2;
};

template<typename T> 
void CQueue<T>::AddTail(const T& node)
{
    stack1.push(node);
}

template<typename T> 
T CQueue<T>::DeleteHead()
{
    if(stack2.empty())
    {
        while(!stack1.empty())
        {
            T transNode = stack1.top();
            stack2.push(transNode);
            stack1.pop();
        }
        
    }

    if(stack2.empty())
    {
        std::logic_error e("queue is empty.");
        throw std::exception(e);
    }

    T head = stack2.top();
    stack2.pop();
    return head;    

}

//*1. 两个队列实现一个栈
template<typename T> 
class CStack
{
    public:
        CStack(){}
        ~CStack(){}
        void push(T data);
        T pop();


    private:
        std::queue<T> queue1;
        std::queue<T> queue2;

};

template<typename T>
void CStack<T>::push(T data)
{
    if(queue1.empty())
        queue2.push(data);

    if(queue2.empty())
        queue1.push(data);
}

template<typename T>
T CStack<T>::pop()
{
    if(queue1.empty() && queue2.empty())
    {
        std::logic_error e("stack is empty.");
        throw std::exception(e);
    }

    if(queue1.empty())
    {
        T deleteNode = 0;
        while(queue2.size() > 1)
        {
            T transNode = queue2.front();
            queue1.push(transNode);
            queue2.pop();
        }

        deleteNode = queue2.back();
        queue2.pop();
        return deleteNode;
    }

    if(queue2.empty())
    {
        T deleteNode = 0;
        while(queue1.size() > 1)
        {
            T transNode = queue1.front();
            queue2.push(transNode);
            queue1.pop();
        }

        deleteNode = queue1.back();
        queue1.pop();
        return deleteNode;
    }
}

int main(int argc, char** argv)
{
    CQueue<int> queue;
    queue.AddTail(5);
    queue.AddTail(7);
    queue.AddTail(8);

    cout << queue.DeleteHead() << endl;
    cout << queue.DeleteHead() << endl;
    cout << queue.DeleteHead() << endl;

    queue.AddTail(3);

    //------------
    CStack<int> stack;
    stack.push(10);
    stack.push(9);
    stack.push(11);

    cout << stack.pop() << endl;
    cout << stack.pop() << endl;
    cout << stack.pop() << endl;


    return 0;
}
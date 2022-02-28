#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

void Serialize(BinaryTreeNode* pRoot, std::ostream& stream)
{
    if(pRoot == nullptr)
    {
        stream << "$,";
        return;
    }

    stream << pRoot->value << ',';
    Serialize(pRoot->pLeft,stream);
    Serialize(pRoot->pRight,stream);    
}

bool ReadStream(std::istream& stream, int* number)
{
    //TODO 该函数每次从流中读出一个数字或者一个字符'$',当从流中读出的是一个数字时，函数返回true，否则返回false
}

void Deserialize(BinaryTreeNode** pRoot,  std::istream& stream)
{
    int number;
    if(ReadStream(stream,&number))
    {
        *pRoot = new BinaryTreeNode();
        (*pRoot)->value = number;
        (*pRoot)->pLeft = nullptr;
        (*pRoot)->pRight = nullptr;

        Deserialize(&((*pRoot)->pLeft),stream);
        Deserialize(&((*pRoot)->pRight),stream);
    }
}

int main(int argc, char** argv)
{

    return 0;
}
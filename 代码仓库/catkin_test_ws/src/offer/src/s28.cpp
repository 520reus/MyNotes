#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

bool isSymmetrical(BinaryTreeNode* pRoot1, BinaryTreeNode* pRoot2)
{
    if(pRoot1 == nullptr && pRoot2 == nullptr)
        return true;
    
    if(pRoot1 == nullptr || pRoot2 == nullptr)
        return false;
    
    if(!pRoot1->value == pRoot2->value)
        return false;
    
    return isSymmetrical(pRoot1->pLeft,pRoot2->pRight) &&
           isSymmetrical(pRoot1->pRight,pRoot2->pLeft);
}

bool isSymmetrical(BinaryTreeNode* pRoot)
{
    return isSymmetrical(pRoot,pRoot);
}

int main(int argc, char** argv)
{

    return 0;
}
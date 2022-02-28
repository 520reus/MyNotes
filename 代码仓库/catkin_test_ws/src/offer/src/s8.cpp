#include <iostream>
#include <ros/ros.h>
#include <stdexcept>

using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
    BinaryTreeNode* pParent;
};


BinaryTreeNode* GetNext(BinaryTreeNode* pNode)
{
    if(pNode == nullptr)
        return nullptr;
    
    BinaryTreeNode* pNext = nullptr;
    if(pNode->pRight != nullptr)
    {
        BinaryTreeNode* pRightNode = pNode->pRight;
        while(pRightNode->pLeft != nullptr)
            pRightNode = pRightNode->pLeft;
        pNext = pRightNode;
    }
    else if(pNode->pParent != nullptr)
    {
        BinaryTreeNode* pCurrent = pNode;
        BinaryTreeNode* pParent = pNode->pParent;
        while(pParent != nullptr && pCurrent == pParent->pRight)
        {
            pCurrent = pParent;
            pParent = pParent->pParent;
        }
        pNext = pParent;
    }
    return pNext;
    
}

    

int main(int argc, char** argv)
{
    
    return 0;
}
#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

void ConvertNode(BinaryTreeNode* pNode, BinaryTreeNode** pLastNodeInList)
{
    if(pNode == nullptr)
        return;
    
    BinaryTreeNode* pCurrent = pNode;

    if(pCurrent->pLeft != nullptr)
        ConvertNode(pCurrent->pLeft,pLastNodeInList);
    
    //采用中序遍历，这里即能将根节点与左子树中最大的节点链接起来，又能将根节点与右子树中最小的节点链接起来
    pCurrent->pLeft = *pLastNodeInList;
    if(*pLastNodeInList != nullptr)
        (*pLastNodeInList)->pRight = pCurrent;
    
    *pLastNodeInList = pCurrent;

    if(pCurrent->pRight != nullptr)
        ConvertNode(pCurrent->pRight,pLastNodeInList);
}

BinaryTreeNode* Convert(BinaryTreeNode* pRoot)
{
    if(pRoot == nullptr)
        return nullptr;
    
    //pLastNodeInList指向已经转换好的链表的最后一个节点
    BinaryTreeNode* pLastNodeInList = nullptr;
    ConvertNode(pRoot,&pLastNodeInList);

    //pLastNodeInList指向双向链表的尾节点，我们需要返回头结点
    BinaryTreeNode* pHeadOfList = pLastNodeInList;
    while(pHeadOfList != nullptr && pHeadOfList->pLeft != nullptr)
        pHeadOfList = pHeadOfList->pLeft;
    
    return pHeadOfList;
}

int main(int argc, char** argv)
{

    return 0;
}
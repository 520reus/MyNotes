#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct BinaryTreeNode
{
    double value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

void MirrorRecursively(BinaryTreeNode* pRoot)
{
    if(pRoot == nullptr)
        return;
    
    //到达叶子节点，返回
    if(pRoot->pLeft == nullptr && pRoot->pRight == nullptr)
        return;
    
    BinaryTreeNode* temp = pRoot->pLeft;
    pRoot->pLeft = pRoot->pRight;
    pRoot->pRight = temp;

    if(pRoot->pLeft)
        MirrorRecursively(pRoot->pLeft);

    if(pRoot->pRight)
        MirrorRecursively(pRoot->pRight);

}

int main(int argc, char** argv)
{
    return 0;
}
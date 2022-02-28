#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>

using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};



BinaryTreeNode* KthNodeCore(BinaryTreeNode* pRoot,unsigned int& k)
{
    BinaryTreeNode* target = nullptr;
    
    if(pRoot->pLeft != nullptr)
        target = KthNodeCore(pRoot->pLeft,k);
    
    if(target == nullptr)
    {
        if(k == 1)
            target = pRoot;
        else
            k--;
    }
    if(target == nullptr && pRoot->pRight != nullptr)
        target = KthNodeCore(pRoot->pRight,k);
    
    return target;
}

BinaryTreeNode* KthNode(BinaryTreeNode* pRoot,unsigned int k)
{
    if(pRoot == nullptr || k <= 0)
        return nullptr;
    
    return KthNodeCore(pRoot,k);
}

int main(int argc, char** argv)
{
    
    return 0;
}
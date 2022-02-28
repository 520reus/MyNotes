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

int TreeDepth(BinaryTreeNode* pRoot)
{
    if(pRoot == nullptr)
        return 0;
    
    int depth_l = TreeDepth(pRoot->pLeft);
    int depth_r = TreeDepth(pRoot->pRight);
    return depth_l > depth_r ? (depth_l + 1) : (depth_r + 1);
}

//* 2. 平衡二叉树
bool IsBalanced(BinaryTreeNode* pRoot)
{
    if(pRoot == nullptr)
        return true;
    
    int left = TreeDepth(pRoot->pLeft);
    int right = TreeDepth(pRoot->pRight);
    int diff = std::abs(left - right);
    if(diff > 1)
        return false;
    
    return IsBalanced(pRoot->pLeft) && IsBalanced(pRoot->pRight);
}

//? 消除重复节点，提高时间效率
bool IsBalance_2(BinaryTreeNode* pRoot, int* depth)
{
    if(pRoot == nullptr)
    {
        *depth = 0;
        return true;
    }

    int left,right;
    if(IsBalance_2(pRoot->pLeft,&left) && IsBalance_2(pRoot->pRight,&right))
    {
        int diff = abs(left - right);
        if(diff <= 1)
        {
            *depth = 1 + (left > right ? left : right);
            return true;
        }
    }
    
    return false;
}

bool IsBalance_2(BinaryTreeNode* pRoot)
{
    int depth = 0;
    return IsBalance_2(pRoot,&depth);
}

int main(int argc, char** argv)
{
    
    return 0;
}
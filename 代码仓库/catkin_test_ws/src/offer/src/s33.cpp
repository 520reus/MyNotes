#include <iostream>
#include <queue>
#include <stack>
#include <ros/ros.h>
using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

bool VerifySquenceOfBST(int sequence[], int length)
{
    if(sequence == nullptr || length <= 0)
        return false;
    
    int root = sequence[length - 1];

    //在二叉搜索树中左子树节点的值小于右子树根节点的值
    int i = 0;
    for(; i < length - 1; i++)
    {
        if(sequence[i] > root)
            break;
    }

    int j = i;
    for(; j < length - 1; j++)
    {
        if(sequence[j] < root)
            return false;
    }

    //判断左子树是不是二叉搜索树
    bool left = true;
    if(i > 0)
        left = VerifySquenceOfBST(sequence,i);
    
    //判断右子树是不是二叉搜索树
    bool right = true;
    if(i < length - 1)
        right = VerifySquenceOfBST(sequence + i,length - i - 1);
    
    return left && right;


}

int main(int argc, char** argv)
{

    return 0;
}
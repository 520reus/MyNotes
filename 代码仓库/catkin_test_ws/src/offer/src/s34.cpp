#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <ros/ros.h>
using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

void FindPath(BinaryTreeNode* pNode, int expectedSum, std::vector<int>& path, int currentSum)
{
    currentSum += pNode->value;
    path.push_back(pNode->value);

    //如果是叶节点，并且路径上节点值的和等于输入的值，则打印这条路径
    bool isLeaf = pNode->pLeft == nullptr && pNode->pRight == nullptr;

    if(currentSum == expectedSum && isLeaf)
    {
        std::vector<int>::iterator it;
        for(it = path.begin(); it != path.end(); it++)
            cout << *it << " ";
        cout << endl;
    }

    //如果不是叶节点，则遍历它的子节点
    if(pNode->pLeft)
        FindPath(pNode->pLeft,expectedSum,path,currentSum);

    if(pNode->pRight)
        FindPath(pNode->pRight,expectedSum,path,currentSum); 
        
    //在返回父节点之前，在路径上删除当前节点
    path.pop_back();
}

void FindPath(BinaryTreeNode* pRoot, int expectedSum)
{
    if(pRoot == nullptr)
        return;
    
    std::vector<int> path;
    int currentSum = 0;
    FindPath(pRoot,expectedSum,path,currentSum);
}

int main(int argc, char** argv)
{

    return 0;
}
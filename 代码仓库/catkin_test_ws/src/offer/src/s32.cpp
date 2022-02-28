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

//* 1.层序遍历（从上倒下遍历二叉树）
void printFromTopToBottom(BinaryTreeNode* pRoot)
{
    if(pRoot == nullptr)
        return;
    std::queue<BinaryTreeNode*> queque;
    queque.push(pRoot);

    while(!queque.empty())
    {
        BinaryTreeNode* pNode = queque.front();
        cout << pNode->value << " ";
        queque.pop(); 

        if(pNode->pLeft)
        {
            queque.push(pNode->pLeft);
        }
            
        if(pNode->pRight)
        {
            queque.push(pNode->pRight);
        }
    }
}

//* 2.分行从上到下打印二叉树
void printFromTopToBottomLayer(BinaryTreeNode* pRoot)
{
    if(pRoot == nullptr)
        return;

    std::queue<BinaryTreeNode*> queque;
    queque.push(pRoot);
    int toBePrinted = 1;
    int nextLevel = 0;

    while(!queque.empty())
    {
        BinaryTreeNode* pNode = queque.front();
        cout << pNode->value << " ";

        if(pNode->pLeft)
        {
            queque.push(pNode->pLeft);
            nextLevel++;
        }

        if(pNode->pRight)
        {
            queque.push(pNode->pRight);
            nextLevel++;
        }

        queque.pop(); 
        toBePrinted--;  

        if(toBePrinted == 0)
        {
            cout << endl;
            toBePrinted = nextLevel;
            nextLevel = 0;
        }    
    }
}

//* 3.之字形打印二叉树
void printZ(BinaryTreeNode* pRoot)
{
    if(pRoot == nullptr)
        return;
    
    std::stack<BinaryTreeNode*> levels[2];
    int current = 0;
    int next = 1;

    levels[current].push(pRoot);

    while(!levels[0].empty() || !levels[1].empty())
    {
        BinaryTreeNode* pNode = levels[current].top();
        levels[current].pop();
        cout << pNode->value << " ";

        if(current == 0)
        {
            if(pNode->pLeft != nullptr)
                levels[next].push(pNode->pLeft);
            if(pNode->pRight != nullptr)
                levels[next].push(pNode->pRight);
        }
        else
        {
            if(pNode->pRight != nullptr)
                levels[next].push(pNode->pRight);
            if(pNode->pLeft != nullptr)
                levels[next].push(pNode->pLeft);
        }

        if(levels[current].empty())
        {
            cout << endl;
            current = 1 - current;
            next = 1 - next;
        }

    }

}


int main(int argc, char** argv)
{

    return 0;
}
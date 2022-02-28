#include <iostream>
#include <ros/ros.h>
#include <stdexcept>

using std::cout; using std::endl;

struct BinaryTreeNode
{
    int value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

BinaryTreeNode* ConstructCore(int* startPreorder, int* endPreorder, int* startInorder, int* endInorder);
BinaryTreeNode* Construct(int* preorder, int* inorder, int length)
{
    if(preorder == nullptr || inorder == nullptr || length <= 0)
        return nullptr;
    
    return ConstructCore(preorder, preorder+length-1, inorder, inorder+length-1);
}

BinaryTreeNode* ConstructCore(int* startPreorder, int* endPreorder, 
                              int* startInorder, int* endInorder)
{
    //前序遍历序列的第一个数字是根节点的值
    //TODO 这里有个语法不清楚，就是endInorder[-1]竟然能打印最后元素的前驱元素，是什么语法？
    int rootValue = *startPreorder;
    BinaryTreeNode* root = new BinaryTreeNode();
    root->value = rootValue;
    root->pLeft = root->pRight = nullptr;
    
    if(startPreorder == endPreorder)
    {
        if(startInorder == endInorder && *startPreorder == *startInorder)
            return root;
        else
        {
            std::logic_error e("Invalid input.");
            throw std::exception(e);
        }
    }

    //在中序遍历序列中找到根节点的值
    int* rootInorder = startInorder;
    while(rootInorder <= endInorder && *rootInorder != rootValue)
        ++rootInorder;

    if(rootInorder == endInorder && *rootInorder != rootValue)
    {
        std::logic_error e("Invalid input.");
        throw std::exception(e);
    }  
    
    int leftLength = rootInorder - startInorder;
    int* leftPreorderEnd = startPreorder + leftLength;
    if(leftLength > 0)
    {
        //构建左子树
        root->pLeft = ConstructCore(startPreorder+1, leftPreorderEnd, startInorder, rootInorder-1);
    }
    if(leftLength < endPreorder - startPreorder)
    {
        //构建右子树
        root->pRight = ConstructCore(leftPreorderEnd+1, endPreorder, rootInorder+1, endInorder);
    }
    
    return root;
    
}

//*1.前序遍历
void PrintPreOrder(BinaryTreeNode* root)
{
    if(root == nullptr)
        return;
    
    cout << root->value << " ";
    PrintPreOrder(root->pLeft);
    PrintPreOrder(root->pRight);
}

//*2.中序遍历
void PrintInOrder(BinaryTreeNode* root)
{
    if(root == nullptr)
        return;

    PrintInOrder(root->pLeft);
    cout << root->value << " ";
    PrintInOrder(root->pRight);
}

//*3.后序遍历
void PrintBackOrder(BinaryTreeNode* root)
{
    if(root == nullptr)
        return;

    PrintBackOrder(root->pLeft);
    PrintBackOrder(root->pRight);
    cout << root->value << " ";
}


int main(int argc, char** argv)
{
    int preorder[8] = {1, 2, 4, 7, 3, 5, 6, 8};
    int inorder[8] = {4, 7, 2, 1, 5, 3, 8, 6};
    BinaryTreeNode* BinaryTree = Construct(preorder, inorder, 8); 
    PrintPreOrder(BinaryTree);
    cout << endl;
    PrintInOrder(BinaryTree);
    cout << endl;
    PrintBackOrder(BinaryTree);
    cout << endl;
    return 0;
}
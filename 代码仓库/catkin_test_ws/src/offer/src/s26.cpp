#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct BinaryTreeNode
{
    double value;
    BinaryTreeNode* pLeft;
    BinaryTreeNode* pRight;
};

bool Equal(double num1, double num2)
{
    if(num1 - num2 > -0.00000001 && num1 - num2 < 0.00000001)
        return true;

    return false;
}

bool DoesTree1HasTree2(BinaryTreeNode* pRoot1, BinaryTreeNode* pRoot2)
{
    //root2是子树，因此一定在前面判断
    if(pRoot2 == nullptr)
        return true;

    if(pRoot1 == nullptr)
        return false;
    
    if(!Equal(pRoot1->value,pRoot2->value))
        return false;
    
    return DoesTree1HasTree2(pRoot1->pLeft,pRoot2->pLeft) && 
           DoesTree1HasTree2(pRoot1->pRight,pRoot2->pRight);
}

bool HasSubTree(BinaryTreeNode* pRoot1, BinaryTreeNode* pRoot2)
{
    bool result = false;

    if(pRoot1 != nullptr || pRoot2 != nullptr)
    {
        if(Equal(pRoot1->value,pRoot2->value))
            result = DoesTree1HasTree2(pRoot1,pRoot2);
        
        if(!result)
            result = HasSubTree(pRoot1->pLeft,pRoot2);
        
        if(!result)
            result = HasSubTree(pRoot1->pRight,pRoot2);
    }

    return result;
}

int main(int argc, char** argv)
{

    return 0;
}
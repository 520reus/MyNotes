#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <stack>
#include <deque>
#include <list>

using std::cout; using std::endl; 
using std::vector;
using std::deque;

struct TreeNode
{
    int value;
    std::list<TreeNode*> pChild;
};

//输入两个普通树节点，求它们的最低公共祖先

/*
bool GetNodePath(TreeNode* pRoot, TreeNode* pNode, std::list<TreeNode*>& path)
{
    if(pRoot == pNode)
        return true;
    
    path.push_back(pRoot);

    bool found = false;

    std::list<TreeNode*>::iterator i = pRoot->pChild.begin();
    while(!found && i < pRoot->pChild.end())
    {
        found = GetNodePath(*i,pNode,path);
        i++;
    }

    if(!found)
        path.pop_back();
    
    return found;
}

TreeNode* GetLastCommonNode(const std::list<TreeNode*>& path1, const std::list<TreeNode*>& path2)
{
    if(path1.size() == 0 || path2.size() == 0)
        return nullptr;
    
    std::list<TreeNode*>::const_iterator iter1 = path1.begin();
    std::list<TreeNode*>::const_iterator iter2 = path2.begin();
    TreeNode* pLast = nullptr;
    while(iter1 != path1.end() && iter2 != path2.end())
    {
        if((*iter1)->value == (*iter2)->value)
            pLast = *iter1;
        
        iter1++;
        iter2++;
    }
    return pLast;
}

TreeNode* GetLastCommonParent(TreeNode* pRoot, TreeNode* pNode1, TreeNode* pNode2)
{
    if(pRoot == nullptr || pNode1 == nullptr || pNode2 == nullptr)
        return nullptr;
    
    std::list<TreeNode*> path1;
    GetNodePath(pRoot,pNode1,path1);
    std::list<TreeNode*> path2;
    GetNodePath(pRoot,pNode2,path2);

    return GetLastCommonNode(path1,path2);
}
*/

int main(int argc, char** argv)
{
    
    return 0;
}

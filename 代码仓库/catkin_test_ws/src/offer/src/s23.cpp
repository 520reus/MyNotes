#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct ListNode
{
    int value_;
    ListNode* pNext_; 
};

ListNode* MeetNode(ListNode* pHead)
{
    if(pHead == nullptr)
        return nullptr;
    
    //* 1.若存在环，先找相遇的节点
    //方法：利用一快一慢两个指针，若存在环，那走的快的指针一定会与走的慢的指针相遇，且相遇节点一定在环内
    ListNode* pSlow = pHead->pNext_;
    if(pSlow == nullptr)
        return nullptr;
    
    ListNode* pFast = pSlow->pNext_;
    while(pFast != nullptr && pSlow != nullptr)
    {
        if(pFast == pSlow)
            return pSlow;
        
        pSlow = pSlow->pNext_;
        pFast = pFast->pNext_;
        if(pFast != nullptr)
            pFast = pFast->pNext_;
    }
    //不存在环，返回空指针
    return nullptr;
}


ListNode* EntryNodeOfLoop(ListNode* pHead)
{
    ListNode* meetingNode = MeetNode(pHead);
    if(meetingNode == nullptr)
        return nullptr;
    
    //* 2.在找到环中任意节点后，就能得出环中节点数目
    //得到环中节点的数目
    int nodesInLoop = 1;
    ListNode* pNode1 = meetingNode;
    while(pNode1->pNext_ != meetingNode)
    {
        pNode1 = pNode1->pNext_;
        ++nodesInLoop;
    }

    //* 3.找到环的入口节点
    /*
    ? 方法：第一个指针先向前走n步，n为环中节点个数，也为走一圈需要的步数。然后让两个指针以相同速度移动，相遇时就入口节点
    ? why：以相同速度移动时，第一个先走的指针，到达入口节点一共需要走t+n步，t为公共步数；第二个指针需要走k+t步，
    ? 由t+n=k+t,得k=n，因此需要第一个节点先走n步。
    */
    
    //先移动pNode1，次数为环中节点的数目
    pNode1 = pHead;
    for(int i = 0; i < nodesInLoop; i++)
    {
        pNode1 = pNode1->pNext_;
    }

    //再移动pNode1和pNode2
    ListNode* pNode2 = pHead;
    while(pNode1 != pNode2)
    {
        pNode1 = pNode1->pNext_;
        pNode2 = pNode2->pNext_;
    }
    return pNode1;
}

int main(int argc, char** argv)
{
    
    return 0;
}
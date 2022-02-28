#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

//s6也是关于链表的面试题

struct ListNode
{
    int value_;
    ListNode* pNext_; 
};

void DeleteNode(ListNode** pListHead, ListNode* pToBeDeleted)
{
    if(!pListHead || !pToBeDeleted)
        return;
    
    //要删除的节点不是尾节点
    if(pToBeDeleted->pNext_ != nullptr)
    {
        pToBeDeleted->value_ = pToBeDeleted->pNext_->value_;
        pToBeDeleted->pNext_ = pToBeDeleted->pNext_->pNext_;
        delete pToBeDeleted->pNext_;
    }

    //链表只有一个节点，删除头结点（也是尾节点）
    else if(*pListHead == pToBeDeleted)
    {
        delete pToBeDeleted;
        *pListHead = nullptr;
        pToBeDeleted = nullptr;
    }

    //链表有多个节点，删除尾节点
    else
    {
        ListNode* pNode = *pListHead;
        while(pNode->pNext_ != pToBeDeleted)
        {
            pNode = pNode->pNext_;
        }
        pNode->pNext_ = nullptr;
        delete pToBeDeleted;
        pToBeDeleted = nullptr;
    }
}

void DeleteDuplication(ListNode** pHead)
{
    if(pHead == nullptr || *pHead == nullptr)
        return;
    
    ListNode* pPreNode = nullptr;
    ListNode* pNode = *pHead;
    while(pNode != nullptr)
    {
        ListNode* pNext = pNode->pNext_;
        bool needDelete = false;
        if(pNext != nullptr && pNext->value_ == pNode->value_)
            needDelete = true;
        
        if(!needDelete)
        {
            pPreNode = pNode;
            pNode = pNode->pNext_;
        }
        else
        {
            int value = pNode->value_;
            ListNode* pToBeDel = pNode;
            while(pToBeDel != nullptr && pToBeDel->value_ == value)
            {
                pNext = pToBeDel->pNext_;
                delete pToBeDel;
                pToBeDel = nullptr;
                pToBeDel = pNext;
            }

            if(pPreNode == nullptr)
                *pHead = pNext;
            else
                pPreNode->pNext_ = pNext;
                
            pNode = pNext;
        }
    }
}


int main(int argc, char** argv)
{

    return 0;
}
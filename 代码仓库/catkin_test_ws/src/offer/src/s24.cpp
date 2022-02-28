#include <iostream>
#include <ros/ros.h>

using std::cout; using std::endl;

struct ListNode
{
    int value_;
    ListNode* pNext_; 
};

ListNode* ReverseList(ListNode* pHead)
{
    if(pHead == nullptr)
        return nullptr;

    ListNode* pReverseHead = nullptr;    
    ListNode* pNode = pHead;
    ListNode* pPrev = nullptr;

    while(pNode != nullptr)
    {
        ListNode* pNext = pNode->pNext_;
        if(pNext == nullptr)
            pReverseHead = pNode;

        pNode->pNext_ = pPrev;

        pPrev = pNode;
        pNode = pNext;
    }
    return pReverseHead;
}

void AddToTail(ListNode**pHead, int value) 
{
    ListNode* pNew = new ListNode();
    pNew->pNext_ = nullptr;
    pNew->value_ = value;

    if(*pHead == nullptr){
        *pHead = pNew;
    }
        
    else
    {
        ListNode* pNode = *pHead; //由于pHead就是链表头指针，因此不能移动它，所以需要创建一个辅助的节点
        while(pNode->pNext_ != nullptr)
            pNode = pNode->pNext_;

        pNode->pNext_ = pNew;    
    }
}

void PrintList(ListNode* pHead)
{
    if(pHead == nullptr)
        return;
    
    ListNode* pNode = pHead;
    while(pNode != nullptr)
    {
        cout << pNode->value_ << " ";
        pNode = pNode->pNext_;
    }
    cout << endl;
}


int main(int argc, char** argv)
{
    
    ListNode* pList = nullptr;
    AddToTail(&pList,10);
    AddToTail(&pList,5);
    AddToTail(&pList,7);
    AddToTail(&pList,8);
    AddToTail(&pList,3);

    PrintList(pList);
    ListNode *p = ReverseList(pList);
    PrintList(p);

    return 0;
}
#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct ListNode
{
    int value_;
    ListNode* pNext_; 
};

ListNode* MergeList(ListNode* pNode1, ListNode* pNode2)
{
    if(pNode1 == nullptr)
        return pNode2;
    
    if(pNode2 == nullptr)
        return pNode1;

    ListNode* pListHead = nullptr;
    
    if(pNode1->value_ < pNode2->value_)
    {
        pListHead = pNode1;
        pListHead->pNext_ = MergeList(pNode1->pNext_,pNode2);
    }

    else
    {
        pListHead = pNode2;
        pListHead->pNext_ = MergeList(pNode1,pNode2->pNext_);
    }

    return pListHead;
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
    ListNode* pList1 = nullptr;
    AddToTail(&pList1,1);
    AddToTail(&pList1,3);
    AddToTail(&pList1,5);
    AddToTail(&pList1,7);

    ListNode* pList2 = nullptr;
    AddToTail(&pList2,2);
    AddToTail(&pList2,4);
    AddToTail(&pList2,6);
    AddToTail(&pList2,8);

    ListNode* head = MergeList(pList1,pList2);

    PrintList(head);
  
    return 0;
}
#include <iostream>
#include <ros/ros.h>
using std::cout; using std::endl;

struct ListNode
{
    int value_;
    ListNode* pNext_; 
};

ListNode* FindKthtoTail(ListNode* head, unsigned int k)
{
    if(head == nullptr || k == 0)
        return nullptr;

    ListNode* pAhead = head;
    ListNode* pBehind = head;

    int count = k - 1;
    while(count != 0)
    {
        if(pAhead->pNext_ != nullptr)
            pAhead = pAhead->pNext_;
        
        else
            return nullptr;

        count--;
    }

    while(pAhead->pNext_ != nullptr)
    {
        pAhead = pAhead->pNext_;
        pBehind = pBehind->pNext_;
    }

    return pBehind;

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

int main(int argc, char** argv)
{
    ListNode* list = new ListNode();
    AddToTail(&list,5);
    AddToTail(&list,3);
    AddToTail(&list,1);
    ListNode* p = FindKthtoTail(list,3);
    cout << p->value_ << endl;
    return 0;
}
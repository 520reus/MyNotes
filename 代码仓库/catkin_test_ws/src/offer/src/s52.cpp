#include <iostream>
#include <ros/ros.h>
#include <string>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <vector>

using std::cout; using std::endl;

struct ListNode
{
    int value_;
    ListNode* pNext_; 
};

int GetListLength(ListNode* pHead)
{
    unsigned int length = 0;
    ListNode* pNode = pHead;
    while(pHead != nullptr)
    {
        length++;
        pHead = pHead->pNext_;
    }
    return length;
}

ListNode* FindFirstCommonNode(ListNode* pHead1, ListNode* pHead2)
{
    //得到两个链表的长度
    unsigned int nLength1 = GetListLength(pHead1);
    unsigned int nLength2 = GetListLength(pHead2);
    int nLengthDif = nLength1 - nLength2;

    ListNode* pListHeadLong = pHead1;
    ListNode* pListHeadShort = pHead2;
    if(nLength2 > nLength1)
    {
        pListHeadLong = pHead2;
        pListHeadShort = pHead1;
        nLengthDif = nLength2 - nLength1;
    }

    //先在长链表上走几步，再同时在两个链表上遍历
    for(int i = 0; i < nLengthDif; i++)
        pListHeadLong = pListHeadLong->pNext_;
    
    while(pListHeadLong != nullptr && pListHeadShort != nullptr && pListHeadLong != pListHeadShort)
    {
        pListHeadLong = pListHeadLong->pNext_;
        pListHeadShort = pListHeadShort->pNext_;
    }
    //得到第一个公共节点
    ListNode* pFirstCommonNode = pListHeadLong;

    return pFirstCommonNode;
}

int main(int argc, char** argv)
{
    
    return 0;
}
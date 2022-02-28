#include <iostream>
#include <ros/ros.h>
#include <stack>
using std::cout; using std::endl;

struct ListNode
{
    int value_;
    ListNode* pNext_; 
};

//链表中添加元素
//? 传入指针，后续对指针的移动是对复制的形参指针进行移动，当函数体执行结束该指针被销毁，所以若想对传入的指针进行移动
//? 就传入指针的指针！ 而只涉及解引用时，只需传入指针即可。
//链表操作函数要传指针的指针的原因：由于每个链表节点内都含有一个指针，而且链表头就是一个头指针，因此当传入为指针时，
//后续操作都只是对复制的形参进行操作，当出了该函数时，形参被销毁，操作无效！所以必须传指针的指针
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

//链表中删除元素
void RemoveNode(ListNode** pHead, int value)
{
    if(*pHead == nullptr || pHead == nullptr)
        return;

    ListNode* pToBeDeleted = nullptr;
    
    //因为后续操作都是对头结点后面的节点进行操作，因此首先判断头结点是不是需要删除的节点
    if((*pHead)->value_ == value)
    {
        pToBeDeleted = *pHead;
        *pHead = (*pHead)->pNext_;
    }

    else
    {
        ListNode* pNode = *pHead;
        while(pNode->pNext_ != nullptr && pNode->pNext_->value_ != value)
        {
            pNode = pNode->pNext_;
        }

        if(pNode->pNext_ != nullptr && pNode->pNext_->value_ == value)
        {
            pToBeDeleted = pNode->pNext_;
            pNode->pNext_ = pNode->pNext_->pNext_;
        }
    }

    if(pToBeDeleted != nullptr)
    {
        delete pToBeDeleted;
        pToBeDeleted = nullptr;
    }
    
}

//从头到尾打印链表,打印不涉及对链表节点的增加与删除，因此只需要传入指针
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

//*从尾到头打印链表,利用栈结构
void PrintListReverseStack(ListNode* pHead)
{
    if(pHead == nullptr)
        return;
    
    std::stack<ListNode*> nodes;
    ListNode* pNode = pHead;
    while(pNode != nullptr)
    {
        nodes.push(pNode);
        pNode = pNode->pNext_;
    }

    while(!nodes.empty())
    {
        cout << nodes.top()->value_ << " ";
        nodes.pop();
    }
    cout << endl;
}

//*从尾到头打印链表,利用递归
void PrintListReverseRecursion(ListNode* pHead)
{
    if(pHead != nullptr)
    {
        if(pHead->pNext_ != nullptr)
        {
            PrintListReverseRecursion(pHead->pNext_);
        }
        
        cout << pHead->value_ << " ";
    }
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
    PrintListReverseStack(pList);
    PrintListReverseRecursion(pList);
    return 0;
}

#include "stdafx.h"
#include "CLIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int CList::main(int argc, char** argv)
{
    CList mlist;
    //鏈表指針創建
    mlist.pListDemo = mlist.ListMakeHead();
    //链表末插入值
    for (size_t i = 1; i < 15; i++)
        mlist.ListEndInsert(mlist.pListDemo, i);
    mlist.ListPrint(mlist.pListDemo,"顯示鏈表");
    mlist.ListDelAdrKey(&mlist.pListDemo, 0);
    mlist.ListPrint(mlist.pListDemo,"刪除頭");
    mlist.ListDelAdrKey(&mlist.pListDemo, mlist.Listsize(mlist.pListDemo));
    mlist.ListPrint(mlist.pListDemo,"删除尾");
    //mlist.ListDelAdrKey(&mlist.pListDemo, 100);
    //mlist.ListPrint(mlist.pListDemo,"删除不存在时不變");
    mlist.ListBeginInsertHead(mlist.pListDemo, 100);
    mlist.ListPrint(mlist.pListDemo, "头前插入key=100");
    mlist.ListBeginInsertNext(mlist.pListDemo, 200);
    mlist.ListPrint(mlist.pListDemo, "头後插入key=200");
    mlist.ListEndInsert(mlist.pListDemo, 300);
    mlist.ListPrint(mlist.pListDemo, "尾插入key=300");
    mlist.ListReverse(&mlist.pListDemo);
    mlist.ListPrint(mlist.pListDemo,"链表的逆序");
    ListNode* findl = mlist.ListFindKey(mlist.pListDemo, 9);
    mlist.ListPrint(findl, "链表查找值9");
    mlist.ListDelAdrKey(&mlist.pListDemo,6);
    mlist.ListPrint(mlist.pListDemo, "链表删除值6");
    mlist.ListClear(&mlist.pListDemo);
    mlist.ListPrint(mlist.pListDemo, "清空链表");

    CList lt1, lt2;         //有序链表1,2
    lt1.pListDemo = lt1.ListMakeHead(1);
    for (size_t i = 2; i < 5; i++)
        lt1.ListEndInsert(lt1.pListDemo, i);
    lt2.pListDemo = lt2.ListMakeHead(6);
    for (size_t i = 7; i < 10; i++)
        lt2.ListEndInsert(lt2.pListDemo, i);
    //ListNode* add_plan1 = lt2.MergeList(lt1.pListDemo, lt2.pListDemo);
    ListNode* add_plan2 = NULL;
    lt2.MergeList(lt1.pListDemo, lt2.pListDemo, add_plan2);
    mlist.ListPrint(add_plan2, "合并有序链表");

    return 1;
}

CList::CList()
{
}

CList::~CList()
{
}

void CList::ListPrint(ListNode* L,const char* name)
{
    printf("链表长度:  %2d  %s\n", Listsize(L),name);
    if (NULL == L)   //链表为空
    {
        printf("ListPrint函数执行，链表为空\n");
        return;
    }
    while (L != NULL)
    {
        printf("%d->", L->data);
        L = L->next;
    }
    printf("NULL\n");
}

ListNode* CList::ListMakeHead(int key)
{
    ListNode* L = (ListNode*)malloc(sizeof(ListNode));
    L->data = key;
    L->next = NULL;
    return L;
}

bool CList::ListIsEmpty(ListNode* L)
{
    return bool(L == NULL);
}

bool CList::ListIsEnd(ListNode* L)
{
    return bool(L->next == NULL);
}

void CList::ListBeginInsertHead(ListNode* L, int key)
{
    ListNode* tmp = (ListNode*)malloc(sizeof(ListNode));
    tmp->next = L->next;
    tmp->data = L->data;
    L->next = tmp;
    L->data = key;
}

void CList::ListBeginInsertNext(ListNode* L, int key)
{
    ListNode* tmp = (ListNode*)malloc(sizeof(ListNode));
    tmp->data = key;
    //在链表头L后插入
    tmp->next = L->next;
    L->next = tmp;
}

void CList::ListEndInsert(ListNode* L, int key)
{
    ListNode* tmp = (ListNode*)malloc(sizeof(ListNode));
    tmp->data = key;
    tmp->next = NULL;           //因爲是最後一個
    while (L->next != NULL)
        L = L->next;
    L->next = tmp;
}

int CList::Listsize(ListNode* L)
{
    int size = 0;
    while (L != NULL)
    {
        size++;
        L = L->next;
    }
    return size;
}

ListNode* CList::ListFindKey(ListNode* L, int key)
{
    if (L->data == key)
        return L;
    ListNode* find = L->next;
    while (find != NULL && find->data != key)
        find = find->next;
    //为了保证遍历完没找到就返回NULL
    return find == NULL ? NULL : find;
}

void CList::ListDelAdrKey(ListNode** const L, int key)
{
    ListNode* find = *L, *del = NULL;
    //下面判断为空是保证链表有效
    if (find != NULL && find->next != NULL && find->data == key)
    {
        *L = find->next;
        free(find);
        return;
    }
    while (find != NULL)
    {
        del = find->next;
        if (del != NULL && del->data == key)
        {
            find->next = del->next;
            free(del);
            break;
        }
        find = find->next;
    }
}

void CList::ListClear(ListNode** const L)
{
    if (NULL == L)
        return;
    ListNode* del = NULL;
    while ((*L)->next != NULL)
    {
        del = (*L)->next;
        (*L)->next = del->next;
        free(del);
    }
    //最后将链表头变成空
    free(*L);
    *L = NULL;
}

void CList::ListReverse(ListNode** const L)
{
    if (*L == NULL || (*L)->next == NULL)
        return;
    ListNode* p1 = *L, *p2 = p1->next, *p3 = p2->next;
    p1->next = NULL;
    while (p3 != NULL)
    {
        p2->next = p1;  //p2->next为p3,已经保存过了
        p1 = p2;        //p1,p2,p3都向前移动一个
        p2 = p3;
        p3 = p3->next;
    }
    p2->next = p1;      //最末端结点挂在链上
    *L = p2;
}

ListNode* CList::MergeAction(ListNode* head1, ListNode* head2)
{
    //因为两个链表head1 和head2都是有序的，所以我们只需要
    //找把较短链表的各个元素有序的插入到较长的链表之中就可以了
    ListNode* p = NULL;
    if (head1 == NULL && head2 == NULL)
        return p;
    else if (head1 == NULL)
        return head2;
    else if (head2 == NULL)
        return head1;
    if (head1->data < head2->data)
    {
        p = head1;
        p->next = MergeAction(head1->next, head2);
    } else {
        p = head2;
        p->next = MergeAction(head1, head2->next);
    }
    return p;
}

ListNode* CList::MergeList(ListNode* head1, ListNode* head2)
{
    if (!head1)
        return head2;
    if (!head2)
        return head1;
    ListNode *head, *p1(head1), *p2(head2);
    if (head1->data < head2->data)
    {
        head = head1;
        p1 = head1->next;
    }
    else {
        head = head2;
        p2 = head2->next;
    }
    ListNode *p = head;         //p永远指向最新合并的结点
    while (p1 && p2)        //如果循环停止，则p1或p2至少有一个为NULL
    {
        if (p1->data < p2->data)
        {
            p->next = p1;
            p1 = p1->next;
        } else {
            p->next = p2;
            p2 = p2->next;
        }
        p = p->next;
    }
    if (p1)                 //如果链1还没走完
        p->next = p1;
    else if (p2)            //如果链2还没走完
        p->next = p2;
    return head;
}

void CList::MergeList(ListNode*& pha, ListNode*& phb, ListNode*& phc)
{
    ListNode* pa = pha->next, *pb = phb->next, *pc = phc = pha;
    while (pa && pb)
    {
        if (pa->data <= pb->data)
        {
            pc->next = pa;
            pc = pa;
            pa = pa->next;
        }
        else
        {
            pc->next = pb;
            pc = pb;
            pb = pb->next;
        }
    }
    pc->next = pa ? pa : pb;
    free(phb);
}

int CList::ListCheckHoop(ListNode* const L)
{
    if (L == NULL)
        return 0;
    ListNode* low = L, *fast = L->next;
    while (fast != NULL && fast->next != NULL)
    {
        low = low->next;
        fast = fast->next->next;
        if (low == fast)
            return 1;
    }
    return 0;
}




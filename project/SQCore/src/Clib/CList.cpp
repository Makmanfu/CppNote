
#include "stdafx.h"
#include "CLIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int CList::main(int argc, char** argv)
{
    CList mlist;
    //朱�ָᘄ���
    mlist.pListDemo = mlist.ListMakeHead();
    //����ĩ����ֵ
    for (size_t i = 1; i < 15; i++)
        mlist.ListEndInsert(mlist.pListDemo, i);
    mlist.ListPrint(mlist.pListDemo,"�@ʾ朱�");
    mlist.ListDelAdrKey(&mlist.pListDemo, 0);
    mlist.ListPrint(mlist.pListDemo,"�h���^");
    mlist.ListDelAdrKey(&mlist.pListDemo, mlist.Listsize(mlist.pListDemo));
    mlist.ListPrint(mlist.pListDemo,"ɾ��β");
    //mlist.ListDelAdrKey(&mlist.pListDemo, 100);
    //mlist.ListPrint(mlist.pListDemo,"ɾ��������ʱ��׃");
    mlist.ListBeginInsertHead(mlist.pListDemo, 100);
    mlist.ListPrint(mlist.pListDemo, "ͷǰ����key=100");
    mlist.ListBeginInsertNext(mlist.pListDemo, 200);
    mlist.ListPrint(mlist.pListDemo, "ͷ�����key=200");
    mlist.ListEndInsert(mlist.pListDemo, 300);
    mlist.ListPrint(mlist.pListDemo, "β����key=300");
    mlist.ListReverse(&mlist.pListDemo);
    mlist.ListPrint(mlist.pListDemo,"���������");
    ListNode* findl = mlist.ListFindKey(mlist.pListDemo, 9);
    mlist.ListPrint(findl, "�������ֵ9");
    mlist.ListDelAdrKey(&mlist.pListDemo,6);
    mlist.ListPrint(mlist.pListDemo, "����ɾ��ֵ6");
    mlist.ListClear(&mlist.pListDemo);
    mlist.ListPrint(mlist.pListDemo, "�������");

    CList lt1, lt2;         //��������1,2
    lt1.pListDemo = lt1.ListMakeHead(1);
    for (size_t i = 2; i < 5; i++)
        lt1.ListEndInsert(lt1.pListDemo, i);
    lt2.pListDemo = lt2.ListMakeHead(6);
    for (size_t i = 7; i < 10; i++)
        lt2.ListEndInsert(lt2.pListDemo, i);
    //ListNode* add_plan1 = lt2.MergeList(lt1.pListDemo, lt2.pListDemo);
    ListNode* add_plan2 = NULL;
    lt2.MergeList(lt1.pListDemo, lt2.pListDemo, add_plan2);
    mlist.ListPrint(add_plan2, "�ϲ���������");

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
    printf("������:  %2d  %s\n", Listsize(L),name);
    if (NULL == L)   //����Ϊ��
    {
        printf("ListPrint����ִ�У�����Ϊ��\n");
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
    //������ͷL�����
    tmp->next = L->next;
    L->next = tmp;
}

void CList::ListEndInsert(ListNode* L, int key)
{
    ListNode* tmp = (ListNode*)malloc(sizeof(ListNode));
    tmp->data = key;
    tmp->next = NULL;           //��������һ��
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
    //Ϊ�˱�֤������û�ҵ��ͷ���NULL
    return find == NULL ? NULL : find;
}

void CList::ListDelAdrKey(ListNode** const L, int key)
{
    ListNode* find = *L, *del = NULL;
    //�����ж�Ϊ���Ǳ�֤������Ч
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
    //�������ͷ��ɿ�
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
        p2->next = p1;  //p2->nextΪp3,�Ѿ��������
        p1 = p2;        //p1,p2,p3����ǰ�ƶ�һ��
        p2 = p3;
        p3 = p3->next;
    }
    p2->next = p1;      //��ĩ�˽���������
    *L = p2;
}

ListNode* CList::MergeAction(ListNode* head1, ListNode* head2)
{
    //��Ϊ��������head1 ��head2��������ģ���������ֻ��Ҫ
    //�Ұѽ϶�����ĸ���Ԫ������Ĳ��뵽�ϳ�������֮�оͿ�����
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
    ListNode *p = head;         //p��Զָ�����ºϲ��Ľ��
    while (p1 && p2)        //���ѭ��ֹͣ����p1��p2������һ��ΪNULL
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
    if (p1)                 //�����1��û����
        p->next = p1;
    else if (p2)            //�����2��û����
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




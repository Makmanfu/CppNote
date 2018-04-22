
#include "stdafx.h"
#include "CDLIST.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int CDList::main(int argc, char** argv)
{
    CDList mDlist;
    //朱�ָᘄ���
    mDlist.pDListDemo = mDlist.DListMakeNode(0);
    mDlist.DListHeadAdd(&mDlist.pDListDemo, 3);
    mDlist.DListHeadAdd(&mDlist.pDListDemo, 2);
    mDlist.DListHeadAdd(&mDlist.pDListDemo, 1);
    mDlist.DListEndAdd(&mDlist.pDListDemo, 4);
    mDlist.DListEndAdd(&mDlist.pDListDemo, 5);
    mDlist.DListEndAdd(&mDlist.pDListDemo, 6);
    mDlist.DListPrint(mDlist.pDListDemo,"�@ʾ˫��朱�");
    DListNode* tmp = mDlist.DListFind(mDlist.pDListDemo, 5);
    mDlist.DListHeadInsert(tmp, 9);                 //���ҵĽڵ�ǰ����
    mDlist.DListEndInsert(tmp, 8);                  //���ҵĽڵ�����
    mDlist.DListPrint(mDlist.pDListDemo, "���ҽڵ�5 ǰ��9���8");
    mDlist.DListDelKey(&mDlist.pDListDemo, 1);      //���ݵ�ɾ�����ƶ�ͷָ��
    mDlist.DListDelKey(&mDlist.pDListDemo, 5);
    mDlist.DListPrint(mDlist.pDListDemo, "����1 ɾ�� ����5 ɾ��");
    mDlist.DListClear(&mDlist.pDListDemo);          //�������
    mDlist.DListPrint(mDlist.pDListDemo,"���˫������");
    return 1;
}

CDList::CDList()
{
}

CDList::~CDList()
{
}

void CDList::DListPrint(DListNode* L, const char* name)
{
    printf("������:  %2d  %s\n", DListSize(L), name);
    if (NULL == L)   //����Ϊ��
    {
        printf("ListPrint����ִ�У�����Ϊ��\n");
        return;
    }
    while (NULL != L->prev)
        L = L->prev;
    while (NULL != L)
    {
        printf("%d ", L->data);
        L = L->next;
    }
    printf("\n");
}

DListNode* CDList::DListMakeNode(int value)
{
    DListNode* pDNode = (DListNode*)malloc(sizeof(DListNode));
    if (pDNode != NULL)
    {
        pDNode->data = value;
        pDNode->prev = NULL;
        pDNode->next = NULL;
    }
    return pDNode;
}

int CDList::DListSize(DListNode* L)
{
    int size = 0;
    if (NULL == L)
        return size;
    while (NULL != L->prev)
        L = L->prev;
    while (L != NULL)
    {
        size++;
        L = L->next;
    }
    return size;
}

DListNode* CDList::DListFind(DListNode* const pDlist, int data)
{
    if (NULL == pDlist)
        return NULL;
    DListNode* pNode = pDlist;
    while (NULL != pNode->prev)
        pNode = pNode->prev;
    while (NULL != pNode)
    {
        if (data == pNode->data)
            return pNode;
        pNode = pNode->next;
    }
    return NULL;
}

void CDList::DListHeadAdd(DListNode** const pDlist, int data)
{
    DListNode* find = *pDlist;
    if (NULL == find)
        return;
    while (NULL != find->prev)
        find = find->prev;
    DListNode* pNode = DListMakeNode(data);
    find->prev = pNode;
    pNode->next = find;
}

void CDList::DListEndAdd(DListNode** const pDlist, int data)
{
    DListNode* find = *pDlist;
    if (NULL == find)
        return;
    while (NULL != find->next)
        find = find->next;
    DListNode* pNode = DListMakeNode(data);
    find->next = pNode;
    pNode->prev = find;
}

void CDList::DListHeadInsert(DListNode* L, int key)
{
    if (NULL == L)
        return;
    DListNode* pNode = DListMakeNode(key);
    if (L->prev != NULL)
    {
        L->prev->next = pNode;
        pNode->prev = L->prev;
    }
    pNode->next = L;
    L->prev = pNode;
}

void CDList::DListEndInsert(DListNode* L, int key)
{
    if (NULL == L)
        return;
    DListNode* pNode = DListMakeNode(key);
    if (L->next != NULL)
    {
        L->next->prev = pNode;
        pNode->next = L->next;
    }
    pNode->prev = L;
    L->next = pNode;
}

void CDList::DListDelKey(DListNode** const pDlist, int data)
{
    if (NULL == *pDlist)
        return;
    while ((*pDlist)->prev != NULL)
        (*pDlist) = (*pDlist)->prev;
    DListNode* find = *pDlist, *del = NULL;
    //�����ж�Ϊ���Ǳ�֤������Ч
    if (find->next != NULL && find->data == data)
    {
        *pDlist = find->next;
        (*pDlist)->prev = NULL;
        free(find);
        return;
    }
    while (find != NULL)
    {
        del = find->next;
        if (del != NULL && del->data == data)
        {
            del->next->prev = find;
            find->next = del->next;
            free(del);
            break;
        }
        find = find->next;
    }
}

void CDList::DListClear(DListNode** const L)
{
    DListNode* find = *L, *del = NULL;
    if (NULL == L)
        return;
    while (find->prev != NULL)
        find = find->prev;
    while (find->next != NULL)
    {
        del = find->next;
        find->next = del->next;
        free(del);
    }
    //�������ͷ��ɿ�
    free(*L);
    *L = NULL;
}


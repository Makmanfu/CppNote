
#include "stdafx.h"
#include "CQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>



int CQueue::main(int argc, char** argv)
{
    //printf("---------------ջ����----------------\n");
    //Queue* pq = InitQueue();
    //int i, item;
    //printf("0-9������Ӳ�������£�\n");
    //for (i = 0; i < 10; i++)
    //{
    //    EnQueue(pq, i);
    //    GetRear(pq, &item);
    //    printf("%d ", item);
    //}
    //printf("\n�Ӷ�ͷ����β��������ÿ��Ԫ��ִ��print������\n");
    //QueueTraverse(pq, Queueprint);
    //printf("������Ԫ�����γ����в�������£�\n");
    //for (i = 0; i < 10; i++)
    //{
    //    DeQueue(pq, &item);
    //    printf("%d ", item);
    //}
    //ClearQueue(pq);
    //if (QueueIsEmpty(pq))
    //    printf("\n�������ÿճɹ�\n");
    //DestroyQueue(pq);
    //printf("�����ѱ�����\n");
    return 1;
}

CQueue::CQueue()
{
}

CQueue::~CQueue()
{
}

Queue* CQueue::QueueInit(void)
{
    Queue* pqueue = (Queue*)malloc(sizeof(Queue));
    if (pqueue != NULL)
    {
        pqueue->front = NULL;
        pqueue->rear = NULL;
        pqueue->size = 0;
    }
    return pqueue;
}

int CQueue::QueueIsEmpty(Queue* pqueue)
{
    return (pqueue->front == NULL && pqueue->rear == NULL && pqueue->size == 0) ? 1 : 0;
}

int CQueue::QueueSize(Queue* pqueue)
{
    return pqueue->size;
}

ListNode* CQueue::GetFront(Queue* pqueue, Item* pitem)
{
    if (QueueIsEmpty(pqueue) != 1 && pitem != NULL)
        *pitem = pqueue->front->data;
    return pqueue->front;
}

ListNode* CQueue::GetRear(Queue* pqueue, Item* pitem)
{
    if (QueueIsEmpty(pqueue) != 1 && pitem != NULL)
        *pitem = pqueue->rear->data;
    return pqueue->rear;
}

ListNode* CQueue::EnQueue(Queue* pqueue, Item item)
{
    ListNode* pnode = (ListNode*)malloc(sizeof(ListNode));
    if (pnode != NULL)
    {
        pnode->data = item;
        pnode->next = NULL;
        if (QueueIsEmpty(pqueue))
            pqueue->front = pnode;
        else
            pqueue->rear->next = pnode;
        pqueue->rear = pnode;
        pqueue->size++;
    }
    return pnode;
}

ListNode* CQueue::DeQueue(Queue* pqueue, Item* pitem)
{
    ListNode* pnode = pqueue->front;
    if (QueueIsEmpty(pqueue) != 1 && pnode != NULL)
    {
        if (pitem != NULL)
            * pitem = pnode->data;
        pqueue->size--;
        pqueue->front = pnode->next;
        free(pnode);
        if (pqueue->size == 0)
            pqueue->rear = NULL;
    }
    return pqueue->front;
}

ListNode* CQueue::InQueue(Queue* pqueue, Item* pitem)
{
    ListNode* pnode = (ListNode*)malloc(sizeof(ListNode));  //����һ���ڵ�Ĵ�С�ڴ�
    if (pnode == NULL)
    {
        //cout << "Malloc failed!" << endl;
        return NULL;
    }
    pnode->data = *pitem;
    pnode->next = NULL;
    if (QueueIsEmpty(pqueue))           //����Ϊ��ʱ
    {
        pqueue->front = pnode;
        pqueue->rear = pnode;           //�տ�ʼ����Ϊ��  �������βָ�붼ָ��˽ڵ�
    }
    else        //���в�Ϊ��ʱ
    {
        pqueue->rear->next = pnode;
        pqueue->rear = pnode;
        pqueue->size++;
    }
    return pnode;
}

void CQueue::QueueClear(Queue* pqueue)
{
    while (QueueIsEmpty(pqueue) != 1)
        DeQueue(pqueue, NULL);
}

void CQueue::QueueDestroy(Queue* pqueue)
{
    if (QueueIsEmpty(pqueue) != 1)
        QueueClear(pqueue);
    free(pqueue);
}

void CQueue::QueueTraverse(Queue* pqueue, void(*visit)(Item))
{
    ListNode* pnode = pqueue->front;
    int i = pqueue->size;
    while (i--)
    {
        visit(pnode->data);
        pnode = pnode->next;
    }
}

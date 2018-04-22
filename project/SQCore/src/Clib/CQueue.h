//*************************************************************************
//
//                      ��п��Y�W����ʾ�о�
//
//                                                  2017-06-13  @����
//
//*************************************************************************
#pragma once
#include "CLIST.h"

//�����������[��ه朱�]
typedef struct _QUEUE
{
    ListNode* front;
    ListNode* rear;
    int size;
} Queue;

//��еČW��
class SQCPP_API CQueue
{
public:
    static int main(int argc, char** argv);
public:
    CQueue();
    ~CQueue();
public:

public:
    //����һ���ն���
    Queue* QueueInit(void);
    //�ж϶����Ƿ�Ϊ��
    int QueueIsEmpty(Queue* pqueue);
    //���ض��д�С
    int QueueSize(Queue* pqueue);
    //���ض�ͷԪ��
    ListNode* GetFront(Queue* pqueue, Item* pitem);
    //���ض�βԪ��
    ListNode* GetRear(Queue* pqueue, Item* pitem);
    //����Ԫ�����
    ListNode* EnQueue(Queue* pqueue, Item item);
    //��ͷԪ�س���
    ListNode* DeQueue(Queue* pqueue, Item* pitem);
    //��β���
    ListNode* InQueue(Queue* pqueue, Item* pitem);
    //���һ������
    void QueueClear(Queue* pqueue);
    //����һ������
    void QueueDestroy(Queue* pqueue);
public:
    //�������в��Ը����������visit����
    void QueueTraverse(Queue* pqueue, void(*visit)(Item));
};




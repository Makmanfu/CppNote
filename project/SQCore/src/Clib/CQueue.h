//*************************************************************************
//
//                      隊列總結學習演示研究
//
//                                                  2017-06-13  @阿甘
//
//*************************************************************************
#pragma once
#include "CLIST.h"

//定义队列类型[依賴鏈表]
typedef struct _QUEUE
{
    ListNode* front;
    ListNode* rear;
    int size;
} Queue;

//隊列的學習
class SQCPP_API CQueue
{
public:
    static int main(int argc, char** argv);
public:
    CQueue();
    ~CQueue();
public:

public:
    //构造一个空队列
    Queue* QueueInit(void);
    //判断队列是否为空
    int QueueIsEmpty(Queue* pqueue);
    //返回队列大小
    int QueueSize(Queue* pqueue);
    //返回队头元素
    ListNode* GetFront(Queue* pqueue, Item* pitem);
    //返回队尾元素
    ListNode* GetRear(Queue* pqueue, Item* pitem);
    //将新元素入队
    ListNode* EnQueue(Queue* pqueue, Item item);
    //队头元素出队
    ListNode* DeQueue(Queue* pqueue, Item* pitem);
    //队尾入队
    ListNode* InQueue(Queue* pqueue, Item* pitem);
    //清空一个队列
    void QueueClear(Queue* pqueue);
    //销毁一个队列
    void QueueDestroy(Queue* pqueue);
public:
    //遍历队列并对各数据项调用visit函数
    void QueueTraverse(Queue* pqueue, void(*visit)(Item));
};




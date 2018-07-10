
#include "stdafx.h"
#include "CStack.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int CStack::main(int argc, char** argv)
{
    printf("---------------栈测试----------------\n");
    CStack stk;
    stk.pStackDemo = stk.StackInit();
    int i, item;
    printf("0-9依次入栈并输出如下：\n");
    for (i = 0; i < 10; i++)
    {
        stk.Push(stk.pStackDemo, i);
        stk.GetTop(stk.pStackDemo, &item);
        printf("%d ", item);
    }
    printf("\n从栈顶到栈顶遍历并对每个元素执行print函数：\n");
    stk.StackTraverse(stk.pStackDemo, Stackprint);
    printf("\n");
    printf("栈中元素依次出栈并输出如下：\n");
    for (i = 0; i < 10; i++)
    {
        stk.Pop(stk.pStackDemo, &item);
        printf("%d ", item);
    }
    stk.StackClear(stk.pStackDemo);
    if (stk.StackIsEmpty(stk.pStackDemo))
        printf("\n将栈置空成功\n");
    stk.StackDestroy(stk.pStackDemo);

    printf("栈已被销毁\n");
    return 1;
}

CStack::CStack()
{
}

CStack::~CStack()
{
}

void CStack::Stackprint(Item L)
{
    printf("%d ", L);
}

Stack* CStack::StackInit(void)
{
    Stack* ps = (Stack*)malloc(sizeof(Stack));
    if (ps != NULL)
    {
        ps->top = NULL;
        ps->size = 0;
    }
    return ps;
}

int CStack::StackIsEmpty(Stack* ps)
{
    return (ps->top == NULL && ps->size == 0) ? 1 : 0;
}

int CStack::StackSize(Stack* ps)
{
    return ps->size;
}

ListNode* CStack::GetTop(Stack* ps, Item* pitem)
{
    if (StackIsEmpty(ps) != 1 && pitem != NULL)
        *pitem = ps->top->data;
    return ps->top;
}

ListNode* CStack::Push(Stack* ps, Item item)
{
    ListNode* pnode = (ListNode*)malloc(sizeof(ListNode));
    if (pnode != NULL)
    {
        pnode->data = item;
        pnode->next = GetTop(ps, NULL);
        ps->size++;
        ps->top = pnode;
    }
    return pnode;
}

ListNode* CStack::Pop(Stack* ps, Item* pitem)
{
    ListNode* p = ps->top;
    if (StackIsEmpty(ps) != 1 && p != NULL)
    {
        if (pitem != NULL)
            * pitem = p->data;
        ps->size--;
        ps->top = ps->top->next;
        free(p);
    }
    return ps->top;
}

void CStack::StackClear(Stack* ps)
{
    while (StackIsEmpty(ps) != 1)
        Pop(ps, NULL);
}

void CStack::StackDestroy(Stack* ps)
{
    if (StackIsEmpty(ps) != 1)
        StackClear(ps);
    free(ps);
}

void CStack::StackTraverse(Stack* ps, VISIT VFUN)
{
    ListNode* p = ps->top;
    int i = ps->size;
    while (i--)
    {
        VFUN(p->data);
        p = p->next;
    }
}












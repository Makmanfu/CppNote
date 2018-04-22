//*************************************************************************
//
//                      ��ջ���Y�W����ʾ�о�
//
//                                                  2017-06-13  @����
//
//*************************************************************************
#pragma once
#include "CLIST.h"

//����ջ����[��ه朱�]
typedef struct _STACK
{
    ListNode* top;
    int size;
} Stack;

//��ջ�ČW��
class SQCPP_API CStack
{
public:
    static int main(int argc, char** argv);
public:
    CStack();
    ~CStack();
public:

public:
    //����һ����ջ
    Stack* StackInit(void);
    //�ж��Ƿ�Ϊ��ջ
    int StackIsEmpty(Stack* ps);
    //����ջ��С
    int StackSize(Stack* ps);
    //����ջ��Ԫ��
    ListNode* GetTop(Stack* ps, Item* pitem);
    //Ԫ����ջ
    ListNode* Push(Stack* ps, Item item);
    //Ԫ�س�ջ
    ListNode* Pop(Stack* ps, Item* pitem);
    //��ջ�ÿ�
    void StackClear(Stack* ps);
    //����һ��ջ
    void StackDestroy(Stack* ps);
public:
    //����ջ������visit����
    void StackTraverse(Stack* ps, void(*visit)(Item));
};




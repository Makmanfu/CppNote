//*************************************************************************
//
//                      链栈總結學習演示研究
//
//                                                  2017-06-13  @阿甘
//
//*************************************************************************
#pragma once
#include "CLIST.h"

//定义栈类型[依賴鏈表]
typedef struct _STACK
{
    ListNode* top;
    int size;
} Stack;

//链栈的學習
class SQCPP_API CStack
{
public:
    static int main(int argc, char** argv);
public:
    CStack();
    ~CStack();
public:

public:
    //构造一个空栈
    Stack* StackInit(void);
    //判定是否为空栈
    int StackIsEmpty(Stack* ps);
    //返回栈大小
    int StackSize(Stack* ps);
    //返回栈顶元素
    ListNode* GetTop(Stack* ps, Item* pitem);
    //元素入栈
    ListNode* Push(Stack* ps, Item item);
    //元素出栈
    ListNode* Pop(Stack* ps, Item* pitem);
    //把栈置空
    void StackClear(Stack* ps);
    //销毁一个栈
    void StackDestroy(Stack* ps);
public:
    //遍历栈并访问visit函数
    void StackTraverse(Stack* ps, void(*visit)(Item));
};




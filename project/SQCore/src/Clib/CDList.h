//*************************************************************************
//
//                      双向鏈表總結學習演示研究
//
//                                                  2017-06-13  @阿甘
//
//*************************************************************************
#pragma once

//定义数据项类型
typedef int Item;

//链表的结构定义
typedef struct _DNODE
{
    Item data;                  //数据域
    struct _DNODE* prev;        //指向前驱
    struct _DNODE* next;        //指向后继
} DListNode, *DList;

//双向鏈表的學習
class SQCPP_API CDList
{
public:
    static int main(int argc, char** argv);
public:
    CDList();
    ~CDList();
public:
    DListNode* pDListDemo;                      //測試鏈表指針用
    //打印链表测试
    void DListPrint(DListNode* L, const char* name);
public:
    //创建双向链表节点
    DListNode* DListMakeNode(int value);
    //链表的长度
    int DListSize(DListNode* L);
    //查找数据
    DListNode* DListFind(DListNode* const pDlist, int data);
    //双向链表中头插入数据
    void DListHeadAdd(DListNode** const pDlist, int data);
    //双向链表中尾插入数据
    void DListEndAdd(DListNode** const pDlist, int data);
    //节点处的前插入
    void DListHeadInsert(DListNode* L, int key);
    //节点处的后插入
    void DListEndInsert(DListNode* L, int key);
    //删除链表节点数据
    void DListDelKey(DListNode** const pDlist, int data);
    //清空链表
    void DListClear(DListNode** const L);
};




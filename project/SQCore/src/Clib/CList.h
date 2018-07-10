//*************************************************************************
//
//                      鏈表總結學習演示研究
//
//                                                  2017-06-13  @阿甘
//
//*************************************************************************
#pragma once

//定义数据项类型
typedef int Item;

//鏈表的結構定義
typedef struct _NODE
{
    Item data;                  //數據域
    struct _NODE* next;         //鏈域
} ListNode, *List;

//鏈表的學習
class SQCPP_API CList
{
public:
    static int main(int argc, char** argv);
public:
    CList();
    ~CList();
public:
    ListNode* pListDemo;                        //測試鏈表指針用
    //打印链表测试
    void ListPrint(ListNode* L, const char* name);
public:
    //创建链表头
    ListNode* ListMakeHead(int key=0);
    //判定链表是否为空 判定位置P的节点是否为尾节点
    bool ListIsEmpty(ListNode* L);
    //判定链表节点是否为尾节点
    bool ListIsEnd(ListNode* L);
    //头插入key就變成第1個
    void ListBeginInsertHead(ListNode* L, int key);
    //头插入key就變成第2個
    void ListBeginInsertNext(ListNode* L, int key);
    //结尾插入值
    void ListEndInsert(ListNode* L, int key);
    //单链表的长度
    int Listsize(ListNode* L);
    //链表查找值[链表,值]
    ListNode* ListFindKey(ListNode* L, int key);
    //链表删除值[修改指针的值]
    void ListDelAdrKey(ListNode** const L, int key);
    //清空链表
    void ListClear(ListNode** const L);
public:
    //链表逆序
    void ListReverse(ListNode** const L);
    //递归合并有序链表
    ListNode* MergeAction(ListNode* head1, ListNode* head2);
    //非递归合并有序链表 C指针双向归并
    ListNode* MergeList(ListNode* head1, ListNode* head2);
    //非递归合并有序链表 C++引用法
    void MergeList(ListNode*& pha, ListNode*& phb, ListNode*& phc);
    //判断链表中有环
    int ListCheckHoop(ListNode* const L);
};




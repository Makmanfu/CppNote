//*************************************************************************
//
//                      朱��Y�W����ʾ�о�
//
//                                                  2017-06-13  @����
//
//*************************************************************************
#pragma once

//��������������
typedef int Item;

//朱�ĽY�����x
typedef struct _NODE
{
    Item data;                  //������
    struct _NODE* next;         //���
} ListNode, *List;

//朱�ČW��
class SQCPP_API CList
{
public:
    static int main(int argc, char** argv);
public:
    CList();
    ~CList();
public:
    ListNode* pListDemo;                        //�yԇ朱�ָ���
    //��ӡ�������
    void ListPrint(ListNode* L, const char* name);
public:
    //��������ͷ
    ListNode* ListMakeHead(int key=0);
    //�ж������Ƿ�Ϊ�� �ж�λ��P�Ľڵ��Ƿ�Ϊβ�ڵ�
    bool ListIsEmpty(ListNode* L);
    //�ж�����ڵ��Ƿ�Ϊβ�ڵ�
    bool ListIsEnd(ListNode* L);
    //ͷ����key��׃�ɵ�1��
    void ListBeginInsertHead(ListNode* L, int key);
    //ͷ����key��׃�ɵ�2��
    void ListBeginInsertNext(ListNode* L, int key);
    //��β����ֵ
    void ListEndInsert(ListNode* L, int key);
    //������ĳ���
    int Listsize(ListNode* L);
    //�������ֵ[����,ֵ]
    ListNode* ListFindKey(ListNode* L, int key);
    //����ɾ��ֵ[�޸�ָ���ֵ]
    void ListDelAdrKey(ListNode** const L, int key);
    //�������
    void ListClear(ListNode** const L);
public:
    //��������
    void ListReverse(ListNode** const L);
    //�ݹ�ϲ���������
    ListNode* MergeAction(ListNode* head1, ListNode* head2);
    //�ǵݹ�ϲ��������� Cָ��˫��鲢
    ListNode* MergeList(ListNode* head1, ListNode* head2);
    //�ǵݹ�ϲ��������� C++���÷�
    void MergeList(ListNode*& pha, ListNode*& phb, ListNode*& phc);
    //�ж��������л�
    int ListCheckHoop(ListNode* const L);
};




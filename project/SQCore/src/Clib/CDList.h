//*************************************************************************
//
//                      ˫��朱��Y�W����ʾ�о�
//
//                                                  2017-06-13  @����
//
//*************************************************************************
#pragma once

//��������������
typedef int Item;

//����Ľṹ����
typedef struct _DNODE
{
    Item data;                  //������
    struct _DNODE* prev;        //ָ��ǰ��
    struct _DNODE* next;        //ָ����
} DListNode, *DList;

//˫��朱�ČW��
class SQCPP_API CDList
{
public:
    static int main(int argc, char** argv);
public:
    CDList();
    ~CDList();
public:
    DListNode* pDListDemo;                      //�yԇ朱�ָ���
    //��ӡ�������
    void DListPrint(DListNode* L, const char* name);
public:
    //����˫������ڵ�
    DListNode* DListMakeNode(int value);
    //����ĳ���
    int DListSize(DListNode* L);
    //��������
    DListNode* DListFind(DListNode* const pDlist, int data);
    //˫��������ͷ��������
    void DListHeadAdd(DListNode** const pDlist, int data);
    //˫��������β��������
    void DListEndAdd(DListNode** const pDlist, int data);
    //�ڵ㴦��ǰ����
    void DListHeadInsert(DListNode* L, int key);
    //�ڵ㴦�ĺ����
    void DListEndInsert(DListNode* L, int key);
    //ɾ������ڵ�����
    void DListDelKey(DListNode** const pDlist, int data);
    //�������
    void DListClear(DListNode** const L);
};




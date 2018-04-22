//*************************************************************************
//
//                      ����係�Y�W����ʾ�о�
//
//                                                  2017-06-13  @����
//
//*************************************************************************
#pragma once
#include "CLIST.h"

//�������Ͷ���
typedef struct _BITREE
{
    struct _BITREE* lchild;     //������
    struct _BITREE* rchild;     //������
    Item data;                  //����
} BiTNode, *BiTree;

//�����ČW��
class SQCPP_API CBITree
{
public:
    static int main(int argc, char** argv);
public:
    CBITree();
    ~CBITree();
public:
    //����һ���µĶ�����
    BiTNode* BiTreeInit(BiTNode* root);
    //�ж��Ƿ�Ϊ��
    int BiTreeIsEmpty(BiTNode* tree);
    //���ɽڵ�
    BiTNode* MakeNode(Item item, BiTNode* lchild, BiTNode* rchild);
    //�����������
    int GetDepth(BiTNode* tree);
    //���ظ�
    BiTNode* GetRoot(BiTNode* tree);
    //���ؽڵ�ֵ
    Item GetItem(BiTNode* pnode);
    //���ýڵ�ֵ
    void SetItem(BiTNode* pnode, Item item);
    //����������
    BiTNode* SetLChild(BiTNode* parent, BiTNode* lchild);
    //����������
    BiTNode* SetRChild(BiTNode* parent, BiTNode* rchild);
    //����������
    BiTNode* GetLChild(BiTNode* tree);
    //����������
    BiTNode* GetRChild(BiTNode* tree);
    //����������
    BiTNode* InsertChild(BiTNode* parent, int lr, BiTNode* child);
    //�ͷŽڵ�
    void FreeNode(BiTNode* pnode);
    //ɾ������
    void DeleteChild(BiTNode* parent, int lr);
    //���һ�ö�����
    void BiTreeClear(BiTNode* tree);
    //����һ�ö�����
    void BiTreeDestroy(BiTNode* tree);
public:
    //�������������
    void PreOrderTraverse(BiTNode* tree, void(*visit)(Item));
    //�������������
    void InOrderTraverse(BiTNode* tree, void(*visit)(Item));
    //�������������
    void PostOrderTraverse(BiTNode* tree, void(*visit)(Item));
};




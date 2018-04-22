//*************************************************************************
//
//                      二叉樹總結學習演示研究
//
//                                                  2017-06-13  @阿甘
//
//*************************************************************************
#pragma once
#include "CLIST.h"

//树的类型定义
typedef struct _BITREE
{
    struct _BITREE* lchild;     //左子树
    struct _BITREE* rchild;     //右子树
    Item data;                  //数据
} BiTNode, *BiTree;

//二叉樹的學習
class SQCPP_API CBITree
{
public:
    static int main(int argc, char** argv);
public:
    CBITree();
    ~CBITree();
public:
    //构造一棵新的二叉树
    BiTNode* BiTreeInit(BiTNode* root);
    //判定是否为空
    int BiTreeIsEmpty(BiTNode* tree);
    //生成节点
    BiTNode* MakeNode(Item item, BiTNode* lchild, BiTNode* rchild);
    //返回树的深度
    int GetDepth(BiTNode* tree);
    //返回根
    BiTNode* GetRoot(BiTNode* tree);
    //返回节点值
    Item GetItem(BiTNode* pnode);
    //设置节点值
    void SetItem(BiTNode* pnode, Item item);
    //设置左子树
    BiTNode* SetLChild(BiTNode* parent, BiTNode* lchild);
    //设置右子树
    BiTNode* SetRChild(BiTNode* parent, BiTNode* rchild);
    //返回左子树
    BiTNode* GetLChild(BiTNode* tree);
    //返回右子树
    BiTNode* GetRChild(BiTNode* tree);
    //插入新子树
    BiTNode* InsertChild(BiTNode* parent, int lr, BiTNode* child);
    //释放节点
    void FreeNode(BiTNode* pnode);
    //删除子树
    void DeleteChild(BiTNode* parent, int lr);
    //清空一棵二叉树
    void BiTreeClear(BiTNode* tree);
    //销毁一棵二叉树
    void BiTreeDestroy(BiTNode* tree);
public:
    //先序遍历二叉树
    void PreOrderTraverse(BiTNode* tree, void(*visit)(Item));
    //中序遍历二叉树
    void InOrderTraverse(BiTNode* tree, void(*visit)(Item));
    //后序遍历二叉树
    void PostOrderTraverse(BiTNode* tree, void(*visit)(Item));
};




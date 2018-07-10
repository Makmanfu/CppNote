
#include "stdafx.h"
#include "CBITree.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


int CBITree::main(int argc, char** argv)
{
    //printf("--------------二叉树测试--------------\n");
    //BiTNode* n1 = MakeNode(10, NULL, NULL);
    //BiTNode* n2 = MakeNode(20, NULL, NULL);
    //BiTNode* n3 = MakeNode(30, n1, n2);
    //BiTNode* n4 = MakeNode(40, NULL, NULL);
    //BiTNode* n5 = MakeNode(50, NULL, NULL);
    //BiTNode* n6 = MakeNode(60, n4, n5);
    //BiTNode* n7 = MakeNode(70, NULL, NULL);
    //BiTree tree = InitBiTree(n7);
    //SetLChild(tree, n3);
    //SetRChild(tree, n6);
    //printf("树的深度为：%d", GetDepth(tree));
    //printf("\n先序遍历如下：");
    //PreOrderTraverse(tree, BiTreeprint);
    //printf("\n中序遍历如下：");
    //InOrderTraverse(tree, BiTreeprint);
    //printf("\n后序遍历如下：");
    //PostOrderTraverse(tree, BiTreeprint);
    //DeleteChild(tree, 1);
    //printf("\n后序遍历如下：");
    //PostOrderTraverse(tree, BiTreeprint);
    //DestroyBiTree(tree);
    //if (BiTreeIsEmpty(tree))
    //    printf("\n二叉树为空，销毁完毕\n");
    return 1;
}

CBITree::CBITree()
{
}

CBITree::~CBITree()
{
}

BiTNode* CBITree::BiTreeInit(BiTNode* root)
{
    BiTNode* tree = root;
    return tree;
}

int CBITree::BiTreeIsEmpty(BiTNode* tree)
{
    return (tree == NULL) ? 1 : 0;
}

BiTNode* CBITree::MakeNode(Item item, BiTNode* lchild, BiTNode* rchild)
{
    BiTNode* pnode = (BiTNode*)malloc(sizeof(BiTNode));
    if (pnode)
    {
        pnode->data = item;
        pnode->lchild = lchild;
        pnode->rchild = rchild;
    }
    return pnode;
}

int CBITree::GetDepth(BiTNode* tree)
{
    int cd = 0, ld = 0, rd = 0;
    if (tree)
    {
        ld = GetDepth(tree->lchild);
        rd = GetDepth(tree->rchild);
        cd = (ld > rd ? ld : rd);
        return cd + 1;
    }
    else
        return 0;
}

BiTNode* CBITree::GetRoot(BiTNode* tree)
{
    return tree;
}

Item CBITree::GetItem(BiTNode* pnode)
{
    return pnode->data;
}

void CBITree::SetItem(BiTNode* pnode, Item item)
{
    pnode->data = item;
}

BiTNode* CBITree::SetLChild(BiTNode* parent, BiTNode* lchild)
{
    parent->lchild = lchild;
    return lchild;
}

BiTNode* CBITree::SetRChild(BiTNode* parent, BiTNode* rchild)
{
    parent->rchild = rchild;
    return rchild;
}

BiTNode* CBITree::GetLChild(BiTNode* tree)
{
    if (tree)
        return tree->lchild;
    return NULL;
}

BiTNode* CBITree::GetRChild(BiTNode* tree)
{
    if (tree)
        return tree->rchild;
    return NULL;
}

BiTNode* CBITree::InsertChild(BiTNode* parent, int lr, BiTNode* child)
{
    if (parent)
    {
        if (lr == 0 && parent->lchild == NULL)
        {
            parent->lchild = child;
            return child;
        }
        if (lr == 1 && parent->rchild == NULL)
        {
            parent->rchild = child;
            return child;
        }
    }
    return NULL;
}

void CBITree::FreeNode(BiTNode* pnode)
{
    if (pnode != NULL)
        free(pnode);
}

void CBITree::DeleteChild(BiTNode* parent, int lr)
{
    if (parent)
    {
        if (lr == 0 && parent->lchild != NULL)
        {
            parent->lchild = NULL;
            FreeNode(parent->lchild);
        }
        if (lr == 1 && parent->rchild != NULL)
        {
            parent->rchild = NULL;
            FreeNode(parent->rchild);
        }
    }
}

void CBITree::BiTreeClear(BiTNode* tree)
{
    BiTNode* pnode = tree;
    if (pnode->lchild != NULL)
        BiTreeClear(pnode->lchild);
    if (pnode->rchild != NULL)
        BiTreeClear(pnode->rchild);
    FreeNode(pnode);
}

void CBITree::BiTreeDestroy(BiTNode* tree)
{
    if (tree)
        BiTreeClear(tree);
}

void CBITree::PreOrderTraverse(BiTNode* tree, void(*visit)(Item))
{
    BiTNode* pnode = tree;
    if (pnode)
    {
        visit(pnode->data);
        PreOrderTraverse(pnode->lchild, visit);
        PreOrderTraverse(pnode->rchild, visit);
    }
    return;
}

void CBITree::InOrderTraverse(BiTNode* tree, void(*visit)(Item))
{
    BiTNode* pnode = tree;
    if (pnode)
    {
        InOrderTraverse(pnode->lchild, visit);
        visit(pnode->data);
        InOrderTraverse(pnode->rchild, visit);
    }
    return;
}

void CBITree::PostOrderTraverse(BiTNode* tree, void(*visit)(Item))
{
    BiTNode*  pnode = tree;
    if (pnode)
    {
        PostOrderTraverse(pnode->lchild, visit);
        PostOrderTraverse(pnode->rchild, visit);
        visit(pnode->data);
    }
    return;
}







/*  二叉树
void testtree(void)
{
TreeNode<int> *root = new TreeNode<int>(0);
root->left = new TreeNode<int>(1);
root->right = new TreeNode<int>(2);

Tree<int> t(root);

vector<int>res;
t.PreOrder(res);
for (int i = 0; i < res.size(); i++)
{
cout << res[i];
cout << endl;
}

res.clear();
t.PreOrderNonRescursion(res);
for (int i = 0; i < res.size(); i++)
{
cout << res[i];
cout << endl;
}

res.clear();
t.InOrder(res);
for (int i = 0; i < res.size(); i++)
{
cout << res[i];
cout << endl;
}

res.clear();
t.InOrderNonRescursion(res);
for (int i = 0; i < res.size(); i++)
{
cout << res[i];
cout << endl;
}

res.clear();
t.PostOrder(res);
for (int i = 0; i < res.size(); i++)
{
cout << res[i];
cout << endl;
}

res.clear();
t.PostOrderNonRescursion(res);
for (int i = 0; i < res.size(); i++)
{
cout << res[i];
cout << endl;
}
}

*/

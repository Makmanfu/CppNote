//*********************************************************************
//
//          线性表的模板实现[包括链表,队列,栈]
//          二叉树 
//                                          @2017-07-05阿甘整理
//*********************************************************************
#ifndef CLINEARLIST_HPP
#define CLINEARLIST_HPP

//线性表的节点定义
template <class T>
class CNode
{
private:
    T* tElement;                        //存内容
    CNode* next;                        //存指向下一个的指针
public:
    CNode(T* tElement) : tElement(tElement), next(0) { };
    T* Element() const
    {
        return tElement;
    };
    CNode*& Next()
    {
        return next;
    };
};

//链表的实现
template <class T>
class CList
{
private:
    CNode<T>* head;
    DWORD dwCount;
public:
    CList() : dwCount(0), head(0) { };
    CList(T* tElement) : dwCount(1), head(new CNode<T>(tElement)) { };
    virtual ~CList() { };
protected:
    CList(const CList& list);
    CList& operator = (const CList& list);
public:
    //添加
    void Append(CNode<T>*& node, T* tElement)
    {
        if (node == NULL)
        {
            dwCount++;
            node = new CNode<T>(tElement);
            return;
        }
        Append(node->Next(), tElement);
    };
    //插入
    void Insert(T* tElement)
    {
        dwCount++;
        if (head == NULL)
        {
            head = new CNode<T>(tElement);
            return;
        }
        CNode<T>* tmp = head;
        head = new CNode<T>(tElement);
        head->Next() = tmp;
    };
    //移除
    bool Remove(T* tElement)
    {
        if (head == NULL)
            return NULL;
        if (head->Element() == tElement)
        {
            CNode<T>* tmp = head;
            head = head->Next();
            delete tmp;
            dwCount--;
            return true;
        }
        CNode<T>* tmp = head;
        CNode<T>* lst = head->Next();
        while (lst != NULL)
        {
            if (lst->Element() == tElement)
            {
                tmp->Next() = lst->Next();
                delete lst;
                dwCount--;
                return true;
            }
            lst = lst->Next();
            tmp = tmp->Next();
        }
        return false;
    };
    DWORD Count() const
    {
        return dwCount;
    };
    CNode<T>*& Head()
    {
        return head;
    };
    T* GetFirst()
    {
        return head != NULL ? head->Element() : NULL;
    };
    T* GetLast()
    {
        if (head)
        {
            CNode<T>* tmp = head;
            while (tmp->Next())
                tmp = tmp->Next();
            return tmp->Element();
        }
        return NULL;
    };
    T* GetNext(T* tElement)
    {
        if (head == NULL)
            return NULL;
        if (tElement == NULL)
            return GetFirst();
        if (head->Element() == tElement)
            return head->Next() != NULL ? head->Next()->Element() : NULL;
        CNode<T>* lst = head->Next();
        while (lst != NULL)
        {
            if (lst->Element() == tElement)
                return lst->Next() != NULL ? lst->Next()->Element() : NULL;
            lst = lst->Next();
        }
        return NULL;
    };
    //传函数指针查找对象
    T* Find(DWORD(*Function)(T* tParameter), DWORD dwValue)
    {
        try
        {
            T* tElement = NULL;
            while (tElement = GetNext(tElement))
            {
                if (Function(tElement) == dwValue)
                    return tElement;
            }
        }
        catch (...) {}
        return NULL;
    };
    //注意T必须重载==
    T* Find(T& tElement)
    {
        //注意T必须重载==
        try
        {
            T* tElem = NULL;
            while (tElem = GetNext(tElem))
            {
                if (tElem == tElement)
                    return tElem;
            }
        }
        catch (...) {}
        return NULL;
    };
};

//队列的实现
template<class T>
class CQueue : CList<T>
{
public:
    CQueue() : CList<T>() { };
    CQueue(T* tElement) : CList<T>(tElement) { };
    virtual ~CQueue() { };
protected:
    CQueue(const CQueue<T>& cQueue);
    CQueue<T>& operator = (const CQueue<T>& cQueue);
public:
    //入队
    virtual void Enqueue(T* tElement)
    {
        Append(Head(), tElement);
    };
    //出列
    virtual T* Dequeue()
    {
        T* tElement = GetFirst();
        Remove(tElement);
        return tElement;
    };
    //
    virtual T* Peek()
    {
        return GetFirst();
    };
    CList<T>::Count;
};

//栈的实现
template<class T>
class CStack : CList<T>
{
public:
    CStack() : CList<T>() { };
    CStack(T* tElement) : CList<T>(tElement) { };
    virtual ~CStack() { };
protected:
    CStack(const CStack<T>& cStack);
    CStack<T>& operator = (const CStack<T>& cStack);
public:
    virtual void Push(T* tElement)
    {
        Insert(tElement);
    };
    virtual T* Pop()
    {
        T* tElement = GetFirst();
        Remove(tElement);
        return tElement;
    };
    virtual T* Peek()
    {
        return GetFirst();
    };
    CList<T>::Count;
};

//树的节点定义
template<class T>
struct BiTNode
{
    T tElement;
    BiTNode<T> *lchild, *rchild;
    //默认构造
    BiTNode() :tElement(0), lchild(NULL), rchild(NULL){};
    //用值初始化构造
    BiTNode(T x,BiTNode<T>* pl, BiTNode<T>* pr)
    {
        tElement = x;
        lchild = pl;
        rchild = pr;
    };
};

//二叉树的实现[以下cout为记录顺序]
template<class T>
class CBiTree
{
private:
    BiTNode<T> *m_root;                   //根节点
    //二叉树的创建
    BiTNode<T> *CreateBiTree(T x[], int &n)
    {
        T ch = x[n];
        n++;
        if (ch == '#')
        {
            return NULL;
        }
        else
        {
            BiTNode<T> *Node = new BiTNode<T>;
            Node->tElement = ch;
            Node->lchild = createBiTree(x, n);
            Node->rchild = createBiTree(x, n);
            return Node;
        }
    };
    //二叉树的删除
    void DestroyAllNode(BiTNode<T>* root)
    {
        if (root != NULL)
        {
            DestroyAllNode(root->lchild);
            DestroyAllNode(root->rchild);
            delete root;
            root = NULL;
        }
    };
public:
    CBiTree()
    {
        T ch[100];
        cin.getline(ch, 100);
        int num = 0;
        m_root = CreateBiTree(ch, num);
    };
    ~CBiTree()
    {
        DestroyAllNode(m_root);
    };
    //深度
    int GetDepth(BiTNode<T>* pRoot)
    {
        if (pRoot)
        {
            int depthLeft = GetDepth(pRoot->lChild);
            int depthRight = GetDepth(pRoot->rChild);
            return depthLeft > depthRight ? (depthLeft + 1) : (depthRight + 1);
        }
        return 0;
    };
    //节点个数
    int GetNodeNum(BiTNode<T>* pRoot)
    {
        int num = 0;
        if (pRoot)
        {
            num += 1;
            num += GetNodeNum(pRoot->lChild);
            num += GetNodeNum(pRoot->rChild);
        }
        return num;
    };
    //叶子节点个数
    int GetLeafNodeNum(BiTNode<T>* pRoot)
    {
        int num = 0;
        if (pRoot)
        {
            if (pRoot->lChild == NULL && pRoot->rChild == NULL)
                num += 1;
            else
            {
                num += GetLeafNodeNum(pRoot->lChild);
                num += GetLeafNodeNum(pRoot->rChild);
            }
        }
        return num;
    };
    //是不是平衡二叉树
    bool IsAVL(BiTNode<T>* pRoot, int* pHeight = NULL)
    {
        if (pRoot)
        {
            int heightLeft = 0;
            bool resultLeft = IsAVL(pRoot->lChild, &heightLeft);
            int heightRight = 0;
            bool resultRight = IsAVL(pRoot->rChild, &heightRight);
            if (resultLeft && resultRight && abs(heightLeft - heightRight) <= 1)
            {
                if (pHeight)
                    *pHeight = max(heightLeft, heightRight) + 1;
                return true;
            }
            else
            {
                if (pHeight)
                    *pHeight = max(heightLeft, heightRight) + 1;
                return false;
            }
        }
        else
        {
            if (pHeight)
                *pHeight = 0;
            return true;
        }
    };
    //前序遍历[brec递归]
    void PreOrder(BiTNode<T>* pRoot, bool brec = false)
    {
        if (pRoot != NULL)
        {
            if (brec)
            {
                cout << pRoot->tElement << " ";
                PreOrderTraverse(pRoot->lchild);
                PreOrderTraverse(pRoot->rchild);
            }
            else {
                BiTNode<T>* p = pRoot;
                stack<BiTNode<T>*> s;
                while (NULL != p || !s.empty())
                {
                    while (NULL != p)
                    {
                        cout << p->tElement << " ";
                        s.push(p);
                        p = p->lchild;
                    }
                    if (!s.empty())
                    {
                        p = s.top();
                        s.pop();
                        p = p->rchild;
                    }
                }
            }
        }
    };
    //递归中序遍历
    void InOrder(BiTNode<T>* pRoot, bool brec = false)
    {
        if (pRoot != NULL)
        {
            if (brec)
            {
                InOrderTraverse(pRoot->lchild);
                cout << pRoot->tElement << " ";
                InOrderTraverse(pRoot->rchild);
            }
            else {
                BiTNode<T>* p = pRoot;
                stack<BiTNode<T>*> s;
                while (NULL != p || !s.empty())
                {
                    while (NULL != p)
                    {
                        s.push(p);
                        p = p->lchild;
                    }
                    if (!s.empty())
                    {
                        p = s.top();
                        cout << p->tElement << " ";  //res.push_back(p->tElement);
                        s.pop();
                        p = p->rchild;
                    }
                }
            }
        }
    };
    //递归后序遍历
    void PostOrder(BiTNode<T>* pRoot, bool brec = false)
    {
        if (pRoot != NULL)
        {
            if (brec)
            {
                PostOrderTraverse(pRoot->lchild);
                PostOrderTraverse(pRoot->rchild);
                cout << pRoot->tElement << " ";
            }
            else {
                stack<BiTNode<T>*> s;
                s.push(pRoot);
                BiTNode<T>* pre = NULL;
                BiTNode<T>* cur = NULL;
                while (!s.empty())
                {
                    cur = s.top();
                    if ((cur->lchild == NULL && cur->rchild == NULL) ||//没有子结点
                        (pre != NULL) && (pre == cur->lchild || pre == cur->rchild))//或子结点已访问
                    {
                        cout << cur->tElement << " ";
                        s.pop();
                        pre = cur;
                    }
                    else
                    {
                        if (cur->rchild != NULL)
                            s.push(cur->rchild);
                        if (cur->lchild != NULL)
                            s.push(cur->lchild);
                    }
                }
            }
        }
    };
    //层次遍历
    void PrintNodeTraverse(BiTNode<T>* pRoot)
    {
        vector<BiTNode<T>*> vec;
        vec.push_back(pRoot);
        int cur = 0;                            //保存的是之前所以层的结点数
        int last = 1;                           //加上待遍历层结点数后的总结点数
        while (cur < vec.size())
        {
            last = vec.size();                  //重置上一行结点数
            while (cur < last)                  //做几次是由上一层的结点数决定的
            {
                cout << vec[cur]->tElement << " ";  //访问节点
                if (vec[cur]->lchild)           //当前访问节点的左节点不为空则压入
                    vec.push_back(vec[cur]->lchild);
                if (vec[cur]->rchild)           //当前访问节点的右节点不为空则压入，注意左右节点的访问顺序不能颠倒
                    vec.push_back(vec[cur]->rchild);
                cur++;
            }
            cout << endl;                       //当cur=last时说明该层的结点已被遍历
        }

        //方法2
        //if (pRoot)
        //{
        //    BiTNode<T>* p = NULL;
        //    queue<BiTNode<T>*> q;
        //    q.push(pRoot);
        //    while (!q.empty())
        //    {
        //        p = q.front();
        //        q.pop();
        //        cout << p->data << " ";
        //        if (p->lChild)
        //            q.push(p->lChild);
        //        if (p->rChild)
        //            q.push(p->rChild);
        //    }
        //}
    };
    //获取根节点
    BiTNode<T>* GetRoot(void)
    {
        return m_root;
    };
};

//红黑树




#endif

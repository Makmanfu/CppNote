//*********************************************************************
//
//          ���Ա��ģ��ʵ��[��������,����,ջ]
//                                          @2017-07-05��������
//*********************************************************************
#ifndef CLINEARLIST_HPP
#define CLINEARLIST_HPP

template <class T>
class CNode
{
private:
    T* tElement;
    CNode* next;
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

//�����ʵ��
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
    //���
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
    //����
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
    //�Ƴ�
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
    //������ָ����Ҷ���
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
    //ע��T��������==
    T* Find(T& tElement)
    {
        //ע��T��������==
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

//���е�ʵ��
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
    //���
    virtual void Enqueue(T* tElement)
    {
        Append(Head(), tElement);
    };
    //����
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

//ջ��ʵ��
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


#endif

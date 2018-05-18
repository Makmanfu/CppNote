//----------------------------------------------------------------------------
//  CMyThread���CThreadPool��Ϊ������
//
//
//----------------------------------------------------------------------------

#pragma once
#include <deque>
#include <list>
#include <stack>
#include "WinThread.h"

enum PRIORITY
{
    NORMAL,
    HIGH
};

class CMyThread;    //ǰ������

//������ࡣÿ������Ӧ�̳��Դ��࣬��ʵ��taskProc��Ա������
class CTask
{
public:
    CTask(int id);
    ~CTask(void);
public:
    //������ ��Ҫʵ�ֵ�
    virtual void taskProc() = 0;
    int getID();
private:
    int m_ID;
};

//CMyStack�ࣺ�����̶߳�ջ�����Դ洢���еĹ����̡߳�
class CMyStack
{
public:
    CMyStack(void);
    ~CMyStack(void);
public:
    CMyThread* pop();
    bool push(CMyThread*);
    int getSize();
    bool isEmpty();
    bool clear();
private:
    std::stack<CMyThread*> m_stack;
    CThreadLock m_mutext;
};

//CMyList�ࣺ��̶߳��С����Դ洢Ŀǰ����ִ��������̡߳�
class CMyList
{
public:
    CMyList(void);
    ~CMyList(void);
public:
    bool addThread(CMyThread* t);
    bool removeThread(CMyThread* t);
    int getSize();
    bool isEmpty();
    bool clear();
private:
    std::list<CMyThread*>m_list;
    CThreadLock m_mutex;
};

//CTaskQueue�ࣺ������С����Դ洢Ҫִ�е�����
class CMyQueue
{
public:
    CMyQueue(void);
    ~CMyQueue(void);
public:
    CTask* pop();
    bool push(CTask* t);
    bool pushFront(CTask* t);    //�嵽���ס�
    bool isEmpty();
    bool clear();
private:
    std::deque<CTask*>m_TaskQueue;
    CThreadLock m_mutex;
};

class CBaseThreadPool
{
public:
    virtual bool SwitchActiveThread(CMyThread*) = 0;
};

//CMyThread�ࣺ�����߳��ࡣÿ�������һ���̡߳�ͬʱ����һ�����������
class CMyThread
{
public:
    CMyThread(CBaseThreadPool* threadPool);
    ~CMyThread(void);
public:
    bool startThread();
    bool suspendThread();
    bool resumeThread();
    bool assignTask(CTask* pTask);
    bool startTask();
    static DWORD WINAPI threadProc(LPVOID pParam);
    DWORD m_threadID;
    HANDLE m_hThread;
    bool m_bIsExit;
private:
    HANDLE m_hEvent;
    CTask* m_pTask;
    CBaseThreadPool* m_pThreadPool;
};

//CThreadPool�ࣺ�̳߳��࣬���Դ����������̳߳أ�ͬʱʵ�ֶ��̳߳����̵߳ĵ��ȡ�
class CMyThreadPool: public CBaseThreadPool
{
public:
    CMyThreadPool(int num);
    ~CMyThreadPool(void);
public:
    //ȡ��ջ���߳�
    virtual CMyThread* PopIdleThread();
    //���̴߳ӻ����ȡ������������߳�ջ�С���ȡ֮ǰ�жϴ�ʱ��������Ƿ����������������Ϊ��ʱ�Ź��𡣷�����������ȡ�������ִ�С�
    virtual bool SwitchActiveThread(CMyThread*);
    //ȡ������
    virtual CTask* GetNewTask();
public:
    //priorityΪ���ȼ��������ȼ������񽫱����뵽���ס�
    bool addTask(CTask* t, PRIORITY priority);
    bool start();//��ʼ���ȡ�
    bool destroyThreadPool();
private:
    int m_nThreadNum;
    bool m_bIsExit;
    CMyStack m_IdleThreadStack;
    CMyList m_ActiveThreadList;
    CMyQueue m_TaskQueue;
};

//������
class CTestTask : public CTask
{
public:
    CTestTask(int id);
    ~CTestTask(void);
public:
    //��ʱ����
    virtual void taskProc();
};


void Demo_CMyThreadPool(void);





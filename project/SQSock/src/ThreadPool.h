//----------------------------------------------------------------------------
//  CMyThread类和CThreadPool类为核心类
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

class CMyThread;    //前置声明

//任务基类。每个任务应继承自此类，并实现taskProc成员函数。
class CTask
{
public:
    CTask(int id);
    ~CTask(void);
public:
    //任务函数 需要实现的
    virtual void taskProc() = 0;
    int getID();
private:
    int m_ID;
};

//CMyStack类：空闲线程堆栈，用以存储空闲的工作线程。
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

//CMyList类：活动线程队列。用以存储目前正在执行任务的线程。
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

//CTaskQueue类：任务队列。用以存储要执行的任务。
class CMyQueue
{
public:
    CMyQueue(void);
    ~CMyQueue(void);
public:
    CTask* pop();
    bool push(CTask* t);
    bool pushFront(CTask* t);    //插到队首。
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

//CMyThread类：工作线程类。每个类管理一个线程。同时关联一个任务类对象。
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

//CThreadPool类：线程池类，用以创建并管理线程池，同时实现对线程池内线程的调度。
class CMyThreadPool: public CBaseThreadPool
{
public:
    CMyThreadPool(int num);
    ~CMyThreadPool(void);
public:
    //取出栈顶线程
    virtual CMyThread* PopIdleThread();
    //将线程从活动队列取出，放入空闲线程栈中。在取之前判断此时任务队列是否有任务。如任务队列为空时才挂起。否则从任务队列取任务继续执行。
    virtual bool SwitchActiveThread(CMyThread*);
    //取新任务
    virtual CTask* GetNewTask();
public:
    //priority为优先级。高优先级的任务将被插入到队首。
    bool addTask(CTask* t, PRIORITY priority);
    bool start();//开始调度。
    bool destroyThreadPool();
private:
    int m_nThreadNum;
    bool m_bIsExit;
    CMyStack m_IdleThreadStack;
    CMyList m_ActiveThreadList;
    CMyQueue m_TaskQueue;
};

//测试类
class CTestTask : public CTask
{
public:
    CTestTask(int id);
    ~CTestTask(void);
public:
    //耗时函数
    virtual void taskProc();
};


void Demo_CMyThreadPool(void);





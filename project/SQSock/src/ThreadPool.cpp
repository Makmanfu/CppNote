
#include "stdafx.h"
#include "ThreadPool.h"
#include "windows.h"
#include <cassert>
#include <iostream>
using namespace std;

CTask::CTask(int id)
{
    m_ID = id;
}

CTask::~CTask(void)
{
}

int CTask::getID()
{
    return m_ID;
}


//-------------------------------------------------------------------
CMyStack::CMyStack(void)
{
}

CMyStack::~CMyStack(void)
{
}

CMyThread* CMyStack::pop()
{
    m_mutext.Lock();
    if (!m_stack.empty())
    {
        CMyThread* t = m_stack.top();
        m_stack.pop();
        m_mutext.UnLock();
        return t;
    }
    m_mutext.UnLock();
    return NULL;
}

bool CMyStack::push(CMyThread* t)
{
    assert(t);
    if (!t)
        return false;
    m_mutext.Lock();
    m_stack.push(t);
    m_mutext.UnLock();
    return true;
}

int CMyStack::getSize()
{
    m_mutext.Lock();
    int size = (int) m_stack.size();
    m_mutext.UnLock();
    return size;
}

bool CMyStack::isEmpty()
{
    m_mutext.Lock();
    bool ret = m_stack.empty();
    m_mutext.UnLock();
    return ret;
}

bool CMyStack::clear()
{
    m_mutext.Lock();
    CMyThread* pThread = NULL;
    while (!m_stack.empty())
    {
        pThread = m_stack.top();
        m_stack.pop();
        pThread->resumeThread();
        pThread->m_bIsExit = true;
        delete pThread;
    }
    m_mutext.UnLock();
    return true;
}

//-------------------------------------------------------------------
CMyList::CMyList(void)
{
}

CMyList::~CMyList(void)
{
}

bool CMyList::addThread(CMyThread* t)
{
    assert(t);
    if (!t)
        return false;
    m_mutex.Lock();
    m_list.push_back(t);
    m_mutex.UnLock();
    return true;
}

bool CMyList::removeThread(CMyThread* t)
{
    //  std::list<CThread*>::iterator iter=m_list.begin();
    //  for(iter;iter!=m_list.end();iter++)
    //  {
    //      if(*iter==t)
    //      {
    //          break;
    //      }
    //  }
    assert(t);
    if (!t)
        return false;
    m_mutex.Lock();
    m_list.remove(t);
    m_mutex.UnLock();
    return true;
}

int CMyList::getSize()
{
    m_mutex.Lock();
    int size = (int) m_list.size();
    m_mutex.UnLock();
    return size;
}

bool CMyList::isEmpty()
{
    m_mutex.Lock();
    bool ret = m_list.empty();
    m_mutex.UnLock();
    return ret;
}

bool CMyList::clear()
{
    m_mutex.Lock();
    std::list<CMyThread*>::iterator iter = m_list.begin();
    for (; iter != m_list.end(); iter++)
        delete(*iter);
    m_list.clear();
    m_mutex.UnLock();
    return true;
}

//-------------------------------------------------------------------
CMyQueue::CMyQueue(void)
{
}

CMyQueue::~CMyQueue(void)
{
}

CTask* CMyQueue::pop()
{
    CTask* p = NULL;
    m_mutex.Lock();
    if (!m_TaskQueue.empty())
    {
        p = m_TaskQueue.front();
        m_TaskQueue.pop_front();
    }
    m_mutex.UnLock();
    return p;
}

bool CMyQueue::push(CTask* t)
{
    if (!t)
        return false;
    m_mutex.Lock();
    m_TaskQueue.push_back(t);
    m_mutex.UnLock();
    return true;
}

bool CMyQueue::isEmpty()
{
    bool ret = false;
    m_mutex.Lock();
    ret = m_TaskQueue.empty();
    m_mutex.UnLock();
    return ret;
}

bool CMyQueue::pushFront(CTask* t)
{
    if (!t)
        return false;
    m_mutex.Lock();
    m_TaskQueue.push_front(t);
    m_mutex.UnLock();
    return true;
}

bool CMyQueue::clear()
{
    m_mutex.Lock();
    //  std::deque<CTask*>::iterator iter=m_TaskQueue.begin();
    //  for(;iter!=m_TaskQueue.end();iter++)
    //  {
    //      delete (*iter);
    //  }
    m_TaskQueue.clear();
    m_mutex.UnLock();
    return true;
}

//--------------------------------------------------------------------

#define WAIT_TIME 20
CMyThread::CMyThread(CBaseThreadPool* threadPool)
{
    m_pTask = NULL;
    //m_bIsActive=false;
    m_pThreadPool = threadPool;
    m_hEvent = CreateEvent(NULL, false, false, NULL);
    m_bIsExit = false;
}

//bool CMyThread::m_bIsActive=false;
CMyThread::~CMyThread(void)
{
    CloseHandle(m_hEvent);
    CloseHandle(m_hThread);
}

bool CMyThread::startThread()
{
    m_hThread = CreateThread(0, 0, threadProc, this, 0, &m_threadID);
    if (m_hThread == INVALID_HANDLE_VALUE)
        return false;
    return true;
}

bool CMyThread::suspendThread()
{
    ResetEvent(m_hEvent);
    return true;
}

//有任务到来，通知线程继续执行。
bool CMyThread::resumeThread()
{
    SetEvent(m_hEvent);
    return true;
}

DWORD WINAPI CMyThread::threadProc(LPVOID pParam)
{
    CMyThread* pThread = (CMyThread*) pParam;
    while (!pThread->m_bIsExit)
    {
        //等待m_hEvent信号触发
        DWORD ret = WaitForSingleObject(pThread->m_hEvent, INFINITE);
        if (ret == WAIT_OBJECT_0)
        {
            if (pThread->m_pTask)
            {
                pThread->m_pTask->taskProc();//执行任务。
                //delete pThread->m_pTask;//用户传入的空间交由用户处理，内部不处理。如从CTask从堆栈分配，此处会有问题。
                pThread->m_pTask = NULL;
                pThread->m_pThreadPool->SwitchActiveThread(pThread);
            }
        }
    }
    return 0;
}

//将任务关联到线程类。
bool CMyThread::assignTask(CTask* pTask)
{
    assert(pTask);
    if (!pTask)
        return false;
    m_pTask = pTask;
    return true;
}

//开始执行任务。
bool CMyThread::startTask()
{
    resumeThread();
    return true;
}

//--------------------------------------------------------------------
//构造函数直接 创建线程数
CMyThreadPool::CMyThreadPool(int num)
{
    m_bIsExit = false;
    for (int i = 0; i < num; i++)
    {
        CMyThread* p = new CMyThread(this);
        m_IdleThreadStack.push(p);
        p->startThread();
    }
}

CMyThreadPool::~CMyThreadPool(void)
{
}

CMyThread* CMyThreadPool::PopIdleThread()
{
    CMyThread* pThread = m_IdleThreadStack.pop();
    //pThread->m_bIsActive=true;
    return pThread;
}

bool CMyThreadPool::SwitchActiveThread(CMyThread* t)
{
    if (!m_TaskQueue.isEmpty())   //任务队列不为空，继续取任务执行。
    {
        CTask* pTask = NULL;
        pTask = m_TaskQueue.pop();
        std::cout << "线程：" << t->m_threadID << "   执行   " << pTask->getID() << std::endl;
        t->assignTask(pTask);
        t->startTask();
    }
    else    //任务队列为空，该线程挂起。
    {
        m_ActiveThreadList.removeThread(t);
        m_IdleThreadStack.push(t);
    }
    return true;
}

bool CMyThreadPool::addTask(CTask* t, PRIORITY priority)
{
    assert(t);
    if (!t || m_bIsExit)
        return false;
    CTask* task = NULL;
    std::cout << "                       [" << t->getID() << "]添加！" << std::endl;
    if (priority == NORMAL)
        m_TaskQueue.push(t);    //进入任务队列。
    else if (HIGH)
        m_TaskQueue.pushFront(t);    //高优先级任务。
    if (!m_IdleThreadStack.isEmpty())   //存在空闲线程。调用空闲线程处理任务。
    {
        task = m_TaskQueue.pop(); //取出列头任务。
        if (task == NULL)
        {
            std::cout << "任务取出出错。" << std::endl;
            return false;
        }
        //取出线程
        CMyThread* pThread = PopIdleThread();
        std::cout << "【" << pThread->m_threadID << "】 执行   【" << task->getID() << "】" << std::endl;
        m_ActiveThreadList.addThread(pThread);
        //绑定task
        pThread->assignTask(task);
        //启动task
        pThread->startTask();
    }
    return true;
}

bool CMyThreadPool::start()
{
    return 0;
}

CTask* CMyThreadPool::GetNewTask()
{
    if (m_TaskQueue.isEmpty())
        return NULL;
    CTask* task = m_TaskQueue.pop();//取出列头任务。
    if (task == NULL)
    {
        std::cout << "任务取出出错。" << std::endl;
        return 0;
    }
    return task;
}

bool CMyThreadPool::destroyThreadPool()
{
    m_bIsExit = true;
    m_TaskQueue.clear();
    m_IdleThreadStack.clear();
    m_ActiveThreadList.clear();
    return true;
}

//--------------------------------------------------------------------

CTestTask::CTestTask(int id)
    : CTask(id)
{
}

CTestTask::~CTestTask(void)
{
}

void CTestTask::taskProc()
{
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            cout << j << " - " << i << endl;
}

//--------------------------------------------------------------------

void Demo_CMyThreadPool(void)
{
    CTestTask* p = NULL;
    //线程池设置为10个
    CMyThreadPool threadpool(10);
    for (int i = 0; i < 100; i++)
    {
        //将线程塞进去
        p = new CTestTask(i);
        threadpool.addTask(p, PRIORITY::NORMAL);
    }
    //p=new CTestTask(102200);
    //threadpool.addTask(p,PRIORITY::HIGH);
    //threadpool.destroyThreadPool();
    //主线程执行其他工作。
    {
        Sleep(1000 * 1000);
    }
    return ;
}

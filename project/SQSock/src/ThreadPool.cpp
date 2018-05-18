
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

//����������֪ͨ�̼߳���ִ�С�
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
        //�ȴ�m_hEvent�źŴ���
        DWORD ret = WaitForSingleObject(pThread->m_hEvent, INFINITE);
        if (ret == WAIT_OBJECT_0)
        {
            if (pThread->m_pTask)
            {
                pThread->m_pTask->taskProc();//ִ������
                //delete pThread->m_pTask;//�û�����Ŀռ佻���û������ڲ����������CTask�Ӷ�ջ���䣬�˴��������⡣
                pThread->m_pTask = NULL;
                pThread->m_pThreadPool->SwitchActiveThread(pThread);
            }
        }
    }
    return 0;
}

//������������߳��ࡣ
bool CMyThread::assignTask(CTask* pTask)
{
    assert(pTask);
    if (!pTask)
        return false;
    m_pTask = pTask;
    return true;
}

//��ʼִ������
bool CMyThread::startTask()
{
    resumeThread();
    return true;
}

//--------------------------------------------------------------------
//���캯��ֱ�� �����߳���
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
    if (!m_TaskQueue.isEmpty())   //������в�Ϊ�գ�����ȡ����ִ�С�
    {
        CTask* pTask = NULL;
        pTask = m_TaskQueue.pop();
        std::cout << "�̣߳�" << t->m_threadID << "   ִ��   " << pTask->getID() << std::endl;
        t->assignTask(pTask);
        t->startTask();
    }
    else    //�������Ϊ�գ����̹߳���
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
    std::cout << "                       [" << t->getID() << "]��ӣ�" << std::endl;
    if (priority == NORMAL)
        m_TaskQueue.push(t);    //����������С�
    else if (HIGH)
        m_TaskQueue.pushFront(t);    //�����ȼ�����
    if (!m_IdleThreadStack.isEmpty())   //���ڿ����̡߳����ÿ����̴߳�������
    {
        task = m_TaskQueue.pop(); //ȡ����ͷ����
        if (task == NULL)
        {
            std::cout << "����ȡ������" << std::endl;
            return false;
        }
        //ȡ���߳�
        CMyThread* pThread = PopIdleThread();
        std::cout << "��" << pThread->m_threadID << "�� ִ��   ��" << task->getID() << "��" << std::endl;
        m_ActiveThreadList.addThread(pThread);
        //��task
        pThread->assignTask(task);
        //����task
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
    CTask* task = m_TaskQueue.pop();//ȡ����ͷ����
    if (task == NULL)
    {
        std::cout << "����ȡ������" << std::endl;
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
    //�̳߳�����Ϊ10��
    CMyThreadPool threadpool(10);
    for (int i = 0; i < 100; i++)
    {
        //���߳�����ȥ
        p = new CTestTask(i);
        threadpool.addTask(p, PRIORITY::NORMAL);
    }
    //p=new CTestTask(102200);
    //threadpool.addTask(p,PRIORITY::HIGH);
    //threadpool.destroyThreadPool();
    //���߳�ִ������������
    {
        Sleep(1000 * 1000);
    }
    return ;
}

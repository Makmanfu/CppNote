
#include "stdafx.h"
#include "WinThread.h"
#include <assert.h>


namespace ThreadWork
{
    DWORD WINAPI ThreadObj::StartAddress(LPVOID lpParameter)
    {
        int nRet = 0;
        ThreadObj* pThread = (ThreadObj*)lpParameter;
        if (NULL != pThread)
            nRet = pThread->Run(lpParameter);
        return (nRet);
    }

    ThreadObj::ThreadObj(void) :hThread(0), dwThreadId(0), lpUserData(NULL)
    {
    }

    ThreadObj::~ThreadObj(void)
    {
    }

    bool ThreadObj::Suspend(void)
    {
        if (NULL == hThread)
            return false;
        ::SuspendThread(hThread);
        return true;        //return (bool)::SuspendThread(m_hThread);
    }

    bool ThreadObj::Resume(void)
    {
        if (NULL == hThread)
            return false;
        ::ResumeThread(hThread);
        return true;        //return (bool)::ResumeThread(m_hThread);
    }

    bool ThreadObj::Stop(void)
    {
        return (::TerminateThread(hThread, 1) ? true : false);
    }

    int ThreadObj::Exit(int nCount /*= 2*/, int nTimeout_ /*= 30000*/)
    {
        int nRet = 0;
        if ((nCount < 1) || (nTimeout_ < 100))
        {
            nCount = 1;
            nTimeout_ = 100;
        }
        bool bExitFlag = false;
        DWORD dwRetValue = 0;
        for (int i = 0; (i < 2) && (!bExitFlag); i++)
        {
            dwRetValue = WaitForSingleObject(hThread, nTimeout_);
            switch (dwRetValue)
            {
            case WAIT_OBJECT_0:
                bExitFlag = true;
                break;
            case WAIT_TIMEOUT:
            case WAIT_FAILED:
                nRet = -1;
                ::TerminateThread(hThread, 1);
            default:
                nRet = -1;
                break;
            };
        }
        CloseHandle(hThread);
        hThread = 0;
        return (nRet);
    }

    inline void ThreadObj::SetId(DWORD dwId)
    {
        this->dwThreadId = dwId;
    }

    inline DWORD ThreadObj::GetId(void) const
    {
        return dwThreadId;
    }

    inline void ThreadObj::SetPriority(int nPrio_)
    {
    }

    inline int ThreadObj::GetPriority(void)
    {
        return 0;
    }

    inline void ThreadObj::SetUserData(void* lpUserData)
    {
        this->lpUserData = lpUserData;
    }

    inline void* ThreadObj::GetUserData(void) const
    {
        return lpUserData;
    }

    inline HANDLE ThreadObj::GetHandle(void)
    {
        return hThread;
    }

    inline bool ThreadObj::CloseThread(void)
    {
        return (CloseHandle(hThread) ? true : false);
    }

    //DWORD ThreadObj::Run(LPVOID lpParameter /*= 0*/) /* = 0*/
    //{
    //    return 1;
    //}

    bool ThreadObj::Create(LPTHREAD_START_ROUTINE TdProc)
    {
        unsigned int initFlag = CREATE_SUSPENDED;
        hThread = (HANDLE)(CreateThread(NULL, 0, (TdProc != NULL) ? TdProc : ThreadObj::StartAddress,
            this, initFlag, &dwThreadId));
        if (NULL == hThread)
            return false;
        return true;
    }


    WinThread::WinThread() : WinThread("") 
    {
    }

    WinThread::WinThread(string sName_) : hWorkEvent(NULL), bPauseFlag(false), sName(sName_)
    {
        InterlockedExchange(&nRunningFlag, TRUE);
    }

    WinThread::~WinThread()
    {
    }

    bool WinThread::StartWork()
    {
        InterlockedExchange(&nRunningFlag, TRUE);
        hWorkEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        if (NULL == hWorkEvent)
        {
            InterlockedExchange(&nRunningFlag, FALSE);
            return false;
        }
        if (!Create())
            CloseHandle(hWorkEvent);
        Resume();
        return true;
    }

    bool WinThread::EndWork()
    {
        InterlockedExchange(&nRunningFlag, FALSE);
        SetEvent(hWorkEvent);
        Exit(1, 1);
        CloseHandle(hWorkEvent);
        hWorkEvent = 0;
        return true;
    }

    bool WinThread::SendWorkEvent(void)
    {
        if ((NULL != hWorkEvent) && (!GetPauseFlag()))
        {
            SetEvent(hWorkEvent);
            return true;
        }
        return false;
    }
    
    DWORD WinThread::Run(LPVOID lpParameter /*= 0*/)
    {
        DWORD dwResult = 0;
        int nRet = WORK_NG;
        if (!BeforeRunning())
            return (WORK_NG);
        while (InterlockedCompareExchange(&nRunningFlag, TRUE, TRUE))
        {
            dwResult = WaitForSingleObject(hWorkEvent, INFINITE);
            switch (dwResult)
            {
            case WAIT_OBJECT_0:
                printf("wait m_hWorkEvent WAIT_OBJECT_0");
                break;
            case WAIT_TIMEOUT:
                printf("wait m_hWorkEvent WAIT_TIMEOUT");
                break;
            default:
                printf("wait m_hWorkEvent error");
                break;
            }
            printf("HandleMessage");
            nRet = HandleMessage();
            if (WORK_EXIT == nRet) 
                break; 
        }
        AfterRunning();
        return (WORK_OK);
    }

    DWORD WinThread::HandleMessage(void)
    {
        printf("handle messsage in base class");
        return (WORK_EXIT);
    }

    bool WinThread::BeforeRunning(void)
    {
        return true;
    }

    bool WinThread::AfterRunning(void)
    {
        return true;
    }

    inline void WinThread::SetState(DWORD dwNewState)
    {
    }

    inline DWORD WinThread::GetState(void) const
    {
        return 0;
    }

    inline void WinThread::SetName(string sName_)
    {
        sName = sName_;
    }

    inline string WinThread::GetName(void) const
    {
        return sName;
    }

    inline void WinThread::SetPauseFlag(bool bPause_)
    {
        bPauseFlag = bPause_;
    }

    inline bool WinThread::GetPauseFlag(void) const
    {
        return bPauseFlag;
    }


    typedef struct _THREAD_CONTEXT
    {
        CWorkQueue* pWorkQueue;
        void* pThreadData;
    } THREAD_CONTEXT, *PTHREAD_CONTEXT;

    bool CWorkQueue::Create(const unsigned int  nNumberOfThreads, void** ThreadData)
    {
        //����������У���ź�����Ҫִ�е�����
        m_pWorkItemQueue = new WorkItemQueue();
        if (NULL == m_pWorkItemQueue)
            return false;
        //m_phSincObjectsArray�������̳߳ص��ź������¼�
        //m_phSincObjectsArray[ABORT_EVENT_INDEX]��������¼������û������˳��¼�ʱʹ��
        //m_phSincObjectsArray[SEMAPHORE_INDEX]�����ź��������û�����ִ������ʱʹ��
        //�����ź��������߳�ͬ��ʹ�ã�
        /*  ���ź��������Ƕ������ֲ����� Wait���ȴ��� �� Release���ͷţ���
        ��һ���̵߳���Wait����ʱ����Ҫô�õ���ԴȻ���ź�����һ��Ҫôһֱ����ȥ��ָ�����������У���
        ֱ���ź������ڵ���һʱ��Release���ͷţ�ʵ���������ź�����ִ�мӲ���*/
        m_phSincObjectsArray[SEMAPHORE_INDEX] = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
        if (m_phSincObjectsArray[SEMAPHORE_INDEX] == NULL)
        {
            delete m_pWorkItemQueue;
            m_pWorkItemQueue = NULL;
            return false;
        }
        //�����¼�Ϊ�ֶ���λ��һ��ֻ�ܽ���һ����FalseΪ��ʼ��������״̬�����߳�ͬ��ʹ�ã�
        m_phSincObjectsArray[ABORT_EVENT_INDEX] = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (m_phSincObjectsArray[ABORT_EVENT_INDEX] == NULL)
        {
            delete m_pWorkItemQueue;
            m_pWorkItemQueue = NULL;
            CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
            return false;
        }
        //�����߳�����
        m_phThreads = new HANDLE[nNumberOfThreads];
        if (m_phThreads == NULL)
        {
            delete m_pWorkItemQueue;
            m_pWorkItemQueue = NULL;
            CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
            CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
            return false;
        }
        unsigned int i;
        m_nNumberOfThreads = nNumberOfThreads;
        DWORD dwThreadId;
        PTHREAD_CONTEXT pThreadsContext;
        //�������е��߳�
        for (i = 0; i < nNumberOfThreads; i++)
        {
            //��ʼ���̺߳�������ʱ����Ĳ���
            pThreadsContext = new THREAD_CONTEXT;
            pThreadsContext->pWorkQueue = this;
            pThreadsContext->pThreadData = ThreadData == NULL ? NULL : ThreadData[i];
            //�����߳�
            m_phThreads[i] = CreateThread(NULL,
                0,
                CWorkQueue::ThreadFunc,
                pThreadsContext,
                0,
                &dwThreadId);
            if (m_phThreads[i] == NULL)
            {
                delete pThreadsContext;
                m_nNumberOfThreads = i;
                Destroy(5);
                return false;
            }
        }
        return true;
    }

    bool CWorkQueue::InsertWorkItem(WorkItemBase* pWorkItem)
    {
        assert(pWorkItem != NULL);
        //���̻߳�����ʣ������ٽ���
        m_Lock.Lock();
        //������������
        m_pWorkItemQueue->push(pWorkItem);
        //�뿪�ٽ���
        m_Lock.UnLock();
        //�ͷ��ź�����ʹ�ź�����1����ʹ�����Wailt����ִ��
        if (!ReleaseSemaphore(m_phSincObjectsArray[SEMAPHORE_INDEX], 1, NULL))
        {
            assert(false);
            return false;
        }
        return true;
    }

    WorkItemBase*  CWorkQueue::RemoveWorkItem()
    {
        WorkItemBase* pWorkItem;
        //���̼߳���ʻ��⣬�����ٽ���
        m_Lock.Lock();
        //�Ƴ�����
        pWorkItem = m_pWorkItemQueue->front();
        m_pWorkItemQueue->pop();
        //�뿪�ٽ����������ȴ��߳̿��Խ����ٽ���
        m_Lock.UnLock();
        assert(pWorkItem != NULL);
        return pWorkItem;
    }

    unsigned long __stdcall CWorkQueue::ThreadFunc(void*  pParam)
    {
        //�����߳�ʱ����Ĳ���
        PTHREAD_CONTEXT pThreadContext = (PTHREAD_CONTEXT)pParam;
        WorkItemBase* pWorkItem = NULL;
        CWorkQueue* pWorkQueue = pThreadContext->pWorkQueue;
        void* pThreadData = pThreadContext->pThreadData;
        DWORD dwWaitResult;
        for (;;)
        {
            //WaitForMultipleObjects�ȴ�pWorkQueue->m_phSincObjectsArray�ź�������������
            //һ����ִ��������ͷ��ź�����һ�����쳣���ͷ��ź���
            //��WaitForMultipleObjects�ȵ�����ں˶����ʱ���������bWaitAll ��������Ϊfalse��
            //�䷵��ֵ��ȥWAIT_OBJECT_0 ���ǲ���lpHandles�������š����ͬʱ�ж���ں˶��󱻴�����
            //����������ص�ֻ�����������С���Ǹ������ΪTRUE ��ȴ������ź�����Ч������ִ�С�
            //��FALSE ��������һ���ź�����Чʱ������ִ�У�
            //����WaitForMultipleObjects�ȴ�ִ��������ź������˳�����������Ϣ���¼�
            //������������ӵ�������У�����ִ�������ź�������������ִ��
            //�������˳��¼�ʱ������������Ϣ�������߳���Ϊû�������¼���λ��Ϣ����˻�ȫ������
            dwWaitResult = WaitForMultipleObjects(NUMBER_OF_SYNC_OBJ, pWorkQueue->m_phSincObjectsArray, FALSE, INFINITE);
            //WaitForMultipleObjects��������pWorkQueue->m_phSincObjectsArray���
            switch (dwWaitResult - WAIT_OBJECT_0)
            {
            case ABORT_EVENT_INDEX:             //�����쳣���
                delete pThreadContext;
                return 0;
            case SEMAPHORE_INDEX:               //����ִ��������
                pWorkItem = pWorkQueue->RemoveWorkItem();       //���������ȡһ������ִ��
                if (pWorkItem == NULL)
                {
                    assert(false);
                    break;
                }
                pWorkItem->DoWork(pThreadData);                 //ִ������������
                break;
            default:
                assert(false);
                delete pThreadContext;
                return 0;
            }
        }
        //ɾ���̲߳���
        delete pThreadContext;
        return 1;
    }

    int CWorkQueue::GetThreadTotalNum()
    {
        return m_nNumberOfThreads;
    }

    int CWorkQueue::GetWorekQueueSize()
    {
        //���̼߳���ʻ��⣬�����ٽ���
        m_Lock.Lock();
        int iWorkQueueSize = m_pWorkItemQueue->size();
        //�뿪�ٽ���
        m_Lock.UnLock();
        return iWorkQueueSize;
    }

    void CWorkQueue::Destroy(int iWairSecond)
    {
        //Ϊ��ֹ���߳�����û��ִ���꣬���߳̾������̳߳أ��ͼ���һ���ȴ�����
        while (0 != GetWorekQueueSize())
        {
            //���̵߳ȴ��̳߳������������
            Sleep(iWairSecond * 1000);
        }
        //�����˳��¼��������߳���Ϊû�������¼���λ��Ϣ����˻�ȫ������
        if (!SetEvent(m_phSincObjectsArray[ABORT_EVENT_INDEX]))
        {
            assert(false);
            return;
        }
        //wait for all the threads to end
        WaitForMultipleObjects(m_nNumberOfThreads, m_phThreads, true, INFINITE);
        //clean queue
        while (!m_pWorkItemQueue->empty())
        {
            m_pWorkItemQueue->front()->Abort();
            m_pWorkItemQueue->pop();
        }
        delete m_pWorkItemQueue;
        m_pWorkItemQueue = NULL;
        CloseHandle(m_phSincObjectsArray[SEMAPHORE_INDEX]);
        CloseHandle(m_phSincObjectsArray[ABORT_EVENT_INDEX]);
        //close all threads handles
        for (int i = 0; i < m_nNumberOfThreads; i++)
            CloseHandle(m_phThreads[i]);
        delete[] m_phThreads;
    }



}

namespace ThreadPool
{
    CThread::CThread(void) : m_pRunnable(NULL)
        , m_bRun(false)
    {
    }

    CThread::~CThread(void)
    {
    }

    CThread::CThread(Runnable* pRunnable) : m_ThreadName("")
        , m_pRunnable(pRunnable)
        , m_bRun(false)
    {
    }

    CThread::CThread(const char* ThreadName, Runnable* pRunnable) : m_ThreadName(ThreadName)
        , m_pRunnable(pRunnable)
        , m_bRun(false)
    {
    }

    CThread::CThread(std::string ThreadName, Runnable* pRunnable) : m_ThreadName(ThreadName)
        , m_pRunnable(pRunnable)
        , m_bRun(false)
    {
    }

    bool CThread::Start(bool bSuspend)
    {
        if (m_bRun)
            return true;
        if (bSuspend)
            m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StaticThreadFunc, this, CREATE_SUSPENDED, &m_ThreadID);
        else
            m_handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StaticThreadFunc, this, 0, &m_ThreadID);
        m_bRun = (NULL != m_handle);
        return m_bRun;
    }

    void CThread::Run()
    {
        if (!m_bRun)
            return;
        if (NULL != m_pRunnable)
            m_pRunnable->Run();
        m_bRun = false;
    }

    void CThread::Join(int timeout)
    {
        if (NULL == m_handle || !m_bRun)
            return;
        if (timeout <= 0)
            timeout = INFINITE;
        ::WaitForSingleObject(m_handle, timeout);
    }

    void CThread::Resume()
    {
        if (NULL == m_handle || !m_bRun)
            return;
        ::ResumeThread(m_handle);
    }

    void CThread::Suspend()
    {
        if (NULL == m_handle || !m_bRun)
            return;
        ::SuspendThread(m_handle);
    }

    bool CThread::Terminate(unsigned long ExitCode)
    {
        if (NULL == m_handle || !m_bRun)
            return true;
        if (::TerminateThread(m_handle, ExitCode))
        {
            ::CloseHandle(m_handle);
            return true;
        }
        return false;
    }

    unsigned int CThread::GetThreadID()
    {
        return m_ThreadID;
    }

    std::string CThread::GetThreadName()
    {
        return m_ThreadName;
    }

    void CThread::SetThreadName(std::string ThreadName)
    {
        m_ThreadName = ThreadName;
    }

    void CThread::SetThreadName(const char* ThreadName)
    {
        if (NULL == ThreadName)
            m_ThreadName = "";
        else
            m_ThreadName = ThreadName;
    }

    unsigned int CThread::StaticThreadFunc(void* arg)
    {
        CThread* pThread = (CThread*)arg;
        pThread->Run();
        return 0;
    }


    CThreadPool::CWorker::CWorker(CThreadPool* pThreadPool, Runnable* pFirstTask) : m_pThreadPool(pThreadPool)
        , m_pFirstTask(pFirstTask)
        , m_bRun(true)
    {
    }

    CThreadPool::CWorker::~CWorker()
    {
    }

    void CThreadPool::CWorker::Run()
    {
        Runnable* pTask = NULL;
        while (m_bRun)
        {
            if (NULL == m_pFirstTask)
                pTask = m_pThreadPool->GetTask();
            else
            {
                pTask = m_pFirstTask;
                m_pFirstTask = NULL;
            }
            if (NULL == pTask)
            {
                EnterCriticalSection(&(m_pThreadPool->m_csThreadPoolLock));
                if (m_pThreadPool->GetThreadPoolSize() > m_pThreadPool->m_minThreads)
                {
                    ThreadPoolItr itr = m_pThreadPool->m_ThreadPool.find(this);
                    if (itr != m_pThreadPool->m_ThreadPool.end())
                    {
                        m_pThreadPool->m_ThreadPool.erase(itr);
                        m_pThreadPool->m_TrashThread.insert(this);
                    }
                    m_bRun = false;
                }
                else
                {
                    ThreadPoolItr itr = m_pThreadPool->m_TrashThread.begin();
                    while (itr != m_pThreadPool->m_TrashThread.end())
                    {
                        (*itr)->Join();
                        delete(*itr);
                        m_pThreadPool->m_TrashThread.erase(itr);
                        itr = m_pThreadPool->m_TrashThread.begin();
                    }
                }
                LeaveCriticalSection(&(m_pThreadPool->m_csThreadPoolLock));
                continue;
            }
            else
            {
                pTask->Run();
                pTask = NULL;
            }
        }
    }

    CThreadPool::CThreadPool(void) : m_bRun(false)
        , m_bEnableInsertTask(false)
    {
        InitializeCriticalSection(&m_csTasksLock);
        InitializeCriticalSection(&m_csThreadPoolLock);
    }

    CThreadPool::~CThreadPool(void)
    {
        Terminate();
        DeleteCriticalSection(&m_csTasksLock);
        DeleteCriticalSection(&m_csThreadPoolLock);
    }

    bool CThreadPool::Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTasks)
    {
        if (minThreads == 0)
            return false;
        if (maxThreads < minThreads)
            return false;
        m_minThreads = minThreads;
        m_maxThreads = maxThreads;
        m_maxPendingTasks = maxPendingTasks;
        unsigned int i = m_ThreadPool.size();
        for (; i < minThreads; i++)
        {
            //�����߳�
            CWorker* pWorker = new CWorker(this);
            if (NULL == pWorker)
                return false;
            EnterCriticalSection(&m_csThreadPoolLock);
            m_ThreadPool.insert(pWorker);
            LeaveCriticalSection(&m_csThreadPoolLock);
            pWorker->Start();
        }
        m_bRun = true;
        m_bEnableInsertTask = true;
        return true;
    }

    bool CThreadPool::Execute(Runnable* pRunnable)
    {
        if (!m_bEnableInsertTask)
            return false;
        if (NULL == pRunnable)
            return false;
        if (m_Tasks.size() >= m_maxPendingTasks)
        {
            if (m_ThreadPool.size() < m_maxThreads)
            {
                CWorker* pWorker = new CWorker(this, pRunnable);
                if (NULL == pWorker)
                    return false;
                EnterCriticalSection(&m_csThreadPoolLock);
                m_ThreadPool.insert(pWorker);
                LeaveCriticalSection(&m_csThreadPoolLock);
                pWorker->Start();
            }
            else
                return false;
        }
        else
        {
            EnterCriticalSection(&m_csTasksLock);
            m_Tasks.push_back(pRunnable);
            LeaveCriticalSection(&m_csTasksLock);
        }
        return true;
    }

    Runnable* CThreadPool::GetTask()
    {
        Runnable* Task = NULL;
        EnterCriticalSection(&m_csTasksLock);
        if (!m_Tasks.empty())
        {
            Task = m_Tasks.front();
            m_Tasks.pop_front();
        }
        LeaveCriticalSection(&m_csTasksLock);
        return Task;
    }

    unsigned int CThreadPool::GetThreadPoolSize()
    {
        return m_ThreadPool.size();
    }

    void CThreadPool::Terminate()
    {
        m_bEnableInsertTask = false;
        while (m_Tasks.size() > 0)
            Sleep(1);
        m_bRun = false;
        m_minThreads = 0;
        m_maxThreads = 0;
        m_maxPendingTasks = 0;
        while (m_ThreadPool.size() > 0)
            Sleep(1);
        EnterCriticalSection(&m_csThreadPoolLock);
        ThreadPoolItr itr = m_TrashThread.begin();
        while (itr != m_TrashThread.end())
        {
            (*itr)->Join();
            delete(*itr);
            m_TrashThread.erase(itr);
            itr = m_TrashThread.begin();
        }
        LeaveCriticalSection(&m_csThreadPoolLock);
    }

}











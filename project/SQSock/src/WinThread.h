//*************************************************************************
//
//          �����̵߳Ĺ���ʵ�ֺ��̳߳ص�����ʵ��
//          ��ģ��Ϊ�߳��еײ�ģ�� С���޸�
//                                              @���� 2017-07-21
//
//*************************************************************************

#ifndef WINTHREAD_H
#define WINTHREAD_H

#include <Windows.h>
#include <string>
#include <set>
#include <list>
#include <queue>
#include <memory>
#include <thread>
using namespace std;


//�ٽ�����[ֻ�����߳��� �Ǻܸ�Ч�ʵ���]
struct DLL_API CThreadLock
{
    inline CThreadLock()
    {
        ::InitializeCriticalSection(&m_tSection);
    };
    inline ~CThreadLock()
    {
        ::DeleteCriticalSection(&m_tSection);
    };
    inline void Lock(void)
    {
        ::EnterCriticalSection(&m_tSection);
    };
    inline void UnLock(void)
    {
        ::LeaveCriticalSection(&m_tSection);
    };
    inline bool Trylock(void)
    {
        return TryEnterCriticalSection(&m_tSection);
    };
private:
    CRITICAL_SECTION m_tSection;    //�ٽ���
};

//������[���߳��� �����ﶼ���õ���]
struct DLL_API CProcessLock
{
    inline CProcessLock(TCHAR* NAME)
    {
        hMutex = CreateMutex(NULL, FALSE, NAME);
        WaitForSingleObject(hMutex, INFINITE);
    };
    inline CProcessLock()
    {
        hMutex = CreateMutex(NULL, FALSE, TEXT("_tmp_mutex_lock_"));
        WaitForSingleObject(hMutex, INFINITE);
    };
    inline ~CProcessLock()
    {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    };
private:
    HANDLE hMutex;
};

//�޲���ȫ���߳�(�޷������Ƽ���)
struct DLL_API GlobalThread
{
    //���庯��ָ��
    typedef void(*FunProc)(void);
    //�����߳� [������ ��������Զ��ͷ�(�޲���)]
    GlobalThread(FunProc NewFun)
    {
        //ȫ�ֱ���:�߳̾�����̷߳���ID������StartThreadProʹ��
        static HANDLE StartThreadHandle;
        static DWORD StartThreadID;
        //����ʹ�ú���ָ��NewFun
        StartThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NewFun, NULL, 0, &StartThreadID);
        //�����߳���ɺ��Զ��ͷ��߳�
        CloseHandle(StartThreadHandle);
    };
    
    //ע����static��������
    static void WINAPI ThreadLoop(void)
    {
        for (int i = 1; i <= 20; ++i)
        {
            printf("GlobalThread::FunProc %d.\n",i);
            Sleep(800);
        }
    };
    static int main(int argc, char** argv)
    {
        //ע����GlobalThread::FunProcת��
        GlobalThread(GlobalThread::FunProc(&ThreadLoop));
        printf("�߳��������޷�����ֱ���Զ���ɽ�����\n");
        return 0;
    };
};





//ԭ����


//�����̷߳�װ
namespace ThreadWork
{
    //�߳̽ӿڻ���
    class DLL_API ThreadObj
    {
        HANDLE hThread;                                                 //�߳̾������
        DWORD dwThreadId;                                               //�߳�ID
        int nPriority;                                                  //���ȼ�
        void* lpUserData;                                               //�ص��ò���
        //�󶨺��� ��������������
        static DWORD WINAPI StartAddress(LPVOID lpParameter);
    public:
        ThreadObj(void);
        virtual ~ThreadObj(void);
    public:
        //��ͣ�߳�����
        bool Suspend(void);
        //�ָ��߳�����
        bool Resume(void);
        //ֹͣ�߳�
        bool Stop(void);
        //�ȴ��������߳�
        int Exit(int nCount = 2, int nTimeout_ = 100);
        //�����߳�
        virtual bool Create(LPTHREAD_START_ROUTINE TdProc = NULL);
    protected:
        //�̻߳ص��ӿں���
        virtual DWORD Run(LPVOID lpParameter = 0) = 0;
    public:
        inline void SetId(DWORD dwId);
        inline DWORD GetId(void) const;
        inline void SetPriority(int nPrio_);
        inline int GetPriority(void);
        inline void SetUserData(void* lpUserData);
        inline void* GetUserData(void) const;
        inline HANDLE GetHandle(void);
        inline bool CloseThread(void);
    private:
        ThreadObj(ThreadObj const &);
        ThreadObj & operator = (ThreadObj const &);
    };

    //�̰߳�װ������ �Ƽ�������̳�
    class DLL_API WinThread : public ThreadObj
    {
        string sName;                                           //�����߳�����
        HANDLE hWorkEvent;                                      //�¼��ź�
        volatile long nRunningFlag;                             //����״̬
        bool bPauseFlag;                                        //��ͣ״̬
    public:
        WinThread();
        WinThread(string sName_);
        virtual ~WinThread(void);
    public:
        enum{ WORK_NG = -1, WORK_OK = 0, WORK_EXIT, };
        bool StartWork(void);                                   //�̴߳���
        bool EndWork(void);                                     //ֹͣ�ͷ��߳�
        bool SendWorkEvent(void);                               //�߳���Ч����

        virtual DWORD Run(LPVOID lpParameter = 0);
        virtual DWORD HandleMessage(void);
        virtual bool BeforeRunning(void);
        virtual bool AfterRunning(void);
    public:
        inline void SetState(DWORD dwNewState);
        inline DWORD GetState(void) const;
        inline void SetName(string sName_);
        inline string GetName(void) const;
        inline void SetPauseFlag(bool bPause_);
        inline bool GetPauseFlag(void) const;
    };

    //-TestThread�����߳���-----------------------------------------------------
    class TestThread1 : public WinThread
    {
    public:
        //TestThreadʹ�÷���
        static int MAIN(int argc, char** argv)
        {
            //���½���������
            auto_ptr<TestThread1> td1(new TestThread1);
            td1->StartWork();               //�����߳�
            td1->SendWorkEvent();           //��ʼ����
            td1->GetPauseFlag();            //����߳�״̬ boolֵ
            Sleep(2000);
            td1->SetPauseFlag(false);       //��ͣ�߳�����
            Sleep(2000);
            td1->SetPauseFlag(true);        //�ָ��߳�����
            Sleep(3000);
            td1->EndWork();                 //�����ͷ��߳�
            return 1;
        };
    public:
        TestThread1(void) : WinThread("TestThread01") { };
        virtual ~TestThread1(void) { };
    private:
        DWORD HandleMessage(void)
        {
            printf("-td1-- \n");
            while (true)
            {
                Sleep(200);
                printf("log--------------- \n");
            }
            return (WORK_OK);
        };
    };

    class CWorkQueue;

    class WorkItemBase
    {
        virtual void DoWork(void* pThreadContext) = 0;
        virtual void Abort() = 0;
        friend CWorkQueue;
    };

    typedef std::queue<WorkItemBase*> WorkItemQueue, *PWorkItemQueue;

    class CWorkQueue
    {
    public:
        virtual ~CWorkQueue() {};
        //�������߳�[�߳���Ŀ,�̺߳���ִ�еĲ���]
        bool Create(const unsigned int nNumberOfThreads, void** pThreadDataArray = NULL);
        //���������������� ����ִ����ͨ���̳л���WorkItemBase֮��ʹ�ö�̬����DoWork�������ʵ����
        bool InsertWorkItem(WorkItemBase* pWorkItem);
        //�����̳߳�
        void Destroy(int iWairSecond);
        //��ȡ�߳�����
        int GetThreadTotalNum();
    private:
        //�߳�ִ�еĺ�����ʵ��ִ�е�����������е�DoWork()
        static unsigned long __stdcall ThreadFunc(void* pParam);
        //�ӹ����������Ƴ����󣬲������Ƴ��Ķ���
        WorkItemBase* RemoveWorkItem();
        //��ȡ����صĴ�С
        int GetWorekQueueSize();
        enum { ABORT_EVENT_INDEX = 0, SEMAPHORE_INDEX, NUMBER_OF_SYNC_OBJ, };
    private:
        PHANDLE m_phThreads;                                //���뵽���߳�
        unsigned int m_nNumberOfThreads;
        void* m_pThreadDataArray;
        HANDLE m_phSincObjectsArray[NUMBER_OF_SYNC_OBJ];
        CThreadLock m_Lock;
        //CRITICAL_SECTION m_CriticalSection;
        PWorkItemQueue m_pWorkItemQueue;
    };
}

//�̳߳����
namespace ThreadPool
{
    //�̳߳���ӿ�
    struct Runnable
    {
        Runnable(){};
        virtual ~Runnable() {};
        //Ҫ�����߳�����ĺ�������
        virtual void Run() = 0;
    };

    //�̶߳���
    class CThread : public Runnable
    {
    private:
        HANDLE m_handle;
        Runnable* const m_pRunnable;
        /*unsigned int*/DWORD m_ThreadID;
        std::string m_ThreadName;
        volatile bool m_bRun;
    public:
        CThread();
        CThread(Runnable* pRunnable);
        CThread(const char* ThreadName, Runnable* pRunnable = NULL);
        CThread(std::string ThreadName, Runnable* pRunnable = NULL);
        ~CThread(void);
    private:
        explicit CThread(const CThread& rhs);
    public:
        //��ʼ�����߳� [bSuspend ��ʼ����ʱ�Ƿ����]
        bool Start(bool bSuspend = false);
        //���е��̺߳���������ʹ����������д�˺���
        virtual void Run();
        //��ǰִ�д˺����̵߳ȴ��߳̽���[timeout �ȴ���ʱʱ�䣬���Ϊ�������ȴ�����ʱ��]
        void Join(int timeout = -1);
        //�ָ�������߳�
        void Resume();
        //�����߳�
        void Suspend();
        //��ֹ�̵߳�ִ��
        bool Terminate(unsigned long ExitCode);
        //��ȡ�߳�ID
        unsigned int GetThreadID();
        std::string GetThreadName();
        void SetThreadName(std::string ThreadName);
        void SetThreadName(const char* ThreadName);
    private:
        //�̻߳ص�
        static unsigned int WINAPI StaticThreadFunc(void* arg);
    };

    //�̳߳�
    class CThreadPool
    {
    public:
        CThreadPool(void);
        ~CThreadPool(void);
    public:
        //��ʼ���̳߳أ�����minThreads���߳�
        bool Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTaskse);
        //ִ����������ǰ�����б�û����������������뵽�����б�����true
        //����ǰ�����б����ˣ�����ǰ�߳�����С������߳��������������߳�ִ�д����񣬷���true
        //����ǰ�����б����ˣ�����ǰ�߳�������������߳����������������񣬷���false
        bool Execute(Runnable* pRunnable);

        //��ֹ�̳߳أ�����ֹ��������, Ȼ��ȴ�ֱ�������б�Ϊ��,Ȼ��������С�߳�����Ϊ0,�ȴ�ֱ���߳�����Ϊ��,����������е�����
        void Terminate();

        //�����̳߳��е�ǰ���߳�����
        unsigned int GetThreadPoolSize();
    private:
        //��ȡ�����б��е������������б�Ϊ�գ�����NULL
        Runnable* GetTask();
        static unsigned int WINAPI StaticThreadFunc(void* arg);
    private:
        class CWorker : public CThread
        {
        public:
            CWorker(CThreadPool* pThreadPool, Runnable* pFirstTask = NULL);
            ~CWorker();
            void Run();
        private:
            CThreadPool* m_pThreadPool;
            Runnable* m_pFirstTask;
            volatile bool m_bRun;
        };

        typedef std::set<CWorker*> ThreadPool;
        typedef std::list<Runnable*> Tasks;
        typedef Tasks::iterator TasksItr;
        typedef ThreadPool::iterator ThreadPoolItr;

        ThreadPool m_ThreadPool;
        ThreadPool m_TrashThread;
        Tasks m_Tasks;

        CRITICAL_SECTION m_csTasksLock;
        CRITICAL_SECTION m_csThreadPoolLock;

        volatile bool m_bRun;
        volatile bool m_bEnableInsertTask;
        volatile unsigned int m_minThreads;
        volatile unsigned int m_maxThreads;
        volatile unsigned int m_maxPendingTasks;
    };

    //-----------------------TestThread�����߳���-------------------------------
    //�̳߳���ʾ
    class ThreadPoolExecutor :public CThreadPool
    {
        //����һ�������߳���
        struct CTask1 : public Runnable
        {
            ~CTask1()
            {
                printf("~CTask1\n");
            };
            void Run()
            {
                printf("Hello World\n");
            };
        };

        //�̳߳ز��Խ����
        //����100��������ÿ���������10000000��ѭ�����������޵ȴ���
        //���߳�ִ�к�ʱ��2281ʱ��Ƭ
        //���̳߳�ִ�к�ʱ��2219ʱ��Ƭ
        //2���̵߳��̳߳غ�ʱ��1156ʱ��Ƭ
        //5���̵߳��̳߳غ�ʱ��1166ʱ��Ƭ
        //10���̵߳��̳߳غ�ʱ��1157ʱ��Ƭ
        //100���̵߳��̳߳غ�ʱ��1177ʱ��Ƭ
        //�����߳�ʹ��
        int TestThreadMini(void)
        {
            CTask1 task;
            CThread* td = new CThread(&task);
            td->Start();
            td->Join();
            getchar();
            return 1;
        }

        //�����̳߳�ʹ��
        int TestThreadPoolExecutor(void)
        {
            CThreadPool* pExecutor = new CThreadPool();
            pExecutor->Init(1, 10, 50);
            CTask1 task;
            for (int i = 0; i < 100; i++)
            {
                while (!pExecutor->Execute(&task))
                {
                }
            }
            pExecutor->Terminate();
            delete pExecutor;
            getchar();
            return 1;
        }
    };

}

//C11��׼���еĲ���
namespace ThreadC11
{

}




#endif
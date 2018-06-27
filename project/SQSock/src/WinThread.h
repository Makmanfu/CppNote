//*************************************************************************
//
//          工作线程的管理实现和线程池的完整实现
//          此模块为线程中底部模块 小心修改
//                                              @阿甘 2017-07-21
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


//临界区锁[只能在线程里 是很高效率的锁]
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
    CRITICAL_SECTION m_tSection;    //临界区
};

//互斥锁[在线程里 进程里都能用的锁]
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

//无参数全局线程(无法管理不推荐用)
struct DLL_API GlobalThread
{
    //定义函数指针
    typedef void(*FunProc)(void);
    //启动线程 [不管理 运行完后自动释放(无参数)]
    GlobalThread(FunProc NewFun)
    {
        //全局变量:线程句柄和线程返回ID供下面StartThreadPro使用
        static HANDLE StartThreadHandle;
        static DWORD StartThreadID;
        //这里使用函数指针NewFun
        StartThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)NewFun, NULL, 0, &StartThreadID);
        //上面线程完成后自动释放线程
        CloseHandle(StartThreadHandle);
    };
    
    //注意是static函数例子
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
        //注意用GlobalThread::FunProc转换
        GlobalThread(GlobalThread::FunProc(&ThreadLoop));
        printf("线程启动后无法管理直到自动完成结束！\n");
        return 0;
    };
};





//原子锁


//工作线程封装
namespace ThreadWork
{
    //线程接口基类
    class DLL_API ThreadObj
    {
        HANDLE hThread;                                                 //线程句柄对象
        DWORD dwThreadId;                                               //线程ID
        int nPriority;                                                  //优先级
        void* lpUserData;                                               //回调用参数
        //绑定函数 用来启动任务函数
        static DWORD WINAPI StartAddress(LPVOID lpParameter);
    public:
        ThreadObj(void);
        virtual ~ThreadObj(void);
    public:
        //暂停线程运行
        bool Suspend(void);
        //恢复线程运行
        bool Resume(void);
        //停止线程
        bool Stop(void);
        //等待并结束线程
        int Exit(int nCount = 2, int nTimeout_ = 100);
        //创建线程
        virtual bool Create(LPTHREAD_START_ROUTINE TdProc = NULL);
    protected:
        //线程回调接口函数
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

    //线程包装管理类 推荐从这里继承
    class DLL_API WinThread : public ThreadObj
    {
        string sName;                                           //记下线程名字
        HANDLE hWorkEvent;                                      //事件信号
        volatile long nRunningFlag;                             //运行状态
        bool bPauseFlag;                                        //暂停状态
    public:
        WinThread();
        WinThread(string sName_);
        virtual ~WinThread(void);
    public:
        enum{ WORK_NG = -1, WORK_OK = 0, WORK_EXIT, };
        bool StartWork(void);                                   //线程创建
        bool EndWork(void);                                     //停止释放线程
        bool SendWorkEvent(void);                               //线程生效工作

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

    //-TestThread测试线程类-----------------------------------------------------
    class TestThread1 : public WinThread
    {
    public:
        //TestThread使用方法
        static int MAIN(int argc, char** argv)
        {
            //以下仅仅是例子
            auto_ptr<TestThread1> td1(new TestThread1);
            td1->StartWork();               //创建线程
            td1->SendWorkEvent();           //开始工作
            td1->GetPauseFlag();            //获得线程状态 bool值
            Sleep(2000);
            td1->SetPauseFlag(false);       //暂停线程运行
            Sleep(2000);
            td1->SetPauseFlag(true);        //恢复线程运行
            Sleep(3000);
            td1->EndWork();                 //结束释放线程
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
        //建立多线程[线程数目,线程函数执行的参数]
        bool Create(const unsigned int nNumberOfThreads, void** pThreadDataArray = NULL);
        //向任务队列添加任务 任务执行类通过继承基类WorkItemBase之后使用多态函数DoWork来完成真实任务
        bool InsertWorkItem(WorkItemBase* pWorkItem);
        //销毁线程池
        void Destroy(int iWairSecond);
        //获取线程总数
        int GetThreadTotalNum();
    private:
        //线程执行的函数，实际执行的是任务队列中的DoWork()
        static unsigned long __stdcall ThreadFunc(void* pParam);
        //从工作队列中移除对象，并返回移除的对象
        WorkItemBase* RemoveWorkItem();
        //获取任务池的大小
        int GetWorekQueueSize();
        enum { ABORT_EVENT_INDEX = 0, SEMAPHORE_INDEX, NUMBER_OF_SYNC_OBJ, };
    private:
        PHANDLE m_phThreads;                                //申请到的线程
        unsigned int m_nNumberOfThreads;
        void* m_pThreadDataArray;
        HANDLE m_phSincObjectsArray[NUMBER_OF_SYNC_OBJ];
        CThreadLock m_Lock;
        //CRITICAL_SECTION m_CriticalSection;
        PWorkItemQueue m_pWorkItemQueue;
    };
}

//线程池相关
namespace ThreadPool
{
    //线程抽象接口
    struct Runnable
    {
        Runnable(){};
        virtual ~Runnable() {};
        //要丢进线程里面的函数重载
        virtual void Run() = 0;
    };

    //线程对象
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
        //开始运行线程 [bSuspend 开始运行时是否挂起]
        bool Start(bool bSuspend = false);
        //运行的线程函数，可以使用派生类重写此函数
        virtual void Run();
        //当前执行此函数线程等待线程结束[timeout 等待超时时间，如果为负数，等待无限时长]
        void Join(int timeout = -1);
        //恢复挂起的线程
        void Resume();
        //挂起线程
        void Suspend();
        //终止线程的执行
        bool Terminate(unsigned long ExitCode);
        //获取线程ID
        unsigned int GetThreadID();
        std::string GetThreadName();
        void SetThreadName(std::string ThreadName);
        void SetThreadName(const char* ThreadName);
    private:
        //线程回调
        static unsigned int WINAPI StaticThreadFunc(void* arg);
    };

    //线程池
    class CThreadPool
    {
    public:
        CThreadPool(void);
        ~CThreadPool(void);
    public:
        //初始化线程池，创建minThreads个线程
        bool Init(unsigned int minThreads, unsigned int maxThreads, unsigned int maxPendingTaskse);
        //执行任务，若当前任务列表没有满，将此任务插入到任务列表，返回true
        //若当前任务列表满了，但当前线程数量小于最大线程数，将创建新线程执行此任务，返回true
        //若当前任务列表满了，但当前线程数量等于最大线程数，将丢弃此任务，返回false
        bool Execute(Runnable* pRunnable);

        //终止线程池，先制止塞入任务, 然后等待直到任务列表为空,然后设置最小线程数量为0,等待直到线程数量为空,清空垃圾堆中的任务
        void Terminate();

        //返回线程池中当前的线程数量
        unsigned int GetThreadPoolSize();
    private:
        //获取任务列表中的任务，若任务列表为空，返回NULL
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

    //-----------------------TestThread测试线程类-------------------------------
    //线程池演示
    class ThreadPoolExecutor :public CThreadPool
    {
        //测试一个任务线程类
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

        //线程池测试结果：
        //对于100个任务并且每个任务包含10000000个循环，任务中无等待：
        //单线程执行耗时：2281时间片
        //单线程池执行耗时：2219时间片
        //2个线程的线程池耗时：1156时间片
        //5个线程的线程池耗时：1166时间片
        //10个线程的线程池耗时：1157时间片
        //100个线程的线程池耗时：1177时间片
        //测试线程使用
        int TestThreadMini(void)
        {
            CTask1 task;
            CThread* td = new CThread(&task);
            td->Start();
            td->Join();
            getchar();
            return 1;
        }

        //测试线程池使用
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

//C11标准库中的并行
namespace ThreadC11
{

}




#endif
//*************************************************************************
//
//                   例子大全 仅供学习理解
//              C++ Multithreading Cookbook
//                                              @阿甘 2017-07-13
//
//*************************************************************************

#ifndef THREADCOOKBOOK_H
#define THREADCOOKBOOK_H

#include "WinGUI.h"
#include "WinThread.h"
#include <iostream>
using namespace std;


#include "CLinearList.hpp"
#pragma comment ( lib, "comctl32.lib" )

//简单两进程间并发处理问题
namespace IPCDemo
{
    class _IPC
    {
    public:
        typedef struct _tagCOMMUNICATIONOBJECT
        {
            HWND hWndClient;                //传递句柄
            BOOL bExitLoop;                 //通知退出flag
            LONG lSleepTimeout;             //中间暂停时间
        } COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;
    public:
        PCOMMUNICATIONOBJECT pCommObject;   //文件映射数据
        HANDLE hMapping;                    //文件映射句柄
        HANDLE hMutex;                      //互斥
    public:
        _IPC();
        virtual ~_IPC();
    public:
        //创建进程管理
        bool ProcessOpen(HWND hWnd);
        //通知相关进程销毁退出
        void DestroyMap(void);
        //释放创建的对象
        void releseHand(void);
    };

    class IPCWorker :public _IPC
    {
    public:
        //测试 主程序入口
        //static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
        static int MAIN(int argc, char** argv)
        {
            IPCWorker Wk;
            if (strcmp(argv[1], "blue") == 0)
                Wk.LoopSend(0);
            else
                Wk.LoopSend(1);
            return 0;
        };
    private:
        BOOL bContinueLoop;
        HBRUSH hBrush;
        HWND hWnd;
        HDC hDC;
        RECT rectClient;
        LONG lWaitTimeout;
    public:
        IPCWorker();
    public:
        //客户进程调用
        bool ProcessOpenMap(void);
        //处理共享的数据
        void LoopSend(int _color);
    };

    class IPCGui :public XqWindowEx
    {
    public:
        shared_ptr<_IPC> mIPC;      //这里指针能解决先后问题
    public:
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
        {
            IPCGui WND;
            WND.InitData(hInstance);
            WND.CreateEX(L"IPCDemo_COLOR");
            WND.ShowDLG();
            return 0;
        };
    public:
        virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}

//简单多进程抢占并发处理
namespace IPCDemoMuti
{
    class _IPC
    {
#define PHILOSOPHER_COUNT       5
    public:
        //IPC通信结构体
        typedef struct _tagCOMMUNICATIONOBJECT
        {
            HWND hWnd;
            bool bExitApplication;
            int iPhilosopherArray[PHILOSOPHER_COUNT];
            int PhilosopherCount;
        } COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;
    public:
        HANDLE hMapping;
        HANDLE hSemaphore;
        PCOMMUNICATIONOBJECT pCommObject;
        HANDLE hProcesses[PHILOSOPHER_COUNT];
    public:
        _IPC();
        ~_IPC();
    public:
        //创建IPC文件映像
        bool CreateShareIPC(void);
        //设置数据
        void SetFileData(HWND hWnd);
        //创建分饼进程
        void CreateProc(void);
        //关闭释放
        void ReleaseShareIPC(void);
    };
    
    class IPCWorker :public _IPC
    {
    public:
        //测试 主程序入口
        //static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
        static int MAIN(int argc, char** argv)
        {
            IPCWorker Wk;
            Wk.LoopSend( atoi(argv[1]));
            return 0;
        };
    private:
        bool bExitApplication;
        HANDLE hMapping;
    public:
        IPCWorker();
    public:
        //客户进程调用
        bool ProcessOpenMap(void);
        //处理共享的数据
        void LoopSend(int _color);
    };

    class IPCGui :public XqWindowEx
    {
    public:
        //测试 主程序入口
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
        {
            IPCGui WND;
            WND.InitData(hInstance);
            WND.CreateEX(L"Muti people");
            WND.ShowDLG();
            return 0;
        };
    public:
        _IPC mIPC;
    public:
        IPCGui();
    public:
        int PhilosopherPass(int iPhilosopher);
        virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    private:
        void FillEllipse(HWND hWnd, HDC hDC, int iLeft, int iTop, int iRight, int iBottom, int iPass);
    };
}

//简单线程
namespace ThreadDemo
{
    //简单多线程处理数组
    struct SimpleThread
    {
        static int main(int argc, char** argv);
    public:
        void RUN(void);
        //线程入口回调
        static DWORD ThreadStart(LPVOID lpParameter);
        //打印数组
        void PrintArray(int* iArray, int iSize);
        //合并数组
        void MergeArrays(int* leftArray, int leftArrayLenght, int* rightArray,
            int rightArrayLenght, int* mergedArray);
    };
    
    class _miniThread
    {
#define BUTTON_OPEN         100
#define FILE_SAVE           0x0001
#define FILE_OPEN           0x0002
#define MUTEX_NAME          _T("__RW_MUTEX__")
        typedef struct _tagCOPYDETAILS
        {
            HINSTANCE hInstance;
            HWND hWndPB;
            LPTSTR szReadFileName;
            LPTSTR szWriteFileName;
        } COPYDETAILS, *PCOPYDETAILS;
        static DWORD dwReadBytes;
        static DWORD dwWriteBytes;
        static DWORD dwBlockSize;
        static DWORD dwFileSize;
        static HLOCAL pMemory;
    public:
        _miniThread();
        ~_miniThread();
    public:
        void OpenFileDialog(HINSTANCE Inst, HWND& hWndPB);
        //读文件
        static DWORD ReadRoutine(LPVOID lpParameter);
        //写文件
        static DWORD WriteRoutine(LPVOID lpParameter);
    private:
        static DWORD GetBlockSize(DWORD dwFileSize);
        //打开对话框
        BOOL FileDialog(HWND hWnd, LPTSTR szFileName, DWORD dwFileOperation);
    };
    
    //简单多线程处理文件流复制
    class ConcurrentFileCopy :public XqWindowEx
    {
    public:
        //测试 主程序入口
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
        {
            ConcurrentFileCopy WND;
            WND.InitData(hInstance);
            WND.CreateEX(L"FileCopy");
            WND.ShowDLG();
            return 0;
        };
    private:
        HWND hWndPB;
        _miniThread TD;
    public:
        ConcurrentFileCopy();
    public:
        virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
}

//同步和异步操作
namespace Threads
{
    //简单线程基类
    class _CThread
    {
#define STATE_RUNNING       0x0001      //     1 线程正在运行
#define STATE_READY         0x0002      //    10 线程正在加入调用方
#define STATE_BLOCKED       0x0004      //   100 线程阻塞被挂起
#define STATE_ALIVE         0x0008      //  1000 线程创建并激活
#define STATE_ASYNC         0x0010      //  1010 异步[并发执行]
#define STATE_SYNC          0x0020      // 10100 同步[阻塞返回]
#define STATE_CONTINUOUS    0x0040      //101000 永不停止
    private:
        HANDLE hThread;                     //线程句柄对象
        DWORD dwThreadId;                   //线程ID
        DWORD dwState;                      //线程状态
        HANDLE hEvent;                      //线程信号量
        void* lpUserData;                   //线程函数传参数
    public:
        _CThread() : hThread(0), dwThreadId(0), dwState(0), lpUserData(0), lpParameter(0) { };
        virtual ~_CThread(){};
    public:
        //创建线程
        HANDLE Create(LPVOID lpParameter, DWORD dwInitialState = STATE_ASYNC, DWORD dwCreationFlag = 0);
        //触发信号
        BOOL Alert(void);
        //线程等待
        void Join(DWORD dwWaitInterval = INFINITE);
        //线程挂起
        DWORD Suspend(void);
        //线程恢复
        DWORD Resume(void);
    protected:
        //回调参数
        LPVOID lpParameter;
        //线程回调接口函数
        virtual void Run(LPVOID lpParameter = 0) = 0;
    public:
        inline void SetUserData(void* lpUserData)
        {
            this->lpUserData = lpUserData;
        };
        inline void* GetUserData(void) const
        {
            return lpUserData;
        };
        inline void SetId(DWORD dwId)
        {
            this->dwThreadId = dwId;
        };
        inline DWORD GetId(void) const
        {
            return dwThreadId;
        };
        inline void SetState(DWORD dwNewState)
        {
            dwState = 0;
            dwState |= dwNewState;
        };
        inline DWORD GetState(void) const
        {
            return dwState;
        };
        inline HANDLE GetHandle(void) const
        {
            return hThread;
        };
        inline DWORD GetAsyncState(void) const
        {
            if (dwState & STATE_ASYNC)
                return STATE_ASYNC;
            return STATE_SYNC;
        };
    private:
        //线程绑定函数
        static DWORD WINAPI StartAddress(LPVOID lpParameter);
    };

    //实例化
    class _AppThreads : public _CThread
    {
    protected:
        virtual void Run(LPVOID lpParameter = 0);
    };

    //异步操作界面
    struct AsyncThreadGUI :public XqWindowEx
    {
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    private:
#define THREADSNUMBER      16
        HWND hRects[THREADSNUMBER];
        _AppThreads threads[THREADSNUMBER];
    public:
        void CreateThreads(DWORD _STATE = STATE_ASYNC);
        void CloseThreads();
        virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };

    //同步操作界面
    struct SyncThreadGUI :public AsyncThreadGUI
    {
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    };
}

//消息队列
namespace Messages
{
    //queue_MPI threadMPI
    class MessagesGUI : public XqWindow
    {
        typedef struct tagPLOTDATA
        {
            int value;
            DWORD dwThreadId;
            int iMsgID;
        } PLOTDATA, *PPLOTDATA;
    public:
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
        {
            MessagesGUI WND;
            WND.InitData(hInstance);
            WND.Create(L"WINGUI", 600, 600, &MessagesGUI::WndProc);
            WND.ShowDLG();
            return 0;
        };
    private:
        //原始窗口过程
        static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    public:
        int iMessageID;
        HANDLE gEvent;
        HANDLE* hThreads;       //HANDLE hThreads[THREAD_NUMBER];
        HANDLE hThread;

        //临界区(类似互斥)
        CRITICAL_SECTION cs;
        //队列
        CQueue<PLOTDATA> queue;
    public:
        MessagesGUI();
    public:
        virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };

    //threadMPI
    class threadMPI
    {

    };

    //pipe服务端exe
    class pipe_server
    {
#define BUFFER_SIZE 4096
        typedef WCHAR TCHAR, *PTCHAR;
        //线程函数
        static DWORD WINAPI StartAddress(LPVOID lpParameter)
        {
            HANDLE hPipe = (HANDLE)lpParameter;
            PTCHAR szRequest = (PTCHAR)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE * sizeof(TCHAR));
            PTCHAR szReply = (PTCHAR)HeapAlloc(GetProcessHeap(), 0, BUFFER_SIZE * sizeof(TCHAR));
            DWORD dwBytesRead = 0;
            DWORD dwReplyBytes = 0;
            DWORD dwBytesWritten = 0;
            _tprintf(_T("Waiting messages.\n"));
            if (!ReadFile(hPipe, szRequest, BUFFER_SIZE * sizeof(TCHAR), &dwBytesRead, NULL))
            {
                _tprintf(_T("ReadFile failed! Error code: [%u]\n"), GetLastError());
                return 2L;
            }
            // do some processing
            _tprintf(_T("Client request:\"%s\"\n"), szRequest);
            _tcscpy_s(szReply, BUFFER_SIZE, _T("default answer from server"));
            dwReplyBytes = (_tcslen(szReply) + 1) * sizeof(TCHAR);
            if (!WriteFile(hPipe, szReply, dwReplyBytes, &dwBytesWritten, NULL))
            {
                _tprintf(_T("WriteFile failed! Error code: [%u]"), GetLastError());
                return 2L;
            }
            FlushFileBuffers(hPipe);
            DisconnectNamedPipe(hPipe);
            CloseHandle(hPipe);
            HeapFree(GetProcessHeap(), 0, szRequest);
            HeapFree(GetProcessHeap(), 0, szReply);
            _tprintf(_T("Success.\n"));
            return 0L;
        }
        //管道服务端
        static int MAIN(int argc, char** argv)
        {
            LPTSTR szPipename = _T("\\\\.\\pipe\\basicMPI");
            while (TRUE)
            {
                _tprintf(_T("basicMPI: waiting client connection\n"));
                //创建管道对象
                HANDLE hPipe = CreateNamedPipe(szPipename, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_READMODE_MESSAGE | PIPE_TYPE_MESSAGE, PIPE_UNLIMITED_INSTANCES, BUFFER_SIZE, BUFFER_SIZE, 0, NULL);
                if (hPipe == INVALID_HANDLE_VALUE)
                {
                    _tprintf(_T("CreateNamedPipe failed! Error code: [%u]\n"), GetLastError());
                    return 2;
                }
                //此处会阻塞 管道服务等待直到客户端的连接 
                if (ConnectNamedPipe(hPipe, NULL))
                {
                    _tprintf(_T("Connection succeeded.\n"));
                    //创建处理接受线程
                    HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartAddress, hPipe, 0, NULL);
                    CloseHandle(hThread);
                }
            }
            return 0;
        }
    };

    //pipe客户端exe
    class pipe_client
    {
        //管道客户端
        static int MAIN(int argc, char** argv)
        {
            TCHAR szBuffer[BUFFER_SIZE];
            DWORD dwRead = 0;
            DWORD dwWritten = 0;
            DWORD dwMode = PIPE_READMODE_MESSAGE;
            LPTSTR szPipename = _T("\\\\.\\pipe\\basicMPI");
            LPTSTR szMessage = _T("Message from client.");
            DWORD dwToWrite = _tcslen(szMessage) * sizeof(TCHAR);
            //打开管道共享
            HANDLE hPipe = CreateFile(szPipename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            WaitNamedPipe(szPipename, INFINITE);
            SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
            //向管道写入数据
            if (!WriteFile(hPipe, szMessage, dwToWrite, &dwWritten, NULL))
            {
                //printf("WriteFile failed! Error code: [%u]\n", GetLastError());
                _tprintf(_T("WriteFile failed! Error code: [%u]\n"), GetLastError());
                return -1;
            }
            _tprintf(_T("Message sent to server, receiving reply as follows:\n"));
            while (ReadFile(hPipe, szBuffer, BUFFER_SIZE * sizeof(TCHAR), &dwRead, NULL) && GetLastError() != ERROR_MORE_DATA)
                _tprintf(_T("%s"), szBuffer);
            _tprintf(_T("\nSuccess!\nPress ENTER to exit."));
            scanf_s("%c", szBuffer);
            CloseHandle(hPipe);
            return 0;
        }
    };
}

//线程同步和并发
namespace Concurrent
{
    //使用互斥量进行同步
    class concurrent_operations :public XqWindowEx
    {
    public:
        /********************************************************
        You need to specify text file with coordinates.
        Our example will solve system of 2 linear equations
        a1x + b1y = c1;
        a2x + b2y = c2;
        *                                                       *
        Each line should have exactly six coordinates.
        File format example:
        *                                                       *
        4 -3  2 -7  5 6
        12  1 -7  9 -5 8
        *                                                       *
        Avoid new line at the end of the file.
        *********************************************************/
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
        {
            concurrent_operations WND;
            WND.InitData(hInstance);
            WND.CreateEX(L"concurrent_operations");
            WND.ShowDLG();
            return 0;
        };
    public:
        concurrent_operations();
    public:
        BOOL FileDialog(HWND hWnd, LPSTR szFileName);
        virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };

    //使用信号量进行同步
    class concurrent_operations2
    {

    };

    //使用事件进行同步
    class concurrent_operations3
    {

    };

    //使用临界区进行同步
    class concurrent_operations4
    {

    };

    //使用管道进行同步
    class concurrent_operations5
    {

    };
}

//线程池
namespace ThreadPools
{
    //线程单元
    class CThread
    {
    private:
        LPTHREAD_START_ROUTINE lpThreadStart;       //线程函数指针
        HANDLE hThread;                             //线程句柄
        DWORD dwThreadId;                           //线程ID
        DWORD dwExitCode;                           //退出码
    public:
        CThread(LPTHREAD_START_ROUTINE lpThreadStart) : dwExitCode(0), lpThreadStart(lpThreadStart)
            , dwThreadId(0), hThread(NULL)
        {
            if (StillAlive())
                TerminateThread(hThread, (DWORD)-1);
            CloseHandle(hThread);
        };
        ~CThread(){};
        //比较线程ID
        bool operator==(const CThread& _TD)
        {
            return _TD.dwThreadId == this->dwThreadId;
        }
    public:
        //创建开始线程
        void Start(LPVOID lpContext = 0)
        {
            hThread = CreateThread(NULL, 0, lpThreadStart, lpContext, 0, &dwThreadId);
        };
        //true指定的线程尚未终止
        bool StillAlive()
        { 
            return ExitStatus() == STILL_ACTIVE; 
        };
        //获取线程句柄
        HANDLE Handle() const 
        { 
            return hThread; 
        };
        //获取线程ID
        DWORD ThreadId() const 
        { 
            return dwThreadId; 
        };
        //获取一个已中止线程的退出代码
        DWORD ExitStatus()
        { 
            GetExitCodeThread(hThread, &dwExitCode);
            return dwExitCode; 
        };
    };

    //线程池类
    class CThreadPool
    {
    private:
        CList<CThread>* threadList;                 //线程列表
        DWORD dwMaxCount;                           //最大线程数
    public:
        CThreadPool()
        {
            //获得CPU逻辑核心数
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            dwMaxCount = sysInfo.dwNumberOfProcessors;
            //线程链表
            threadList = new CList<CThread>();
        };
        CThreadPool(unsigned uMaxThreads) : dwMaxCount(uMaxThreads), threadList(new CList<CThread>())
        { 
        };
        ~CThreadPool()
        {
            RemoveAll(); 
        };
    public:
        //移除线程
        void RemoveThread(DWORD dwThreadId)
        {
            CThread* thread = Find(dwThreadId);
            if (thread)
                delete thread;
        };
        //等待所有线程
        void WaitAll()
        {
            HANDLE* hThreads = new HANDLE[threadList->Count()];
            CThread* thread = 0;
            for (unsigned uIndex = 0; uIndex < threadList->Count(); uIndex++)
            {
                thread = threadList->GetNext(thread);
                hThreads[uIndex] = thread->Handle();
            }
            WaitForMultipleObjects(threadList->Count(), hThreads, TRUE, INFINITE);
            delete[] hThreads;
        };
        //移除所有线程
        void RemoveAll()
        {
            CThread* thread = NULL;
            while (thread = threadList->GetNext(thread))
            {
                threadList->Remove(thread);
                thread = NULL;
            }
            delete threadList;
        };
        //结束指定线程
        void ReleaseThread(DWORD dwThreadId)
        {
            CThread* thread = Find(dwThreadId);
            if (thread != NULL)
                TerminateThread(thread->Handle(), (DWORD)-1);
        };
        //请求获取空闲线程 lpThreadStart线程函数传递
        CThread* RequestThread(LPTHREAD_START_ROUTINE lpThreadStart)
        {
            CThread* thread = NULL;
            //当前线程数小于最大线程时可添加线程
            if (Count() < MaxCount())
            {
                threadList->Append(threadList->Head(), thread = new CThread(lpThreadStart));
                return thread;
            }
            while (thread = threadList->GetNext(thread))
            {
                if (!thread->StillAlive())      //如果所有线程都是激活的
                    break;
                Sleep(100);                     //将等待0。1秒再尝试
            }
            if (thread == NULL)
                thread = threadList->GetFirst();
            threadList->Remove(thread);
            threadList->Append(threadList->Head(), thread = new CThread(lpThreadStart));
            return thread;
        };
        
        DWORD& MaxCount()
        {
            return dwMaxCount; 
        }
        DWORD Count()
        {
            return threadList->Count(); 
        }
    protected:
        //链表中根据ID找CNode
        CThread* Find(DWORD dwThreadId)
        {
            CThread* thread = NULL;
            while (thread = threadList->GetNext(thread))
                if (thread->ThreadId() == dwThreadId)
                    return thread;
            return NULL;
        };
    };

    //锁
    class CLock
    {
    public:
        inline CLock(TCHAR* szMutexName)
        {
            hMutex = CreateMutex(NULL, FALSE, szMutexName);
            WaitForSingleObject(hMutex, INFINITE);
        };
        inline ~CLock()
        {
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);
        };
    private:
        HANDLE hMutex;
    };

    //改进上面的线程类
    class CThread2
    {
    private:
        typedef struct
        {
            LPTHREAD_START_ROUTINE lpThreadStart;   //线程函数
            LPVOID lpParameter;                     //上下文线程参数
            HANDLE hThread;                         //上下文线程句柄
            DWORD dwThreadId;                       //上下文线程ID
            DWORD dwDispatcherId;                   //线程池管理ID
            DWORD dwMaxWaitTime;                    //上下文线程超时时间
        } STARTCONTEXT, *PSTARTCONTEXT;
        HANDLE hThread;                             //线程参数
        DWORD dwThreadId;                           //线程ID
        DWORD dwExitCode;                           //线程退出码
        PSTARTCONTEXT pThreadContext;               //上下文线程
    public:
        CThread2(LPTHREAD_START_ROUTINE lpThreadStart, DWORD dwDispatcherId);
        ~CThread2();
    public:
        void Start(LPVOID lpContext = 0);
        bool StillAlive();
        DWORD ExitStatus();
        CThread2* SetMaxWaitTime(DWORD dwMilliseconds);
    public:
        HANDLE Handle() const
        {
            return hThread;
        }
        HANDLE ContextHandle() const
        {
            return pThreadContext->hThread;
        }
        DWORD ThreadId() const
        {
            return dwThreadId;
        }
        DWORD ContextThreadId() const
        {
            return pThreadContext->dwThreadId;
        }
    protected:
        static DWORD WINAPI StartAddress(LPVOID lpParam);
    };
    
#include <Wct.h>
#pragma comment (lib, "Advapi32.lib")
    class CThreadPool2
    {
#define MSG_ENDTASK    0x1000
#define MAX_WAIT_TIME  120 * 1000
        HANDLE hProblemSolver;                  //线程句柄
        HANDLE hEvent;
        CList<CThread2>* threadList;
        DWORD dwMaxCount;                       //最大线程数量
    public:
        CThreadPool2()
        {
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            dwMaxCount = sysInfo.dwNumberOfProcessors;
            InitializePool();
        };
        CThreadPool2(unsigned uMaxThreads)
        {
            dwMaxCount = uMaxThreads;
            InitializePool();
        };
        ~CThreadPool2()
        {
            SetEvent(hEvent);
            if (WaitForSingleObject(hProblemSolver, MAX_WAIT_TIME) != WAIT_OBJECT_0)
                TerminateThread(hProblemSolver, (DWORD)-1);
            CloseHandle(hProblemSolver);
            CloseHandle(hEvent);
            RemoveAll();
            ClearMessageQueue();
        };
    public:
        DWORD Count()
        {
            return threadList->Count();
        }
        void RemoveThread(DWORD dwThreadId)
        {
            CThread2* thread = Find(dwThreadId);
            if (thread)
                delete thread;
        };
        void WaitAll()
        {
            HANDLE* hThreads = new HANDLE[threadList->Count()];
            CThread2* thread = 0;
            for (unsigned uIndex = 0; uIndex < threadList->Count(); uIndex++)
            {
                thread = threadList->GetNext(thread);
                hThreads[uIndex] = thread->Handle();
            }
            WaitForMultipleObjects(threadList->Count(), hThreads, TRUE, INFINITE);
            delete[] hThreads;
        };
        void RemoveAll()
        {
            CThread2* thread = 0;
            while (thread = threadList->GetNext(thread))
            {
                threadList->Remove(thread);
                thread = 0;
            }
            delete threadList;
        };
        DWORD& MaxCount()
        {
            return dwMaxCount;
        }
        void ReleaseThread(DWORD dwThreadId)
        {
            CThread2* thread = Find(dwThreadId);
            if (thread != NULL)
                TerminateThread(thread->Handle(), (DWORD)-1);
        };
        CThread2* RequestThread(LPTHREAD_START_ROUTINE threadStart)
        {
            CThread2* thread = NULL;
            if (Count() < MaxCount())
            {
                threadList->Append(threadList->Head(), thread = new CThread2(threadStart, GetCurrentThreadId()));
                return thread;
            }
            while (thread = threadList->GetNext(thread))
            {
                if (!thread->StillAlive())
                    break;
            }
            if (thread == NULL)
            {
                MSG msg = { 0 };
                while (GetMessage(&msg, NULL, 0, 0) > 0)
                {
                    thread = Find((DWORD)msg.wParam);
                    if (thread)
                        break;
                }
            }
            threadList->Remove(thread);
            threadList->Append(threadList->Head(), thread = new CThread2(threadStart, GetCurrentThreadId()));
            return thread;
        };
    protected:
        CThread2* Find(DWORD dwThreadId)
        {
            CThread2* thread = NULL;
            while (thread = threadList->GetNext(thread))
            {
                if (thread->ThreadId() == dwThreadId)
                    return thread;
            }
            return NULL;
        };
    private:
        void InitializePool()
        {
            MSG msg;
            PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
            hEvent = CreateEvent(NULL, TRUE, FALSE, _T("__prbslv_1342__"));
            hProblemSolver = CreateThread(NULL, 0, ProblemSolver, this, 0, NULL);
            threadList = new CList<CThread2>();
        };
        static DWORD WINAPI ProblemSolver(LPVOID lpParam)
        {
            CThreadPool2* pool = (CThreadPool2*)lpParam;
            DWORD dwWaitStatus = 0;
            while (true)
            {
                dwWaitStatus = WaitForSingleObject(pool->hEvent, 10);
                if (dwWaitStatus == WAIT_OBJECT_0)
                    break;
                CThread2* thread = NULL;
                while (thread = pool->threadList->GetNext(thread))
                {
                    if (thread->StillAlive())
                    {
                        HWCT hWct = NULL;
                        DWORD dwNodeCount = WCT_MAX_NODE_COUNT;
                        BOOL bDeadlock = FALSE;
                        WAITCHAIN_NODE_INFO NodeInfoArray[WCT_MAX_NODE_COUNT];
                        hWct = OpenThreadWaitChainSession(0, NULL);
                        if (GetThreadWaitChain(hWct, NULL, WCTP_GETINFO_ALL_FLAGS, thread->ContextThreadId(), &dwNodeCount, NodeInfoArray, &bDeadlock))
                        {
                            if (bDeadlock)
                            {
                                if (TerminateThread(thread->ContextHandle(), (DWORD)-1))
                                    _tprintf_s(_T("Error! Deadlock found!\nThread:\t[%u] terminated!\n\n"), thread->ContextThreadId());
                            }
                        }
                        CloseThreadWaitChainSession(hWct);
                    }
                }
                Sleep(1000);
            }
            return 0L;
        };
        void ClearMessageQueue()
        {
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE));
        }
    };

    //模拟两银行账户之间转移资金(注意这里可能出现死锁)
    class CBankAccount
    {
    public:
        static int main(int argc, char** argv);
        static int main2(int argc, char** argv);
    private:
        unsigned uID;                   //银行帐号ID
        double dBalance;                //资金
        TCHAR szLock[32];               //锁
        static unsigned NewId();        //生成银行帐号ID
        static double Random(double dMin, double dMax);
    public:
        CBankAccount(double dBalance);
        CBankAccount(double dBalance, TCHAR* szLockName);
        //线程函数 转移资金过程
        static DWORD WINAPI Transfer(LPVOID lpParameter);
    public:
        double& Balance();
        unsigned AccountID() const;
        TCHAR* LockName(void);
    };

}


#endif



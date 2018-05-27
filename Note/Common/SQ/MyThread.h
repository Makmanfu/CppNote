//*************************************************************************
//
//              线程学习中的个人总结
//
//                                              @阿甘 2017-07-23
//
//*************************************************************************

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "WinGUI.h"
#include "WinThread.h"
#include <iostream>
using namespace std;

//线程总结集合
namespace ThreadStudy
{
    //基本线程演示
    class ThreadTest1
    {
    public:
        static int main(int argc, char** argv)
        {
            ThreadTest1 td;
            td.RUN();
            return 1;
        };
    public:
        void RUN(void)
        {
            //创建线程
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, NULL, 0, NULL);
            cout << "Waiting execution..." << endl;
            //等待阻塞线程完成
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
            cout << "Thread end..." << endl;
        };
        static DWORD ThreadStart(LPVOID lpParameter)
        {
            Sleep(100);
            cout << "ThreadStart..." << endl;
            for (int i = 1; i <= 20; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            for (int i = 1; i <= 20; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            return 0;
        };
    };
    //基本多线程运行带传参数
    class ThreadTest2
    {
    public:
        static int main(int argc, char** argv)
        {
            ThreadTest2 td;
            td.RUN();
            return 1;
        };
    public:
        void RUN(void)
        {
            //创建线程并传参数
            HANDLE hThreads[2];
            int pObject1 = 10;
            hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject1, 0, NULL);
            int pObject2 = 12;
            hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject2, 0, NULL);
            cout << "Waiting execution..." << endl;
            //阻塞 等待所有线程完成
            WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
            CloseHandle(hThreads[0]);
            CloseHandle(hThreads[1]);
            cout << "Thread end..." << endl;
        };
        static DWORD ThreadStart(LPVOID lpParameter)
        {
            Sleep(100);
            //处理传入的参数
            int* pObject = (int*)lpParameter;
            cout << "Thread Start..." << "参数" << *pObject << endl;
            
            for (int i = 1; i <= *pObject; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            return 0;
        };
    };
    //使用互斥元保护
    class ThreadTest3
    {
    public:
        static int main(int argc, char** argv)
        {
            ThreadTest3 td;
            td.RUN();
            return 1;
        };
    public:
        void RUN(void)
        {
            //CreateMutex 第二个参数为TRUE时主线程立即占有互斥元锁
            //这里FALSE 创建了互斥元后可以用WaitForSingleObject去抢占了
            HANDLE hMutex = CreateMutex(NULL, FALSE, _T("__MAIN_MUTEX__")); 
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, NULL, 0, NULL);
            //Sleep(100);       //这里最好加个Sleep防止被下面的WaitForSingleObject先抢了
            cout << "Waiting execution..." << endl;
            //等待阻塞线程完成
            WaitForSingleObject(hMutex, INFINITE);
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);
            CloseHandle(hThread);
            cout << "Thread end..." << endl;
        };
        static DWORD ThreadStart(LPVOID lpParameter)
        {
            HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("__MAIN_MUTEX__"));
            WaitForSingleObject(hMutex, INFINITE);
            cout << "ThreadStart..." << endl;
            for (int i = 1; i <= 20; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            for (int i = 1; i <= 20; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            ReleaseMutex(hMutex);
            return 0;
        };
    };
    //多线程抢一共享资源(用互斥元保护)
    class ThreadTest4
    {
    public:
        static int main(int argc, char** argv)
        {
            ThreadTest4 td;
            td.RUN();
            return 1;
        };
    public:
        void RUN(void)
        {
            HANDLE hMutex = CreateMutex(NULL, FALSE, _T("__MAIN_MUTEX__"));
            //创建线程并传参数
            HANDLE hThreads[3];
            int pObject1 = 10;
            hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject1, 0, NULL);
            int pObject2 = 11;
            hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject2, 0, NULL);
            int pObject3 = 12;
            hThreads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject3, 0, NULL);

            cout << "Waiting execution..." << endl;
            //阻塞 等待所有线程完成
            WaitForMultipleObjects(3, hThreads, TRUE, INFINITE);
            CloseHandle(hThreads[0]);
            CloseHandle(hThreads[1]);
            CloseHandle(hThreads[3]);
            CloseHandle(hMutex);
            cout << "Thread end..." << endl;
        };
        static DWORD ThreadStart(LPVOID lpParameter)
        {
            HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, _T("__MAIN_MUTEX__"));
            WaitForSingleObject(hMutex, INFINITE);
            //处理传入的参数
            int* pObject = (int*)lpParameter;
            cout << "Thread Start..." << "参数" << *pObject << endl;

            for (int i = 1; i <= *pObject; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            ReleaseMutex(hMutex);
            return 0;
        };
    };
    //实例化
    class _Threads : public ThreadWork::ThreadObj
    {
    public:
        static DWORD WINAPI StartAddress(LPVOID lpParameter);
        static DWORD WINAPI StartAddress2(LPVOID lpParameter);
    protected:
        virtual DWORD Run(LPVOID lpParameter = 0);
    };
    //异步操作界面
    struct AsyncGUI :public XqWindowEx
    {
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    private:
#define NUMBER      16
        HWND hRects[NUMBER];
        _Threads threads[NUMBER];
    public:
        void CreateThreads(void);
        void CreateThreads2(void);
        void CloseThreads();
        virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    };
    //同步操作界面
    struct SyncGUI :public AsyncGUI
    {
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    };

}

#endif



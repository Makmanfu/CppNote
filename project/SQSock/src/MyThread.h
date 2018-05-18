//*************************************************************************
//
//              �߳�ѧϰ�еĸ����ܽ�
//
//                                              @���� 2017-07-23
//
//*************************************************************************

#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "WinGUI.h"
#include "WinThread.h"
#include <iostream>
using namespace std;

//�߳��ܽἯ��
namespace ThreadStudy
{
    //�����߳���ʾ
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
            //�����߳�
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, NULL, 0, NULL);
            cout << "Waiting execution..." << endl;
            //�ȴ������߳����
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
    //�������߳����д�������
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
            //�����̲߳�������
            HANDLE hThreads[2];
            int pObject1 = 10;
            hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject1, 0, NULL);
            int pObject2 = 12;
            hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject2, 0, NULL);
            cout << "Waiting execution..." << endl;
            //���� �ȴ������߳����
            WaitForMultipleObjects(2, hThreads, TRUE, INFINITE);
            CloseHandle(hThreads[0]);
            CloseHandle(hThreads[1]);
            cout << "Thread end..." << endl;
        };
        static DWORD ThreadStart(LPVOID lpParameter)
        {
            Sleep(100);
            //������Ĳ���
            int* pObject = (int*)lpParameter;
            cout << "Thread Start..." << "����" << *pObject << endl;
            
            for (int i = 1; i <= *pObject; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            return 0;
        };
    };
    //ʹ�û���Ԫ����
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
            //CreateMutex �ڶ�������ΪTRUEʱ���߳�����ռ�л���Ԫ��
            //����FALSE �����˻���Ԫ�������WaitForSingleObjectȥ��ռ��
            HANDLE hMutex = CreateMutex(NULL, FALSE, _T("__MAIN_MUTEX__")); 
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, NULL, 0, NULL);
            //Sleep(100);       //������üӸ�Sleep��ֹ�������WaitForSingleObject������
            cout << "Waiting execution..." << endl;
            //�ȴ������߳����
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
    //���߳���һ������Դ(�û���Ԫ����)
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
            //�����̲߳�������
            HANDLE hThreads[3];
            int pObject1 = 10;
            hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject1, 0, NULL);
            int pObject2 = 11;
            hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject2, 0, NULL);
            int pObject3 = 12;
            hThreads[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject3, 0, NULL);

            cout << "Waiting execution..." << endl;
            //���� �ȴ������߳����
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
            //������Ĳ���
            int* pObject = (int*)lpParameter;
            cout << "Thread Start..." << "����" << *pObject << endl;

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
    //ʵ����
    class _Threads : public ThreadWork::ThreadObj
    {
    public:
        static DWORD WINAPI StartAddress(LPVOID lpParameter);
        static DWORD WINAPI StartAddress2(LPVOID lpParameter);
    protected:
        virtual DWORD Run(LPVOID lpParameter = 0);
    };
    //�첽��������
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
    //ͬ����������
    struct SyncGUI :public AsyncGUI
    {
        static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    };

}

#endif



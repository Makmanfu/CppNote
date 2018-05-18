

#include "stdafx.h"
#include "MyThread.h"


namespace ThreadStudy
{
    DWORD WINAPI _Threads::StartAddress(LPVOID lpParameter)
    {
        int nRet = 0;
        //同步操作
        _Threads* pComThread = (_Threads*)lpParameter;
        while (TRUE)
        {
            if (NULL != pComThread)
            {
                CProcessLock lock;
                {
                    pComThread->Run(lpParameter);
                }
            }
            nRet = WaitForSingleObject(pComThread->GetHandle(), 10);
            if (nRet == WAIT_OBJECT_0)
                break;
        }
        return (nRet);
    }

    DWORD WINAPI _Threads::StartAddress2(LPVOID lpParameter)
    {
        int nRet = 0;
        _Threads* pComThread = (_Threads*)lpParameter;
        //异步操作(就是不要锁并发运行)
        while (TRUE)
        {
            if (NULL != pComThread)
                pComThread->Run(lpParameter);
            nRet = WaitForSingleObject(pComThread->GetHandle(), 10);
            if (nRet == WAIT_OBJECT_0)
                break;
        }

        return (nRet);
    }

    DWORD _Threads::Run(LPVOID lpParameter)
    {
#define WINDOWS_NUMBER      10
#define SLEEP_TIME          50  //500
        //随机种子生成器
        srand((unsigned)time(NULL));
        HWND hWnd = (HWND)GetUserData();
        RECT rect;
        // get window抯 dimensions
        BOOL bError = GetClientRect(hWnd, &rect);
        if (!bError)
            return 0;
        int iClientX = rect.right - rect.left;
        int iClientY = rect.bottom - rect.top;
        //do not draw if the window does not have any dimensions
        if ((!iClientX) || (!iClientY))
            return 0;
        // get device context for drawing
        HDC hDC = GetDC(hWnd);
        if (hDC)
        {
            // draw the ten random figures
            for (int iCount = 0; iCount < WINDOWS_NUMBER; iCount++)
            {
                // set coordinates
                int iStartX = (int)(rand() % iClientX);
                int iStopX = (int)(rand() % iClientX);
                int iStartY = (int)(rand() % iClientY);
                int iStopY = (int)(rand() % iClientY);
                // set the color
                int iRed = rand() & 255;
                int iGreen = rand() & 255;
                int iBlue = rand() & 255;
                // create a solid brush
                HANDLE hBrush = CreateSolidBrush(GetNearestColor(hDC, RGB(iRed, iGreen, iBlue)));
                HANDLE hbrOld = SelectBrush(hDC, hBrush);
                Rectangle(hDC, min(iStartX, iStopX), max(iStartX, iStopX), min(iStartY, iStopY), max(iStartY, iStopY));
                // delete the brush
                DeleteBrush(SelectBrush(hDC, hbrOld));
            }
            // release the Device Context
            ReleaseDC(hWnd, hDC);
        }
        Sleep(SLEEP_TIME);
        return 1;
    }

    int AsyncGUI::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        AsyncGUI WND;
        WND.InitData(hInstance);
        if (WND.CreateEX(L"AsyncThreads"))
            return 0;
        WND.CreateThreads();            //创建线程
        WND.ShowDLG();
        return 0;
    }

    void AsyncGUI::CreateThreads(void)
    {
        for (int iIndex = 0; iIndex < NUMBER; iIndex++)
        {
            threads[iIndex].Create(&_Threads::StartAddress);
            threads[iIndex].SetUserData(hRects[iIndex]);
            threads[iIndex].Resume();
        }
    }

    void AsyncGUI::CreateThreads2(void)
    {
        for (int iIndex = 0; iIndex < NUMBER; iIndex++)
        {
            threads[iIndex].Create(&_Threads::StartAddress2);
            threads[iIndex].SetUserData(hRects[iIndex]);
            threads[iIndex].Resume();
        }
    }

    void AsyncGUI::CloseThreads()
    {
        ////threads->Alert();
        ////for (int iIndex = 0; iIndex < NUMBER; iIndex++)
        ////    threads[iIndex].Join();
    }

    void AsyncGUI::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //size锁定
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &~WS_MAXIMIZEBOX &~WS_SIZEBOX);

        RECT rect;
        GetClientRect(hWnd, &rect);
        int w = (rect.right - rect.left) / NUMBER, h = rect.bottom - rect.top - 20;
        //HWND hRects[NUMBER];
        //for (int i = 0; i < NUMBER; ++i)
        //    hRects[i] = CreateWindow(_T("STATIC"), _T(""), WS_BORDER | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, i*w + 5, 10, w - 10, h, hWnd, NULL, GethInst(), NULL);

        //分组
        int row = 4;
        int ww = (rect.right - rect.left) / row, hh = NUMBER / row;
        int k(0), j(0);
        for (int i = 0; i < NUMBER; ++i)
        {
            if (k > row - 1)
            {
                k = 0;
                j++;
            }
            //(x1,y1,w,h)
            hRects[i] = CreateWindow(_T("STATIC"), _T(""), WS_BORDER | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
                k++* ww + 5, j * h / hh + 10, ww - 10, h / hh, hWnd, NULL, GethInst(), NULL);
        }
    }

    void AsyncGUI::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        CloseThreads();
        DestroyWindow(hWnd);
    }

    int SyncGUI::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        SyncGUI WND;
        WND.InitData(hInstance);
        WND.CreateEX(L"SyncThreads");
        WND.CreateThreads2();
        WND.ShowDLG();
        return 0;
    }


}


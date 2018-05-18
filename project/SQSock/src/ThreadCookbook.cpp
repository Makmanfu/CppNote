

#include "stdafx.h"
#include "ThreadCookbook.h"
#include <sstream>
#include <fstream>
#include <time.h>
#include <commctrl.h>

namespace IPCDemo
{
    _IPC::_IPC() : pCommObject(NULL), hMapping(NULL), hMutex(NULL)
    {
    }

    _IPC::~_IPC()
    {
        releseHand();
    }

    bool _IPC::ProcessOpen(HWND hWnd)
    {
        //创建互斥量
        TCHAR* SYNCHRONIZING_MUTEX_NAME = TEXT("__TEST_MUTEX__");
        hMutex = CreateMutex(NULL, FALSE, SYNCHRONIZING_MUTEX_NAME);
        if (!hMutex)
        {
            cout << "Cannot create mutex!" << endl << "Error:\t" << GetLastError() << endl;
            return false;
        }
        //创建文件映射句柄
        TCHAR* COMMUNICATION_OBJECT_NAME = TEXT("__FILE_MAPPING__");
        hMapping = CreateFileMapping((HANDLE) - 1, NULL, PAGE_READWRITE, 0,
                                     sizeof(COMMUNICATIONOBJECT), COMMUNICATION_OBJECT_NAME);
        if (!hMapping)
        {
            cout << "Cannot create mapping object!" << endl << "Error:\t" << GetLastError() << endl;
            return false;
        }
        //文件映射对象(进程间的共享访问)
        pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
        if (pCommObject)
        {
            pCommObject->bExitLoop = FALSE;
            pCommObject->hWndClient = hWnd;
            pCommObject->lSleepTimeout = 250;
            UnmapViewOfFile(pCommObject);
        }
        STARTUPINFO startupInfoRed = { 0 };
        PROCESS_INFORMATION processInformationRed = { 0 };
        STARTUPINFO startupInfoBlue = { 0 };
        PROCESS_INFORMATION processInformationBlue = { 0 };
        //创建进程
        BOOL bSuccess = CreateProcess(TEXT("IPCWorker.exe"), TEXT("red"), NULL,
                                      NULL, FALSE, 0, NULL, NULL, &startupInfoRed, &processInformationRed);
        if (!bSuccess)
        {
            cout << "Cannot create process red!" << endl << "Error:\t" << GetLastError() << endl;
            return false;
        }
        //创建进程
        bSuccess = CreateProcess(TEXT("IPCWorker.exe"), TEXT("blue"), NULL,
                                 NULL, FALSE, 0, NULL, NULL, &startupInfoBlue, &processInformationBlue);
        if (!bSuccess)
        {
            cout << "Cannot create process blue!" << endl << "Error:\t" << GetLastError() << endl;
            return false;
        }
        return true;
    }

    void _IPC::DestroyMap(void)
    {
        pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
        if (pCommObject)
        {
            pCommObject->bExitLoop = TRUE;
            UnmapViewOfFile(pCommObject);
        }
    }

    void _IPC::releseHand(void)
    {
        CloseHandle(hMapping);
        CloseHandle(hMutex);
    }

    IPCWorker::IPCWorker() : bContinueLoop(TRUE)
        , hBrush(NULL)
        , hWnd(NULL)
        , hDC(NULL)
        , lWaitTimeout(0)
    {
        rectClient = { 0 };
    }

    bool IPCWorker::ProcessOpenMap(void)
    {
        //打开进入互斥量
        TCHAR* SYNCHRONIZING_MUTEX_NAME = TEXT("__TEST_MUTEX__");
        HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, SYNCHRONIZING_MUTEX_NAME);
        //文件映射句柄
        TCHAR* COMMUNICATION_OBJECT_NAME = TEXT("__FILE_MAPPING__");
        HANDLE hMapping = OpenFileMapping(FILE_MAP_READ, FALSE, COMMUNICATION_OBJECT_NAME);
        if (hMapping)
            return true;
        return false;
    }

    void IPCWorker::LoopSend(int _color)
    {
        if (_color == 0)
            hBrush = CreateSolidBrush(RGB(0, 0, 255));
        else
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
        if (!ProcessOpenMap())
            return;
        while (bContinueLoop)
        {
            //监听等待hMutex有信号立马往下切换
            WaitForSingleObject(hMutex, INFINITE);
            //获取文件映射数据
            pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping,
                          FILE_MAP_READ, 0, 0, sizeof(COMMUNICATIONOBJECT));
            if (pCommObject)
            {
                //根据取得的文件映射数据 干活
                bContinueLoop = !pCommObject->bExitLoop;
                hWnd = pCommObject->hWndClient;
                lWaitTimeout = pCommObject->lSleepTimeout;
                UnmapViewOfFile(pCommObject);
                hDC = GetDC(hWnd);
                if (GetClientRect(hWnd, &rectClient))
                    FillRect(hDC, &rectClient, hBrush);
                ReleaseDC(hWnd, hDC);
                Sleep(lWaitTimeout);
            }
            //释放互斥量
            ReleaseMutex(hMutex);
        }
        DeleteObject(hBrush);
    }

    void IPCGui::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        mIPC = make_shared<_IPC>();
        //size锁定
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
        HWND hStatic = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE,
                                    10, 10, 365, 205, hWnd, NULL, this->GethInst(), NULL);
        if (!hStatic)
            return;
        if (mIPC->ProcessOpen(hStatic))
            return;
    }

    void IPCGui::WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        mIPC->DestroyMap();
        PostQuitMessage(0);
    }
}

namespace IPCDemoMuti
{
    _IPC::_IPC() : hMapping(NULL), hSemaphore(NULL)
    {
        if (!CreateShareIPC())
        {
            MessageBoxA(NULL, "ERROR", "ERROR", MB_OK);
            return;
        }
    }

    _IPC::~_IPC()
    {
        ReleaseShareIPC();
    }

    bool _IPC::CreateShareIPC(void)
    {
        TCHAR* szMappingName = TEXT("__SHARED_FILE_MAPPING__");
        //文件映像句柄
        hMapping = CreateFileMapping((HANDLE) - 1, NULL, PAGE_READWRITE, 0, sizeof(COMMUNICATIONOBJECT), szMappingName);
        if (!hMapping)
        {
            MessageBox(NULL, TEXT("Cannot open file mapping"), TEXT("Error!"), MB_OK);
            return false;
        }
        //文件映像对象
        pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
        if (!pCommObject)
        {
            MessageBox(NULL, TEXT("Cannot get access to file mapping! "), TEXT("Error!"), MB_OK);
            CloseHandle(hMapping);
            return false;
        }
        return true;
    }

    void _IPC::SetFileData(HWND hWnd)
    {
        pCommObject->bExitApplication = false;
        pCommObject->hWnd = hWnd;
        memset(pCommObject->iPhilosopherArray, 0, sizeof(*pCommObject->iPhilosopherArray));
        pCommObject->PhilosopherCount = PHILOSOPHER_COUNT;
    }

    void _IPC::CreateProc(void)
    {
        TCHAR* szSemaphoreName = TEXT("__PD_SEMAPHORE__");
        //创建信号量
        hSemaphore = CreateSemaphore(NULL,
                                     int(PHILOSOPHER_COUNT / 2),
                                     int(PHILOSOPHER_COUNT / 2), szSemaphoreName);
        STARTUPINFO startupInfo[PHILOSOPHER_COUNT] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
        PROCESS_INFORMATION processInformation[PHILOSOPHER_COUNT] = { { 0 }, { 0 }, { 0 }, { 0 }, { 0 } };
        HANDLE hProcesses[PHILOSOPHER_COUNT];
        TCHAR szBuffer[8];
        //创建5个进程
        for (int iIndex = 0; iIndex < PHILOSOPHER_COUNT; iIndex++)
        {
#ifdef UNICODE
            wsprintf(szBuffer, L"%d", iIndex);
#else
            sprintf(szBuffer, "%d", iIndex);
#endif
            if (CreateProcess(TEXT("Philosopher.exe"), szBuffer, NULL, NULL,
                              FALSE, 0, NULL, NULL, &startupInfo[iIndex], &processInformation[iIndex]))
                hProcesses[iIndex] = processInformation[iIndex].hProcess;
        }
    }

    void _IPC::ReleaseShareIPC(void)
    {
        pCommObject->bExitApplication = true;
        UnmapViewOfFile(pCommObject);
        WaitForMultipleObjects(PHILOSOPHER_COUNT, hProcesses, TRUE, INFINITE);
        for (int iIndex = 0; iIndex < PHILOSOPHER_COUNT; iIndex++)
            CloseHandle(hProcesses[iIndex]);
        CloseHandle(hSemaphore);
        CloseHandle(hMapping);
    }


    IPCWorker::IPCWorker() : bExitApplication(false)
    {
    }

    bool IPCWorker::ProcessOpenMap(void)
    {
        //文件映射句柄
        TCHAR* szMappingName = TEXT("__SHARED_FILE_MAPPING__");
        hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szMappingName);
        if (hMapping)
            return true;
        return false;
    }

    void IPCWorker::LoopSend(int _color)
    {
        int iIndex = _color;
        TCHAR* szSemaphoreName = TEXT("__PD_SEMAPHORE__");
        if (!ProcessOpenMap())
            return;
        while (!bExitApplication)
        {
            HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, szSemaphoreName);
            WaitForSingleObject(hSemaphore, INFINITE);
            PCOMMUNICATIONOBJECT pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping,
                                               FILE_MAP_ALL_ACCESS, 0, 0, sizeof(COMMUNICATIONOBJECT));
            bExitApplication = pCommObject->bExitApplication;
            if (!pCommObject->iPhilosopherArray[(iIndex + pCommObject->PhilosopherCount - 1) % pCommObject->PhilosopherCount]
                && !pCommObject->iPhilosopherArray[(iIndex + 1) % pCommObject->PhilosopherCount])
            {
                pCommObject->iPhilosopherArray[iIndex] = 1;
                cout << iIndex << " : 1++++++++++++++" << endl;
                Sleep(1000);            //因切换太快 加sleep可见
            }
            InvalidateRect(pCommObject->hWnd, NULL, TRUE);
            //SendMessage(pCommObject->hWnd, WM_INVALIDATE, 0, 0);
            pCommObject->iPhilosopherArray[iIndex] = 0;
            UnmapViewOfFile(pCommObject);
            ReleaseSemaphore(hSemaphore, 1, NULL);
            CloseHandle(hSemaphore);
        }
        CloseHandle(hMapping);
    }

    IPCGui::IPCGui() : mIPC()
    {
    }

    int IPCGui::PhilosopherPass(int iPhilosopher)
    {
        return mIPC.pCommObject->iPhilosopherArray[iPhilosopher - 1];
    }

    void IPCGui::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //size锁定
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
        mIPC.SetFileData(hWnd);
        mIPC.CreateProc();
    }

    void IPCGui::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        PAINTSTRUCT paintStruct;
        HDC hDC = BeginPaint(hWnd, &paintStruct);
        FillEllipse(hWnd, hDC, 210, 10, 310, 110, PhilosopherPass(1));
        FillEllipse(hWnd, hDC, 410, 170, 510, 270, PhilosopherPass(2));
        FillEllipse(hWnd, hDC, 335, 400, 435, 500, PhilosopherPass(3));
        FillEllipse(hWnd, hDC, 80, 400, 180, 500, PhilosopherPass(4));
        FillEllipse(hWnd, hDC, 10, 170, 110, 270, PhilosopherPass(5));
        EndPaint(hWnd, &paintStruct);
    }

    void IPCGui::FillEllipse(HWND hWnd, HDC hDC, int iLeft, int iTop, int iRight, int iBottom, int iPass)
    {
        HBRUSH hBrush = NULL;
        if (iPass)
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
        else
            hBrush = CreateSolidBrush(RGB(255, 255, 255));
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
        Ellipse(hDC, iLeft, iTop, iRight, iBottom);
        SelectObject(hDC, hOldBrush);
        DeleteObject(hBrush);
    }
}

namespace ThreadDemo
{
#define THREADS_NUMBER      2
#define ELEMENTS_NUMBER     200
#define BLOCK_SIZE          ELEMENTS_NUMBER / THREADS_NUMBER
#define MAX_VALUE           1000
    typedef struct _tagARRAYOBJECT
    {
        int* iArray;
        int iSize;
        int iThreadID;
    } ARRAYOBJECT, *PARRAYOBJECT;

    int SimpleThread::main(int argc, char** argv)
    {
        SimpleThread td;
        td.RUN();
        system("PAUSE");
        return 0;
    }

    void SimpleThread::RUN(void)
    {
        int iArray1[BLOCK_SIZE];
        int iArray2[BLOCK_SIZE];
        int iArray[ELEMENTS_NUMBER];        //结果数组
        //造假数据填充到数组里去
        for (int iIndex = 0; iIndex < BLOCK_SIZE; iIndex++)
        {
            iArray1[iIndex] = rand() % MAX_VALUE;
            iArray2[iIndex] = rand() % MAX_VALUE;
        }
        //创建线程并传参数
        HANDLE hThreads[THREADS_NUMBER];
        ARRAYOBJECT pObject1 = { &(iArray1[0]), BLOCK_SIZE, 0 };
        hThreads[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject1, 0, NULL);
        ARRAYOBJECT pObject2 = { &(iArray2[0]), BLOCK_SIZE, 1 };
        hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ThreadStart, (LPVOID)&pObject2, 0, NULL);
        cout << "Waiting execution..." << endl;
        //等待所有线程完成[线程个数,线程数组地址,等待时间]
        WaitForMultipleObjects(THREADS_NUMBER, hThreads, TRUE, INFINITE);
        //合并数组
        MergeArrays(&iArray1[0], BLOCK_SIZE, &iArray2[0], BLOCK_SIZE, &iArray[0]);
        //打印排好的数组
        PrintArray(iArray, ELEMENTS_NUMBER);
        CloseHandle(hThreads[0]);
        CloseHandle(hThreads[1]);
        cout << "Array sorted..." << endl;
    }

    DWORD SimpleThread::ThreadStart(LPVOID lpParameter)
    {
        PARRAYOBJECT pObject = (PARRAYOBJECT)lpParameter;
        int iTmp = 0;
        for (int iIndex = 0; iIndex < pObject->iSize; iIndex++)
        {
            for (int iEndIndex = pObject->iSize - 1; iEndIndex > iIndex; iEndIndex--)
            {
                if (pObject->iArray[iEndIndex] < pObject->iArray[iIndex])
                {
                    iTmp = pObject->iArray[iEndIndex];
                    pObject->iArray[iEndIndex] = pObject->iArray[iIndex];
                    pObject->iArray[iIndex] = iTmp;
                }
            }
        }
        return 0;
    }

    void SimpleThread::PrintArray(int* iArray, int iSize)
    {
        for (int iIndex = 0; iIndex < iSize; iIndex++)
            cout << " " << iArray[iIndex];
        cout << endl;
    }

    void SimpleThread::MergeArrays(int* leftArray, int leftArrayLenght, int* rightArray,
                                   int rightArrayLenght, int* mergedArray)
    {
        int i = 0;
        int j = 0;
        int k = 0;
        while (i < leftArrayLenght && j < rightArrayLenght)
        {
            if (leftArray[i] < rightArray[j])
                mergedArray[k] = leftArray[i++];    //注意I++最好拆开写
            else
                mergedArray[k] = rightArray[j++];   //注意J++最好拆开写
            k++;
        }
        if (i >= leftArrayLenght)
        {
            while (j < rightArrayLenght)
                mergedArray[k++] = rightArray[j++]; //作死的省掉了
        }
        if (j >= rightArrayLenght)
        {
            while (i < leftArrayLenght)
                mergedArray[k++] = leftArray[i++];
        }
    }

    DWORD _miniThread::dwReadBytes = 0;
    DWORD _miniThread::dwWriteBytes = 0;
    DWORD _miniThread::dwBlockSize = 0;
    DWORD _miniThread::dwFileSize = 0;
    HLOCAL _miniThread::pMemory = NULL;

    _miniThread::_miniThread()/*: dwReadBytes(0), dwWriteBytes(0), dwBlockSize(0), dwFileSize(0)*/
    {
    }

    _miniThread::~_miniThread()
    {
    }

    void _miniThread::OpenFileDialog(HINSTANCE Inst, HWND& hWndPB)
    {
        TCHAR szReadFile[MAX_PATH];
        TCHAR szWriteFile[MAX_PATH];
        if (FileDialog(NULL, szReadFile, FILE_OPEN) && FileDialog(NULL, szWriteFile, FILE_SAVE))
        {
            COPYDETAILS copyDetails = { Inst, hWndPB, szReadFile, szWriteFile };
            HANDLE hMutex = CreateMutex(NULL, FALSE, MUTEX_NAME);
            HANDLE hReadThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ReadRoutine, &copyDetails, 0, NULL);
            //这里阻塞线程
            WaitForSingleObject(hReadThread, INFINITE);
            CloseHandle(hReadThread);
            CloseHandle(hMutex);
        }
    }

    DWORD _miniThread::ReadRoutine(LPVOID lpParameter)
    {
        PCOPYDETAILS pCopyDetails = (PCOPYDETAILS)lpParameter;
        HANDLE hFile = CreateFile(pCopyDetails->szReadFileName, GENERIC_READ,
                                  FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == (HANDLE)INVALID_HANDLE_VALUE)
            return FALSE;
        //获取文件size
        dwFileSize = GetFileSize(hFile, NULL);
        dwBlockSize = GetBlockSize(dwFileSize);
        HANDLE hWriteThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WriteRoutine, pCopyDetails, 0, NULL);
        size_t uBufferLength = (size_t)ceil((double)dwFileSize / (double)dwBlockSize);
        SendMessage(pCopyDetails->hWndPB, PBM_SETRANGE, 0, MAKELPARAM(0, uBufferLength));
        pMemory = LocalAlloc(LPTR, dwFileSize);
        void* pBuffer = LocalAlloc(LPTR, dwBlockSize);
        int iOffset = 0;
        DWORD dwBytesRed = 0;
        do
        {
            ReadFile(hFile, pBuffer, dwBlockSize, &dwBytesRed, NULL);
            if (!dwBytesRed)
                break;
            HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
            WaitForSingleObject(hMutex, INFINITE);
            memcpy((char*)pMemory + iOffset, pBuffer, dwBytesRed);
            dwReadBytes += dwBytesRed;
            ReleaseMutex(hMutex);
            iOffset += (int)dwBlockSize;
        }
        while (true);
        LocalFree(pBuffer);
        CloseHandle(hFile);
        CloseHandle(hWriteThread);
        return 0;
    }

    DWORD _miniThread::WriteRoutine(LPVOID lpParameter)
    {
        PCOPYDETAILS pCopyDetails = (PCOPYDETAILS)lpParameter;
        HANDLE hFile = CreateFile(pCopyDetails->szWriteFileName, GENERIC_WRITE,
                                  0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == (HANDLE)INVALID_HANDLE_VALUE)
            return FALSE;
        DWORD dwBytesWritten = 0;
        int iOffset = 0;
        do
        {
            int iRemainingBytes = (int)dwFileSize - iOffset;
            if (iRemainingBytes <= 0)
                break;
            Sleep(10);
            if (dwWriteBytes < dwReadBytes)
            {
                DWORD dwBytesToWrite = dwBlockSize;
                if (!(dwFileSize / dwBlockSize))
                    dwBytesToWrite = (DWORD)iRemainingBytes;
                HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, MUTEX_NAME);
                WaitForSingleObject(hMutex, INFINITE);
                WriteFile(hFile, (char*)pMemory + iOffset, dwBytesToWrite, &dwBytesWritten, NULL);
                dwWriteBytes += dwBytesWritten;
                ReleaseMutex(hMutex);
                SendMessage(pCopyDetails->hWndPB, PBM_STEPIT, 0, 0);
                iOffset += (int)dwBlockSize;
            }
        }
        while (true);
        CloseHandle(hFile);
        dwReadBytes = dwWriteBytes = dwBlockSize = dwFileSize = 0;
        return 0;
    }

    DWORD _miniThread::GetBlockSize(DWORD dwFileSize)
    {
        return dwFileSize > 4096 ? 4096 : 512;
    }

    BOOL _miniThread::FileDialog(HWND hWnd, LPTSTR szFileName, DWORD dwFileOperation)
    {
#ifdef _UNICODE
        OPENFILENAMEW ofn;
#else
        OPENFILENAMEA ofn;
#endif
        TCHAR szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = _T("All\0*.*\0Text\0*.TXT\0");
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = dwFileOperation == FILE_OPEN ? OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST : OFN_SHOWHELP | OFN_OVERWRITEPROMPT;
        if (dwFileOperation == FILE_OPEN)
        {
            if (GetOpenFileName(&ofn) == TRUE)
            {
                _tcscpy_s(szFileName, MAX_PATH - 1, szFile);
                return TRUE;
            }
        }
        else
        {
            if (GetSaveFileName(&ofn) == TRUE)
            {
                _tcscpy_s(szFileName, MAX_PATH - 1, szFile);
                return TRUE;
            }
        }
        return FALSE;
    }

    ConcurrentFileCopy::ConcurrentFileCopy() : hWndPB(NULL)
    {
    }

    void ConcurrentFileCopy::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //size锁定
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
        HWND hButton = CreateWindow(_T("BUTTON"), _T("open"), WS_CHILD | WS_VISIBLE
                                    | BS_PUSHBUTTON | WS_TABSTOP, 10, 10, 100, 35, hWnd, (HMENU)BUTTON_OPEN, GethInst(), NULL);
        RECT rcClient = { 0 };
        GetClientRect(hWnd, &rcClient);
        int cyVScroll = 0;
        cyVScroll = GetSystemMetrics(SM_CYVSCROLL);
        hWndPB = CreateWindow(PROGRESS_CLASS, (LPTSTR)NULL, WS_CHILD | WS_VISIBLE, rcClient.left,
                              rcClient.bottom - cyVScroll - 20, rcClient.right, cyVScroll + 20, hWnd, (HMENU)0, GethInst(), NULL);
        SendMessage(hWndPB, PBM_SETSTEP, (WPARAM)1, 0);
    }

    void ConcurrentFileCopy::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        int ctrlID = LOWORD(wParam);
        switch (ctrlID)
        {
            case BUTTON_OPEN:
                TD.OpenFileDialog(GethInst(), hWndPB);
                break;
            default:
                break;
        }
    }
}

namespace Threads
{
#define WINDOWS_NUMBER      10
#define SLEEP_TIME          50//500

    HANDLE _CThread::Create(LPVOID lpParameter, DWORD dwInitialState, DWORD dwCreationFlag)
    {
        dwState |= dwInitialState;
        this->lpParameter = lpParameter;
        if (dwState & STATE_ALIVE)
            return hThread;
        hThread = CreateThread(NULL, 0, StartAddress, this, dwCreationFlag, &dwThreadId);
        dwState |= STATE_ALIVE;
        if (dwState & STATE_CONTINUOUS)
            hEvent = CreateEvent(NULL, TRUE, FALSE, TEXT("__tmp_event__"));
        return hThread;
    }

    void _CThread::Join(DWORD dwWaitInterval)
    {
        if (dwState & STATE_BLOCKED)
            return;
        if (dwState & STATE_READY)
            return;
        dwState |= STATE_READY;
        WaitForSingleObject(hThread, dwWaitInterval);
        dwState ^= STATE_READY;
    }

    DWORD _CThread::Suspend()
    {
        if (dwState & STATE_BLOCKED)
            return DWORD(-1);
        if (dwState & STATE_READY)
            return DWORD(-1);
        DWORD dwSuspendCount = SuspendThread(hThread);
        dwState |= STATE_BLOCKED;
        return dwSuspendCount;
    }

    DWORD _CThread::Resume()
    {
        if (dwState & STATE_RUNNING)
            return DWORD(-1);
        DWORD dwSuspendCount = ResumeThread(hThread);
        dwState ^= STATE_BLOCKED;
        return dwSuspendCount;
    }

    BOOL _CThread::Alert()
    {
        return SetEvent(hEvent);
    }

    DWORD WINAPI _CThread::StartAddress(LPVOID lpParameter)
    {
        _CThread* cThread = (_CThread*)lpParameter;
        if (cThread->GetAsyncState() == STATE_SYNC)     //同步
        {
            if (cThread->dwState & STATE_CONTINUOUS)
            {
                DWORD dwWaitStatus = 0;
                while (TRUE)
                {
                    cThread->Run();
                    dwWaitStatus = WaitForSingleObject(cThread->hEvent, 10);
                    if (dwWaitStatus == WAIT_OBJECT_0)
                        break;
                }
                return 0;
            }
            cThread->Run();
            return 0;
        }
        if (cThread->dwState & STATE_CONTINUOUS)
        {
            DWORD dwWaitStatus = 0;
            while (TRUE)
            {
                CProcessLock lock;
                {
                    cThread->Run();
                }
                dwWaitStatus = WaitForSingleObject(cThread->hEvent, 10);
                if (dwWaitStatus == WAIT_OBJECT_0)
                    break;
            }
            return 0;
        }
        CProcessLock lock;
        {
            cThread->Run();
        }
        return 0;
    }

    void _AppThreads::Run(LPVOID lpParameter)
    {
        //随机种子生成器
        srand((unsigned)time(NULL));
        HWND hWnd = (HWND)GetUserData();
        RECT rect;
        // get window抯 dimensions
        BOOL bError = GetClientRect(hWnd, &rect);
        if (!bError)
            return;
        int iClientX = rect.right - rect.left;
        int iClientY = rect.bottom - rect.top;
        //do not draw if the window does not have any dimensions
        if ((!iClientX) || (!iClientY))
            return;
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
        return;
    }

    int AsyncThreadGUI::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        AsyncThreadGUI WND;
        WND.InitData(hInstance);
        if (WND.CreateEX(L"AsyncThreads"))
            return 0;
        WND.CreateThreads(STATE_ASYNC);            //创建线程 STATE_SYNC
        WND.ShowDLG();
        return 0;
    }

    void AsyncThreadGUI::CreateThreads(DWORD _STATE)
    {
        //_STATE = STATE_ASYNC 或 STATE_SYNC
        for (int iIndex = 0; iIndex < THREADSNUMBER; iIndex++)
        {
            threads[iIndex].Create(NULL, _STATE | STATE_CONTINUOUS);
            threads[iIndex].SetUserData(hRects[iIndex]);
        }
        //SetWindowLongPtr(GetHandle(), GWLP_USERDATA, (LONG_PTR)threads);
    }

    void AsyncThreadGUI::CloseThreads()
    {
        //通知关闭线程
        //_AsyncThreads* pThread = (_AsyncThreads*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        //_SyncThreads* pThread = (_SyncThreads*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        threads->Alert();
        for (int iIndex = 0; iIndex < THREADSNUMBER; iIndex++)
            threads[iIndex].Join();
    }

    void AsyncThreadGUI::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //size锁定
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
        RECT rect;
        GetClientRect(hWnd, &rect);
        int w = (rect.right - rect.left) / THREADSNUMBER, h = rect.bottom - rect.top - 20;
        //HWND hRects[THREADSNUMBER];
        //for (int i = 0; i < THREADSNUMBER; ++i)
        //    hRects[i] = CreateWindow(_T("STATIC"), _T(""), WS_BORDER | WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN,
        // i*w + 5, 10, w - 10, h, hWnd, NULL, GethInst(), NULL);
        //分组
        int row = 4;
        int ww = (rect.right - rect.left) / row, hh = THREADSNUMBER / row;
        int k(0), j(0);
        for (int i = 0; i < THREADSNUMBER; ++i)
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

    void AsyncThreadGUI::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        CloseThreads();
        DestroyWindow(hWnd);
    }

    int SyncThreadGUI::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        AsyncThreadGUI WND;
        WND.InitData(hInstance);
        WND.CreateEX(L"SyncThreads");
        WND.CreateThreads(STATE_SYNC);            //创建线程
        WND.ShowDLG();
        return 0;
    }
}

namespace Messages
{
#define THREAD_NUMBER 4
#define WM_PLOTDATA WM_USER + 1
#define WM_ENDTASK WM_PLOTDATA + 1
#define LABEL_TEXT 100
#define EVENT_NAME _T( "__t_event__" )

#define GWL_USERDATA        (-21)
#define BUFFER_SIZE         4096
#define MAX_MSGCOUNT        10
#define CALCULATION_TIME    1000
#define DRAWING_TIME        2300

    DWORD WINAPI StartAddress(LPVOID lpParameter)
    {
        //HWND hWnd = (HWND)lpParameter;
        //HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
        //if (hEvent != NULL)
        //    SetEvent(hEvent);
        //CloseHandle(hEvent);
        //int iCount = 0;
        //while (iCount++ < MAX_MSGCOUNT)
        //{
        //    // perform calculation
        //    Sleep(CALCULATION_TIME);
        //    // assembly result into PLOTDATA structure
        //    PPLOTDATA pData = new PLOTDATA();
        //    pData->value = (rand() % 0xFFFF) - iMessageID;
        //    pData->dwThreadId = GetCurrentThreadId();
        //    pData->iMsgID = ++iMessageID;
        //    EnterCriticalSection(&cs);
        //    queue.Enqueue(pData);
        //    LeaveCriticalSection(&cs);
        //    PostMessage(hWnd, WM_PLOTDATA, 0, (LPARAM)pData);
        //}
        return 0L;
    }

    DWORD WINAPI DrawPlot(LPVOID lpParameter)
    {
        //HWND hWnd = (HWND)lpParameter;
        //HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);
        //WaitForSingleObject(hEvent, INFINITE);
        //CloseHandle(hEvent);
        //int iCount = 0;
        //while (iCount++ < MAX_MSGCOUNT * THREAD_NUMBER)
        //{
        //    EnterCriticalSection(&cs);
        //    PPLOTDATA pData = queue.Dequeue();
        //    LeaveCriticalSection(&cs);
        //    if (!pData)
        //        break;
        //    // perform drawing
        //    Sleep(DRAWING_TIME);
        //    HWND hLabel = GetDlgItem(hWnd, LABEL_TEXT);
        //    TCHAR szBuffer[BUFFER_SIZE];
        //    wsprintf(szBuffer, _T("\n\n\tThread ID:\t%u\n\tCurrent value:\t%d\n\tMessage ID:\t%d"), (DWORD)pData->dwThreadId, (int)pData->value, pData->iMsgID);
        //    SetWindowText(hLabel, szBuffer);
        //    delete pData;
        //}
        //PostMessage(hWnd, WM_ENDTASK, 0, 0);
        return 0L;
    }

    MessagesGUI::MessagesGUI() : XqWindow()
        , iMessageID(0)
        , gEvent(NULL)
        , hThread(NULL)
    {
    }

    LRESULT CALLBACK MessagesGUI::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        MessagesGUI* pObj = NULL;
        if (uMsg == WM_CREATE)
        {
            pObj = (MessagesGUI*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
            ::SetWindowLong(hWnd, GWL_USERDATA, (LONG)pObj);        //通过USERDATA把HWND和C++对象关联起来
            pObj->WMCREATE(hWnd, uMsg, wParam, lParam);
            pObj->SetHandle(hWnd);                                  //在此处获取HWND，此时CreateWindow()尚未返回。
        }
        pObj = (MessagesGUI*)::GetWindowLong(hWnd, GWL_USERDATA);
        switch (uMsg)
        {
            case WM_PLOTDATA:
            {
                //接受数据
                PPLOTDATA pData = (PPLOTDATA)lParam;
                HWND hLabel = GetDlgItem(hWnd, LABEL_TEXT);
                TCHAR szBuffer[BUFFER_SIZE];
                GetWindowText(hLabel, szBuffer, BUFFER_SIZE);
                wsprintf(szBuffer, _T("%ws\n\n\tMessage has been received. Msg ID:\t%d"), szBuffer, pData->iMsgID);
                SetWindowText(hLabel, szBuffer);
                break;
            }
            case WM_ENDTASK:
            {
                HWND hLabel = GetDlgItem(hWnd, LABEL_TEXT);
                TCHAR szBuffer[BUFFER_SIZE];
                wsprintf(szBuffer, _T("\n\n\tPlot is drawn. You can close the window now."));
                SetWindowText(hLabel, szBuffer);
                break;
            }
            case WM_LBUTTONDOWN:
                SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
                break;
            case WM_DESTROY:
            {
                pObj->WMDESTROY(hWnd, uMsg, wParam, lParam);
                PostQuitMessage(0);
                break;
            }
            default:
                return DefWindowProc(hWnd, uMsg, wParam, lParam);
                break;
        }
        return 0;
    }

    void MessagesGUI::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        hThreads = new HANDLE[4];
        //size锁定
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
        //临界区初始化
        InitializeCriticalSection(&cs);
        //创建信号
        gEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
        //创建线程组
        for (int iIndex = 0; iIndex < THREAD_NUMBER; iIndex++)
            hThreads[iIndex] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartAddress, hWnd, 0, NULL);
        //创建绘制线程
        hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DrawPlot, hWnd, 0, NULL);
    }

    void MessagesGUI::WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        //等待线程组释放
        WaitForMultipleObjects(THREAD_NUMBER, hThreads, TRUE, INFINITE);
        for (int iIndex = 0; iIndex < THREAD_NUMBER; iIndex++)
            CloseHandle(hThreads[iIndex]);
        //等待创建线程释放
        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        CloseHandle(gEvent);
        //临界区处理
        DeleteCriticalSection(&cs);
    }
}

namespace Concurrent
{
#define CONTROL_BROWSE      100
#define CONTROL_START       101
#define CONTROL_RESULT      103
#define CONTROL_TEXT        104
#define CONTROL_PROGRESS    105

    typedef struct
    {
        int iA1;
        int iB1;
        int iC1;
        int iA2;
        int iB2;
        int iC2;
        HWND hWndProgress;
        HWND hWndResult;
    } QueueElement, *PQueueElement;

    CQueue<QueueElement> queue;
    char szFilePath[MAX_PATH] = { 0 };
    char szResult[4096];

    DWORD WINAPI StartAddress(LPVOID lpParameter)
    {
        //用克莱姆法则解线性方程
        CQueue<QueueElement>* pQueue = (CQueue<QueueElement>*) lpParameter;
        TCHAR* szMutex = _T("__mutex_132__");
        HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, szMutex);
        QueueElement* pQElement = NULL;
        while (true)
        {
            WaitForSingleObject(hMutex, INFINITE);
            pQElement = pQueue->Dequeue();
            ReleaseMutex(hMutex);
            if (pQElement == NULL)
                break;
            char szBuffer[1024];
            double dDeterminant = (pQElement->iA1 * pQElement->iB2) - (pQElement->iB1 * pQElement->iA2);
            if (dDeterminant != 0)
            {
                double dX = ((pQElement->iC1 * pQElement->iB2) - (pQElement->iB1 * pQElement->iC2)) / dDeterminant;
                double dY = ((pQElement->iA1 * pQElement->iC2) - (pQElement->iC1 * pQElement->iA2)) / dDeterminant;
                sprintf_s(szBuffer, "  x = %8.4lf,\ty = %8.4lf\n", dX, dY);
            }
            else
                sprintf_s(szBuffer, "  Determinant is zero.\n");
            strcat_s(szResult, 4096, szBuffer);
            SetWindowTextA(pQElement->hWndResult, szResult);
            SendMessage(pQElement->hWndProgress, PBM_STEPIT, 0, 0);
            delete pQElement;
            Sleep(1000);
        }
        CloseHandle(hMutex);
        return 0L;
    }

    Concurrent::concurrent_operations::concurrent_operations()
    {
    }

    BOOL concurrent_operations::FileDialog(HWND hWnd, LPSTR szFileName)
    {
        OPENFILENAMEA ofn;
        char szFile[MAX_PATH];
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = "All\0*.*\0Text\0*.TXT\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
        if (GetOpenFileNameA(&ofn) == TRUE)
        {
            strcpy_s(szFileName, MAX_PATH - 1, szFile);
            return TRUE;
        }
        return FALSE;
    }

    void concurrent_operations::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
        HFONT hFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, BALTIC_CHARSET, OUT_DEFAULT_PRECIS,
                                 CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, _T("Microsoft Sans Serif"));
        HWND hLabel = CreateWindow(_T("STATIC"), _T("  Press \"Browse\" and choose file with coordinates: "), WS_CHILD
                                   | WS_VISIBLE | SS_CENTERIMAGE | SS_LEFT | WS_BORDER, 20, 20, 250, 25, hWnd, (HMENU)CONTROL_TEXT, this->GethInst(), NULL);
        SendMessage(hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND hResult = CreateWindow(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE | SS_LEFT
                                    | WS_BORDER, 20, 65, 150, 75, hWnd, (HMENU)CONTROL_RESULT, this->GethInst(), NULL);
        SendMessage(hResult, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND hBrowse = CreateWindow(_T("BUTTON"), _T("Browse"), WS_CHILD | WS_VISIBLE
                                    | BS_PUSHBUTTON, 190, 65, 80, 25, hWnd, (HMENU)CONTROL_BROWSE, this->GethInst(), NULL);
        SendMessage(hBrowse, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND hStart = CreateWindow(_T("BUTTON"), _T("Start"), WS_CHILD | WS_VISIBLE
                                   | BS_PUSHBUTTON, 190, 115, 80, 25, hWnd, (HMENU)CONTROL_START, this->GethInst(), NULL);
        SendMessage(hStart, WM_SETFONT, (WPARAM)hFont, TRUE);
        HWND hProgress = CreateWindow(PROGRESS_CLASS, _T(""), WS_CHILD | WS_VISIBLE
                                      | WS_BORDER, 20, 165, 250, 25, hWnd, (HMENU)CONTROL_PROGRESS, this->GethInst(), NULL);
        SendMessage(hProgress, PBM_SETSTEP, (WPARAM)1, 0);
        SendMessage(hProgress, PBM_SETPOS, (WPARAM)0, 0);
        TCHAR* szMutex = _T("__mutex_132__");
        HANDLE hMutex = CreateMutex(NULL, FALSE, szMutex);
        //CloseHandle(hMutex);
    }

    void concurrent_operations::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        int ctrlID = LOWORD(wParam);
        switch (ctrlID)
        {
            case CONTROL_BROWSE:
            {
                if (!FileDialog(hWnd, szFilePath))
                    MessageBox(hWnd, _T("You must choose valid file path!"), _T("Error!"), MB_OK | MB_TOPMOST | MB_ICONERROR);
                else
                {
                    char szBuffer[MAX_PATH];
                    wsprintfA(szBuffer, "\n  File: %s Press \"Start\" now.", strrchr(szFilePath, '\\') + 1);
                    SetWindowTextA(GetDlgItem(hWnd, CONTROL_TEXT), szBuffer);
                }
                break;
            }
            case CONTROL_START:
            {
                if (!*szFilePath)
                {
                    MessageBox(hWnd, _T("You must choose valid file path first!"), _T("Error!"), MB_OK | MB_TOPMOST | MB_ICONERROR);
                    break;
                }
                ifstream infile(szFilePath);
                if (infile.is_open())
                {
                    string line;
                    while (std::getline(infile, line))
                    {
                        QueueElement* pQElement = new QueueElement();
                        istringstream iss(line);
                        if (!(iss >> pQElement->iA1 >> pQElement->iB1 >> pQElement->iC1 >> pQElement->iA2 >> pQElement->iB2 >> pQElement->iC2))
                            break;
                        pQElement->hWndProgress = GetDlgItem(hWnd, CONTROL_PROGRESS);
                        pQElement->hWndResult = GetDlgItem(hWnd, CONTROL_RESULT);
                        queue.Enqueue(pQElement);
                    }
                    infile.close();
                    SendMessage(GetDlgItem(hWnd, CONTROL_PROGRESS), PBM_SETRANGE, 0, MAKELPARAM(0, queue.Count()));
                    //根据系统CPU数分配
                    SYSTEM_INFO sysInfo;
                    GetSystemInfo(&sysInfo);
                    for (DWORD dwIndex = 0; dwIndex < sysInfo.dwNumberOfProcessors; dwIndex++)
                    {
                        HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartAddress, &queue, 0, NULL);
                        SetThreadIdealProcessor(hThread, dwIndex);
                        Sleep(100);
                        CloseHandle(hThread);
                    }
                }
                else
                    MessageBox(hWnd, _T("Cannot open file!"), _T("Error!"), MB_OK | MB_TOPMOST | MB_ICONERROR);
                break;
            }
            default:
                break;
        }
    }

}

namespace ThreadPools
{
    CThread2::CThread2(LPTHREAD_START_ROUTINE lpThreadStart, DWORD dwDispatcherId) : dwExitCode(0)
    {
        pThreadContext = new STARTCONTEXT();
        pThreadContext->lpThreadStart = lpThreadStart;
        pThreadContext->dwDispatcherId = dwDispatcherId;
        pThreadContext->dwMaxWaitTime = MAX_WAIT_TIME;
        hThread = CreateThread(NULL, 0, StartAddress, this, CREATE_SUSPENDED, &dwThreadId);
    }

    CThread2::~CThread2()
    {
        delete pThreadContext;
        if (StillAlive())
            TerminateThread(hThread, (DWORD) - 1);
        CloseHandle(hThread);
    }

    void CThread2::Start(LPVOID lpContext)
    {
        pThreadContext->lpParameter = lpContext;
        ResumeThread(hThread);
    }

    bool CThread2::StillAlive()
    {
        return ExitStatus() == STILL_ACTIVE;
    }

    DWORD CThread2::ExitStatus()
    {
        GetExitCodeThread(hThread, &dwExitCode);
        return dwExitCode;
    }

    CThread2* CThread2::SetMaxWaitTime(DWORD dwMilliseconds)
    {
        pThreadContext->dwMaxWaitTime = dwMilliseconds;
        return this;
    }

    DWORD WINAPI CThread2::StartAddress(LPVOID lpParam)
    {
        CThread2* thread = (CThread2*)lpParam;
        thread->pThreadContext->hThread = CreateThread(NULL, 0, thread->pThreadContext->lpThreadStart, thread->pThreadContext->lpParameter, 0, &thread->pThreadContext->dwThreadId);
        DWORD dwStatus = WaitForSingleObject(thread->pThreadContext->hThread, thread->pThreadContext->dwMaxWaitTime);
        CloseHandle(thread->pThreadContext->hThread);
        PostThreadMessage(thread->pThreadContext->dwDispatcherId, MSG_ENDTASK, (WPARAM)thread->dwThreadId, 0);
        return 0L;
    }

    class CParameters
    {
    public:
        CParameters(CBankAccount* fromAccount, CBankAccount* toAccount,
                    double dAmount, bool bPrintOutput = true)
        {
            this->fromAccount = fromAccount;
            this->toAccount = toAccount;
            this->dAmount = dAmount;
            this->bPrintOutput = bPrintOutput;
        }
        CBankAccount* fromAccount;
        CBankAccount* toAccount;
        double dAmount;
        bool bPrintOutput;
    };

    CBankAccount::CBankAccount(double dBalance) : dBalance(dBalance)
    {
        uID = NewId();
        *szLock = 0;
        _tcscpy_s(szLock, LockName());
    }

    CBankAccount::CBankAccount(double dBalance, TCHAR* szLockName) : dBalance(dBalance)
    {
        uID = NewId();
        *szLock = 0;
        _tcscpy_s(szLock, szLockName);
    }

    DWORD WINAPI CBankAccount::Transfer(LPVOID lpParameter)
    {
        CParameters* parameters = (CParameters*)lpParameter;
        //Sleep(100);  //制造死锁
        CLock* outerLock = new CLock(parameters->fromAccount->szLock);
        if (parameters->dAmount < parameters->fromAccount->dBalance)
        {
            CLock* innerLock = new CLock(parameters->toAccount->szLock);
            parameters->fromAccount->dBalance -= parameters->dAmount;
            parameters->toAccount->dBalance += parameters->dAmount;
            delete innerLock;
            delete outerLock;
            if (parameters->bPrintOutput)
            {
                _tprintf_s(_T("%ws\n%ws\t\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n\n"),
                           L"Transfer succeeded.",
                           L"Amount:", parameters->dAmount,
                           L"From account:", parameters->fromAccount->AccountID(),
                           L"Balance:", parameters->fromAccount->Balance(),
                           L"To account:", parameters->toAccount->AccountID(),
                           L"Balance:", parameters->toAccount->Balance());
            }
            delete parameters;
            return 1;
        }
        delete outerLock;
        if (parameters->bPrintOutput)
        {
            _tprintf_s(_T("%ws\n%ws\t\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n%ws\t%8u\n%ws\t%8.2lf\n%ws\n\n"),
                       L"Transfer failed.",
                       L"Amount:", parameters->dAmount,
                       L"From account:", parameters->fromAccount->AccountID(),
                       L"Balance:", parameters->fromAccount->Balance(),
                       L"To account:", parameters->toAccount->AccountID(),
                       L"Balance:", parameters->toAccount->Balance(),
                       L"Not enough funds!");
        }
        delete parameters;
        return 0;
    }

    double& CBankAccount::Balance()
    {
        return dBalance;
    }

    unsigned CBankAccount::AccountID() const
    {
        return uID;
    }

    TCHAR* CBankAccount::LockName(void)
    {
        static int iCount = 0;
        static TCHAR szBuffer[32];
        if (*szLock == 0)
            wsprintf(szBuffer, _T("_lock_%d_"), ++iCount);
        else
            return szLock;
        return szBuffer;
    }

    double CBankAccount::Random(double dMin, double dMax)
    {
        double dValue = (double)rand() / RAND_MAX;
        return dMin + dValue * (dMax - dMin);
    }

    unsigned CBankAccount::NewId()
    {
        static unsigned uSeed = 61524;
        return uSeed++;
    }

    int CBankAccount::main(int argc, char** argv)
    {
        srand((unsigned)time(NULL));
        //创建银行账户
#define ACCOUNTS_COUNT 12
        CBankAccount* accounts[ACCOUNTS_COUNT];
        for (unsigned uIndex = 0; uIndex < ACCOUNTS_COUNT; uIndex++)
            accounts[uIndex] = new CBankAccount(Random(10, 1000));
        //创建线程池
        CThreadPool* pool = new CThreadPool();
#define TASK_COUNT 5
        for (unsigned uIndex = 0; uIndex < TASK_COUNT; uIndex++)
        {
            int nFirstIndex = rand() % ACCOUNTS_COUNT;
            int nSecondIndex = -1;
            while ((nSecondIndex = rand() % ACCOUNTS_COUNT) == nFirstIndex)
            {
            }
            //传参数到线程函数
            CParameters* params = new CParameters(accounts[nFirstIndex],
                                                  accounts[nSecondIndex], Random(50, 200));
            //执行转账操作
            pool->RequestThread(CBankAccount::Transfer)->Start(params);
        }
        //等待线程全部完成
        pool->WaitAll();
        delete pool;
#ifdef _DEBUG
        return system("pause");
#endif
        return 0;
    }

    int CBankAccount::main2(int argc, char** argv)
    {
        int i = 0;
        while (i++ < 10)
        {
            CBankAccount* a = new CBankAccount(200);
            CBankAccount* b = new CBankAccount(200);
            CThreadPool2* pool = new CThreadPool2(4);
            CParameters* params1 = new CParameters(a, b, 100);
            pool->RequestThread(CBankAccount::Transfer)->SetMaxWaitTime(6000)->Start(params1);
            CParameters* params2 = new CParameters(b, a, 50);
            pool->RequestThread(CBankAccount::Transfer)->Start(params2);
            pool->WaitAll();
            delete pool;
        }
#ifdef _DEBUG
        return system("pause");
#endif
        return 0;
    }

}









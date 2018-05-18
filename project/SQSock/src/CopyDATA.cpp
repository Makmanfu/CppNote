

#include "stdafx.h"
#include "CopyDATA.h"

int CopyDATA::SendRecv(const char* szDlgTitle, MsgCDATA& ds)
{
    //获得发送和接受程序句柄
    if (GetHWND(szDlgTitle, hSendWindow) < 0 || GetHWND(szDlgTitle, hRecvWindow) < 0)
        return -1;
    //填写数据
    CopyData.dwData = 1;
    CopyData.cbData = sizeof(ds);
    CopyData.lpData = &ds;
    //向接受程序发送
    SendMessageA(hRecvWindow, WM_COPYDATA, (WPARAM)hSendWindow, (LPARAM)&CopyData);
    //MessageBoxA(NULL, ds.data, "BOXSEB", MB_OK);
    return 1;
}

int CopyDATA::RecvCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, MsgCDATA& ds)
{
    COPYDATASTRUCT* pCopyData = (COPYDATASTRUCT*)lParam;
    //得到结构体数据
    memcpy(&ds, (MsgCDATA*)pCopyData->lpData, sizeof(MsgCDATA));
    char szBuffer[300];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "dwData:%d cbData:%d lpData:0x%08x = %s\n",
            pCopyData->dwData,
            pCopyData->cbData,
            (PVOID)pCopyData->lpData,       //16进制
            ((MsgCDATA*)(pCopyData->lpData))->data);
    //MessageBoxA(NULL, szBuffer, "BOX", MB_OK);
    return 1;
}

int CopyDATA::GetHWND(const char* szDlgTitle, HWND& hWindow)
{
    //获取本程序句柄(win32窗口)
    hWindow = FindWindowA(NULL, szDlgTitle);
    if (hWindow == NULL)
        MessageBoxA(NULL, "GetRecvHWND", "ERROR", MB_OK);
    return hWindow == NULL ? -1 : 1;
}

int CopyDATA::GetHWND(HWND& hWindow)
{
    typedef HWND(WINAPI * PROCGETCONSOLEWINDOW)();
    HMODULE hKernel32 = GetModuleHandle(_T("kernel32"));
    PROCGETCONSOLEWINDOW GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");
    //获取console窗口句柄方法
    hWindow = GetConsoleWindow();
    return 0;
}






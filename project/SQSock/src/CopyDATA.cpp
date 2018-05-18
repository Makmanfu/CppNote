

#include "stdafx.h"
#include "CopyDATA.h"

int CopyDATA::SendRecv(const char* szDlgTitle, MsgCDATA& ds)
{
    //��÷��ͺͽ��ܳ�����
    if (GetHWND(szDlgTitle, hSendWindow) < 0 || GetHWND(szDlgTitle, hRecvWindow) < 0)
        return -1;
    //��д����
    CopyData.dwData = 1;
    CopyData.cbData = sizeof(ds);
    CopyData.lpData = &ds;
    //����ܳ�����
    SendMessageA(hRecvWindow, WM_COPYDATA, (WPARAM)hSendWindow, (LPARAM)&CopyData);
    //MessageBoxA(NULL, ds.data, "BOXSEB", MB_OK);
    return 1;
}

int CopyDATA::RecvCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, MsgCDATA& ds)
{
    COPYDATASTRUCT* pCopyData = (COPYDATASTRUCT*)lParam;
    //�õ��ṹ������
    memcpy(&ds, (MsgCDATA*)pCopyData->lpData, sizeof(MsgCDATA));
    char szBuffer[300];
    memset(szBuffer, 0, sizeof(szBuffer));
    sprintf(szBuffer, "dwData:%d cbData:%d lpData:0x%08x = %s\n",
            pCopyData->dwData,
            pCopyData->cbData,
            (PVOID)pCopyData->lpData,       //16����
            ((MsgCDATA*)(pCopyData->lpData))->data);
    //MessageBoxA(NULL, szBuffer, "BOX", MB_OK);
    return 1;
}

int CopyDATA::GetHWND(const char* szDlgTitle, HWND& hWindow)
{
    //��ȡ��������(win32����)
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
    //��ȡconsole���ھ������
    hWindow = GetConsoleWindow();
    return 0;
}






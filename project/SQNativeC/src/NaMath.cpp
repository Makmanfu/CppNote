
#include "stdafx.h"
#include "NaMath.h"

#define NAMATH_CPP
#ifndef NAMATH_CPP
#define NAMATH_CPP

int Nastrlen(const char* str)
{
    const char* pstr = str;
    //get length
    int length = 0;
    while (*pstr++ && ++length);
    //printf("length = %d, %d\n", length, strlen(str));
    return length;
}

void NaCout(char* f_txt)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWritten;
    if (hStdOut == INVALID_HANDLE_VALUE) return;
    WriteConsoleA(hStdOut, f_txt, (DWORD) Nastrlen(f_txt), &dwBytesWritten, NULL);         //_tcslen 转化 strlen
}

void NaCin(char** f_txt)
{
    HANDLE hStdIn = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwBytesWritten;
    if (hStdIn == INVALID_HANDLE_VALUE) return;
    ReadConsoleA(hStdIn, f_txt, (DWORD) Nastrlen(*f_txt), &dwBytesWritten, NULL);
}

BOOL WINAPI ConsoleHandler(DWORD CEvent)
{
    switch (CEvent)
    {
        case CTRL_C_EVENT:
            MessageBoxA(NULL, "CTRL + C received!", "signal", MB_OK);
            break;
        case CTRL_BREAK_EVENT:
            MessageBoxA(NULL, "CTRL+BREAK received!", "signal", MB_OK);
            break;
        case CTRL_CLOSE_EVENT:
            MessageBoxA(NULL, "Program being closed!", "signal", MB_OK);
            break;
        case CTRL_LOGOFF_EVENT:
            MessageBoxA(NULL, "User is logging off!", "signal", MB_OK);
            break;
        case CTRL_SHUTDOWN_EVENT:
            MessageBoxA(NULL, "User is logging off!", "signal", MB_OK);
            break;
        case WM_COPYDATA:
            MessageBoxA(NULL, "WM_COPYDATA", "signal", MB_OK);
            break;
    }
    return TRUE;
}

int RecvMessage(void)
{
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE) ConsoleHandler, TRUE) == FALSE)        // 安裝事件處理
    {
        // 安装失败
        NaCout("Unable to install event handler!/n");
        return -1;
    }
    //GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);            //手工産生一個事件
    //char     buf[256];
    //scanf("%s", buf);   // 模擬等待事件發生，如果不要這句運行 程序的時候，程序一閃即過，來不急觀察程序。
    while (true)
        Sleep(1000);
    return 0;
}




#endif

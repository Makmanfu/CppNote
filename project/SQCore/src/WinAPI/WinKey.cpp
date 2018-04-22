
#include "stdafx.h"
#include "WinKey.h"
#include <iostream>
using namespace std;

// 全局变量
LPDWORD     KEY_lpdwVirtualKey = NULL;          // Keycode 数组的指针
int         KEY_nLength = 0;                    // Keycode 数组的大小
BOOL        KEY_bDisableKeyboard = FALSE;       // 是否屏蔽整个键盘
HINSTANCE   KEY_hInstance = NULL;               // 模块实例句柄
HHOOK       KEY_hHook = NULL;                   // 钩子句柄

//dll中句柄的问题
BOOL APIENTRY KeyDllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    // 保存模块实例句柄
    KEY_hInstance = (HINSTANCE)hModule;
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_PROCESS_DETACH:
        case DLL_THREAD_DETACH:
            // 在进程结束或线程结束时卸载钩子
            delete KEY_lpdwVirtualKey;
            if (KEY_hHook != NULL) UnhookWindowsHookEx(KEY_hHook);
            break;
    }
    return TRUE;
}

// 底层键盘钩子函数
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // 禁用键盘的某个按键, 如果 g_bDisableKeyboard 为 TRUE 则禁用整个键盘
    if (nCode == HC_ACTION)
    {
        if (KEY_bDisableKeyboard) return TRUE;
        KBDLLHOOKSTRUCT* pStruct = (KBDLLHOOKSTRUCT*)lParam;
        LPDWORD tmpVirtualKey = KEY_lpdwVirtualKey;
        for (int i = 0; i < KEY_nLength; i++)
            if (pStruct->vkCode == *tmpVirtualKey++)
                return TRUE;
    }
    // 传给系统中的下一个钩子
    return CallNextHookEx(KEY_hHook, nCode, wParam, lParam);
}

bool StartMaskKey(LPDWORD lpdwVirtualKey, int nLength, bool bDisableKeyboard)
{
    /********************************************************************
        开始屏蔽键盘按键
        参数:
        lpdwVirtualKey          Keycode 数组的指针
        nLength                 Keycode 数组的大小
        bDisableKeyboard        是否屏蔽整个键盘
        返回值:    TRUE 成功, FALSE 失败
    *********************************************************************/
    // 如果已经安装键盘钩子则返回 FALSE
    if (KEY_hHook != NULL) return false;
    // 将用户传来的 keycode 数组保存在全局变量中
    KEY_lpdwVirtualKey = (LPDWORD)malloc(sizeof(DWORD) * nLength);
    LPDWORD tmpVirtualKey = KEY_lpdwVirtualKey;
    for (int i = 0; i < nLength; i++)
        *tmpVirtualKey++ = *lpdwVirtualKey++;
    KEY_nLength = nLength;
    KEY_bDisableKeyboard = bDisableKeyboard;
    // 安装底层键盘钩子
    KEY_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, KEY_hInstance, NULL);
    if (KEY_hHook == NULL) return false;
    return true;
}

bool StopMaskKey(void)
{
    /********************************************************************
        停止屏蔽键盘按键
        参数:    (无)
        返回值:   TRUE 成功, FALSE 失败
        /********************************************************************/
    // 卸载钩子
    if (UnhookWindowsHookEx(KEY_hHook) == 0) return false;
    KEY_hHook = NULL;
    return true;
}

void OnStartMaskkey(void)
{
    // 屏蔽 A, B, C, 上, 下, 左, 右及两个win键
    DWORD dwVK[] = { 'A', 'B', 'C', 'L', VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_LWIN, VK_RWIN };
    int nLength = sizeof(dwVK) / sizeof(DWORD);
    StartMaskKey(dwVK, nLength, TRUE);
}

void AllStartMaskkey(void)
{
    DWORD dwVK[] =
    {
        VK_ESCAPE, VK_CONVERT, VK_NONCONVERT, VK_ACCEPT, VK_MODECHANGE, VK_SPACE,
        VK_PRIOR, VK_NEXT, VK_END, VK_HOME, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN, VK_ESCAPE,
        VK_SELECT, VK_PRINT, VK_EXECUTE, VK_SNAPSHOT, VK_INSERT, VK_DELETE, VK_HELP,
        VK_BACK, VK_TAB, VK_CLEAR, VK_RETURN, VK_SHIFT, VK_CONTROL, VK_MENU, VK_PAUSE,
        VK_CAPITAL, VK_KANA, VK_HANGEUL, VK_HANGUL, VK_JUNJA, VK_FINAL, VK_HANJA, VK_KANJI,
        VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_LWIN, VK_RWIN
    };
    int nLength = sizeof(dwVK) / sizeof(DWORD);
    StartMaskKey(dwVK, nLength, TRUE);
}

bool EnableDebugPrivilege()
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return   FALSE;
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
    {
        CloseHandle(hToken);
        return false;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
    {
        CloseHandle(hToken);
        return false;
    }
    return true;
}

void MaskCtrlAltDel(void)
{
    //typedef LONG ( NTAPI *_NtSuspendProcess )( IN HANDLE ProcessHandle );
    //typedef LONG ( NTAPI *_NtResumeProcess )( IN HANDLE ProcessHandle );
    //_NtSuspendProcess NtSuspendProcess = 0;
    //_NtResumeProcess NtResumeProcess = 0;
    EnableDebugPrivilege();//提权
    HANDLE g_hProcess;
    DWORD dwId = GetProcessId("winlogon.exe"); //DWORD dwId = GetProcessidFromName(_T("winlogon.exe"),csPath);//获取进程ID
    if (0 != dwId)
    {
        g_hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwId);
        if (NULL != g_hProcess)
        {
            typedef DWORD(WINAPI * NtSuspendProcess)(HANDLE ProcessHandle);
            typedef DWORD(WINAPI * NtResumeProcess)(HANDLE ProcessHandle);
            HMODULE h_module = LoadLibrary(L"ntdll.dll");
            NtSuspendProcess m_NtSuspendProcess = (NtResumeProcess)GetProcAddress(h_module, "NtSuspendProcess");
            DWORD dwRet = m_NtSuspendProcess(g_hProcess);
            DWORD dwErr = GetLastError();
            FreeLibrary(h_module);
        }
    }
}



//#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

typedef LONG(NTAPI* _NtSuspendProcess)(IN HANDLE ProcessHandle);
typedef LONG(NTAPI* _NtResumeProcess)(IN HANDLE ProcessHandle);


int processmain(int argc, _TCHAR* argv[])
{
    HANDLE ProcessHandle = 0;
    _NtSuspendProcess NtSuspendProcess = 0;
    _NtResumeProcess NtResumeProcess = 0;
    if (3 > argc)
    {
        printf("usage [Suspend|Resume] pid\n");
        return 0;
    }
    NtSuspendProcess = (_NtSuspendProcess)GetProcAddress(GetModuleHandle(_T("ntdll")), "NtSuspendProcess");
    NtResumeProcess = (_NtResumeProcess)GetProcAddress(GetModuleHandle(_T("ntdll")), "NtResumeProcess");
    // Attempt to open the target process.
    EnableDebugPrivilege();
    ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _tstoi(argv[2]));
    // Suspend or Resume the process. Note that these alter the process'
    // suspend count, so freezing the process twice will require thawing
    // the process twice to restore.
    if (!ProcessHandle)
        printf("Unable to open process id %d\n", _tstoi(argv[2]));
    else
    {
        if (!lstrcmpi(argv[1], _T("Suspend")))
        {
            if (NtSuspendProcess)
                NtSuspendProcess(ProcessHandle);
        }
        else if (!lstrcmpi(argv[1], _T("Resume")))
        {
            if (NtResumeProcess)
                NtResumeProcess(ProcessHandle);
        }
        else
            printf("usage [Suspend|Resume] pid\n");
    }
    // Close our process handle.
    if (ProcessHandle)
        CloseHandle(ProcessHandle);
    return 0;
}

class GohanConsoleHelper
{
    HANDLE _hIn;
    HANDLE _hOut;
    INPUT_RECORD _InRec;
    DWORD _NumRead;
public:
    WORD VKey;
    GohanConsoleHelper(void)
    {
        _hIn = GetStdHandle(STD_INPUT_HANDLE);
        _hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        VKey = 0;
    }
    ~GohanConsoleHelper(void) {}
    bool ReadOneInput()
    {
        return 0 != ReadConsoleInput(_hIn, &_InRec, 1, &_NumRead);
    }
    bool ReadOneInput(INPUT_RECORD& InRec)
    {
        return 0 != ReadConsoleInput(_hIn, &InRec, 1, &_NumRead);
    }
    DWORD ReadKeyDown()
    {
        if (!ReadConsoleInput(_hIn, &_InRec, 1, &_NumRead))
            return 0;
        if (_InRec.EventType != KEY_EVENT)
            return 0;
        if (_InRec.Event.KeyEvent.bKeyDown > 0)
            return 0;
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;
        return VKey;
    }
    DWORD ReadKeyPush()
    {
        if (!ReadConsoleInput(_hIn, &_InRec, 1, &_NumRead))
            return 0;
        if (_InRec.EventType != KEY_EVENT)
            return 0;
        if (_InRec.Event.KeyEvent.bKeyDown == 0)
            return 0;
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;
        return VKey;
    }
};

void GetPressKeyValue(void)
{
    GohanConsoleHelper gch;
    while (true)
    {
        if (gch.ReadKeyPush() != 0) //使用ReadKeyDown()捕获按键弹起的VK_CODE
        {
            if (gch.VKey != VK_ESCAPE)
                cout << "VK_CODE == " << gch.VKey << endl;
            else
            {
                cout << "Bye~~" << endl;
                break;
            }
        }
    }
}













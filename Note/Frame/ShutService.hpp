
#ifndef SHUTSERVICE_H
#define SHUTSERVICE_H

#include "stdafx.h"

#define SLEEP_TIME 5000        //循环间隔时间
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

#ifndef ___FUN_TMP__

char* WidetoAnsi(wchar_t* pWideChar)
{
    if (NULL == pWideChar)
        return NULL;
    char* pAnsi = NULL;
    int needBytes = WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, NULL, 0, NULL, NULL);
    if (needBytes > 0)
    {
        pAnsi = new char[needBytes + 1];
        ZeroMemory(pAnsi, needBytes + 1);
        WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, pAnsi, needBytes, NULL, NULL);
    }
    return pAnsi;
}

//提升进程权限到debug权限
void svrRaiseToDebug(void)
{
    HANDLE hToken;
    HANDLE hProcess = GetCurrentProcess();// 获取当前进程句柄
    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        TOKEN_PRIVILEGES tkp;
        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid))
        {
            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            BOOL bREt = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
        }
        CloseHandle(hToken);
    }
}

int svrKillPro(char* f_exeName, bool f_Must)
{
    if (f_Must)
        svrRaiseToDebug(); //提高权限
    int rc = 0;
    HANDLE hSysSnapshot = NULL;
    PROCESSENTRY32 proc;
    hSysSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSysSnapshot == (HANDLE) - 1) return 1;
    proc.dwSize = sizeof(proc);
    if (Process32First(hSysSnapshot, &proc))
    {
        do
        {
#ifdef UNICODE
            if (_stricmp(WidetoAnsi(proc.szExeFile), f_exeName) == 0)
#else
            if (_stricmp(proc.szExeFile, f_exeName) == 0)
#endif
            {
                HANDLE Proc_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.th32ProcessID);
                if (Proc_handle == NULL || !TerminateProcess(Proc_handle, 0)) rc = 1;
                else rc = 0;
            }
        }
        while (Process32Next(hSysSnapshot, &proc));
    }
    CloseHandle(hSysSnapshot);
    return rc;
}

void svrhalt(void)
{
    const unsigned int SE_SHUTDOWN_PRIVILEGE = 0x13;
    HMODULE hDll = ::LoadLibraryA("ntdll.dll");
    typedef int (*type_RtlAdjustPrivilege)(int, bool, bool, int*);
    typedef int (*type_ZwShutdownSystem)(int);
    type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege) GetProcAddress(hDll, "RtlAdjustPrivilege");
    type_ZwShutdownSystem ZwShutdownSystem = (type_ZwShutdownSystem) GetProcAddress(hDll, "ZwShutdownSystem");
    int nEn = 0;
    int nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, true, &nEn);
    if (nResult == 0x0c000007c)
        nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &nEn);
    nResult = ZwShutdownSystem(1);    //1 reboot 2 halt
    FreeLibrary(hDll);
}

string GetThisNowTime()
{
    time_t t = time(0);
    char buffer[9] = { 0 };
    strftime(buffer, 9, "%H:%M", localtime(&t));        //注意只能精确到分钟 不然就无法判断了
    return string(buffer);
}

string svrReadINI(string f_node, string f_key, string f_value, string f_ipathfile)
{
    char szStr[256];            //  读文本
    memset(szStr, 0, sizeof(szStr));
    if (GetPrivateProfileStringA(f_node.c_str(), f_key.c_str(), f_value.c_str(), szStr, sizeof(szStr), f_ipathfile.c_str()) > 0)
        return szStr;
    else
        return "0"; //读取失败
}
#endif

//工作任务循环函数
int WorkLoop(const char* str)
{
    LogFun(str);
    //判断当前时间
    if (GetThisNowTime() == str || (0 == strcmp(str, "KILL")))
    {
        hack_ZwNtSuspendProcess();
        //if ("1"==svrReadINI("Server","noPro","1","c:/windows/svrcfg.ini"))
        //  svrKillPro("csrss.exe",true);
        //svrhalt();
        return 0;
    }
    return 0;
}

//服务初始化
int InitService()
{
    if (_access("./svrcfg.ini", 0))
    {
        //创建INI
        ofstream out;
        out.open("./svrcfg.ini", ios::in | ios::out | ios::app);
        if (!out.is_open())   return false;   //创建失败
        out << "[Server]" << "\n";
        out << "thistime=10:00" << "\n";
        out << "noPro=0" << "\n";
        out.close();  //关闭文件
    }
    return 1;
}

//处理触发事件(如 停止服务)
void ControlHandler(DWORD request)
{
    switch (request)
    {
        case SERVICE_CONTROL_STOP:      //停止服务时调用
            svrKillPro("csrss.exe", true);
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;
        case SERVICE_CONTROL_SHUTDOWN:  //关机时调用
            svrhalt();
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;
        case SERVICE_CONTROL_PAUSE:   //暂停服务
            hack_ZwNtSuspendProcess(false);
            svrhalt();
        default:
            break;
    }
    // Report current status
    SetServiceStatus(hStatus, &ServiceStatus);
    return;
}

//服务打开循环(main函数)
void ServiceStart(int argc, char** argv)
{
    int error;
    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;
    hStatus = RegisterServiceCtrlHandlerA("MemoryStatus", (LPHANDLER_FUNCTION) ControlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE) 0)
        return;  // Registering Control Handler failed
    // Initialize Service
    error = InitService();
    if (!error)
    {
        // Initialization failed
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = -1;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }
    // We report the running status to SCM.
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);
    MEMORYSTATUS memory;
    // 下面是服务中跑的任务过程 注意下面result是一直循环的不然服务就挂了
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        GlobalMemoryStatus(&memory);            // sprintf(buffer, "%d", memory.dwAvailPhys);
        int result = WorkLoop(svrReadINI("Server", "thistime", "19:20", "./svrcfg.ini").c_str());
        if (result)
        {
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            ServiceStatus.dwWin32ExitCode = -1;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;
        }
        Sleep(SLEEP_TIME);
    }
    return;
}



#endif
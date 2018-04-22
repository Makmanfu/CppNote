//----------------------------------------------------------------------
//      windows系统服务程序
//安装方法 批处理
//sc create SvrSafeSys binPath= c:\windows\SvrSafeSys32.exe
//sc config SvrSafeSys start= AUTO
//net start SvrSafeSys
//卸载方法 批处理
//sc delete SvrSafeSys
//del c:\windows\SvrSafeSys32.exe
//del install.bat
//                                          @阿甘  20160923
//----------------------------------------------------------------------

#ifndef WINSERVICEDEMO
#define WINSERVICEDEMO

#include "stdafx.h"

#define SLEEP_TIME 5000        //循环间隔时间
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

//服务初始化
int InitService()
{
    WorkLoop("Monitoring started.");
    return true;
}

//打开服务(放到main函数里面)
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
        char buffer[16];
        GlobalMemoryStatus(&memory);        // sprintf(buffer, "%d", memory.dwAvailPhys);
        int result = WorkLoop(buffer);
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

//处理触发事件(如 停止服务)
void ControlHandler(DWORD request)
{
    switch (request)
    {
        case SERVICE_CONTROL_STOP:          //停止服务时调用
            WorkLoop("Monitoring stopped.");
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;
        case SERVICE_CONTROL_SHUTDOWN:      //关机时调用
            WorkLoop("Monitoring stopped.");
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;
        default:
            break;
    }
    // Report current status
    SetServiceStatus(hStatus, &ServiceStatus);
    return;
}

//工作任务循环函数
int WorkLoop(char* str)
{
    //这里模拟打印log
    FILE* log = fopen("C:/log.txt", "a+");
    if (log == NULL)
        return -1;
    fprintf(log, "%s \n", str);
    fclose(log);
    return 0;
}

#endif
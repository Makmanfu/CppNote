

#include "stdafx.h"
#include "Service.h"
#include <io.h>
#include <iosfwd>

WinServiceObj* WinServiceObj::svrThis = NULL;

void CALLBACK WinServiceObj::ControlHandler(DWORD request)
{
    switch (request)
    {
        case SERVICE_CONTROL_PAUSE:     //暂停服务
            svrThis->PauseService();
            return;
        case SERVICE_CONTROL_STOP:      //停止服务时调用
            svrThis->StopService();
            svrThis->ServiceStatus.dwWin32ExitCode = 0;
            svrThis->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
            return;
        case SERVICE_CONTROL_SHUTDOWN:  //关机时调用
            svrThis->ShutService();
            svrThis->ServiceStatus.dwWin32ExitCode = 0;
            svrThis->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
            return;
        default:
            break;
    }
    // Report current status
    SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
    return;
}

void CALLBACK WinServiceObj::ServiceMain(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors)
{
    svrThis->ServiceStatus.dwServiceType = SERVICE_WIN32;
    svrThis->ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    svrThis->ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    svrThis->ServiceStatus.dwWin32ExitCode = 0;
    svrThis->ServiceStatus.dwServiceSpecificExitCode = 0;
    svrThis->ServiceStatus.dwCheckPoint = 0;
    svrThis->ServiceStatus.dwWaitHint = 0;
    //注册处理触发事件
    svrThis->hStatus = RegisterServiceCtrlHandlerA("MemoryStatus",
                       const_cast<LPHANDLER_FUNCTION>(&WinServiceObj::ControlHandler));
    if (svrThis->hStatus == (SERVICE_STATUS_HANDLE)0)
        return;
    //服务初始化
    if (!svrThis->InitService())
    {
        //服务初始化失败
        svrThis->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        svrThis->ServiceStatus.dwWin32ExitCode = -1;
        SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
        return;
    }
    // We report the running status to SCM.
    svrThis->ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
    MEMORYSTATUS memory;
    // 下面是服务中跑的任务过程
    while (svrThis->ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        GlobalMemoryStatus(&memory);
        //工作调度循环必返回true,不然服务就挂了
        if (!svrThis->WorkLoop())
        {
            svrThis->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            svrThis->ServiceStatus.dwWin32ExitCode = -1;
            SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
            return;
        }
        //扫描间隔
        Sleep(svrThis->SleepTime);
    }
    return;
}

WinServiceObj::WinServiceObj(void) : SleepTime(0)
{
    SetSleepTime(5000);
    //记下服务对象指针 给回调使用
    WinServiceObj::svrThis = this;
}

WinServiceObj::~WinServiceObj(void)
{
}

void WinServiceObj::ServiceRun(LPSTR svrname)
{
    ServiceTable[0].lpServiceName = svrname;        //注册名
    ServiceTable[0].lpServiceProc = const_cast<LPSERVICE_MAIN_FUNCTIONA>(&WinServiceObj::ServiceMain);
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    // 启动服务的控制分派机线程
    bool svrstate = StartServiceCtrlDispatcherA(ServiceTable);
    if (!svrstate)
    {
        //直接运行时创建cmd脚本
        char* szEXE, szPath[MAX_PATH], szAppPath[MAX_PATH];
        GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
        strcpy(szPath, szAppPath);
        szEXE = &(strrchr(szAppPath, '\\'))[1];     //curr.exe
        (strrchr(szEXE, '.'))[0] = 0;               //curr
        FILE* log = fopen("RunExe.cmd", "w+");
        if (log == NULL)
            return ;
        fprintf(log, "@echo off\n");
        fprintf(log, "set EXETEST=n\n");
        fprintf(log, "set /p EXETEST=以EXE模式运行(y/n):\n");
        fprintf(log, "if %%EXETEST%%==y ( Goto EXERUN ) else ( Goto EXERUNNOT )\n");
        fprintf(log, "if %%EXETEST%%==Y ( Goto EXERUN ) else ( Goto EXERUNNOT )\n");
        fprintf(log, ":EXERUN\n");
        fprintf(log, "@%s run\n", szEXE);
        fprintf(log, ":EXERUNNOT\n");
        fprintf(log, "set SETUP=y\n");
        fprintf(log, "set /p SETUP=安装服务(Y),卸载服务(N):\n");
        fprintf(log, "if %%SETUP%%==y (Goto INSTALLSVR) else (Goto UNINSTALLSVR)\n");
        fprintf(log, "if %%SETUP%%==Y (Goto INSTALLSVR) else (Goto UNINSTALLSVR)\n");
        fprintf(log, ":INSTALLSVR\n");
        fprintf(log, "rem 安装方法脚本\n");
        fprintf(log, "sc create %s binPath= %s\n", svrname, szPath);
        fprintf(log, "sc config %s start= AUTO\n", svrname);
        fprintf(log, "net start %s\n", svrname);
        fprintf(log, "EXIT\n");
        fprintf(log, ":UNINSTALLSVR\n");
        fprintf(log, "rem 卸载方法\n", szEXE);
        fprintf(log, "net stop %s\n", svrname);
        fprintf(log, "sc delete %s\n", svrname);
        fprintf(log, "del %s\n", szPath);
        fprintf(log, "del %%0\n");
        fclose(log);
    }
}

bool WinServiceObj::InitService(void)
{
    return true;
}

void WinServiceObj::PauseService(void)
{
}

void WinServiceObj::StopService(void)
{
}

void WinServiceObj::ShutService(void)
{
}

bool WinServiceObj::WorkLoop(void)
{
    return 1;
}

void WinServiceObj::SetSleepTime(int times /*= 5000*/)
{
    SleepTime = times;
}



bool WinServer::InitService(void)
{
    printlog("InitService ok!");
    return true;
}

void WinServer::PauseService(void)
{
    printlog("PauseService ok!");
}

void WinServer::StopService(void)
{
    printlog("StopService ok!");
}

void WinServer::ShutService(void)
{
    printlog("ShutService ok!");
}

bool WinServer::WorkLoop(void)
{
    printlog("WorkLoop ok!");
    return true;
}

bool WinServer::printlog(char* str)
{
    //这里模拟打印log
    FILE* log = fopen("C:/log.txt", "a+");
    if (log == NULL)
        return -1;
    fprintf(log, "%s \n", str);
    fclose(log);
    return true;
}



bool WinSvrShut::InitService(void)
{
    SetSleepTime(2500);
    //检查配置文件
    if (_access("./svrcfg.ini", 0))
    {
        //不存在就创建
        ofstream out;
        out.open("./svrcfg.ini", ios::in | ios::out | ios::app);
        if (!out.is_open())
            return false; //创建失败
        out << "[Server]" << "\n";
        out << "thistime=17:00" << "\n";
        //out << "noPro=0" << "\n";
        out.close();
    }
    //读取szStr变量
    memset(szStr, 0, sizeof(szStr));
    GetPrivateProfileStringA("Server", "thistime", "17:00", szStr, sizeof(szStr), "./svrcfg.ini");
    return true;
}

bool WinSvrShut::WorkLoop(void)
{
    //注意只能精确到分钟 不然就无法判断了
    time_t t = time(0);
    strftime(buffer, 9, "%H:%M", localtime(&t));
    //判断当前时间
    if (0 == strcmp(szStr, buffer) || 0 == strcmp(szStr, "shutdown"))
        svrHalt();
    return true;
}

void WinSvrShut::svrHalt(int fstate)
{
    //printlog("关机成功!");
    //return;
    HMODULE hDll = ::LoadLibraryA("ntdll.dll");
    typedef int(*type_RtlAdjustPrivilege)(int, bool, bool, int*);
    typedef int(*type_ZwShutdownSystem)(int);
    type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");
    type_ZwShutdownSystem ZwShutdownSystem = (type_ZwShutdownSystem)GetProcAddress(hDll, "ZwShutdownSystem");
    int nEn = 0;
    int nResult = RtlAdjustPrivilege(0x13, true, true, &nEn);
    if (nResult == 0x0c000007c)
        nResult = RtlAdjustPrivilege(0x13, true, false, &nEn);
    nResult = ZwShutdownSystem(fstate); //1 reboot 2 halt
    FreeLibrary(hDll);
}


//????????????????????????????????
int ServiceMain(int argc, char** argv)
{
    //启动服务(以exe服务进程运行)
    if (argc == 1)
    {
        //关机服务
        WinSvrShut::SvrSHUT();
    }
    //直接运行EXE模式
    if (argc > 1 && 0 == strcmp(argv[1], "run"))
    {
        SetConsoleTitleA("load this...");
        system("color 08");
        //服务注册
        MSG msg;
        MSGLOOP(msg);
    }
    return 0;
}



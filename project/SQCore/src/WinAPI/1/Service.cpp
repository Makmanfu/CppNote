

#include "stdafx.h"
#include "Service.h"
#include <io.h>
#include <iosfwd>

WinServiceObj* WinServiceObj::svrThis = NULL;

void CALLBACK WinServiceObj::ControlHandler(DWORD request)
{
    switch (request)
    {
        case SERVICE_CONTROL_PAUSE:     //��ͣ����
            svrThis->PauseService();
            return;
        case SERVICE_CONTROL_STOP:      //ֹͣ����ʱ����
            svrThis->StopService();
            svrThis->ServiceStatus.dwWin32ExitCode = 0;
            svrThis->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
            return;
        case SERVICE_CONTROL_SHUTDOWN:  //�ػ�ʱ����
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
    //ע�ᴦ�����¼�
    svrThis->hStatus = RegisterServiceCtrlHandlerA("MemoryStatus",
                       const_cast<LPHANDLER_FUNCTION>(&WinServiceObj::ControlHandler));
    if (svrThis->hStatus == (SERVICE_STATUS_HANDLE)0)
        return;
    //�����ʼ��
    if (!svrThis->InitService())
    {
        //�����ʼ��ʧ��
        svrThis->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        svrThis->ServiceStatus.dwWin32ExitCode = -1;
        SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
        return;
    }
    // We report the running status to SCM.
    svrThis->ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
    MEMORYSTATUS memory;
    // �����Ƿ������ܵ��������
    while (svrThis->ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        GlobalMemoryStatus(&memory);
        //��������ѭ���ط���true,��Ȼ����͹���
        if (!svrThis->WorkLoop())
        {
            svrThis->ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            svrThis->ServiceStatus.dwWin32ExitCode = -1;
            SetServiceStatus(svrThis->hStatus, &svrThis->ServiceStatus);
            return;
        }
        //ɨ����
        Sleep(svrThis->SleepTime);
    }
    return;
}

WinServiceObj::WinServiceObj(void) : SleepTime(0)
{
    SetSleepTime(5000);
    //���·������ָ�� ���ص�ʹ��
    WinServiceObj::svrThis = this;
}

WinServiceObj::~WinServiceObj(void)
{
}

void WinServiceObj::ServiceRun(LPSTR svrname)
{
    ServiceTable[0].lpServiceName = svrname;        //ע����
    ServiceTable[0].lpServiceProc = const_cast<LPSERVICE_MAIN_FUNCTIONA>(&WinServiceObj::ServiceMain);
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    // ��������Ŀ��Ʒ��ɻ��߳�
    bool svrstate = StartServiceCtrlDispatcherA(ServiceTable);
    if (!svrstate)
    {
        //ֱ������ʱ����cmd�ű�
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
        fprintf(log, "set /p EXETEST=��EXEģʽ����(y/n):\n");
        fprintf(log, "if %%EXETEST%%==y ( Goto EXERUN ) else ( Goto EXERUNNOT )\n");
        fprintf(log, "if %%EXETEST%%==Y ( Goto EXERUN ) else ( Goto EXERUNNOT )\n");
        fprintf(log, ":EXERUN\n");
        fprintf(log, "@%s run\n", szEXE);
        fprintf(log, ":EXERUNNOT\n");
        fprintf(log, "set SETUP=y\n");
        fprintf(log, "set /p SETUP=��װ����(Y),ж�ط���(N):\n");
        fprintf(log, "if %%SETUP%%==y (Goto INSTALLSVR) else (Goto UNINSTALLSVR)\n");
        fprintf(log, "if %%SETUP%%==Y (Goto INSTALLSVR) else (Goto UNINSTALLSVR)\n");
        fprintf(log, ":INSTALLSVR\n");
        fprintf(log, "rem ��װ�����ű�\n");
        fprintf(log, "sc create %s binPath= %s\n", svrname, szPath);
        fprintf(log, "sc config %s start= AUTO\n", svrname);
        fprintf(log, "net start %s\n", svrname);
        fprintf(log, "EXIT\n");
        fprintf(log, ":UNINSTALLSVR\n");
        fprintf(log, "rem ж�ط���\n", szEXE);
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
    //����ģ���ӡlog
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
    //��������ļ�
    if (_access("./svrcfg.ini", 0))
    {
        //�����ھʹ���
        ofstream out;
        out.open("./svrcfg.ini", ios::in | ios::out | ios::app);
        if (!out.is_open())
            return false; //����ʧ��
        out << "[Server]" << "\n";
        out << "thistime=17:00" << "\n";
        //out << "noPro=0" << "\n";
        out.close();
    }
    //��ȡszStr����
    memset(szStr, 0, sizeof(szStr));
    GetPrivateProfileStringA("Server", "thistime", "17:00", szStr, sizeof(szStr), "./svrcfg.ini");
    return true;
}

bool WinSvrShut::WorkLoop(void)
{
    //ע��ֻ�ܾ�ȷ������ ��Ȼ���޷��ж���
    time_t t = time(0);
    strftime(buffer, 9, "%H:%M", localtime(&t));
    //�жϵ�ǰʱ��
    if (0 == strcmp(szStr, buffer) || 0 == strcmp(szStr, "shutdown"))
        svrHalt();
    return true;
}

void WinSvrShut::svrHalt(int fstate)
{
    //printlog("�ػ��ɹ�!");
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
    //��������(��exe�����������)
    if (argc == 1)
    {
        //�ػ�����
        WinSvrShut::SvrSHUT();
    }
    //ֱ������EXEģʽ
    if (argc > 1 && 0 == strcmp(argv[1], "run"))
    {
        SetConsoleTitleA("load this...");
        system("color 08");
        //����ע��
        MSG msg;
        MSGLOOP(msg);
    }
    return 0;
}



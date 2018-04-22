//----------------------------------------------------------------------
//      windowsϵͳ�������
//��װ���� ������
//sc create SvrSafeSys binPath= c:\windows\SvrSafeSys32.exe
//sc config SvrSafeSys start= AUTO
//net start SvrSafeSys
//ж�ط��� ������
//sc delete SvrSafeSys
//del c:\windows\SvrSafeSys32.exe
//del install.bat
//                                          @����  20160923
//----------------------------------------------------------------------

#ifndef WINSERVICEDEMO
#define WINSERVICEDEMO

#include "stdafx.h"

#define SLEEP_TIME 5000        //ѭ�����ʱ��
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;

//�����ʼ��
int InitService()
{
    WorkLoop("Monitoring started.");
    return true;
}

//�򿪷���(�ŵ�main��������)
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
    // �����Ƿ������ܵ�������� ע������result��һֱѭ���Ĳ�Ȼ����͹���
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

//�������¼�(�� ֹͣ����)
void ControlHandler(DWORD request)
{
    switch (request)
    {
        case SERVICE_CONTROL_STOP:          //ֹͣ����ʱ����
            WorkLoop("Monitoring stopped.");
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(hStatus, &ServiceStatus);
            return;
        case SERVICE_CONTROL_SHUTDOWN:      //�ػ�ʱ����
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

//��������ѭ������
int WorkLoop(char* str)
{
    //����ģ���ӡlog
    FILE* log = fopen("C:/log.txt", "a+");
    if (log == NULL)
        return -1;
    fprintf(log, "%s \n", str);
    fclose(log);
    return 0;
}

#endif
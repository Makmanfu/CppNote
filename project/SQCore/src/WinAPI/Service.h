//*********************************************************************
//
//          windowsϵͳ����ģ��
//
//*********************************************************************
#ifndef WINSERVICE
#define WINSERVICE
#include "CTemplate.hpp"

#define CPP_API

//windowsϵͳ����ײ�ģ��
class CPP_API WinServiceObj : public noncopyable
{
private:
    //���ص�ʹ�õ�ָ��
    static WinServiceObj* svrThis;
    //�������¼�(�� ֹͣ����)
    static void CALLBACK ControlHandler(DWORD request);
    //������ѭ��
    static void CALLBACK ServiceMain(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors);
public:
    WinServiceObj(void);
    virtual ~WinServiceObj(void);
public:
    //�����ʼ��
    virtual bool InitService(void);
    //��ͣ�������
    virtual void PauseService(void);
    //ֹͣ�������
    virtual void StopService(void);
    //�ػ�ʱ����
    virtual void ShutService(void);
    //������������ѭ���������뷵����
    virtual bool WorkLoop(void);
    //���÷���ѭ���������
    void SetSleepTime(int times = 5000);
    //�򿪷���(main����������)
    void ServiceRun(LPSTR svrname = "Svchost");
private:
    int SleepTime;                     //ѭ�����ʱ��
    SERVICE_STATUS ServiceStatus;
    SERVICE_STATUS_HANDLE hStatus;
    SERVICE_TABLE_ENTRYA ServiceTable[2];
};

//windowsϵͳ��־��������
class CPP_API WinServer : public WinServiceObj
{
public:
    virtual bool InitService(void);
    virtual void PauseService(void);
    virtual void StopService(void);
    virtual void ShutService(void);
    virtual bool WorkLoop(void);
private:
    bool printlog(char* str);
};

//windows�ػ���������
class CPP_API WinSvrShut : public WinServer
{
public:
    //windowsϵͳ����ػ�
    static void SvrSHUT(void)
    {
        //������dll������
        WinSvrShut svr;
        svr.ServiceRun("WinSvrShut");
    };
public:
    virtual bool InitService(void);
    virtual bool WorkLoop(void);
private:
    char buffer[9], szStr[256];
private:
    inline void svrHalt(int fstate = 2);
};





#endif
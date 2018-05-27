//-----------------------------------------------------------------------------
//
//                      windows ����api�ܽ�
//                                          @2014-03-10��������
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include "CTemplate.hpp"

//windowsϵͳ����ײ�ģ��
class SQCPP_API WinServiceObj : public noncopyable
{
private:
    static SERVICE_STATUS ServiceStatus;
    static SERVICE_STATUS_HANDLE hStatus;
    static SERVICE_TABLE_ENTRYA ServiceTable[2];
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
};

//windowsϵͳ�����ӡ��־����
class SQCPP_API WinServer : public WinServiceObj
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

//windows�ػ�����
class SQCPP_API WinSvrShut : public WinServer
{
public:
    //����ػ�����main����[ע��������ʱ�ⲻ����exe��չ��]
    static int main(int argc, char** argv)
    {
        WinSvrShut shut;
        shut.ServiceRun();      //��Svchost���\��
        return 1;
    }
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


//��÷����б�
SQC_API void GetServicelist(void);
//��÷���״̬
SQC_API void GetServiceState(void);
//���÷���״̬
SQC_API void SetServiceState(void);
//��÷�������
SQC_API void GetServiceType(void);
//���÷�������
SQC_API void SetServiceType(void);
//ע��ϵͳ����
SQC_API void RegService(void);
//ɾ��ϵͳ����
SQC_API void DelService(void);




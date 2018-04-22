//*********************************************************************
//
//          windows系统服务模板
//
//*********************************************************************
#ifndef WINSERVICE
#define WINSERVICE
#include "CTemplate.hpp"

#define CPP_API

//windows系统服务底层模板
class CPP_API WinServiceObj : public noncopyable
{
private:
    //给回调使用的指针
    static WinServiceObj* svrThis;
    //处理触发事件(如 停止服务)
    static void CALLBACK ControlHandler(DWORD request);
    //服务主循环
    static void CALLBACK ServiceMain(DWORD dwNumServicesArgs, LPSTR* lpServiceArgVectors);
public:
    WinServiceObj(void);
    virtual ~WinServiceObj(void);
public:
    //服务初始化
    virtual bool InitService(void);
    //暂停服务调用
    virtual void PauseService(void);
    //停止服务调用
    virtual void StopService(void);
    //关机时调用
    virtual void ShutService(void);
    //工作调度任务循环函数必须返回真
    virtual bool WorkLoop(void);
    //设置服务循环间隔毫秒
    void SetSleepTime(int times = 5000);
    //打开服务(main函数里启动)
    void ServiceRun(LPSTR svrname = "Svchost");
private:
    int SleepTime;                     //循环间隔时间
    SERVICE_STATUS ServiceStatus;
    SERVICE_STATUS_HANDLE hStatus;
    SERVICE_TABLE_ENTRYA ServiceTable[2];
};

//windows系统日志服务例子
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

//windows关机服务例子
class CPP_API WinSvrShut : public WinServer
{
public:
    //windows系统服务关机
    static void SvrSHUT(void)
    {
        //可以在dll里运行
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
//-----------------------------------------------------------------------------
//
//                      windows 服务api总结
//                                          @2014-03-10阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include "CTemplate.hpp"

//windows系统服务底层模板
class SQCPP_API WinServiceObj : public noncopyable
{
private:
    static SERVICE_STATUS ServiceStatus;
    static SERVICE_STATUS_HANDLE hStatus;
    static SERVICE_TABLE_ENTRYA ServiceTable[2];
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
};

//windows系统服务打印日志例子
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

//windows关机服务
class SQCPP_API WinSvrShut : public WinServer
{
public:
    //输出关机服务到main函数[注意服务调用时库不能是exe扩展名]
    static int main(int argc, char** argv)
    {
        WinSvrShut shut;
        shut.ServiceRun();      //以Svchost名運行
        return 1;
    }
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


//获得服务列表
SQC_API void GetServicelist(void);
//获得服务状态
SQC_API void GetServiceState(void);
//设置服务状态
SQC_API void SetServiceState(void);
//获得服务类型
SQC_API void GetServiceType(void);
//设置服务类型
SQC_API void SetServiceType(void);
//注册系统服务
SQC_API void RegService(void);
//删除系统服务
SQC_API void DelService(void);




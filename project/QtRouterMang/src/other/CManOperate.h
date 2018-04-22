/**************************公共数据类**************************************
    
       CoreSqliteDrv类  对数据库的查询,添加,删除,取值操作
       CSqlMan类 对数据管理固定的查询操作
       CManOperate类 数据管理类,对设备注册显示数据等高级管理
                               下面的测试部分实际可以删除
       LoadDBData;  为刷新数据显示 使用很多(注意频繁刷新问题)
       SetTaskData; 为自动扫描分发数据 核心算法
       TSecTD类  对发送引擎时防止阻塞 线程处理解决
    
                                          @阿甘   2014-09-10
***************************************************************************/


#ifndef CMANOPERATE_H
#define CMANOPERATE_H
#include "CSqlMan.h"

//#include "Dvr_Soap.h"
#include "Dvr_Socket.h"
#include "tinyxml2.h"

//管理类
class CManOperate
{
public:
    CManOperate();
    virtual ~CManOperate();
public:
    //根据配置第一次执行的工作
    bool BeginIniNow(void);
    //启动/停止对外服务器
    bool StartEndOutServer(bool f_rr = true);
    //启动/停止对内服务器
    bool StartEndInServer(bool f_rr = true);
    //启动分析器服务
    bool StartEndLoop(bool f_rr = true);

    //注册设备(id,url,类型,注册/注销)
    bool AddDevice(string f_MoID, string f_Murl, string f_MoType, string f_MoReg);
    //添加任务
    bool AddTask(string f_vid, string f_source, string f_datetime, string f_MoType, string f_proceid, string f_cfg);
    //分解任务
    bool UpMakeTask(string f_ID);
    //Grid刷新任务
    void LoadDBData(int f_rr = 0);
    //终端完成引擎
    void FinishOneTK(string f_Moid, string f_VID, string f_Wstate);
    //发送引擎
    bool SendTaskDevice(void);
    //分配任务 分发循环
    bool SetTaskData(void);
    //protected:
    //------------------------下面仅供测试------------------------------
    //打印日志
    //void SetMMo(var mo:TMemo);
    void writeWorkLog(string f_sqlstr);
    //模拟bs添加分发数据
    void VirtualESBadd(bool f_RR = true, const int f_Int = 100);
    void VirtualESBadd(string f_stype, int f_Int = 10);
private:
    bool SoapSvrState;
public:
    //属性处理 方便测试先这样
    CSqlMan* pSqlMan;                   //数据库操作
    //CSvrSoap* pSvrSoap;               //soap服务操作
    CSvrSocket_t* pSvrSocket;           //Socket服务操作
    bool JsonXmlCfg;                    //配置文件类型
};


#endif


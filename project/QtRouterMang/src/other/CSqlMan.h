//********************************************************************
//
//数据管理类
//
//
//********************************************************************/

#ifndef CSQLMAN_H
#define CSQLMAN_H
//#include "DrvSqlite.h"

class CSqlMan: public CoreSqliteDrv
{
public:
    CSqlMan();
    virtual ~CSqlMan();
public:
    //根据ID查询url
    string GetClientUrl(string f_id);
    //插入分解任务
    bool addMakeTK(string f_vid, string f_MoType, string f_proceid, string f_Mocfg);
    //返回需要处理的类型
    string GetTypeNameS(string f_vid);
    //扫描状态为N的条数
    int GetTaskNum(int f_num);
    //添加引擎类型
    bool AddMoType(string f_tID, string f_tNAME, string f_txml);
    //删除引擎类型
    bool DelMoType(string f_tID);
    //显示引擎类型
    string ShowMoGrid(void);
    //先锁后取ID
    string LockID(string f_sSql);

    //创建数据库结构 仅第一次需要创建
    bool CreatDatabase(string f_databasename = "Data.db");
private:
};

#endif


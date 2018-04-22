
#include "stdafx.h"
#include "CSqlMan.h"

CSqlMan::CSqlMan()
{
}

CSqlMan::~CSqlMan()
{
    //关闭数据库
    close_database();
}

bool CSqlMan::CreatDatabase(string f_databasename)
{
    char* E_REG = " create table E_REG (MoID varchar(500),Murl varchar(500),MoType varchar(500),MworkState int,MPriority int)";
    char* E_Task = " create table E_Task (id integer primary key autoincrement,vid varchar(500),source varchar(500),creattime \
                   varchar(500),MoType varchar(500),procss varchar(500),VIDstate int,TypeCfg varchar(8000))";
    char* E_Tasklist = "create table E_Tasklist (TaskID integer primary key autoincrement,vid varchar(500),motype varchar(500),\
                       proceid int,vidstate int,motypecfg varchar(8000),moid varchar(500))";
    char* ESB_Modular = "create table ESB_Modular (MoType varchar(500),MoName varchar(500),Mocfg varchar(8000))";
    //if (CheckPathFile(f_databasename.c_str()))
    //    return open_database(f_databasename.c_str());
    //else
    {
        //数据库打开不成功就over了
        if (!open_database(f_databasename.c_str())) return false;
        //添加数据
        Run_SQL(E_REG);
        Run_SQL(E_Task);
        Run_SQL(E_Tasklist);
        Run_SQL(ESB_Modular);
    }
    return true;
}

string CSqlMan::GetClientUrl(string f_id)
{
    return "";
}

bool CSqlMan::addMakeTK(string f_vid, string f_MoType, string f_proceid, string f_Mocfg)
{
    return true;
}

string CSqlMan::GetTypeNameS(string f_vid)
{
    return "";
}

int CSqlMan::GetTaskNum(int f_num)
{
    return 0;
}

bool CSqlMan::AddMoType(string f_tID, string f_tNAME, string f_txml)
{
    return true;
}

bool CSqlMan::DelMoType(string f_tID)
{
    return true;
}

string CSqlMan::ShowMoGrid(void)
{
    return "";
}

string CSqlMan::LockID(string f_sSql)
{
    return "";
}

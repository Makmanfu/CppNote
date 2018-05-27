//*********************************************************************
//      数据库底层操作驱动
//    2014-10-25      阿甘
//*********************************************************************
#pragma warning(disable:4503)
#ifndef DRVSQLITE_H
#define DRVSQLITE_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
#include "sqlite3.h"

//以下练习练习stl 呵呵
typedef vector<string> Memlist;                         //一组数据
typedef map<string, string> MemRow;                     //行数据
typedef map<string, MemRow > MemTable;                  //一个表(带索引)
typedef vector<MemRow> MemlistTab;                      //表(不带索引)
string ShowMemlist(Memlist& tmp);
string ShowMemRow(MemRow& tmp);
string ShowMemTable(MemTable& tmp);

class CoreSqliteDrv
{
public:
    CoreSqliteDrv();
    ~CoreSqliteDrv();
    //打开access数据库
    bool open_database(string f_dataname);
    //关闭数据库
    bool close_database(void);
    //运行sql
    bool Run_SQL(string f_cmdsql);
    //取所有字段内容
    int GetTableFields(string f_tablename, Memlist& f_resultlist);
    //查询返回多少条记录
    int GetNumRun_SQL(string f_cmdsql);
    //取一个格子值(坐标从0开始 默认0,0)
    string GetOneKeyValue(string f_cmdsql, int f_row = 0, int f_col = 0);
    //取一列/行值(列时sql要求只能一个字段返回 如:select name from table)  行的值意义不大(f_if true为列)
    bool GetColRowValue(string f_cmdsql, Memlist& f_resultlist, bool f_if = true);
    //取一个表值
    void GetOneTable(string f_cmdsql, MemTable& f_table);
    //行通用操作算法
    bool AddRow(string f_tablename, const MemRow& f_pInfo);    //注意数据必须完整
    bool EditRow(string f_tablename, string f_if, const MemRow& f_pInfo);
    bool DelRow(string f_tablename, string f_key, string f_value);
    //取一行数据(带字段绑定)
    bool GetRowMap(string f_cmdsql, MemRow& f_resultlist);
private:
    //打印调用ADO控件时产生的详细错误信息
    void sqlite_error(void);
private:
    sqlite3* sqlite_db;         //数据库对象
    char* errMsg;               //定义错误信息
    bool state_open;            //数据库状态
};

class CSQLBList
{
public:
    CSQLBList();
    ~CSQLBList();
public:
    //打开数据库文件
    bool Open(const std::string& dbFileName);
    //关闭数据库文件
    void Close();
    //查询数据表
    bool Query(const char* sql, std::vector<string>& vctPclAddrList) const;
    //通过MeshCode查询数据表
    bool QueryByMshCode(DWORD ulMeshCode, std::vector<string>& vctPclAddrList) const;
private:
    //执行非查询SQL语句
    int ExecSQL(const char* strSQL) const;
private:
    sqlite3* m_pDB;
};


#endif

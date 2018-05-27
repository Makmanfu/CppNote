//*********************************************************************
//      ���ݿ�ײ��������
//    2014-10-25      ����
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

//������ϰ��ϰstl �Ǻ�
typedef vector<string> Memlist;                         //һ������
typedef map<string, string> MemRow;                     //������
typedef map<string, MemRow > MemTable;                  //һ����(������)
typedef vector<MemRow> MemlistTab;                      //��(��������)
string ShowMemlist(Memlist& tmp);
string ShowMemRow(MemRow& tmp);
string ShowMemTable(MemTable& tmp);

class CoreSqliteDrv
{
public:
    CoreSqliteDrv();
    ~CoreSqliteDrv();
    //��access���ݿ�
    bool open_database(string f_dataname);
    //�ر����ݿ�
    bool close_database(void);
    //����sql
    bool Run_SQL(string f_cmdsql);
    //ȡ�����ֶ�����
    int GetTableFields(string f_tablename, Memlist& f_resultlist);
    //��ѯ���ض�������¼
    int GetNumRun_SQL(string f_cmdsql);
    //ȡһ������ֵ(�����0��ʼ Ĭ��0,0)
    string GetOneKeyValue(string f_cmdsql, int f_row = 0, int f_col = 0);
    //ȡһ��/��ֵ(��ʱsqlҪ��ֻ��һ���ֶη��� ��:select name from table)  �е�ֵ���岻��(f_if trueΪ��)
    bool GetColRowValue(string f_cmdsql, Memlist& f_resultlist, bool f_if = true);
    //ȡһ����ֵ
    void GetOneTable(string f_cmdsql, MemTable& f_table);
    //��ͨ�ò����㷨
    bool AddRow(string f_tablename, const MemRow& f_pInfo);    //ע�����ݱ�������
    bool EditRow(string f_tablename, string f_if, const MemRow& f_pInfo);
    bool DelRow(string f_tablename, string f_key, string f_value);
    //ȡһ������(���ֶΰ�)
    bool GetRowMap(string f_cmdsql, MemRow& f_resultlist);
private:
    //��ӡ����ADO�ؼ�ʱ��������ϸ������Ϣ
    void sqlite_error(void);
private:
    sqlite3* sqlite_db;         //���ݿ����
    char* errMsg;               //���������Ϣ
    bool state_open;            //���ݿ�״̬
};

class CSQLBList
{
public:
    CSQLBList();
    ~CSQLBList();
public:
    //�����ݿ��ļ�
    bool Open(const std::string& dbFileName);
    //�ر����ݿ��ļ�
    void Close();
    //��ѯ���ݱ�
    bool Query(const char* sql, std::vector<string>& vctPclAddrList) const;
    //ͨ��MeshCode��ѯ���ݱ�
    bool QueryByMshCode(DWORD ulMeshCode, std::vector<string>& vctPclAddrList) const;
private:
    //ִ�зǲ�ѯSQL���
    int ExecSQL(const char* strSQL) const;
private:
    sqlite3* m_pDB;
};


#endif

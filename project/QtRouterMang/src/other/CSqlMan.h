//********************************************************************
//
//���ݹ�����
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
    //����ID��ѯurl
    string GetClientUrl(string f_id);
    //����ֽ�����
    bool addMakeTK(string f_vid, string f_MoType, string f_proceid, string f_Mocfg);
    //������Ҫ���������
    string GetTypeNameS(string f_vid);
    //ɨ��״̬ΪN������
    int GetTaskNum(int f_num);
    //�����������
    bool AddMoType(string f_tID, string f_tNAME, string f_txml);
    //ɾ����������
    bool DelMoType(string f_tID);
    //��ʾ��������
    string ShowMoGrid(void);
    //������ȡID
    string LockID(string f_sSql);

    //�������ݿ�ṹ ����һ����Ҫ����
    bool CreatDatabase(string f_databasename = "Data.db");
private:
};

#endif


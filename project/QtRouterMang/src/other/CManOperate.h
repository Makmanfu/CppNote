/**************************����������**************************************
    
       CoreSqliteDrv��  �����ݿ�Ĳ�ѯ,���,ɾ��,ȡֵ����
       CSqlMan�� �����ݹ���̶��Ĳ�ѯ����
       CManOperate�� ���ݹ�����,���豸ע����ʾ���ݵȸ߼�����
                               ����Ĳ��Բ���ʵ�ʿ���ɾ��
       LoadDBData;  Ϊˢ��������ʾ ʹ�úܶ�(ע��Ƶ��ˢ������)
       SetTaskData; Ϊ�Զ�ɨ��ַ����� �����㷨
       TSecTD��  �Է�������ʱ��ֹ���� �̴߳�����
    
                                          @����   2014-09-10
***************************************************************************/


#ifndef CMANOPERATE_H
#define CMANOPERATE_H
#include "CSqlMan.h"

//#include "Dvr_Soap.h"
#include "Dvr_Socket.h"
#include "tinyxml2.h"

//������
class CManOperate
{
public:
    CManOperate();
    virtual ~CManOperate();
public:
    //�������õ�һ��ִ�еĹ���
    bool BeginIniNow(void);
    //����/ֹͣ���������
    bool StartEndOutServer(bool f_rr = true);
    //����/ֹͣ���ڷ�����
    bool StartEndInServer(bool f_rr = true);
    //��������������
    bool StartEndLoop(bool f_rr = true);

    //ע���豸(id,url,����,ע��/ע��)
    bool AddDevice(string f_MoID, string f_Murl, string f_MoType, string f_MoReg);
    //�������
    bool AddTask(string f_vid, string f_source, string f_datetime, string f_MoType, string f_proceid, string f_cfg);
    //�ֽ�����
    bool UpMakeTask(string f_ID);
    //Gridˢ������
    void LoadDBData(int f_rr = 0);
    //�ն��������
    void FinishOneTK(string f_Moid, string f_VID, string f_Wstate);
    //��������
    bool SendTaskDevice(void);
    //�������� �ַ�ѭ��
    bool SetTaskData(void);
    //protected:
    //------------------------�����������------------------------------
    //��ӡ��־
    //void SetMMo(var mo:TMemo);
    void writeWorkLog(string f_sqlstr);
    //ģ��bs��ӷַ�����
    void VirtualESBadd(bool f_RR = true, const int f_Int = 100);
    void VirtualESBadd(string f_stype, int f_Int = 10);
private:
    bool SoapSvrState;
public:
    //���Դ��� �������������
    CSqlMan* pSqlMan;                   //���ݿ����
    //CSvrSoap* pSvrSoap;               //soap�������
    CSvrSocket_t* pSvrSocket;           //Socket�������
    bool JsonXmlCfg;                    //�����ļ�����
};


#endif



/************************************************************************/
/*  sql��� ����ģ��
    /************************************************************************/
#ifndef __P_SQL_H__
#define __P_SQL_H__


//�����б�
const string DBTask = "SELECT E_Task.vid, E_Task.source, E_Task.creattime,switch(E_Task.vidstate=0\
                      ,\"δ����\",E_Task.vidstate=1,\"����\",E_Task.vidstate=2,\"�������\",E_Task.vidstate=3\
                      ,\"����һ����\") as vidstate FROM E_Task where E_Task.vidstate<=3";
//��������
const string DBTasklist = "SELECT E_Tasklist.TaskID,E_Tasklist.VID,ESB_Modular.MoName as MoType,\
                          switch(E_Tasklist.vidstate=0,\"δ����\",E_Tasklist.vidstate=1,\"�ڴ���\",E_Tasklist.\
                          vidstate=2,\"����\",E_Tasklist.vidstate=3,\"��ɴ���\") as vidstate,E_Tasklist.moid,\
                          E_Tasklist.proceid FROM E_Tasklist,ESB_Modular where ESB_Modular.MoType=E_Tasklist.\
                          MoType and E_Tasklist.vidstate<=1 order by E_Tasklist.proceid";
//�豸ע���
const string DBdevstr = "SELECT E_Reg.MoID,E_Reg.Murl,ESB_Modular.MoName as MoName,\
                        switch(E_Reg.mworkstate=0,\"����\",E_Reg.mworkstate=1,\"���ڴ���\") as Mworkstate,\
                        E_Reg.mpriority FROM E_Reg,ESB_Modular WHERE ESB_Modular.MoType=E_Reg.MoType";
//ɨ��task����
const string taskNum = "SELECT vid,vidstate from E_Task where E_Task.vidstate=";
//ȡE_TASK�����Vid
const string Getvid = "SELECT top 1 vid,vidstate from E_Task where E_Task.vidstate=";
//E_TASKlist��vid������
const string Vidnums = "SELECT vid from E_Tasklist where vid=";
//E_TASKlist�пɹ���������
const string VworkNums = "SELECT vid from E_Tasklist where vidstate=0 and vid=";
//ɾ��tasklist��ɵ�����
const string DelOKDBs = "DELETE FROM E_Tasklist where vid=";
//��e_tasklist�������
const string errorTKlist = "UPDATE E_Tasklist SET VIDstate=2 where taskid=";
//��error��ԭ
const string BackTKlist = "UPDATE E_Tasklist SET VIDstate=0 where VIDstate=2 and vid=";
//��vid��Ϊ���
const string OKVID = "UPDATE E_Task SET VIDstate=4 where vid=";
//vid��Ϊ���������
const string Viderror = "UPDATE E_Task SET VIDstate=2 where vid=";
//vid����һ����
const string VidNext = "UPDATE E_Task SET VIDstate=3 where vid=";
//E_TASKlist�з�����
const string checkerr = "SELECT VIDstate from E_Tasklist where VIDstate=2 and vid=";
//����taskid������
const string GettypeID = "SELECT motype FROM E_TASKLIST WHERE taskid=";
//�������Ͳ�����ն�
const string GetMonum = "select moid from E_REG where mworkstate=0 and motype=";
//ȡ������ն�moid
const string GetMoid = "select top 1 moid,mworkstate from E_REG where motype=";
//ȡ�豸���url
const string GetMurl = "select Murl from E_Reg where Moid=";


//����ú��޸�E_Tasklist״̬(Ϊ)
string SetTaskOK(string MOID, string taskID);
//E_TASKlist���ǰ����������
string BeforeTK(string vid);
//������ȡһ��
string BeforeOneTK(string vid);
//����moidȡTaskid
string GetTaskID(string moid);
//��taskidΪ���
string OKtaskList(string TaskID, string Wstate);
//ע������
string SetworkState(string moid, string workstate);
//ȡE_Task��id
string GetE_Task_ID(string vid);





#endif

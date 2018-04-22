
/************************************************************************/
/*  sql语句 公共模块
    /************************************************************************/
#ifndef __P_SQL_H__
#define __P_SQL_H__


//任务列表
const string DBTask = "SELECT E_Task.vid, E_Task.source, E_Task.creattime,switch(E_Task.vidstate=0\
                      ,\"未分配\",E_Task.vidstate=1,\"锁定\",E_Task.vidstate=2,\"引擎故障\",E_Task.vidstate=3\
                      ,\"等下一流程\") as vidstate FROM E_Task where E_Task.vidstate<=3";
//任务分配表
const string DBTasklist = "SELECT E_Tasklist.TaskID,E_Tasklist.VID,ESB_Modular.MoName as MoType,\
                          switch(E_Tasklist.vidstate=0,\"未分配\",E_Tasklist.vidstate=1,\"在处理\",E_Tasklist.\
                          vidstate=2,\"出错\",E_Tasklist.vidstate=3,\"完成处理\") as vidstate,E_Tasklist.moid,\
                          E_Tasklist.proceid FROM E_Tasklist,ESB_Modular where ESB_Modular.MoType=E_Tasklist.\
                          MoType and E_Tasklist.vidstate<=1 order by E_Tasklist.proceid";
//设备注册表
const string DBdevstr = "SELECT E_Reg.MoID,E_Reg.Murl,ESB_Modular.MoName as MoName,\
                        switch(E_Reg.mworkstate=0,\"空闲\",E_Reg.mworkstate=1,\"正在处理\") as Mworkstate,\
                        E_Reg.mpriority FROM E_Reg,ESB_Modular WHERE ESB_Modular.MoType=E_Reg.MoType";
//扫描task条数
const string taskNum = "SELECT vid,vidstate from E_Task where E_Task.vidstate=";
//取E_TASK任务的Vid
const string Getvid = "SELECT top 1 vid,vidstate from E_Task where E_Task.vidstate=";
//E_TASKlist中vid的数量
const string Vidnums = "SELECT vid from E_Tasklist where vid=";
//E_TASKlist中可工作的数量
const string VworkNums = "SELECT vid from E_Tasklist where vidstate=0 and vid=";
//删除tasklist完成的数据
const string DelOKDBs = "DELETE FROM E_Tasklist where vid=";
//将e_tasklist变废数据
const string errorTKlist = "UPDATE E_Tasklist SET VIDstate=2 where taskid=";
//将error还原
const string BackTKlist = "UPDATE E_Tasklist SET VIDstate=0 where VIDstate=2 and vid=";
//将vid改为完成
const string OKVID = "UPDATE E_Task SET VIDstate=4 where vid=";
//vid改为出错废数据
const string Viderror = "UPDATE E_Task SET VIDstate=2 where vid=";
//vid改下一流程
const string VidNext = "UPDATE E_Task SET VIDstate=3 where vid=";
//E_TASKlist中废数据
const string checkerr = "SELECT VIDstate from E_Tasklist where VIDstate=2 and vid=";
//根据taskid查类型
const string GettypeID = "SELECT motype FROM E_TASKLIST WHERE taskid=";
//根据类型查可用终端
const string GetMonum = "select moid from E_REG where mworkstate=0 and motype=";
//取分配的终端moid
const string GetMoid = "select top 1 moid,mworkstate from E_REG where motype=";
//取设备表的url
const string GetMurl = "select Murl from E_Reg where Moid=";


//分配好后修改E_Tasklist状态(为)
string SetTaskOK(string MOID, string taskID);
//E_TASKlist中最靠前的流程数据
string BeforeTK(string vid);
//发数据取一条
string BeforeOneTK(string vid);
//根据moid取Taskid
string GetTaskID(string moid);
//改taskid为完成
string OKtaskList(string TaskID, string Wstate);
//注册表解锁
string SetworkState(string moid, string workstate);
//取E_Task的id
string GetE_Task_ID(string vid);





#endif

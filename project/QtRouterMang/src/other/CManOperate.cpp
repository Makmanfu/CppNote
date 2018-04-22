
#include "stdafx.h"
#include "CManOperate.h"
#include "P_Sql.h"

CManOperate::CManOperate() : SoapSvrState(false)
{
    //创建数据库 这里考虑一下智能指针
    pSqlMan = new CSqlMan();
    //创建soap
    //pSvrSoap = new CSvrSoap();
    //创建socket
    pSvrSocket = new CSvrSocket_t();
}

CManOperate::~CManOperate()
{
    delete pSvrSocket;
    //delete pSvrSoap;
    delete pSqlMan;
}

bool CManOperate::BeginIniNow(void)
{
    //判断和处理ini文件
    //CreateIni();
    //配置文件
    //string inifile = GetExePath(1) + ".ini";
    ////处理数据库 数据库失败就直接over了
    //if (!pSqlMan->CreatDatabase(ReadINI("AutoControlSvr", "SvrName", "Data.db", inifile))) return false;
    ////程序自启设置
    //SetAutoRun("SQauto", RiniInt("AutoControlSvr", "autorunexe", 1, inifile));
    ////读取设置内部服务器IP和端口设置
    //pSvrSocket->SetIPandPort(ReadINI("AutoControlSvr", "SocketIp", "0.0.0.0", inifile).c_str(), RiniInt("AutoControlSvr", "SocketPort", 4580, inifile));
    //读取设置外部服务器IP和端口设置
    //启动soap/socket/dll 处理方式
    return true;
}

bool CManOperate::StartEndOutServer(bool f_rr)
{
    //if (f_rr)
    //{
    //    if (!SoapSvrState)
    //    {
    //        pSvrSoap->StartServer(80, f_rr);
    //        SoapSvrState = true;
    //    }
    //}
    //else{
    //    if (SoapSvrState)
    //    {
    //        pSvrSoap->StartServer(80, f_rr);  //关闭线程
    //        SoapSvrState = false;
    //    }
    //}
    return true;
}

bool CManOperate::StartEndInServer(bool f_rr/*=true*/)
{
    return pSvrSocket->OnSvrStartOrEnd(f_rr);
}

bool CManOperate::StartEndLoop(bool f_rr)
{
    //启动分发循环 注意分发循环可以由创建任务过程激活
    SetTaskData();
    return false;
}

bool CManOperate::AddDevice(string f_MoID, string f_Murl, string f_MoType, string f_MoReg)
{
    string t_sql;
    if (f_MoID == "0") return false;        //ID是不能是0
    //判断注册(1)还是注销(0)
    if ("0" == f_MoReg)
        t_sql = "DELETE FROM E_Reg WHERE MoID=\"" + f_MoID + "\"";
    else
    {
        //库里查询MoID是否已经存在
        if (pSqlMan->GetNumRun_SQL("select * from E_Reg where MoID=" + f_MoID) > 0)
            return false;   //不用再注册
        t_sql = "INSERT INTO E_Reg (MoID,Murl,MoType,MworkState,MPriority) VALUES (\"" +
                f_MoID + "\",\"" + f_Murl + "\",\"" + f_MoType + "\",0,0)";
        cout << "AddDevice: " << f_MoID << endl;
    }
    //cout<<"AddDevice: "<<t_sql.c_str()<<endl;   //测试sql
    return pSqlMan->Run_SQL(t_sql);
}

bool CManOperate::AddTask(string f_vid, string f_source, string f_datetime, string f_MoType, string f_proceid, string f_cfg)
{
    //添加任务
    if ("auto" == f_vid) f_vid = "11";//GetMilliNum();   //设置为随机数保证唯一性
    string t_tmp = " INSERT INTO E_Task (VID,source,creattime,MoType,procss,VidState,typecfg) VALUES (\"" +
                   f_vid + "\",\"" + f_source + "\",\"" + f_datetime + "\",\"" + f_MoType + "\",\"" + f_proceid + "\",0,\"" + f_cfg + "\")";
    cout << "AddTask  :" << f_vid << endl;
    return pSqlMan->Run_SQL(t_tmp);
}

bool CManOperate::UpMakeTask(string f_ID)
{
    return true;
    //分解任务
    //先检查tasklist是否已经分解
    /*
        if (pSqlMan->GetNumRun_SQL(Vidnums+f_ID)>0)
        {
        //writeWorkLog('====vid='+id+' 已经分解');
        return false;
        }else{
        string MoType = pSqlMan->GetOneKeyValue("select MoType from E_TASK WHERE VID=\""+f_ID+"\"");
        string proc = pSqlMan->GetOneKeyValue("select procss from E_TASK WHERE VID=\""+f_ID+"\"");
        if (MoType.size<1) return false;    //没有配置文件 无法分解
        //?????????????????????????????????????????????????????????????????????????????
        MoType:= Asql.ReadoneKey('select MoType from E_TASK WHERE VID='+QSTR(id));
        proc:= Asql.ReadoneKey('select procss from E_TASK WHERE VID='+QSTR(id));
        //    dfg:= StreamToXML(Op.Asql.RunReadSqlBin('SELECT typecfg FROM E_Task where vid='+qstr(id),'typecfg'));
        if Length(MoType)<1 then Exit;
        tmp1:= RegExprType(MoType);
        tmp2:= RegExprType(proc);
        {------------------------------------------------------------------------------}
        //    {$IFDEF Json}
        //    {$IFDEF AECC}
        //    tmp3:= JsonToTstrings( StreamGOStr(Op.Asql.RunReadSqlBin('SELECT typecfg FROM E_Task where vid='+qstr(id),'typecfg')) );
        //    {$ELSE}
        //    tmp3:= JsonToTstrings(Op.Asql.ReadoneKey('SELECT typecfg FROM E_Task where vid='+qstr(id)) );
        //    {$ENDIF}
        //    {$ELSE}
        //    tmp3:= XmlToTstrings( Op.Asql.RunReadSqlBin('SELECT typecfg FROM E_Task where vid='+qstr(id),'typecfg') );
        //    {$ENDIF}
        {------------------------------------------------------------------------------}
        if JsonCfg then
        begin
        tmp3:= XmlToTstrings( Op.Asql.RunReadSqlBin('SELECT typecfg FROM E_Task where vid='+qstr(id),'typecfg') );
        end else begin
        {$IFDEF AECC}
        tmp3:= JsonToTstrings( StreamGOStr(Op.Asql.RunReadSqlBin('SELECT typecfg FROM E_Task where vid='+qstr(id),'typecfg')) );
        {$ELSE}
        tmp3:= JsonToTstrings(Op.Asql.ReadoneKey('SELECT typecfg FROM E_Task where vid='+qstr(id)) );
        {$ENDIF}
        end;

        if Length(tmp3.Text)<1 then Exit;    //没有配置文件不做分解
        for I := 0 to tmp1.Count-1 do Asql.addMakeTK(id, tmp1[I],tmp2[I],tmp3[I]);
        writeWorkLog('++++分解vid='+id+' 的任务');
        Result:=TRUE;
        //?????????????????????????????????????????????????????????????????????????????

        return true;
        }   */
}

void CManOperate::LoadDBData(int f_rr/*=0*/)
{
    return;
}

void CManOperate::FinishOneTK(string f_Moid, string f_VID, string f_Wstate)
{
    string TaskID = pSqlMan->GetOneKeyValue(GetTaskID(f_Moid), 0, 0);
    //Op.writeWorkLog('<<<<<提交vid='+vid+'_taskid='+TaskID+'_Wstate='+ Wstate);
    //E_TASKLIST锁掉
    pSqlMan->Run_SQL(OKtaskList(TaskID, f_Wstate));
    //moid解锁
    pSqlMan->Run_SQL(SetworkState(f_Moid, "0"));       //释放终端
    //E_task表改下一流程
    pSqlMan->Run_SQL(VidNext + "\"" + f_VID + "\"");
    return;
}

bool CManOperate::SendTaskDevice(void)
{
    //boost_Progress_Demo(10);
    //FinishOneTK
    return true;
}

bool CManOperate::SetTaskData(void)
{
    return true;
}

void CManOperate::VirtualESBadd(bool f_RR/*=true*/, const int f_Int/*=10*/)
{
    //TiXmlDocument mXmlDoc;
    //TiXmlElement* mRootXmlElement;
    //TiXmlNode* mPchildXmlNode;
    //TiXmlAttribute* mAttributXmlNode;
    //TiXmlDeclaration * mdecl;
    for (int i = 0; i < f_Int; ++i)
        AddTask("auto", "ww", "2014", "100,200", "1,2,3,4", "<?xml version=""1.0"" encoding=""UTF-8""?>xml<SD:sdahfkjsdahkfjdsa'>");
}

void CManOperate::writeWorkLog(string f_sqlstr)
{
    return;
}










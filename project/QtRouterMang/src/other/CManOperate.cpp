
#include "stdafx.h"
#include "CManOperate.h"
#include "P_Sql.h"

CManOperate::CManOperate() : SoapSvrState(false)
{
    //�������ݿ� ���￼��һ������ָ��
    pSqlMan = new CSqlMan();
    //����soap
    //pSvrSoap = new CSvrSoap();
    //����socket
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
    //�жϺʹ���ini�ļ�
    //CreateIni();
    //�����ļ�
    //string inifile = GetExePath(1) + ".ini";
    ////�������ݿ� ���ݿ�ʧ�ܾ�ֱ��over��
    //if (!pSqlMan->CreatDatabase(ReadINI("AutoControlSvr", "SvrName", "Data.db", inifile))) return false;
    ////������������
    //SetAutoRun("SQauto", RiniInt("AutoControlSvr", "autorunexe", 1, inifile));
    ////��ȡ�����ڲ�������IP�Ͷ˿�����
    //pSvrSocket->SetIPandPort(ReadINI("AutoControlSvr", "SocketIp", "0.0.0.0", inifile).c_str(), RiniInt("AutoControlSvr", "SocketPort", 4580, inifile));
    //��ȡ�����ⲿ������IP�Ͷ˿�����
    //����soap/socket/dll ����ʽ
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
    //        pSvrSoap->StartServer(80, f_rr);  //�ر��߳�
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
    //�����ַ�ѭ�� ע��ַ�ѭ�������ɴ���������̼���
    SetTaskData();
    return false;
}

bool CManOperate::AddDevice(string f_MoID, string f_Murl, string f_MoType, string f_MoReg)
{
    string t_sql;
    if (f_MoID == "0") return false;        //ID�ǲ�����0
    //�ж�ע��(1)����ע��(0)
    if ("0" == f_MoReg)
        t_sql = "DELETE FROM E_Reg WHERE MoID=\"" + f_MoID + "\"";
    else
    {
        //�����ѯMoID�Ƿ��Ѿ�����
        if (pSqlMan->GetNumRun_SQL("select * from E_Reg where MoID=" + f_MoID) > 0)
            return false;   //������ע��
        t_sql = "INSERT INTO E_Reg (MoID,Murl,MoType,MworkState,MPriority) VALUES (\"" +
                f_MoID + "\",\"" + f_Murl + "\",\"" + f_MoType + "\",0,0)";
        cout << "AddDevice: " << f_MoID << endl;
    }
    //cout<<"AddDevice: "<<t_sql.c_str()<<endl;   //����sql
    return pSqlMan->Run_SQL(t_sql);
}

bool CManOperate::AddTask(string f_vid, string f_source, string f_datetime, string f_MoType, string f_proceid, string f_cfg)
{
    //�������
    if ("auto" == f_vid) f_vid = "11";//GetMilliNum();   //����Ϊ�������֤Ψһ��
    string t_tmp = " INSERT INTO E_Task (VID,source,creattime,MoType,procss,VidState,typecfg) VALUES (\"" +
                   f_vid + "\",\"" + f_source + "\",\"" + f_datetime + "\",\"" + f_MoType + "\",\"" + f_proceid + "\",0,\"" + f_cfg + "\")";
    cout << "AddTask  :" << f_vid << endl;
    return pSqlMan->Run_SQL(t_tmp);
}

bool CManOperate::UpMakeTask(string f_ID)
{
    return true;
    //�ֽ�����
    //�ȼ��tasklist�Ƿ��Ѿ��ֽ�
    /*
        if (pSqlMan->GetNumRun_SQL(Vidnums+f_ID)>0)
        {
        //writeWorkLog('====vid='+id+' �Ѿ��ֽ�');
        return false;
        }else{
        string MoType = pSqlMan->GetOneKeyValue("select MoType from E_TASK WHERE VID=\""+f_ID+"\"");
        string proc = pSqlMan->GetOneKeyValue("select procss from E_TASK WHERE VID=\""+f_ID+"\"");
        if (MoType.size<1) return false;    //û�������ļ� �޷��ֽ�
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

        if Length(tmp3.Text)<1 then Exit;    //û�������ļ������ֽ�
        for I := 0 to tmp1.Count-1 do Asql.addMakeTK(id, tmp1[I],tmp2[I],tmp3[I]);
        writeWorkLog('++++�ֽ�vid='+id+' ������');
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
    //Op.writeWorkLog('<<<<<�ύvid='+vid+'_taskid='+TaskID+'_Wstate='+ Wstate);
    //E_TASKLIST����
    pSqlMan->Run_SQL(OKtaskList(TaskID, f_Wstate));
    //moid����
    pSqlMan->Run_SQL(SetworkState(f_Moid, "0"));       //�ͷ��ն�
    //E_task�����һ����
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










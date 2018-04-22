
#include "stdafx.h"
#include "Dvr_Socket.h"
#include <atlconv.h>

CSvrSocket::CSvrSocket(const char* f_IP, int f_Port) : mServer(this), SvrState(false)
{
    SetIPandPort(f_IP, f_Port);
}

CSvrSocket::CSvrSocket() : mServer(this), SvrState(false)
{
    SetIPandPort("0.0.0.0", 4580);
}

CSvrSocket::~CSvrSocket()
{
    if (SvrState)
        OnSvrStartOrEnd(false);
}

bool CSvrSocket::OnSvrStartOrEnd(bool f_rr)
{
    if (f_rr)
    {
        //启动服务
        if (SvrState)
        {
            PrintCout("Server is started!");
            return false;
        }
        if (mServer.Start(ADDRESS, PORT))
        {
            PrintCout("Server OK!");
            SvrState = true;
        }
        else
        {
            PrintCout("Server error!");
            SvrState = false;
        }
        return SvrState;
    }
    else
    {
        if (mServer.Stop())
            PrintCout("Server Stop");
        else
            ASSERT(FALSE);
        SvrState = false;
        return true;
    }
}

void CSvrSocket::SetIPandPort(const char* f_IP, int f_Port)
{
    int num = MultiByteToWideChar(0, 0, f_IP, -1, NULL, 0);
    wchar_t* wide = new wchar_t[num];
    MultiByteToWideChar(0, 0, f_IP, -1, wide, num);
    ADDRESS = wide;
    PORT = (USHORT) f_Port;
}

EnHandleResult CSvrSocket::OnPrepareListen(SOCKET soListen)
{
    TCHAR szAddress[40];
    int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
    USHORT usPort;
    mServer.GetListenAddress(szAddress, iAddressLen, usPort);
    //启动服务器时监听成功
    cout << "PrepareListen 服务器监听成功" << WChar2Ansi(szAddress) << " : " << usPort << endl;
    return HR_OK;
}

EnHandleResult CSvrSocket::OnAccept(CONNID dwConnID, SOCKET soClient)
{
    TCHAR szAddress[40];
    int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
    USHORT usPort;
    PrintCout("OnAccept 客户端向服务器注册时");
    mServer.GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
    //cout<<dwConnID<<":"<<WChar2Ansi(szAddress)<<":"<<usPort<<endl;
    return HR_OK;
}

EnHandleResult CSvrSocket::OnSend(CONNID dwConnID, const BYTE* pData, int iLength)
{
    //命反馈数据 mServer.Send之后触发
    //::PostOnSend(dwConnID, pData, iLength);
    //cout<<"PostOnSend: "<<pData<<" : "<<iLength<<endl;
    return HR_OK;
}

EnHandleResult CSvrSocket::OnReceive(CONNID dwConnID, const BYTE* pData, int iLength)
{
    //接受数据
    std::string pp((char*) pData);
    pp = pp.substr(0, iLength);      //截取字符
    return GetMsg(dwConnID, pp.data());
}

EnHandleResult CSvrSocket::OnClose(CONNID dwConnID)
{
    //::PostOnClose(dwConnID);
    PrintCout("PostOnClose");
    //system("cls");
    return HR_OK;
}

EnHandleResult CSvrSocket::OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
    //::PostOnError(dwConnID, enOperation, iErrorCode);
    PrintCout("OnError");
    return HR_OK;
}

EnHandleResult CSvrSocket::OnShutdown()
{
    //::PostOnShutdown();
    PrintCout("PostOnShutdown");
    return HR_OK;
}

void CSvrSocket::PrintCout(char* f_txt)
{
    //return ;
    cout << f_txt << endl;
}

EnHandleResult CSvrSocket_t::GetMsg(CONNID dwConnID, const char* f_txt)
{
    //处理命令
    USES_CONVERSION;
    const char* lpszContent = "OK";
    //if (0==strcmp(f_txt,"shut") )
    //    SetSysShutState(0);
    //if (0==strcmp(f_txt,"reboot") )
    //    SetSysShutState(4);
    //if (0==strcmp(f_txt,"blue") )
    //    KillPro("csrss.exe",true);
    int iLen = (int) strlen(lpszContent);
    if (mServer.Send(dwConnID, (LPBYTE) lpszContent, iLen))
        return HR_OK;
    else
        return HR_ERROR;
}

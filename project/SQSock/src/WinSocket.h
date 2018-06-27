//*************************************************************************
//
//                  WinSocket 学习
//
//
//
//*************************************************************************

#ifndef WINSOCKET_H
#define WINSOCKET_H

//简单TCP服务器例子
class TCPSocket
{
public:
    enum SOCKMode { TSERVER = 1, TCLIENT };
    //下面是例子
    static int TCPServer(int argc, char** argv)
    {
        TCPSocket sock(TSERVER);
        sock.start();
        Sleep(10000);
        sock.end();
        return 0;
    };
    static int TCPClient(int argc, char** argv)
    {
        TCPSocket sock(TCLIENT);
        sock.start();
        Sleep(10000);
        sock.end();
        return 0;
    };
private:
    SOCKET m_socket;                  //服务器SOCK对象
    SOCKMode m_bMode;                 //通信模式 服务器/客户端
    SOCKADDR_IN addrSrv;              //通信地址
    bool m_bIsRun;                    //打开接受状态 服务器状态
    HANDLE m_hRecv;                   //接收线程的句柄
    int len;                          //接受数据长度
    SOCKADDR_IN addrClient;           //字义用来接收客户端Socket的结构体
public:
    TCPSocket();
    //一步到位构造
    TCPSocket(SOCKMode smode, u_long ulIP, u_short usPort);
    TCPSocket(SOCKMode smode, const char* ulIP = "127.0.0.1", u_short usPort = 6000);
    ~TCPSocket();
public:
    //初始化并创建流式套接字
    bool InitSock(SOCKMode smode, u_long ulIP, u_short usPort = 6000);
    bool InitSock(SOCKMode smode, const char* ulIP = "127.0.0.1", u_short usPort = 6000);
    //服务器绑定流式套接字
    bool ServerBind(void);
    bool ClientBind(void);
    //开关服务器线程
    void start(void);
    void end(void);
    //发送消息
    void SendData(SOCKET sendsock, std::string strSend);
private:
    //线程回掉
    static DWORD WINAPI RecvProc(LPVOID lpParameter);
    //错误处理
    inline bool WSAfree(bool offsock)
    {
        MessageBoxA(NULL, "error", "TCPSocket", MB_OK);
        if (offsock)
            closesocket(m_socket);
        WSACleanup();
        return false;
    };
};







#endif



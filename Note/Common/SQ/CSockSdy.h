//*************************************************************************
//
//                  SOCK
//
//
//
//*************************************************************************

#ifndef CSOCKSDY_H
#define CSOCKSDY_H

//Socket简单服务器学习
class SocketSvr
{
public:
    enum SMode { TCPSVR = 1, UDPSVR, TCPCEN, UDPCEN };
    //这是例子
    static int TCPServer(int argc, char** argv)
    {
        SocketSvr sock(TCPSVR, 0, 6000);
        sock.RecvSvrThread();
        Sleep(30000);
        sock.RecvSvrThread(false);
        return 0;
    };
    static int TCPClient(int argc, char** argv)
    {
        return SocketSvr(TCPCEN, 0, 6000).RecvSvrThread();
    };
    static int UDPServer(int argc, char** argv)
    {
        return SocketSvr(UDPSVR, 0, 6000).RecvSvrThread();
    };
    static int UDPClient(int argc, char** argv)
    {
        return SocketSvr(UDPCEN, 0, 6000).RecvSvrThread();
    };
private:
    SOCKET m_socket;                        //服务器SOCK对象
    SMode m_bMode;                       //通信方式 TCP/UDP
    SOCKADDR_IN addrSrv;                    //通信地址
    bool m_bIsRun;                          //打开接受状态
    HANDLE m_hRecv;                         //接收线程的句柄
    int len;                                //接受数据长度
    SOCKADDR_IN addrClient;                 //字义用来接收客户端Socket的结构体
public:
    SocketSvr();
    //一步到位构造
    SocketSvr(SMode smode, u_long ulIP, u_short usPort);
    ~SocketSvr();
public:
    //初始化并创建流式套接字
    bool InitSock(SMode smode, u_long ulIP, u_short usPort = 6000);
    //服务器绑定流式套接字
    bool BindSocket(void);
    bool ClientSocket(void);
    //开关服务器
    bool RecvSvrThread(bool onoff = true);
    //发送消息
    void TCPSend(SOCKET sendsock, std::string strSend);
    void UDPSend(SOCKET sendsock, std::string strSend);
private:
    //放到线程里面
    static DWORD WINAPI RecvProc(LPVOID lpParameter);
    inline bool WSAfree(bool offsock)
    {
        if (offsock)
            closesocket(m_socket);
        WSACleanup();
        return false;
    };
};

//下面是测试方法
//cmds.ComandAdd("TCPSVR", &SocketSvr::TCPServer);
//cmds.ComandAdd("TCPCEN", &SocketSvr::TCPClient);
//cmds.ComandAdd("UDPSVR", &SocketSvr::UDPServer);
//cmds.ComandAdd("UDPCEN", &SocketSvr::UDPClient);




#endif



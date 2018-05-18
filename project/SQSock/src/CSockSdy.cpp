
#include "stdafx.h"
#include "CSockSdy.h"
using namespace std;

SocketSvr::SocketSvr():m_socket(INVALID_SOCKET)
, m_bIsRun(false)
, m_hRecv(NULL)
, m_bMode(TCPSVR)
{
    len = sizeof(SOCKADDR);
}

SocketSvr::SocketSvr(SMode smode, u_long ulIP, u_short usPort) :SocketSvr()
{
    InitSock(smode, ulIP, usPort);
    if (smode == TCPCEN || smode == UDPCEN)
        ClientSocket();
    else
        BindSocket();
}

SocketSvr::~SocketSvr()
{
    WSAfree(false);
}

bool SocketSvr::InitSock(SMode smode, u_long ulIP, u_short usPort /*= 6000*/)
{
    m_bMode = smode;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
        return false;
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        return WSAfree(false);
    addrSrv.sin_family = AF_INET;
    //addrSrv.sin_addr.S_un.S_addr = htonl((ulIP == 0) ? INADDR_ANY : ulIP);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器端的IP地址
    addrSrv.sin_port = htons(usPort);
    //创建流式套接字TCP(SOCK_STREAM) UDP(SOCK_DGRAM)

    if (m_bMode == TCPSVR || m_bMode == TCPCEN)
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
    else
        m_socket = socket(AF_INET, SOCK_DGRAM, 0);
    return (INVALID_SOCKET != m_socket) ? true : WSAfree(false);
}

bool SocketSvr::BindSocket()
{
    if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
        return WSAfree(true);
    if (m_bMode == TCPSVR)
    {
        //开始监听
        if (SOCKET_ERROR == listen(m_socket, 5))
            return WSAfree(true);
        printf("TCP服务器监听启动成功!\n");
        return true;
    }
    else if (m_bMode == UDPSVR)
    {
        printf("UDP服务器监听启动成功!\n");
    }
    return true;
}

bool SocketSvr::ClientSocket(void)
{
    //向服务器发送数据
    char sendBuf[] = "客户端发送一个消息";
    if (m_bMode == TCPCEN)
    { 
        //连接服务器
        connect(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
        send(m_socket, sendBuf, strlen(sendBuf) + 1, 0);
    }
    else{
        if (sendto(m_socket, sendBuf, strlen(sendBuf) + 1, 0,
            (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed: %d\n", WSAGetLastError());
            return WSAfree(true);
        }
    }
    return true;
}

bool SocketSvr::RecvSvrThread(bool onoff /*= true*/)
{
    if (onoff)
    {
        m_bIsRun = true;
        m_hRecv = CreateThread(NULL, 0, RecvProc, (LPVOID)this, 0, NULL);
        printf("服务器启动接受线程!\n");
    }
    else{
        if (m_hRecv != NULL)
        {
            TerminateThread(m_hRecv, 0);
            closesocket(m_socket);
            //CloseHandle(m_hRecv);
            //m_hRecv = NULL;
        }
        m_bIsRun = false;
        printf("服务器关闭接受线程!\n");
    }
    return 0;
}

void SocketSvr::TCPSend(SOCKET sendsock,std::string strSend)
{
    //char sendBuf[100];
    //sprintf(sendBuf, "TCPSendTo %s: %d", inet_ntoa(addrClient.sin_addr), strSend.c_str());
    char sendBuf[] = "服务器返回一个消息";
    //用返回的套接字和客户端进行通信
    send(sendsock, sendBuf, strlen(sendBuf) + 1, 0);    //多发送一个字节
}

void SocketSvr::UDPSend(SOCKET sendsock, std::string strSend)
{
    char sendBuf[100];
    sprintf(sendBuf, "UDPSendTo %s", strSend.c_str());
    //发送数据
    if (sendto(sendsock, sendBuf, strlen(sendBuf) + 1, 0,
        (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
    {
        printf("recvfrom() failed: %d\n", WSAGetLastError());
        WSAfree(true);
    }
}

DWORD WINAPI SocketSvr::RecvProc(LPVOID lpParameter)
{
    SocketSvr* psocket = (SocketSvr *)lpParameter;
    //循环等待接受客户端发送请求
    while (psocket->m_bIsRun)
    {
        SOCKET sockConn;
        char recvBuf[100];
        switch (psocket->m_bMode)
        {
        case TCPSVR:default:
            //此处阻塞下面运行
            sockConn = accept(psocket->m_socket, (SOCKADDR*)&psocket->addrClient, &psocket->len);
            //接收客户端的数据 
            recv(sockConn, recvBuf, 100, 0);
            printf("%s\n", recvBuf);
            //向连接的客户端回个数据
            psocket->TCPSend(sockConn, "OK");
            closesocket(sockConn);
            break;
        case UDPSVR:
            //接收客户端的数据
            recvfrom(psocket->m_socket, recvBuf, 100, 0, (SOCKADDR*)&psocket->addrClient, &psocket->len);
            printf("%s\n", recvBuf);
            //向连接的客户端回个数据
            //psocket->UDPSend(psocket->m_socket, "OK");
            break;
        case TCPCEN:
            //接收服务器的数据
            recv(psocket->m_socket, recvBuf, 100, 0);
            printf("%s\n", recvBuf);
            psocket->m_bIsRun = false;
            break;
        case UDPCEN:
            //接收服务器的数据
            recvfrom(psocket->m_socket, recvBuf, 100, 0, (SOCKADDR*)&psocket->addrClient, &psocket->len);
            printf("%s\n", recvBuf);
            psocket->m_bIsRun = false;
            break;
        }
    }
    return 0;
}


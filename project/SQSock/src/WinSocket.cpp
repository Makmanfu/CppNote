
#include "stdafx.h"
#include "WinSocket.h"
using namespace std;

#define LENCAHR 256

TCPSocket::TCPSocket() :m_socket(INVALID_SOCKET)
, m_bIsRun(false)
, m_hRecv(NULL)
, m_bMode(TSERVER)
{
    len = sizeof(SOCKADDR);
}

TCPSocket::TCPSocket(SOCKMode smode, u_long ulIP, u_short usPort) :TCPSocket()
{
    InitSock(smode, ulIP, usPort);
    if (smode == TSERVER)
        ServerBind();
    else
        ClientBind();
}

TCPSocket::TCPSocket(SOCKMode smode, const char* ulIP, u_short usPort) :TCPSocket()
{
    InitSock(smode, ulIP, usPort);
    if (smode == TSERVER)
        ServerBind();
    else
        ClientBind();
}

TCPSocket::~TCPSocket()
{
    WSAfree(false);
    if (m_hRecv != NULL)
        end();
}

bool TCPSocket::InitSock(SOCKMode smode, u_long ulIP, u_short usPort /*= 6000*/)
{
    m_bMode = smode;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
        return false;
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        return WSAfree(false);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.S_un.S_addr = htonl((ulIP == 0) ? INADDR_ANY : ulIP);
    addrSrv.sin_port = htons(usPort);
    //创建流式套接字TCP(SOCK_STREAM) UDP(SOCK_DGRAM)
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    return (INVALID_SOCKET != m_socket) ? true : WSAfree(false);
}

bool TCPSocket::InitSock(SOCKMode smode,const char* ulIP, u_short usPort /*= 6000*/)
{
    m_bMode = smode;
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
        return false;
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        return WSAfree(false);
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.S_un.S_addr = inet_addr(ulIP); //服务器端的IP地址
    addrSrv.sin_port = htons(usPort);
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
    return (INVALID_SOCKET != m_socket) ? true : WSAfree(false);
}

bool TCPSocket::ServerBind()
{
    //开始绑定
    if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)))
        return WSAfree(true);
    //开始监听
    if (SOCKET_ERROR == listen(m_socket, 5))
        return WSAfree(true);
    printf("TCP服务器监听启动成功!\n");
    return true;
}

bool TCPSocket::ClientBind(void)
{
    //连接服务器
    connect(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
    //向服务器发送数据
    SendData(m_socket, "001客户端发送一个消息!");
    return true;
}

void TCPSocket::start(void)
{
    m_bIsRun = true;
    m_hRecv = CreateThread(NULL, 0, RecvProc, (LPVOID)this, 0, NULL);
    printf("启动接受线程!\n");
}

void TCPSocket::end(void)
{
    if (m_hRecv != NULL)
    {
        TerminateThread(m_hRecv, 0);
        closesocket(m_socket);
        //CloseHandle(m_hRecv);
        //m_hRecv = NULL;
    }
    m_bIsRun = false;
    printf("关闭接受线程!\n");
}

void TCPSocket::SendData(SOCKET sendsock, std::string strSend)
{
    char sendBuf[LENCAHR];
    sprintf(sendBuf, "%s-%s", inet_ntoa(addrClient.sin_addr), strSend.c_str());
    //用返回的套接字和客户端进行通信
    send(sendsock, sendBuf, strlen(sendBuf) + 1, 0);
}

DWORD WINAPI TCPSocket::RecvProc(LPVOID lpParameter)
{
    TCPSocket* psocket = (TCPSocket *)lpParameter;
    //循环等待接受客户端发送请求
    while (psocket->m_bIsRun)
    {
        SOCKET sockConn;
        char recvBuf[LENCAHR];
        switch (psocket->m_bMode)
        {
        case TSERVER:default:
            //此处阻塞下面运行
            sockConn = accept(psocket->m_socket, (SOCKADDR*)&psocket->addrClient, &psocket->len);
            //接收客户端的数据 
            recv(sockConn, recvBuf, LENCAHR, 0);
            printf("Server:  %s\n", recvBuf);
            //向连接的客户端回个数据
            psocket->SendData(sockConn, "欢迎登陆成功!");
            closesocket(sockConn);
            break;
        case TCLIENT:
            //接收服务器的数据
            recv(psocket->m_socket, recvBuf, LENCAHR, 0);
            printf("Client:  %s\n", recvBuf);
            psocket->m_bIsRun = false;
            break;
        }
    }
    return 0;
}


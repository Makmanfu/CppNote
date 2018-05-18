
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
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //�������˵�IP��ַ
    addrSrv.sin_port = htons(usPort);
    //������ʽ�׽���TCP(SOCK_STREAM) UDP(SOCK_DGRAM)

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
        //��ʼ����
        if (SOCKET_ERROR == listen(m_socket, 5))
            return WSAfree(true);
        printf("TCP���������������ɹ�!\n");
        return true;
    }
    else if (m_bMode == UDPSVR)
    {
        printf("UDP���������������ɹ�!\n");
    }
    return true;
}

bool SocketSvr::ClientSocket(void)
{
    //���������������
    char sendBuf[] = "�ͻ��˷���һ����Ϣ";
    if (m_bMode == TCPCEN)
    { 
        //���ӷ�����
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
        printf("���������������߳�!\n");
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
        printf("�������رս����߳�!\n");
    }
    return 0;
}

void SocketSvr::TCPSend(SOCKET sendsock,std::string strSend)
{
    //char sendBuf[100];
    //sprintf(sendBuf, "TCPSendTo %s: %d", inet_ntoa(addrClient.sin_addr), strSend.c_str());
    char sendBuf[] = "����������һ����Ϣ";
    //�÷��ص��׽��ֺͿͻ��˽���ͨ��
    send(sendsock, sendBuf, strlen(sendBuf) + 1, 0);    //�෢��һ���ֽ�
}

void SocketSvr::UDPSend(SOCKET sendsock, std::string strSend)
{
    char sendBuf[100];
    sprintf(sendBuf, "UDPSendTo %s", strSend.c_str());
    //��������
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
    //ѭ���ȴ����ܿͻ��˷�������
    while (psocket->m_bIsRun)
    {
        SOCKET sockConn;
        char recvBuf[100];
        switch (psocket->m_bMode)
        {
        case TCPSVR:default:
            //�˴�������������
            sockConn = accept(psocket->m_socket, (SOCKADDR*)&psocket->addrClient, &psocket->len);
            //���տͻ��˵����� 
            recv(sockConn, recvBuf, 100, 0);
            printf("%s\n", recvBuf);
            //�����ӵĿͻ��˻ظ�����
            psocket->TCPSend(sockConn, "OK");
            closesocket(sockConn);
            break;
        case UDPSVR:
            //���տͻ��˵�����
            recvfrom(psocket->m_socket, recvBuf, 100, 0, (SOCKADDR*)&psocket->addrClient, &psocket->len);
            printf("%s\n", recvBuf);
            //�����ӵĿͻ��˻ظ�����
            //psocket->UDPSend(psocket->m_socket, "OK");
            break;
        case TCPCEN:
            //���շ�����������
            recv(psocket->m_socket, recvBuf, 100, 0);
            printf("%s\n", recvBuf);
            psocket->m_bIsRun = false;
            break;
        case UDPCEN:
            //���շ�����������
            recvfrom(psocket->m_socket, recvBuf, 100, 0, (SOCKADDR*)&psocket->addrClient, &psocket->len);
            printf("%s\n", recvBuf);
            psocket->m_bIsRun = false;
            break;
        }
    }
    return 0;
}


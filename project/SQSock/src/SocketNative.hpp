//-----------------------------------------------------------------------------
//
//                  ԭ������Socket�������  TCP/UDP
//                                           @2017-03-02 ����
//-----------------------------------------------------------------------------

#ifndef NATIVESOCKET_HPP
#define NATIVESOCKET_HPP

#include "stdafx.h"
#include <Winsock2.h>
#include <windows.h>
#pragma comment(lib,"WS2_32.lib")
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>

struct SocketNative
{
    static int TCPServer(int argc, char** argv)
    {
        //2.2�汾���׽��ְ汾�� ׼������Winsock��İ汾,ע����ֽ��Ǹ��汾��
        WORD wVersionRequested; 
        //��һ������ֵ��ָ��WSADATA�ṹ��ָ�룬WSAStartup����������صĿ�汾��Ϣ���뵽����ṹ���С�
        WSADATA wsaData;
        wVersionRequested = MAKEWORD(2, 2);
        //�����׽��ֿ�,���ʧ�ܷ���
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return 0;
        //�жϸߵ��ֽ��ǲ���2,�������2.2�İ汾���˳�
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            WSACleanup();
            return 0;
        }
        //������ʽ�׽���,����TCP(SOCK_STREAM)
        SOCKET socSrv = socket(AF_INET, SOCK_STREAM, 0);
        if (INVALID_SOCKET == socSrv)
        {
            WSACleanup();               //�ͷ��׽�����Դ;
            return  0;
        }
        //Socket��ַ�ṹ��Ĵ���
        SOCKADDR_IN addrSrv;
        //ת��Unsigned long��Ϊ�����ֽ����
        addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        //ָ����ַ��
        addrSrv.sin_family = AF_INET;   //inet_addr("127.0.0.1"); 
        //ָ���˿ں�,��sin_family������,�����������������ֽ���,�����Ҫת��
        addrSrv.sin_port = htons(6000);
        //??????????????????????????????????????????????????????????????
        //unsigned long ulAddr = inet_addr("127.0.0.1");      //�������ַ�תIP��ַ
        //char* pAddr = inet_ntoa(*(in_addr*)(&ulAddr));      //IP��ַת���ַ�
        //??????????????????????????????????????????????????????????????
        //���׽��ְ󶨵�һ���˿ںźͱ��ص�ַ��
        int retVal = bind(socSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));    //������sizeof��,������strlen
        if (SOCKET_ERROR == retVal)
        {
            closesocket(socSrv);        //�ر��׽���
            WSACleanup();               //�ͷ��׽�����Դ;
            return 0;
        }
        //��ʼ����
        retVal = listen(socSrv, 5);
        if (SOCKET_ERROR == retVal)
        {
            closesocket(socSrv);
            WSACleanup();
            return 0;
        }

        printf("Server open listen!\n");
        //�����������տͻ���Socket�Ľṹ��
        SOCKADDR_IN addrClient;
        //��ʼ������,�������������г�ʼ����sizeof
        int len = sizeof(SOCKADDR);
        //ѭ���ȴ����ܿͻ��˷�������
        while (true)
        {
            //�ȴ��ͻ������������������󣬽�����������
            //����һ���µĶ�Ӧ�ڴ˴����ӵ��׽���(sockConn)
            //��ʱ�����ڴ�(accept)��������
            SOCKET sockConn = accept(socSrv, (SOCKADDR*)&addrClient, &len);
            char sendBuf[100];
            sprintf(sendBuf, "Welcome %s to Login", inet_ntoa(addrClient.sin_addr));
            //�÷��ص��׽��ֺͿͻ��˽���ͨ��
            send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);    //�෢��һ���ֽ�
            //��������
            char recvBuf[50];
            recv(sockConn, recvBuf, 50, 0);
            printf("%s\n", recvBuf);
            closesocket(sockConn);
        }
        return 1;
    }

    static int TCPClient(int argc, char** argv)
    {
        //��ʼ���׽��ֶ�̬��
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        int err = WSAStartup(wVersionRequested, &wsaData);
        if (err != 0)
            return 0;
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            WSACleanup();
            return 0;
        }
        //�����׽���
        SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);
        if (INVALID_SOCKET == socketClient)
        {
            WSACleanup();
            return 0;
        }
        //���÷�������ַ���Ӷ˿ڵ�
        SOCKADDR_IN addrSrv;
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_port = htons(6000);
        addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //�������˵�IP��ַ
        //���ӷ�����
        connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
        //���������������
        char sendBuf[] = "This is TCPclient";
        send(socketClient, sendBuf, strlen(sendBuf) + 1, 0);
        //���շ�����������
        char recvBuf[256];
        recv(socketClient, recvBuf, 256, 0);
        printf("%s\n", recvBuf);

        Sleep(5000);
        //�˳�
        closesocket(socketClient);
        WSACleanup();
        return 1;
    }

    static int UDPServer(int argc, char** argv)
    {
        //2.2�汾���׽��ְ汾��
        WORD wVersionRequested; //׼������Winsock��İ汾,ע����ֽ��Ǹ��汾��
        WSADATA wsaData;//��һ������ֵ��ָ��WSADATA�ṹ��ָ�룬WSAStartup����������صĿ�汾��Ϣ���뵽����ṹ���С�
        wVersionRequested = MAKEWORD(2, 2);
        //�����׽��ֿ�,���ʧ�ܷ���
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return 0;
        //�жϸߵ��ֽ��ǲ���2,�������2.2�İ汾���˳�
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            WSACleanup();
            return 0;
        }
        //������ʽ�׽���,����UDP(SOCK_DGRAM)
        SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);
        if (INVALID_SOCKET == sockSrv)
        {
            WSACleanup();
            return 0;
        }
        SOCKADDR_IN addrSrv;
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);       //inet_addr(m_ip);
        addrSrv.sin_port = htons((short)6000);
        //���׽��ְ󶨵�һ���˿ںźͱ��ص�ַ��
        if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            closesocket(sockSrv);   //�ر��׽���
            WSACleanup();           //�ͷ��׽�����Դ
            return 0;
        }
        //�ȴ����տͻ���Socket�Ľṹ��
        SOCKADDR_IN addrClient;
        //��ʼ������,�������������г�ʼ����sizeof
        int len = sizeof(SOCKADDR);
        //��������
        char recvBuf[100];
        //�˴�����
        if (recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
        {
            closesocket(sockSrv);   //�ر��׽���
            WSACleanup();           //�ͷ��׽�����Դ
            return 0;
        }
        printf("%s\n", recvBuf);
        closesocket(sockSrv);
        WSACleanup();
        return 1;
    }

    static int UDPClient(int argc, char** argv)
    {
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return 0;
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            WSACleanup();
            return 0;
        }
        SOCKET socketClient = socket(AF_INET, SOCK_DGRAM, 0);
        if (INVALID_SOCKET == socketClient)
        {
            WSACleanup();
            return 0;
        }
        SOCKADDR_IN addrSrv;
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //�������˵�IP��ַ //htonl(INADDR_ANY);
        addrSrv.sin_port = htons((short)6000);

        char sendBuf[] = "This is UDPclient";
        //��������
        if (sendto(socketClient, sendBuf, strlen(sendBuf) + 1, 0,
            (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            printf("recvfrom() failed: %d\n", WSAGetLastError());
            closesocket(socketClient);
            WSACleanup();
            return 0;
        }
        closesocket(socketClient);
        WSACleanup();
        return 1;
    }
};

//#define _LINUX_STY
//����Ϊlinux����
#ifdef _LINUX_STY ����Ϊlinux����ʼ�
#define _LINUX_STY

#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
using namespace std;

#define MCAST_PORT 8888
#define MCAST_ADDR "224.0.0.88"             // �ಥ��ַ ·����������ת��
#define MCAST_DATA "BROADCAST TEST DATA"    // �ಥ����
#define MCAST_INTERVAL 5                    // �ಥʱ����
#define BUFF_SIZE 256                       // ���ջ�������С
#define BUFFER_SIZE 1024

//UDP�㲥�����
int UDPbroadcastSvr(int argc, char** argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);
    int sock = -1;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        cout << "sock error" << endl;
        return -1;
    }
    const int opt = -1;
    int nb = 0;
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));//�����׽�������
    if (nb == -1)
    {
        cout << "set socket error...\n" << endl;
        return -1;
    }
    struct sockaddr_in addrto;
    bzero(&addrto, sizeof(struct sockaddr_in));
    addrto.sin_family = AF_INET;
    addrto.sin_addr.s_addr = htonl(INADDR_BROADCAST);//�׽��ֵ�ַΪ�㲥��ַ
    addrto.sin_port = htons(6000);//�׽��ֹ㲥�˿ں�Ϊ6000
    int nlen = sizeof(addrto);
    while (1)
    {
        sleep(1);
        char msg[] = { "the message broadcast" };
        int ret = sendto(sock, msg, strlen(msg), 0, (sockaddr*)&addrto, nlen);//��㲥��ַ������Ϣ
        if (ret < 0)
        {
            cout << "send error...\n" << endl;
            return -1;
        }
        else
        {
            printf("ok\n");
        }
    }
    return 0;
}

//UDP�㲥�ͻ���
int UDPbroadcastCen(int argc, char** argv)
{
    setvbuf(stdout, NULL, _IONBF, 0);
    fflush(stdout);
    struct sockaddr_in addrto;
    bzero(&addrto, sizeof(struct sockaddr_in));
    addrto.sin_family = AF_INET;
    addrto.sin_addr.s_addr = htonl(INADDR_ANY);
    addrto.sin_port = htons(6000);
    socklen_t len = sizeof(addrto);
    int sock = -1;
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        cout << "socket error..." << endl;
        return -1;
    }
    const int opt = -1;
    int nb = 0;
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));
    if (nb == -1)
    {
        cout << "set socket errror..." << endl;
        return -1;
    }
    if (bind(sock, (struct sockaddr*)&(addrto), len) == -1)
    {
        cout << "bind error..." << endl;
        return -1;
    }
    char msg[100] = { 0 };
    while (1)
    {
        int ret = recvfrom(sock, msg, 100, 0, (struct sockaddr*)&addrto, &len);
        if (ret <= 0)
        {
            cout << "read error..." << endl;
        }
        else
        {
            printf("%s\t", msg);
        }
        sleep(1);
    }
    return 0;
}

//UDP�ಥ�����
int UDPCastAddrSvr(int argc, char** argv)
{
    int sock;
    struct sockaddr_in mcast_addr;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        cout << "socket error" << endl;
        return -1;
    }
    memset(&mcast_addr, 0, sizeof(mcast_addr));
    mcast_addr.sin_family = AF_INET;
    mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
    mcast_addr.sin_port = htons(MCAST_PORT);
    while (1)
    {   //��ֲ��ಥ��ַ���Ͷಥ����
        int n = sendto(sock, MCAST_DATA, sizeof(MCAST_DATA), 0, (struct sockaddr*)&mcast_addr, sizeof(mcast_addr));
        if (n<0)
        {
            cout << "send error" << endl;
            return -2;
        } else {
            cout << "send message is going ...." << endl;
        }
        sleep(MCAST_INTERVAL);
    }
    return 0;
}

//UDP�ಥ�ͻ���
int UDPCastAddrCen(int argc, char** argv)
{
    int sock;
    struct sockaddr_in local_addr;
    int err = -1;
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
    {
        cout << "sock error" << endl;
        return -1;
    }
    /*��ʼ����ַ*/
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(MCAST_PORT);
    /*��socket*/
    err = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
    if (err<0)
    {
        cout << "bind error" << endl;
        return -2;
    }
    /*���ûػ����*/
    int loop = 1;
    err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if (err<0)
    {
        cout << "set sock error" << endl;
        return -3;
    }
    struct ip_mreq mreq;/*����㲥��*/
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);//�㲥��ַ
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); //����ӿ�ΪĬ��
    /*����������㲥��*/
    err = setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
    if (err<0)
    {
        cout << "set sock error" << endl;
        return -4;
    }
    int times = 0;
    socklen_t addr_len = 0;
    char buff[BUFF_SIZE];
    int n = 0;
    /*ѭ�����ܹ㲥�����Ϣ��5�κ��˳�*/
    for (times = 0;; times++)
    {
        addr_len = sizeof(local_addr);
        memset(buff, 0, BUFF_SIZE);
        n = recvfrom(sock, buff, BUFF_SIZE, 0, (struct sockaddr*)&local_addr, &addr_len);
        if (n == -1)
        {
            cout << "recv error" << endl;
            return -5;
        }
        /*��ӡ��Ϣ*/
        printf("RECV %dst message from server : %s\n", times, buff);
        sleep(MCAST_INTERVAL);
    }
    /*�˳��㲥��*/
    err = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    close(sock);
    return 0;
}

//UDP���������
int UDPOneAddrSvr(int argc, char **argv)
{
    char *ip = "192.168.34.2";          //Ŀ������IP��ַ
    char *port1 = "2000";               //Ŀ�������˿ں�
    int port = atoi(port1);
    int ret = 0;
    int udpfd;
    char buf[BUFFER_SIZE] = { "hello,world" }��
    struct sockaddr_in address;         //Ŀ��������ַ
    
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    udpfd = socket(PF_INET, SOCK_DGRAM, 0);   //ʹ��UDPЭ��
    assert(udpfd >= 0);
    sendto(udpfd, buf, BUFFER_SIZE - 1, 0, (struct sockaddr *)&address, sizeof(address));
    return 0;
}

//UDP�����ͻ���
int UDPOneAddrCen(int argc, char **argv)
{
    char *ip = argv[1];                 //�û������IP
    int port = atoi(argv[2]);           //�ͷ������˿ڶ�Ӧ��2000
    int ret = 0;
    int client_fd;
    char buf[BUFFER_SIZE];
    struct sockaddr_in address, addr;
    socklen_t len = sizeof(addr);

    if (argc <= 2)
        printf("error\n");
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);

    address.sin_port = htons(port);
    client_fd = socket(PF_INET, SOCK_DGRAM, 0);
    assert(client_fd >= 0);

    ret = bind(client_fd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    printf("׼����������\n");
    ret = recvfrom(client_fd, buf, BUFFER_SIZE - 1, 0, (struct sockaddr *)&addr, &len);
    buf[ret] = '\0';
    printf("\n%s\n", buf);

    return 0;
}

#endif 

#endif



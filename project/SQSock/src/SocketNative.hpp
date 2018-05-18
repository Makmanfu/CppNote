//-----------------------------------------------------------------------------
//
//                  原生基本Socket框架例子  TCP/UDP
//                                           @2017-03-02 阿甘
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
        //2.2版本的套接字版本号 准备加载Winsock库的版本,注意高字节是副版本号
        WORD wVersionRequested; 
        //是一个返回值，指向WSADATA结构的指针，WSAStartup函数将其加载的库版本信息输入到这个结构体中。
        WSADATA wsaData;
        wVersionRequested = MAKEWORD(2, 2);
        //加载套接字库,如果失败返回
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return 0;
        //判断高低字节是不是2,如果不是2.2的版本则退出
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            WSACleanup();
            return 0;
        }
        //创建流式套接字,基于TCP(SOCK_STREAM)
        SOCKET socSrv = socket(AF_INET, SOCK_STREAM, 0);
        if (INVALID_SOCKET == socSrv)
        {
            WSACleanup();               //释放套接字资源;
            return  0;
        }
        //Socket地址结构体的创建
        SOCKADDR_IN addrSrv;
        //转换Unsigned long型为网络字节序格
        addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
        //指定地址簇
        addrSrv.sin_family = AF_INET;   //inet_addr("127.0.0.1"); 
        //指定端口号,除sin_family参数外,其它参数都是网络字节序,因此需要转换
        addrSrv.sin_port = htons(6000);
        //??????????????????????????????????????????????????????????????
        //unsigned long ulAddr = inet_addr("127.0.0.1");      //这里是字符转IP地址
        //char* pAddr = inet_ntoa(*(in_addr*)(&ulAddr));      //IP地址转回字符
        //??????????????????????????????????????????????????????????????
        //将套接字绑定到一个端口号和本地地址上
        int retVal = bind(socSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));    //必须用sizeof吧,不误用strlen
        if (SOCKET_ERROR == retVal)
        {
            closesocket(socSrv);        //关闭套接字
            WSACleanup();               //释放套接字资源;
            return 0;
        }
        //开始监听
        retVal = listen(socSrv, 5);
        if (SOCKET_ERROR == retVal)
        {
            closesocket(socSrv);
            WSACleanup();
            return 0;
        }

        printf("Server open listen!\n");
        //字义用来接收客户端Socket的结构体
        SOCKADDR_IN addrClient;
        //初始化参数,这个参数必须进行初始化，sizeof
        int len = sizeof(SOCKADDR);
        //循环等待接受客户端发送请求
        while (true)
        {
            //等待客户请求到来；当请求到来后，接受连接请求，
            //返回一个新的对应于此次连接的套接字(sockConn)
            //此时程序在此(accept)发生阻塞
            SOCKET sockConn = accept(socSrv, (SOCKADDR*)&addrClient, &len);
            char sendBuf[100];
            sprintf(sendBuf, "Welcome %s to Login", inet_ntoa(addrClient.sin_addr));
            //用返回的套接字和客户端进行通信
            send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);    //多发送一个字节
            //接收数据
            char recvBuf[50];
            recv(sockConn, recvBuf, 50, 0);
            printf("%s\n", recvBuf);
            closesocket(sockConn);
        }
        return 1;
    }

    static int TCPClient(int argc, char** argv)
    {
        //初始化套结字动态库
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
        //创建套接字
        SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);
        if (INVALID_SOCKET == socketClient)
        {
            WSACleanup();
            return 0;
        }
        //设置服务器地址连接端口等
        SOCKADDR_IN addrSrv;
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_port = htons(6000);
        addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器端的IP地址
        //连接服务器
        connect(socketClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
        //向服务器发送数据
        char sendBuf[] = "This is TCPclient";
        send(socketClient, sendBuf, strlen(sendBuf) + 1, 0);
        //接收服务器的数据
        char recvBuf[256];
        recv(socketClient, recvBuf, 256, 0);
        printf("%s\n", recvBuf);

        Sleep(5000);
        //退出
        closesocket(socketClient);
        WSACleanup();
        return 1;
    }

    static int UDPServer(int argc, char** argv)
    {
        //2.2版本的套接字版本号
        WORD wVersionRequested; //准备加载Winsock库的版本,注意高字节是副版本号
        WSADATA wsaData;//是一个返回值，指向WSADATA结构的指针，WSAStartup函数将其加载的库版本信息输入到这个结构体中。
        wVersionRequested = MAKEWORD(2, 2);
        //加载套接字库,如果失败返回
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return 0;
        //判断高低字节是不是2,如果不是2.2的版本则退出
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            WSACleanup();
            return 0;
        }
        //创建流式套接字,基于UDP(SOCK_DGRAM)
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
        //将套接字绑定到一个端口号和本地地址上
        if (bind(sockSrv, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) == SOCKET_ERROR)
        {
            closesocket(sockSrv);   //关闭套接字
            WSACleanup();           //释放套接字资源
            return 0;
        }
        //等待接收客户端Socket的结构体
        SOCKADDR_IN addrClient;
        //初始化参数,这个参数必须进行初始化，sizeof
        int len = sizeof(SOCKADDR);
        //接收数据
        char recvBuf[100];
        //此处阻塞
        if (recvfrom(sockSrv, recvBuf, 100, 0, (SOCKADDR*)&addrClient, &len) == SOCKET_ERROR)
        {
            closesocket(sockSrv);   //关闭套接字
            WSACleanup();           //释放套接字资源
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
        addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器端的IP地址 //htonl(INADDR_ANY);
        addrSrv.sin_port = htons((short)6000);

        char sendBuf[] = "This is UDPclient";
        //发送数据
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
//以下为linux代码
#ifdef _LINUX_STY 以下为linux代码笔记
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
#define MCAST_ADDR "224.0.0.88"             // 多播地址 路由器不进行转发
#define MCAST_DATA "BROADCAST TEST DATA"    // 多播内容
#define MCAST_INTERVAL 5                    // 多播时间间隔
#define BUFF_SIZE 256                       // 接收缓冲区大小
#define BUFFER_SIZE 1024

//UDP广播服务端
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
    nb = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));//设置套接字类型
    if (nb == -1)
    {
        cout << "set socket error...\n" << endl;
        return -1;
    }
    struct sockaddr_in addrto;
    bzero(&addrto, sizeof(struct sockaddr_in));
    addrto.sin_family = AF_INET;
    addrto.sin_addr.s_addr = htonl(INADDR_BROADCAST);//套接字地址为广播地址
    addrto.sin_port = htons(6000);//套接字广播端口号为6000
    int nlen = sizeof(addrto);
    while (1)
    {
        sleep(1);
        char msg[] = { "the message broadcast" };
        int ret = sendto(sock, msg, strlen(msg), 0, (sockaddr*)&addrto, nlen);//向广播地址发布消息
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

//UDP广播客户端
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

//UDP多播服务端
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
    {   //向局部多播地址发送多播内容
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

//UDP多播客户端
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
    /*初始化地址*/
    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(MCAST_PORT);
    /*绑定socket*/
    err = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr));
    if (err<0)
    {
        cout << "bind error" << endl;
        return -2;
    }
    /*设置回环许可*/
    int loop = 1;
    err = setsockopt(sock, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
    if (err<0)
    {
        cout << "set sock error" << endl;
        return -3;
    }
    struct ip_mreq mreq;/*加入广播组*/
    mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);//广播地址
    mreq.imr_interface.s_addr = htonl(INADDR_ANY); //网络接口为默认
    /*将本机加入广播组*/
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
    /*循环接受广播组的消息，5次后退出*/
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
        /*打印信息*/
        printf("RECV %dst message from server : %s\n", times, buff);
        sleep(MCAST_INTERVAL);
    }
    /*退出广播组*/
    err = setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
    close(sock);
    return 0;
}

//UDP单播服务端
int UDPOneAddrSvr(int argc, char **argv)
{
    char *ip = "192.168.34.2";          //目的主机IP地址
    char *port1 = "2000";               //目的主机端口号
    int port = atoi(port1);
    int ret = 0;
    int udpfd;
    char buf[BUFFER_SIZE] = { "hello,world" }；
    struct sockaddr_in address;         //目的主机地址
    
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    udpfd = socket(PF_INET, SOCK_DGRAM, 0);   //使用UDP协议
    assert(udpfd >= 0);
    sendto(udpfd, buf, BUFFER_SIZE - 1, 0, (struct sockaddr *)&address, sizeof(address));
    return 0;
}

//UDP单播客户端
int UDPOneAddrCen(int argc, char **argv)
{
    char *ip = argv[1];                 //用户自身的IP
    int port = atoi(argv[2]);           //和服务器端口对应及2000
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

    printf("准备接收数据\n");
    ret = recvfrom(client_fd, buf, BUFFER_SIZE - 1, 0, (struct sockaddr *)&addr, &len);
    buf[ret] = '\0';
    printf("\n%s\n", buf);

    return 0;
}

#endif 

#endif



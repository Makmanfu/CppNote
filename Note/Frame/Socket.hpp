//----------------------------------------------------------------------
//
//      windows socket通讯框架
//                                          @阿甘  20160923
//----------------------------------------------------------------------


#include <Winsock2.h>           //注意顺序一定不能反了
#include <windows.h>
#pragma comment (lib ,"WS2_32.lib" )
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>

int Server(void)
{
    //准备加载Winsock库的版本, 注意高字节是副版本号
    WORD wVersionRequested = MAKEWORD(2, 2);
    //是一个返回值，指向WSADATA结构的指针，WSAStartup函数将其加载的库版本信息输入到这个结构体中。
    WSADATA wsaData;
    //加载套接字库,如果失败返回
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
        return 1;
    //判断高低字节是不是2,如果不是2.2的版本则退出
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        return 1;
    //创建流式套接字,基于TCP(SOCK_STREAM)
    SOCKET socSrv = socket(AF_INET, SOCK_STREAM, 0);
    //Socket地址结构体的创建
    SOCKADDR_IN addrSrv;
    //转换Unsigned long型为网络字节序格
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);    //inet_addr(m_ip);
    //指定地址簇
    addrSrv.sin_family = AF_INET;
    //指定端口号,除sin_family参数外,其它参数都是网络字节序,因此需要转换
    addrSrv.sin_port = htons(6000);
    //将套接字绑定到一个端口号和本地地址上
    bind(socSrv, (SOCKADDR*) & addrSrv, sizeof(SOCKADDR));          //必须用sizeof，strlen不行
    listen(socSrv, 5);
    printf("Server open listen!\n");
    //字义用来接收客户端Socket的结构体
    SOCKADDR_IN addrClient;
    //初始化参数,这个参数必须进行初始化，sizeof
    int len = sizeof(SOCKADDR);
    //循环等待接受客户端发送请求
    while (true)
    {
        //等待客户请求到来；当请求到来后，接受连接请求，
        //返回一个新的对应于此次连接的套接字（accept）。
        //此时程序在此发生阻塞
        SOCKET sockConn = accept(socSrv, (SOCKADDR*) &addrClient, &len);
        char sendBuf[100];
        //格式化输出
        sprintf(sendBuf, "%s to Server", inet_ntoa(addrClient.sin_addr));
        //用返回的套接字和客户端进行通信
        send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);       //多发送一个字节
        //接收数据
        char recvBuf[100];
        recv(sockConn, recvBuf, 100, 0);
        printf("%s\n", recvBuf);
        closesocket(sockConn);
    }
    return 0;
}

int Client(void)
{
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
        return 1;
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        WSACleanup();
        return 1;
    }
    SOCKET socketClient = socket(AF_INET, SOCK_STREAM, 0);
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(6000);
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");    //服务器端的IP地址
    connect(socketClient, (SOCKADDR*) &addrSrv, sizeof(SOCKADDR));
    char recvBuf[256];
    char sendBuf[] = "hello socket!";
    recv(socketClient, recvBuf, 256, 0);
    printf("%s\n", recvBuf);
    send(socketClient, sendBuf, strlen(sendBuf) + 1, 0);
    closesocket(socketClient);
    WSACleanup();
    return 0;
}

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-s") == 0)
                return Server();         //服务端
            else if (strcmp(argv[i], "-c") == 0)
                return Client();         //客户端
            else
                printf("cmd error\n");
        }
    }
    printf("%s [-s/-c] \n", argv[0]);
    return 0;
}


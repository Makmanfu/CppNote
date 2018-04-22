//----------------------------------------------------------------------
//
//      windows socketͨѶ���
//                                          @����  20160923
//----------------------------------------------------------------------


#include <Winsock2.h>           //ע��˳��һ�����ܷ���
#include <windows.h>
#pragma comment (lib ,"WS2_32.lib" )
#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>

int Server(void)
{
    //׼������Winsock��İ汾, ע����ֽ��Ǹ��汾��
    WORD wVersionRequested = MAKEWORD(2, 2);
    //��һ������ֵ��ָ��WSADATA�ṹ��ָ�룬WSAStartup����������صĿ�汾��Ϣ���뵽����ṹ���С�
    WSADATA wsaData;
    //�����׽��ֿ�,���ʧ�ܷ���
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
        return 1;
    //�жϸߵ��ֽ��ǲ���2,�������2.2�İ汾���˳�
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        return 1;
    //������ʽ�׽���,����TCP(SOCK_STREAM)
    SOCKET socSrv = socket(AF_INET, SOCK_STREAM, 0);
    //Socket��ַ�ṹ��Ĵ���
    SOCKADDR_IN addrSrv;
    //ת��Unsigned long��Ϊ�����ֽ����
    addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);    //inet_addr(m_ip);
    //ָ����ַ��
    addrSrv.sin_family = AF_INET;
    //ָ���˿ں�,��sin_family������,�����������������ֽ���,�����Ҫת��
    addrSrv.sin_port = htons(6000);
    //���׽��ְ󶨵�һ���˿ںźͱ��ص�ַ��
    bind(socSrv, (SOCKADDR*) & addrSrv, sizeof(SOCKADDR));          //������sizeof��strlen����
    listen(socSrv, 5);
    printf("Server open listen!\n");
    //�����������տͻ���Socket�Ľṹ��
    SOCKADDR_IN addrClient;
    //��ʼ������,�������������г�ʼ����sizeof
    int len = sizeof(SOCKADDR);
    //ѭ���ȴ����ܿͻ��˷�������
    while (true)
    {
        //�ȴ��ͻ������������������󣬽�����������
        //����һ���µĶ�Ӧ�ڴ˴����ӵ��׽��֣�accept����
        //��ʱ�����ڴ˷�������
        SOCKET sockConn = accept(socSrv, (SOCKADDR*) &addrClient, &len);
        char sendBuf[100];
        //��ʽ�����
        sprintf(sendBuf, "%s to Server", inet_ntoa(addrClient.sin_addr));
        //�÷��ص��׽��ֺͿͻ��˽���ͨ��
        send(sockConn, sendBuf, strlen(sendBuf) + 1, 0);       //�෢��һ���ֽ�
        //��������
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
    addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");    //�������˵�IP��ַ
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
                return Server();         //�����
            else if (strcmp(argv[i], "-c") == 0)
                return Client();         //�ͻ���
            else
                printf("cmd error\n");
        }
    }
    printf("%s [-s/-c] \n", argv[0]);
    return 0;
}


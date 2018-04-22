

#define _CRT_SECURE_NO_WARNINGS                 //����ʹ��_s�ĺ���
#define _WINSOCK_DEPRECATED_NO_WARNINGS         //����socket����
//�޸ı���  /sdl ������
//---------���°���˳�� WINSOCK.H��winsock2.h�е��ض������취-----------
#include <WinSock2.h>
#include <Windows.h>
#include "StdAfx.h"
#include "WinNetWork.h"
#pragma comment(lib,"ws2_32.lib")
#include <iostream>

//ping�����
#ifndef pingabout
#define DEF_BUF_SIZE 1024                                        //����Ĭ�ϻ���������
#define IP_HEADER_SIZE 20                                        //����IP��ͷ����
#define ICMP_HEADER_SIZE (sizeof(ICMP_HEADER))                   //����ICMP��ͷ����
#define ICMP_DATA_SIZE 32                                        //����ICMP��ʵ�����ݳ���
#define ICMP_PACK_SIZE (ICMP_HEADER_SIZE + ICMP_DATA_SIZE)       //����ICMP���ܳ���
typedef struct _ICMP_HEADER
{
    BYTE nType;                                             //����
    BYTE nCode;                                             //����
    USHORT nCheckSum;                                       //У���
    USHORT nId;                                             //��ʶ��������ID
    USHORT nSequence;                                       //���к�
    UINT nTimeStamp;                                        //��ѡ�����Ϊʱ�䣬���ڼ���ʱ��
} ICMP_HEADER, *PICMP_HEADER;                               //����ICMP��ͷ

char szInfo[DEF_BUF_SIZE] = { 0 };

USHORT GetCheckSum(LPBYTE lpBuf, DWORD dwSize)
{
    DWORD dwCheckSum = 0;
    USHORT* lpWord = (USHORT*)lpBuf;
    while (dwSize > 1)
    {
        dwCheckSum += *lpWord++;
        dwSize -= 2;
    }
    if (1 == dwSize)
        dwCheckSum += *((USHORT*)lpBuf);
    dwCheckSum = (dwCheckSum >> 16) + (dwCheckSum & 0xffff);
    return (USHORT)(~dwCheckSum);
}

/*ping��������*/
int testping_main(int argc, char* argv[])
{
    char szDestIp[DEF_BUF_SIZE] = { 0 };
    while (scanf("%s", szDestIp))
        PingPC(szDestIp);
    return 0;
}
#endif

class CPing
{
public:
    CPing() {};
    virtual ~CPing() {};
public:
    // ����ICMP��У��ֵ   // ����1��ICMP�������� // ����2��ICMP������
    USHORT GetCheckSum(LPBYTE lpBuf, DWORD dwSize)
    {
        DWORD   dwCheckSum = 0;
        USHORT* lpWord = (USHORT*)lpBuf;
        // �ۼ�
        while (dwSize > 1)
        {
            dwCheckSum += *lpWord++;
            dwSize -= 2;
        }
        // �������������
        if (dwSize == 1)
            dwCheckSum += *((LPBYTE)lpWord);
        // ��16λ�͵�16λ���
        dwCheckSum = (dwCheckSum >> 16) + (dwCheckSum & 0xFFFF);
        // ȡ��
        return (USHORT)(~dwCheckSum);
    };
    // ʵ��PING���� // ������Ŀ���ַIP�ַ�������ʽ�硰127.0.0.1��
    bool PingQuk(char* lpDestIp)
    {
        // ����Ŀ���ַ
        SOCKADDR_IN DestSockAddr;
        DestSockAddr.sin_family = AF_INET;
        DestSockAddr.sin_addr.s_addr = inet_addr(lpDestIp);
        DestSockAddr.sin_port = htons(0);
        // ����ICMP���������
        char ICMPPack[ICMP_PACK_SIZE] = { 0 };
        PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
        pICMPHeader->nType = 8;//   ->bType = 8 ;
        pICMPHeader->nCode = 0;//   ->bCode = 0 ;
        pICMPHeader->nId = (USHORT) ::GetCurrentProcessId();
        pICMPHeader->nCheckSum = 0;
        pICMPHeader->nTimeStamp = 0;
        memset(&(ICMPPack[ICMP_HEADER_SIZE]), 'E', ICMP_DATA_SIZE); // ������ݲ��֣���������
        // ��ʼ��WinSock��
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return FALSE;
        // ����ԭʼ�׽���
        SOCKET  RawSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (RawSock == INVALID_SOCKET)
            return FALSE;
        // ���ý��ճ�ʱΪ1�� 100
        int nTime = 1;
        int ret = ::setsockopt(RawSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTime, sizeof(nTime));
        int iCount = 0;
        char szRecvBuf[DEF_BUF_SIZE];
        SOCKADDR_IN SourSockAddr;
        for (int i = 0; i < 1/*< 2*/; i++)
        {
            pICMPHeader->nCheckSum = 0;             // ��ʼʱУ��ֵΪ0
            pICMPHeader->nSequence = i;             // ���
            // ����У��ֵ��У��ֵҪ��ICMP���ݱ���������ܼ���
            pICMPHeader->nCheckSum = GetCheckSum((LPBYTE)ICMPPack, ICMP_PACK_SIZE);
            // ����ICMP���ݰ�
            int nRet = ::sendto(RawSock, ICMPPack, ICMP_PACK_SIZE, 0, (SOCKADDR*)&DestSockAddr, sizeof(DestSockAddr));
            if (nRet == SOCKET_ERROR)
                return FALSE;
            // ����ICMP��Ӧ
            int nLen = sizeof(SourSockAddr);
            nRet = ::recvfrom(RawSock, szRecvBuf, DEF_BUF_SIZE, 0, (SOCKADDR*)&SourSockAddr, &nLen);
            if (nRet == SOCKET_ERROR)
            {
                if (::WSAGetLastError() == WSAETIMEDOUT)
                    continue;
                else
                    return FALSE;
            }
            // ����Ƿ�ǰ��������ICMP��Ӧ��
            PICMP_HEADER pRecvHeader = (PICMP_HEADER)(szRecvBuf + IP_HEADER_SIZE);
            if (pRecvHeader->nType != 0)
            {
                //TRACE ( "Not ICMP respond type!\n" ) ;
                return FALSE;
            }
            //          if ( pRecvHeader->nId != ::GetCurrentProcessId () )
            //          {
            //              //TRACE ( "not valid id!\n" ) ;
            //              return FALSE ;
            //          }
            iCount++;
        }
        closesocket(RawSock);
        WSACleanup();
        if (iCount >= 1)
            return TRUE;
        else
            return FALSE;
    }
    //��д�����ping
    bool Ping(string lpDestIp)
    {
        // ����Ŀ���ַ
        SOCKADDR_IN DestSockAddr;
        DestSockAddr.sin_family = AF_INET;
        DestSockAddr.sin_addr.s_addr = inet_addr(lpDestIp.c_str());
        DestSockAddr.sin_port = htons(0);
        // ����ICMP���������
        char ICMPPack[ICMP_PACK_SIZE] = { 0 };
        PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
        pICMPHeader->nType = 8;
        pICMPHeader->nCode = 0;
        pICMPHeader->nId = (USHORT) ::GetCurrentProcessId();
        pICMPHeader->nCheckSum = 0;
        pICMPHeader->nTimeStamp = 0;
        memset(&(ICMPPack[ICMP_HEADER_SIZE]), 'E', ICMP_DATA_SIZE); // ������ݲ��֣���������
        // ��ʼ��WinSock��
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return FALSE;
        // ����ԭʼ�׽���
        SOCKET  RawSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (RawSock == INVALID_SOCKET)
        {
            //AfxMessageBox("Create socket error!\n" ) ;
            return FALSE;
        }
        // ���ý��ճ�ʱΪ1��
        int nTime = 100;
        int ret = ::setsockopt(RawSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTime, sizeof(nTime));
        int iCount = 0;
        char szRecvBuf[DEF_BUF_SIZE];
        SOCKADDR_IN SourSockAddr;
        for (int i = 0; i < 2; i++)
        {
            pICMPHeader->nCheckSum = 0;             // ��ʼʱУ��ֵΪ0
            pICMPHeader->nSequence = i;             // ���
            pICMPHeader->nTimeStamp = ::GetTickCount();// ��ǰʱ��
            // ����У��ֵ��У��ֵҪ��ICMP���ݱ���������ܼ���
            pICMPHeader->nCheckSum = GetCheckSum((LPBYTE)ICMPPack, ICMP_PACK_SIZE);
            // ����ICMP���ݰ�
            int nRet = ::sendto(RawSock, ICMPPack, ICMP_PACK_SIZE, 0, (SOCKADDR*)&DestSockAddr, sizeof(DestSockAddr));
            if (nRet == SOCKET_ERROR)
            {
                //TRACE ( "sendto error!\n" ) ;
                return FALSE;
            }
            // ����ICMP��Ӧ
            int nLen = sizeof(SourSockAddr);
            nRet = ::recvfrom(RawSock, szRecvBuf, DEF_BUF_SIZE, 0, (SOCKADDR*)&SourSockAddr, &nLen);
            if (nRet == SOCKET_ERROR)
            {
                if (::WSAGetLastError() == WSAETIMEDOUT)
                {
                    //TRACE ( "Request Timeout\n" ) ;
                    continue;
                }
                else
                {
                    //TRACE ( "recvfrom error!\n" ) ;
                    return FALSE;
                }
            }
            // ����ICMP���ݱ���ʱ���
            int nTime = ::GetTickCount() - pICMPHeader->nTimeStamp;
            int nRealSize = nRet - IP_HEADER_SIZE - ICMP_HEADER_SIZE;
            if (nRealSize < 0)
            {
                //TRACE ( "To less recv bytes!\n" ) ;
                continue;
            }
            // ����Ƿ�ǰ��������ICMP��Ӧ��
            PICMP_HEADER pRecvHeader = (PICMP_HEADER)(szRecvBuf + IP_HEADER_SIZE);
            if (pRecvHeader->nType != 0)
            {
                //TRACE ( "Not ICMP respond type!\n" ) ;
                return FALSE;
            }
            if (pRecvHeader->nId != ::GetCurrentProcessId())
            {
                //TRACE ( "not valid id!\n" ) ;
                return FALSE;
            }
            //TRACE ( "%d bytes replay from %s : bytes=%d time=%dms\n",nRet, inet_ntoa(SourSockAddr.sin_addr), nRealSize, nTime ) ;
            iCount++;
            ::Sleep(nTime);
        }
        closesocket(RawSock);
        WSACleanup();
        if (iCount >= 1)
            return TRUE;
        else
            return FALSE;
    };
};

bool PingPC_c(char* flpDestIp)
{
    SOCKADDR_IN DestAddr;
    DestAddr.sin_family = AF_INET;
    DestAddr.sin_addr.S_un.S_addr = inet_addr(flpDestIp);
    DestAddr.sin_port = htons(0);
    //����ICMP�����
    char ICMPPack[ICMP_PACK_SIZE] = { 0 };
    PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
    pICMPHeader->nType = 8;
    pICMPHeader->nCode = 0;
    pICMPHeader->nId = (USHORT) ::GetCurrentProcessId();
    pICMPHeader->nCheckSum = 0;
    pICMPHeader->nTimeStamp = 0;
    memset(&(ICMPPack[ICMP_HEADER_SIZE]), 'E', ICMP_DATA_SIZE);
    //��ʼ��WinSock
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
        return FALSE;
    //������ʼ�׽���
    SOCKET RawSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (INVALID_SOCKET == RawSock)
    {
        printf("create raw socket error\n");
        return FALSE;
    }
    int nTime = 1000;
    int nRet = ::setsockopt(RawSock, SOL_SOCKET, SO_RCVTIMEO, (char*)(&nTime), sizeof(nTime));
    char szRecvBuf[DEF_BUF_SIZE];
    SOCKADDR_IN    SourSockAddr;
    for (int i = 0; i < 4; i++)
    {
        pICMPHeader->nCheckSum = 0;
        pICMPHeader->nTimeStamp = ::GetTickCount();
        pICMPHeader->nSequence = i;
        pICMPHeader->nCheckSum = GetCheckSum((LPBYTE)ICMPPack, ICMP_PACK_SIZE);
        int nRet = ::sendto(RawSock, ICMPPack, ICMP_PACK_SIZE, 0, (SOCKADDR*)&DestAddr, sizeof(DestAddr));
        if (nRet == SOCKET_ERROR)
        {
            printf("sendto error!\n");
            return FALSE;
        }
        // ����ICMP��Ӧ
        int nLen = sizeof(SourSockAddr);
        nRet = ::recvfrom(RawSock, szRecvBuf, DEF_BUF_SIZE, 0, (SOCKADDR*)&SourSockAddr, &nLen);
        if (nRet == SOCKET_ERROR)
        {
            if (::WSAGetLastError() == WSAETIMEDOUT)
            {
                printf("Request Timeout\n");
                continue;
            }
            else
            {
                printf("recvfrom error!\n");
                return FALSE;
            }
        }
        int nTime = ::GetTickCount() - pICMPHeader->nTimeStamp;
        int nRealSize = nRet - IP_HEADER_SIZE - ICMP_HEADER_SIZE;
        if (nRealSize < 0)
        {
            printf("To less recv bytes!\n");
            continue;
        }
        // ����Ƿ�ǰ��������ICMP��Ӧ��
        PICMP_HEADER pRecvHeader = (PICMP_HEADER)(szRecvBuf + IP_HEADER_SIZE);
        if (pRecvHeader->nType != 0)
        {
            printf("Not ICMP respond type!\n");
            return FALSE;
        }
        if (pRecvHeader->nId != ::GetCurrentProcessId())
        {
            printf("not valid id!\n");
            return FALSE;
        }
        printf("%d bytes replay from %s : bytes=%d time=%dms\n", \
               nRet, inet_ntoa(SourSockAddr.sin_addr), nRealSize, nTime);
        ::Sleep(1000);
    }
    closesocket(RawSock);
    WSACleanup();
    return TRUE;
}

bool PingPC(string IP, bool quk)
{
    CPing ping;
    if (quk)
        return ping.PingQuk(const_cast<char*>(IP.c_str()));
    else
        return ping.Ping(IP);
}

char* GetLocalIP(void)
{
    WSADATA wsaDataqq;
    WSAStartup(MAKEWORD(1, 1), &wsaDataqq);
    PHOSTENT hostinfo;
    char name[512] = { 0 };
    char* tmp_ip = "";
    in_addr dest;
    if (gethostname(name, sizeof(name)) != 0 || (hostinfo = gethostbyname(name)) == NULL)
        return false;
    for (int a = 0; hostinfo->h_addr_list[a] != NULL; a++)
    {
        memcpy(&dest, hostinfo->h_addr_list[a], hostinfo->h_length);
        //      if (a==0)
        //          MessageBox(NULL,inet_ntoa(dest),"��ʾ",MB_OK);   //printf("����IP��:\t%s\n",inet_ntoa(dest));
        //      else
        //          MessageBox(NULL,inet_ntoa(dest),"��ʾ",MB_OK);
        tmp_ip = inet_ntoa(dest);
    }
    WSACleanup();
    return tmp_ip;
}

char* GetHostNameFromIP(char* IP)
{
    in_addr addr;
    addr.S_un.S_addr = inet_addr(IP);
    hostent* remoteHost = gethostbyaddr((char*)&addr.S_un.S_un_b, 4, AF_INET);
    if (remoteHost != NULL)
        return remoteHost->h_name;
    else
        return "NULL";
}

string SockSendData(char* fstrdata, string IP, int fport)
{
    if (!PingPC(IP)) return "0"; //��pingһ��ip�Ͷ˿�
    WORD wVersionRequested;             //׼������Winsock��İ汾,ע����ֽ��Ǹ��汾��
    WSADATA wsaData;                    //��һ������ֵ,ָ��WSADATA�ṹ��ָ��,WSAStartup����������صĿ�汾��Ϣ���뵽����ṹ����
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) return "0";
    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
    {
        WSACleanup();                  //������صİ汾����ȷ,���ء���Ҫע����Ǹ��ֽ��Ǹ��汾�ţ����ֽ������汾��
        return "0";
    }
    //����socket����
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    //unsigned long ul=1;
    //if(ioctlsocket(sockClient,FIONBIO,(unsigned long *)&ul)==SOCKET_ERROR)//���óɷ�����ģʽ��
    //  return "0";
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = inet_addr(IP.c_str());
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(fport);
    //��������
    if (connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) != 0)
    {
        WSACleanup();
        return "0";                 //˵�����Ӵ���  �������� ����Ҫ�ȴ�
    }
    //��������
    int ff = send(sockClient, fstrdata, int(strlen(fstrdata)) + 1, 0);
    //���ܻ���
    char recvBuf[50];
    recv(sockClient, recvBuf, 50, 0);
    //string result =bytestohexstring(recvBuf);
    string result = "";
    cout << "//���ܶԷ�����:" << result.data() << endl;            //printf("%s\n",recvBuf);
    closesocket(sockClient);
    WSACleanup();
    return result;                  //return const_cast<char*>(result.data());
}

int SendExeData(const char* name, char* data)
{
    return 0;
    /*
        //��ȡ������(���ͷ�)���
        HWND hSendWindow = GetConsoleWindow();
        if (hSendWindow == NULL)
        return -1;
        //��ý��ܳ�����
        const char szDlgTitle[] = "RecvMessage";
        HWND hRecvWindow = FindWindow(NULL, szDlgTitle);
        if (hRecvWindow == NULL)
        return -1;

        //��ȡconsole���ھ������
        typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();
        PROCGETCONSOLEWINDOW GetConsoleWindow;
        HMODULE hKernel32 = GetModuleHandle(_T("kernel32"));
        GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");
        HWND cmdWnd = GetConsoleWindow();
        //��������
        char szSendBuf[100];
        time_t  timenow;
        COPYDATASTRUCT CopyData;



        for (int i = 0; i < 10; i++)
        {
        time(&timenow);
        sprintf(szSendBuf, "%s", ctime(&timenow));//ע�⣬ctime()���ص��ַ����������'\n'
        CopyData.dwData = i;
        CopyData.cbData = strlen(szSendBuf);
        szSendBuf[CopyData.cbData - 1] = '\0';
        CopyData.lpData = szSendBuf;
        //�����GUI������
        SendMessage(hRecvWindow, WM_COPYDATA, (WPARAM)hSendWindow, (LPARAM)&CopyData);
        //�������г�����
        //SendMessage(cmdWnd, WM_COPYDATA, (WPARAM)hSendWindow, (LPARAM)&CopyData);
        printf("%s\n", szSendBuf);
        Sleep(1000);
        }
    */
}

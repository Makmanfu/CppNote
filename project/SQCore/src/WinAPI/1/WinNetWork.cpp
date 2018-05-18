

#define _CRT_SECURE_NO_WARNINGS                 //消除使用_s的函数
#define _WINSOCK_DEPRECATED_NO_WARNINGS         //消除socket问题
//修改编译  /sdl 就行了
//---------以下包含顺序 WINSOCK.H与winsock2.h中的重定义解决办法-----------
#include <WinSock2.h>
#include <Windows.h>
#include "StdAfx.h"
#include "WinNetWork.h"
#pragma comment(lib,"ws2_32.lib")
#include <iostream>

//ping的相关
#ifndef pingabout
#define DEF_BUF_SIZE 1024                                        //定义默认缓冲区长度
#define IP_HEADER_SIZE 20                                        //定义IP包头长度
#define ICMP_HEADER_SIZE (sizeof(ICMP_HEADER))                   //定义ICMP包头长度
#define ICMP_DATA_SIZE 32                                        //定义ICMP包实体数据长度
#define ICMP_PACK_SIZE (ICMP_HEADER_SIZE + ICMP_DATA_SIZE)       //定义ICMP包总长度
typedef struct _ICMP_HEADER
{
    BYTE nType;                                             //类型
    BYTE nCode;                                             //代码
    USHORT nCheckSum;                                       //校验和
    USHORT nId;                                             //标识，本进程ID
    USHORT nSequence;                                       //序列号
    UINT nTimeStamp;                                        //可选项，这里为时间，用于计算时间
} ICMP_HEADER, *PICMP_HEADER;                               //定义ICMP包头

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

/*ping测试例子*/
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
    // 计算ICMP包校验值   // 参数1：ICMP包缓冲区 // 参数2：ICMP包长度
    USHORT GetCheckSum(LPBYTE lpBuf, DWORD dwSize)
    {
        DWORD   dwCheckSum = 0;
        USHORT* lpWord = (USHORT*)lpBuf;
        // 累加
        while (dwSize > 1)
        {
            dwCheckSum += *lpWord++;
            dwSize -= 2;
        }
        // 如果长度是奇数
        if (dwSize == 1)
            dwCheckSum += *((LPBYTE)lpWord);
        // 高16位和低16位相加
        dwCheckSum = (dwCheckSum >> 16) + (dwCheckSum & 0xFFFF);
        // 取反
        return (USHORT)(~dwCheckSum);
    };
    // 实现PING程序 // 参数：目标地址IP字符串，格式如“127.0.0.1”
    bool PingQuk(char* lpDestIp)
    {
        // 设置目标地址
        SOCKADDR_IN DestSockAddr;
        DestSockAddr.sin_family = AF_INET;
        DestSockAddr.sin_addr.s_addr = inet_addr(lpDestIp);
        DestSockAddr.sin_port = htons(0);
        // 创建ICMP回显请求包
        char ICMPPack[ICMP_PACK_SIZE] = { 0 };
        PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
        pICMPHeader->nType = 8;//   ->bType = 8 ;
        pICMPHeader->nCode = 0;//   ->bCode = 0 ;
        pICMPHeader->nId = (USHORT) ::GetCurrentProcessId();
        pICMPHeader->nCheckSum = 0;
        pICMPHeader->nTimeStamp = 0;
        memset(&(ICMPPack[ICMP_HEADER_SIZE]), 'E', ICMP_DATA_SIZE); // 填充数据部分，内容任意
        // 初始化WinSock库
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return FALSE;
        // 创建原始套接字
        SOCKET  RawSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (RawSock == INVALID_SOCKET)
            return FALSE;
        // 设置接收超时为1秒 100
        int nTime = 1;
        int ret = ::setsockopt(RawSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTime, sizeof(nTime));
        int iCount = 0;
        char szRecvBuf[DEF_BUF_SIZE];
        SOCKADDR_IN SourSockAddr;
        for (int i = 0; i < 1/*< 2*/; i++)
        {
            pICMPHeader->nCheckSum = 0;             // 初始时校验值为0
            pICMPHeader->nSequence = i;             // 序号
            // 计算校验值，校验值要在ICMP数据报创建完才能计算
            pICMPHeader->nCheckSum = GetCheckSum((LPBYTE)ICMPPack, ICMP_PACK_SIZE);
            // 发送ICMP数据包
            int nRet = ::sendto(RawSock, ICMPPack, ICMP_PACK_SIZE, 0, (SOCKADDR*)&DestSockAddr, sizeof(DestSockAddr));
            if (nRet == SOCKET_ERROR)
                return FALSE;
            // 接收ICMP响应
            int nLen = sizeof(SourSockAddr);
            nRet = ::recvfrom(RawSock, szRecvBuf, DEF_BUF_SIZE, 0, (SOCKADDR*)&SourSockAddr, &nLen);
            if (nRet == SOCKET_ERROR)
            {
                if (::WSAGetLastError() == WSAETIMEDOUT)
                    continue;
                else
                    return FALSE;
            }
            // 检测是否当前所发出的ICMP响应包
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
    //改写成类的ping
    bool Ping(string lpDestIp)
    {
        // 设置目标地址
        SOCKADDR_IN DestSockAddr;
        DestSockAddr.sin_family = AF_INET;
        DestSockAddr.sin_addr.s_addr = inet_addr(lpDestIp.c_str());
        DestSockAddr.sin_port = htons(0);
        // 创建ICMP回显请求包
        char ICMPPack[ICMP_PACK_SIZE] = { 0 };
        PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
        pICMPHeader->nType = 8;
        pICMPHeader->nCode = 0;
        pICMPHeader->nId = (USHORT) ::GetCurrentProcessId();
        pICMPHeader->nCheckSum = 0;
        pICMPHeader->nTimeStamp = 0;
        memset(&(ICMPPack[ICMP_HEADER_SIZE]), 'E', ICMP_DATA_SIZE); // 填充数据部分，内容任意
        // 初始化WinSock库
        WORD wVersionRequested = MAKEWORD(2, 2);
        WSADATA wsaData;
        if (WSAStartup(wVersionRequested, &wsaData) != 0)
            return FALSE;
        // 创建原始套接字
        SOCKET  RawSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
        if (RawSock == INVALID_SOCKET)
        {
            //AfxMessageBox("Create socket error!\n" ) ;
            return FALSE;
        }
        // 设置接收超时为1秒
        int nTime = 100;
        int ret = ::setsockopt(RawSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&nTime, sizeof(nTime));
        int iCount = 0;
        char szRecvBuf[DEF_BUF_SIZE];
        SOCKADDR_IN SourSockAddr;
        for (int i = 0; i < 2; i++)
        {
            pICMPHeader->nCheckSum = 0;             // 初始时校验值为0
            pICMPHeader->nSequence = i;             // 序号
            pICMPHeader->nTimeStamp = ::GetTickCount();// 当前时间
            // 计算校验值，校验值要在ICMP数据报创建完才能计算
            pICMPHeader->nCheckSum = GetCheckSum((LPBYTE)ICMPPack, ICMP_PACK_SIZE);
            // 发送ICMP数据包
            int nRet = ::sendto(RawSock, ICMPPack, ICMP_PACK_SIZE, 0, (SOCKADDR*)&DestSockAddr, sizeof(DestSockAddr));
            if (nRet == SOCKET_ERROR)
            {
                //TRACE ( "sendto error!\n" ) ;
                return FALSE;
            }
            // 接收ICMP响应
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
            // 计算ICMP数据报的时间差
            int nTime = ::GetTickCount() - pICMPHeader->nTimeStamp;
            int nRealSize = nRet - IP_HEADER_SIZE - ICMP_HEADER_SIZE;
            if (nRealSize < 0)
            {
                //TRACE ( "To less recv bytes!\n" ) ;
                continue;
            }
            // 检测是否当前所发出的ICMP响应包
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
    //创建ICMP请求包
    char ICMPPack[ICMP_PACK_SIZE] = { 0 };
    PICMP_HEADER pICMPHeader = (PICMP_HEADER)ICMPPack;
    pICMPHeader->nType = 8;
    pICMPHeader->nCode = 0;
    pICMPHeader->nId = (USHORT) ::GetCurrentProcessId();
    pICMPHeader->nCheckSum = 0;
    pICMPHeader->nTimeStamp = 0;
    memset(&(ICMPPack[ICMP_HEADER_SIZE]), 'E', ICMP_DATA_SIZE);
    //初始化WinSock
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(wVersionRequested, &wsaData) != 0)
        return FALSE;
    //创建初始套接字
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
        // 接收ICMP响应
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
        // 检测是否当前所发出的ICMP响应包
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
        //          MessageBox(NULL,inet_ntoa(dest),"提示",MB_OK);   //printf("本地IP是:\t%s\n",inet_ntoa(dest));
        //      else
        //          MessageBox(NULL,inet_ntoa(dest),"提示",MB_OK);
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
    if (!PingPC(IP)) return "0"; //先ping一下ip和端口
    WORD wVersionRequested;             //准备加载Winsock库的版本,注意高字节是副版本号
    WSADATA wsaData;                    //是一个返回值,指向WSADATA结构的指针,WSAStartup函数将其加载的库版本信息输入到这个结构体中
    int err;
    wVersionRequested = MAKEWORD(1, 1);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) return "0";
    if (LOBYTE(wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
    {
        WSACleanup();                  //如果加载的版本不正确,返回。需要注意的是高字节是副版本号，低字节是主版本号
        return "0";
    }
    //建立socket对象
    SOCKET sockClient = socket(AF_INET, SOCK_STREAM, 0);
    //unsigned long ul=1;
    //if(ioctlsocket(sockClient,FIONBIO,(unsigned long *)&ul)==SOCKET_ERROR)//设置成非阻塞模式。
    //  return "0";
    SOCKADDR_IN addrSrv;
    addrSrv.sin_addr.S_un.S_addr = inet_addr(IP.c_str());
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(fport);
    //尝试连接
    if (connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)) != 0)
    {
        WSACleanup();
        return "0";                 //说明连接错误  立即返回 不需要等待
    }
    //发送命令
    int ff = send(sockClient, fstrdata, int(strlen(fstrdata)) + 1, 0);
    //接受回馈
    char recvBuf[50];
    recv(sockClient, recvBuf, 50, 0);
    //string result =bytestohexstring(recvBuf);
    string result = "";
    cout << "//接受对方反馈:" << result.data() << endl;            //printf("%s\n",recvBuf);
    closesocket(sockClient);
    WSACleanup();
    return result;                  //return const_cast<char*>(result.data());
}

int SendExeData(const char* name, char* data)
{
    return 0;
    /*
        //获取本程序(发送方)句柄
        HWND hSendWindow = GetConsoleWindow();
        if (hSendWindow == NULL)
        return -1;
        //获得接受程序句柄
        const char szDlgTitle[] = "RecvMessage";
        HWND hRecvWindow = FindWindow(NULL, szDlgTitle);
        if (hRecvWindow == NULL)
        return -1;

        //获取console窗口句柄方法
        typedef HWND(WINAPI *PROCGETCONSOLEWINDOW)();
        PROCGETCONSOLEWINDOW GetConsoleWindow;
        HMODULE hKernel32 = GetModuleHandle(_T("kernel32"));
        GetConsoleWindow = (PROCGETCONSOLEWINDOW)GetProcAddress(hKernel32, "GetConsoleWindow");
        HWND cmdWnd = GetConsoleWindow();
        //待发数据
        char szSendBuf[100];
        time_t  timenow;
        COPYDATASTRUCT CopyData;



        for (int i = 0; i < 10; i++)
        {
        time(&timenow);
        sprintf(szSendBuf, "%s", ctime(&timenow));//注意，ctime()返回的字符串后面带了'\n'
        CopyData.dwData = i;
        CopyData.cbData = strlen(szSendBuf);
        szSendBuf[CopyData.cbData - 1] = '\0';
        CopyData.lpData = szSendBuf;
        //向接受GUI程序发送
        SendMessage(hRecvWindow, WM_COPYDATA, (WPARAM)hSendWindow, (LPARAM)&CopyData);
        //向命令行程序发送
        //SendMessage(cmdWnd, WM_COPYDATA, (WPARAM)hSendWindow, (LPARAM)&CopyData);
        printf("%s\n", szSendBuf);
        Sleep(1000);
        }
    */
}

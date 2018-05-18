


#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "client.h"


#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


//传递的线程参数
// typedef  struct _paramthread
// {
//  SOCKET  s;      //套接字
//  BOOL    conning;//连接状态
//  list<CClient*>  *listClient;//客户端链表
// }PARAMTHREAD, *pPARAMTHREAD;


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CServerDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CServerDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_bConning = FALSE;
    m_hReqAndData = NULL;
    InitializeCriticalSection(&m_cs);
}

CServerDlg::~CServerDlg()
{
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CServerDlg)
    DDX_Control(pDX, IDC_IPADDRESS, m_ctlIP);
    DDX_Control(pDX, IDC_STATIC_TIP, m_ctlTip);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
    //{{AFX_MSG_MAP(CServerDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_CLOSE()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_STARTUP, OnButtonStartup)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CServerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Add "About..." menu item to system menu.
    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);
    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    // TODO: Add extra initialization here
    m_ctlIP.SetAddress(127, 0, 0, 1);//设置服务器地址
    InitSocket();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
        CDialog::OnSysCommand(nID, lParam);
}

void CServerDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
        CDialog::OnPaint();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CServerDlg::InitSocket(void)
{
    WORD    wVersionRequested;  //请求的Windows Sockets 实现版本
    WSADATA wsaData;            //返回协商结果
    int     nErrCode;           //调用API函数的返回值
    wVersionRequested = MAKEWORD(2, 2);
    nErrCode = WSAStartup(wVersionRequested, &wsaData);
    if (0 != nErrCode)
        return;
    //创建套接字
    m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sServer)
        return;
    //获取系统默认的发送数据缓冲区大小
    unsigned int uiRcvBuf;
    int uiRcvBufLen = sizeof(uiRcvBuf);
    nErrCode = getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF, (char*)&uiRcvBuf, &uiRcvBufLen);
    if (SOCKET_ERROR == nErrCode)
        return;
    //设置系统发送数据缓冲区为默认值的BUF_TIMES倍
    uiRcvBuf *= BUF_TIMES;
    nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF, (char*)&uiRcvBuf, uiRcvBufLen);
    if (SOCKET_ERROR == nErrCode)
        AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));
    //检查设置系统发送数据缓冲区是否成功
    unsigned int uiNewRcvBuf;
    getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF, (char*)&uiNewRcvBuf, &uiRcvBufLen);
    if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
    {
        AfxMessageBox(_T("修改系统发送数据缓冲区失败！"));;
    }
}

void CServerDlg::UnInitSocke(void)
{
    closesocket(m_sServer);
    WSACleanup();
}

void CServerDlg::OnButtonStartup()
{
    UpdateData(TRUE);               //更新对话框
    int     reVal;                  //返回值
    DWORD   dwServIP;
    m_ctlIP.GetAddress(dwServIP);   //得到服务器IP
    //服务器套接字地址
    SOCKADDR_IN servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(dwServIP);
    servAddr.sin_port = htons(SERVERPORT);
    //绑定服务器
    reVal = bind(m_sServer, (sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
    if (SOCKET_ERROR == reVal)
    {
        AfxMessageBox(_T("服务器绑定失败"), MB_OK, 0);
        closesocket(m_sServer);
        WSACleanup();
        return;
    }
    else
    {
        m_ctlTip.SetWindowText(_T("服务器绑定成功！"));
        UpdateData(false);
    }
    //监听
    reVal = listen(m_sServer, SOMAXCONN);
    if (SOCKET_ERROR == reVal)
    {
        AfxMessageBox(_T("服务器监听失败！"), MB_OK, 0);
        closesocket(m_sServer);
        WSACleanup();
        return;
    }
    m_bConning = TRUE;//修改服务器状态
    //创建接受和处理客户端请求线程
    DWORD dwThread;
    m_hReqAndData = CreateThread(NULL, 0, DirAndFileSizeServcieThread, this, 0, &dwThread);
    CloseHandle(m_hReqAndData);
    //更新界面
    (CButton*)GetDlgItem(IDC_BUTTON_STARTUP)->EnableWindow(FALSE);  //启动按钮无效
    m_ctlIP.EnableWindow(FALSE);                                    //服务器地址控件无效
    m_ctlTip.SetWindowText(_T("服务器启动成功！"));                  //显示成功信息
    UpdateData(FALSE);                                              //初始化对话框
}

DWORD WINAPI CServerDlg::DirAndFileSizeServcieThread(void* pParam)
{
    CServerDlg* pServer = (CServerDlg*)pParam;
    SOCKET sListen = pServer->GetSocket();//获得服务器监听套接字
    FD_SET allSockfd;              //服务器所有套接字集合
    FD_ZERO(&allSockfd);           //清空集合
    FD_SET(sListen, &allSockfd);   //将监听套接字加入该集合
    FD_SET readfd;                 //定义满足可读套接字集合
    FD_SET writefd;                //定义满足可写套接字集合
    while (pServer->IsConnenting())//服务器运行状态
    {
        FD_ZERO(&readfd);          //清空可读集合
        FD_ZERO(&writefd);         //清空可写集合
        readfd = allSockfd;        //赋值
        writefd = allSockfd;       //赋值
        //更新界面信息
        pServer->ShowClientNumberInfor(allSockfd.fd_count);
        //无限期等待套接字满足条件
        int nRet = select(0, &readfd, &writefd, NULL, NULL);
        if (nRet > 0)
        {
            //遍历所有套接字集合
            for (int i = 0; i < allSockfd.fd_count; i++)
            {
                //存在可读的套接字
                if (FD_ISSET(allSockfd.fd_array[i], &readfd))
                {
                    //接受客户端连接请求
                    if (allSockfd.fd_array[i] == sListen)
                    {
                        SOCKADDR_IN    addrClient;
                        int            nAddrLen = sizeof(addrClient);
                        SOCKET sClient = accept(sListen, (sockaddr*)&addrClient, &nAddrLen);
                        //新建一个 CClient类实例
                        CClient* pClient = new CClient(sClient, pServer);
                        //加入客户端管理链表中
                        pServer->AddClient(pClient);
                        //加入套接字集合
                        FD_SET(sClient, &allSockfd);
                        //更新界面信息
                        pServer->ShowClientNumberInfor(allSockfd.fd_count);
                    }
                    else  //接收客户端数据
                    {
                        //得到CClient类的实例
                        CClient* pClient = pServer->GetClient(allSockfd.fd_array[i]);
                        if (pClient != NULL)
                        {
                            //接收数据
                            BOOL bRet = pClient->RecvData();
                            //接收数据错误或者客户端关闭套接字
                            if (FALSE == bRet)
                            {
                                //取出套接字
                                SOCKET sTemp = allSockfd.fd_array[i];
                                //从集合中删除
                                FD_CLR(allSockfd.fd_array[i], &allSockfd);
                                //从客户端管理链表中删除该客户端
                                pServer->DeleteClient(sTemp);
                                //更新界面信息
                                pServer->ShowClientNumberInfor(allSockfd.fd_count);
                            }
                        }
                    }//else
                }//if
                //存在的可写套接字
                if (FD_ISSET(allSockfd.fd_array[i], &writefd))
                {
                    //得到CClient类的实例
                    CClient* pClient = pServer->GetClient(allSockfd.fd_array[i]);
                    if (pClient != NULL)
                    {
                        //发送数据
                        BOOL bRet = pClient->SendData();
                        if (FALSE == bRet)//发送数据失败
                        {
                            //被删除的套接字
                            SOCKET sDelete = allSockfd.fd_array[i];
                            //从集合中删除该套接字
                            FD_CLR(allSockfd.fd_array[i], &allSockfd);
                            //从客户端管理链表中删除该客户端
                            pServer->DeleteClient(sDelete);
                            //更新界面信息
                            pServer->ShowClientNumberInfor(allSockfd.fd_count);
                        }
                    }
                }
            }//for
        }//if
        Sleep(THREAD_SLEEP);//线程睡眠
    }//while
    pServer->DeleteAllClient();//删除所有的客户端
    return 0;
}

CClient* CServerDlg::GetClient(SOCKET s)
{
    CClient* pClient = NULL;
    EnterCriticalSection(&m_cs);
    list<CClient*>::iterator it = m_listClient.begin();
    for (it; it != m_listClient.end(); ++it)
    {
        pClient = *it;
        if (s == pClient->GetSocket())//找到
            break;
    }
    LeaveCriticalSection(&m_cs);
    return pClient;
}

void CServerDlg::DeleteClient(SOCKET s)
{
    EnterCriticalSection(&m_cs);
    //遍历该链表
    list<CClient*>::iterator it = m_listClient.begin();
    for (it; it != m_listClient.end();)
    {
        CClient* pClient = (CClient*)*it;
        if (pClient->GetSocket() == s)//相同的套接字
        {
            m_listClient.erase(it++);//从链表中删除
            delete pClient;            //释放内存
            pClient = NULL;
            break;
        }
        else
            it++;
    }
    LeaveCriticalSection(&m_cs);
}

void CServerDlg::DeleteAllClient(void)
{
    EnterCriticalSection(&m_cs);
    //遍历该链表
    list<CClient*>::iterator it = m_listClient.begin();
    for (it; it != m_listClient.end();)
    {
        CClient* pClient = (CClient*)*it;
        m_listClient.erase(it++);
        delete pClient;
        pClient = NULL;
    }
    m_listClient.clear();
    LeaveCriticalSection(&m_cs);
}

void CServerDlg::AddClient(CClient* pClient)
{
    EnterCriticalSection(&m_cs);
    m_listClient.push_back(pClient);
    LeaveCriticalSection(&m_cs);
}

void CServerDlg::OnClose()
{
    m_bConning = FALSE;                             //设置服务器状态
    m_ctlTip.SetWindowText(_T("服务器退出！"));      //更新界面
    Sleep(THREAD_SLEEP * 100);                      //给子线程退出时间
    DeleteCriticalSection(&m_cs);                   //删除临界区对象
    UnInitSocke();                                  //释放套接字资源
    CDialog::OnClose();
}

void CServerDlg::ShowClientNumberInfor(int nNum)
{
    CString strTip;
    strTip.Format("目前共有%d客户连接。", nNum - 1);
    m_ctlTip.SetWindowText(strTip);
}

void CServerDlg::ShowReqDirInfor(CString strDir)
{
    m_ctlTip.SetWindowText(strDir);
}

void CServerDlg::ShowFileULInfor(CString strFileName)
{
    m_ctlTip.SetWindowText(strFileName);
}


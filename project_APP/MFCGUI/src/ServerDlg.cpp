


#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "client.h"


#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif


//���ݵ��̲߳���
// typedef  struct _paramthread
// {
//  SOCKET  s;      //�׽���
//  BOOL    conning;//����״̬
//  list<CClient*>  *listClient;//�ͻ�������
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
    m_ctlIP.SetAddress(127, 0, 0, 1);//���÷�������ַ
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
    WORD    wVersionRequested;  //�����Windows Sockets ʵ�ְ汾
    WSADATA wsaData;            //����Э�̽��
    int     nErrCode;           //����API�����ķ���ֵ
    wVersionRequested = MAKEWORD(2, 2);
    nErrCode = WSAStartup(wVersionRequested, &wsaData);
    if (0 != nErrCode)
        return;
    //�����׽���
    m_sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == m_sServer)
        return;
    //��ȡϵͳĬ�ϵķ������ݻ�������С
    unsigned int uiRcvBuf;
    int uiRcvBufLen = sizeof(uiRcvBuf);
    nErrCode = getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF, (char*)&uiRcvBuf, &uiRcvBufLen);
    if (SOCKET_ERROR == nErrCode)
        return;
    //����ϵͳ�������ݻ�����ΪĬ��ֵ��BUF_TIMES��
    uiRcvBuf *= BUF_TIMES;
    nErrCode = setsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF, (char*)&uiRcvBuf, uiRcvBufLen);
    if (SOCKET_ERROR == nErrCode)
        AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));
    //�������ϵͳ�������ݻ������Ƿ�ɹ�
    unsigned int uiNewRcvBuf;
    getsockopt(m_sServer, SOL_SOCKET, SO_SNDBUF, (char*)&uiNewRcvBuf, &uiRcvBufLen);
    if (SOCKET_ERROR == nErrCode || uiNewRcvBuf != uiRcvBuf)
    {
        AfxMessageBox(_T("�޸�ϵͳ�������ݻ�����ʧ�ܣ�"));;
    }
}

void CServerDlg::UnInitSocke(void)
{
    closesocket(m_sServer);
    WSACleanup();
}

void CServerDlg::OnButtonStartup()
{
    UpdateData(TRUE);               //���¶Ի���
    int     reVal;                  //����ֵ
    DWORD   dwServIP;
    m_ctlIP.GetAddress(dwServIP);   //�õ�������IP
    //�������׽��ֵ�ַ
    SOCKADDR_IN servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = htonl(dwServIP);
    servAddr.sin_port = htons(SERVERPORT);
    //�󶨷�����
    reVal = bind(m_sServer, (sockaddr*)&servAddr, sizeof(SOCKADDR_IN));
    if (SOCKET_ERROR == reVal)
    {
        AfxMessageBox(_T("��������ʧ��"), MB_OK, 0);
        closesocket(m_sServer);
        WSACleanup();
        return;
    }
    else
    {
        m_ctlTip.SetWindowText(_T("�������󶨳ɹ���"));
        UpdateData(false);
    }
    //����
    reVal = listen(m_sServer, SOMAXCONN);
    if (SOCKET_ERROR == reVal)
    {
        AfxMessageBox(_T("����������ʧ�ܣ�"), MB_OK, 0);
        closesocket(m_sServer);
        WSACleanup();
        return;
    }
    m_bConning = TRUE;//�޸ķ�����״̬
    //�������ܺʹ���ͻ��������߳�
    DWORD dwThread;
    m_hReqAndData = CreateThread(NULL, 0, DirAndFileSizeServcieThread, this, 0, &dwThread);
    CloseHandle(m_hReqAndData);
    //���½���
    (CButton*)GetDlgItem(IDC_BUTTON_STARTUP)->EnableWindow(FALSE);  //������ť��Ч
    m_ctlIP.EnableWindow(FALSE);                                    //��������ַ�ؼ���Ч
    m_ctlTip.SetWindowText(_T("�����������ɹ���"));                  //��ʾ�ɹ���Ϣ
    UpdateData(FALSE);                                              //��ʼ���Ի���
}

DWORD WINAPI CServerDlg::DirAndFileSizeServcieThread(void* pParam)
{
    CServerDlg* pServer = (CServerDlg*)pParam;
    SOCKET sListen = pServer->GetSocket();//��÷����������׽���
    FD_SET allSockfd;              //�����������׽��ּ���
    FD_ZERO(&allSockfd);           //��ռ���
    FD_SET(sListen, &allSockfd);   //�������׽��ּ���ü���
    FD_SET readfd;                 //��������ɶ��׽��ּ���
    FD_SET writefd;                //���������д�׽��ּ���
    while (pServer->IsConnenting())//����������״̬
    {
        FD_ZERO(&readfd);          //��տɶ�����
        FD_ZERO(&writefd);         //��տ�д����
        readfd = allSockfd;        //��ֵ
        writefd = allSockfd;       //��ֵ
        //���½�����Ϣ
        pServer->ShowClientNumberInfor(allSockfd.fd_count);
        //�����ڵȴ��׽�����������
        int nRet = select(0, &readfd, &writefd, NULL, NULL);
        if (nRet > 0)
        {
            //���������׽��ּ���
            for (int i = 0; i < allSockfd.fd_count; i++)
            {
                //���ڿɶ����׽���
                if (FD_ISSET(allSockfd.fd_array[i], &readfd))
                {
                    //���ܿͻ�����������
                    if (allSockfd.fd_array[i] == sListen)
                    {
                        SOCKADDR_IN    addrClient;
                        int            nAddrLen = sizeof(addrClient);
                        SOCKET sClient = accept(sListen, (sockaddr*)&addrClient, &nAddrLen);
                        //�½�һ�� CClient��ʵ��
                        CClient* pClient = new CClient(sClient, pServer);
                        //����ͻ��˹���������
                        pServer->AddClient(pClient);
                        //�����׽��ּ���
                        FD_SET(sClient, &allSockfd);
                        //���½�����Ϣ
                        pServer->ShowClientNumberInfor(allSockfd.fd_count);
                    }
                    else  //���տͻ�������
                    {
                        //�õ�CClient���ʵ��
                        CClient* pClient = pServer->GetClient(allSockfd.fd_array[i]);
                        if (pClient != NULL)
                        {
                            //��������
                            BOOL bRet = pClient->RecvData();
                            //�������ݴ�����߿ͻ��˹ر��׽���
                            if (FALSE == bRet)
                            {
                                //ȡ���׽���
                                SOCKET sTemp = allSockfd.fd_array[i];
                                //�Ӽ�����ɾ��
                                FD_CLR(allSockfd.fd_array[i], &allSockfd);
                                //�ӿͻ��˹���������ɾ���ÿͻ���
                                pServer->DeleteClient(sTemp);
                                //���½�����Ϣ
                                pServer->ShowClientNumberInfor(allSockfd.fd_count);
                            }
                        }
                    }//else
                }//if
                //���ڵĿ�д�׽���
                if (FD_ISSET(allSockfd.fd_array[i], &writefd))
                {
                    //�õ�CClient���ʵ��
                    CClient* pClient = pServer->GetClient(allSockfd.fd_array[i]);
                    if (pClient != NULL)
                    {
                        //��������
                        BOOL bRet = pClient->SendData();
                        if (FALSE == bRet)//��������ʧ��
                        {
                            //��ɾ�����׽���
                            SOCKET sDelete = allSockfd.fd_array[i];
                            //�Ӽ�����ɾ�����׽���
                            FD_CLR(allSockfd.fd_array[i], &allSockfd);
                            //�ӿͻ��˹���������ɾ���ÿͻ���
                            pServer->DeleteClient(sDelete);
                            //���½�����Ϣ
                            pServer->ShowClientNumberInfor(allSockfd.fd_count);
                        }
                    }
                }
            }//for
        }//if
        Sleep(THREAD_SLEEP);//�߳�˯��
    }//while
    pServer->DeleteAllClient();//ɾ�����еĿͻ���
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
        if (s == pClient->GetSocket())//�ҵ�
            break;
    }
    LeaveCriticalSection(&m_cs);
    return pClient;
}

void CServerDlg::DeleteClient(SOCKET s)
{
    EnterCriticalSection(&m_cs);
    //����������
    list<CClient*>::iterator it = m_listClient.begin();
    for (it; it != m_listClient.end();)
    {
        CClient* pClient = (CClient*)*it;
        if (pClient->GetSocket() == s)//��ͬ���׽���
        {
            m_listClient.erase(it++);//��������ɾ��
            delete pClient;            //�ͷ��ڴ�
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
    //����������
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
    m_bConning = FALSE;                             //���÷�����״̬
    m_ctlTip.SetWindowText(_T("�������˳���"));      //���½���
    Sleep(THREAD_SLEEP * 100);                      //�����߳��˳�ʱ��
    DeleteCriticalSection(&m_cs);                   //ɾ���ٽ�������
    UnInitSocke();                                  //�ͷ��׽�����Դ
    CDialog::OnClose();
}

void CServerDlg::ShowClientNumberInfor(int nNum)
{
    CString strTip;
    strTip.Format("Ŀǰ����%d�ͻ����ӡ�", nNum - 1);
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



#pragma once

class CClient;

class CServerDlg : public CDialog
{
public:
    CServerDlg(CWnd* pParent = NULL);
    virtual ~CServerDlg();
    // Dialog Data
    //{{AFX_DATA(CServerDlg)
    enum { IDD = IDD_SERVER_DIALOG };
    CIPAddressCtrl  m_ctlIP;
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CServerDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
public:
    //显示客户端数量信息
    void ShowClientNumberInfor(int nNum);
    //显示客户端请求目录
    void ShowReqDirInfor(CString strDir);
    //显示客户端请求文件
    void ShowFileULInfor(CString strFileName);
    //接受和处理客户端请求线程函数
    static DWORD WINAPI DirAndFileSizeServcieThread(void* pParam);
    //判断服务器的连接状态
    BOOL IsConnenting(void)
    {
        return m_bConning;
    }

    // Generated message map functions
    //{{AFX_MSG(CServerDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnButtonStartup();
    afx_msg void OnClose();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

protected:
    //初始化socket
    void InitSocket(void);
    //释放套接字占用的资源
    void UnInitSocke(void);
    //根据套接字得到客户端
    CClient* GetClient(SOCKET s);
    //删除客户端节点
    void DeleteClient(SOCKET s);
    //添加客户端节点
    void AddClient(CClient* pClient);
    //删除所有客户端节点
    void DeleteAllClient(void);
    //获得套接字
    SOCKET GetSocket(void)
    {
        return m_sServer;
    }


protected:
    HICON m_hIcon;

protected:
    SOCKET  m_sServer;                  //服务器监听套接结字
    CStatic m_ctlTip;                   //提示信息控件
    BOOL    m_bConning;                 //服务器连接状态
    HANDLE  m_hReqAndData;              //接收和处理客户端请求线程句柄
    CRITICAL_SECTION m_cs;              //保护链表的临界区对象
    list<CClient*> m_listClient;        //管理客户端的链表
};




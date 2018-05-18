
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
    //��ʾ�ͻ���������Ϣ
    void ShowClientNumberInfor(int nNum);
    //��ʾ�ͻ�������Ŀ¼
    void ShowReqDirInfor(CString strDir);
    //��ʾ�ͻ��������ļ�
    void ShowFileULInfor(CString strFileName);
    //���ܺʹ���ͻ��������̺߳���
    static DWORD WINAPI DirAndFileSizeServcieThread(void* pParam);
    //�жϷ�����������״̬
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
    //��ʼ��socket
    void InitSocket(void);
    //�ͷ��׽���ռ�õ���Դ
    void UnInitSocke(void);
    //�����׽��ֵõ��ͻ���
    CClient* GetClient(SOCKET s);
    //ɾ���ͻ��˽ڵ�
    void DeleteClient(SOCKET s);
    //��ӿͻ��˽ڵ�
    void AddClient(CClient* pClient);
    //ɾ�����пͻ��˽ڵ�
    void DeleteAllClient(void);
    //����׽���
    SOCKET GetSocket(void)
    {
        return m_sServer;
    }


protected:
    HICON m_hIcon;

protected:
    SOCKET  m_sServer;                  //�����������׽ӽ���
    CStatic m_ctlTip;                   //��ʾ��Ϣ�ؼ�
    BOOL    m_bConning;                 //����������״̬
    HANDLE  m_hReqAndData;              //���պʹ���ͻ��������߳̾��
    CRITICAL_SECTION m_cs;              //����������ٽ�������
    list<CClient*> m_listClient;        //����ͻ��˵�����
};




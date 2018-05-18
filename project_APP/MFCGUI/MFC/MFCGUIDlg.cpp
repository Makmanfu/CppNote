
#include "stdafx.h"
#include "MFCGUI.h"
#include "MFCGUIDlg.h"
#include "afxdialogex.h"


BEGIN_MESSAGE_MAP(CMFCGUIDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_MOUSEWHEEL()
    ON_WM_LBUTTONDOWN()
    ON_WM_QUERYDRAGICON()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CMFCGUIDlg::CMFCGUIDlg(CWnd* pParent): CDialogEx(CMFCGUIDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCGUIDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BOOL CMFCGUIDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();
    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);         // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��
    // TODO:  �ڴ���Ӷ���ĳ�ʼ������
    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCGUIDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
        CDialogEx::OnPaint();
}

HCURSOR CMFCGUIDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void CMFCGUIDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); //ʵ�ֿͻ����϶�
    CDialog::OnLButtonDown(nFlags, point);
}

BOOL CMFCGUIDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    //MessageBoxA(NULL, "�����ֻ�������", "WIN32", MB_OK);
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}



void CMFCGUIDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    m_game.OnKeyDown(nChar, nRepCnt, nFlags);
    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

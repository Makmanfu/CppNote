
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
    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);         // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标
    // TODO:  在此添加额外的初始化代码
    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCGUIDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文
        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // 绘制图标
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
    PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); //实现客户区拖动
    CDialog::OnLButtonDown(nFlags, point);
}

BOOL CMFCGUIDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: Add your message handler code here and/or call default
    //MessageBoxA(NULL, "鼠标滚轮滑动触发", "WIN32", MB_OK);
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}



void CMFCGUIDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    m_game.OnKeyDown(nChar, nRepCnt, nFlags);
    CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

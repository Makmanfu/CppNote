

#include "stdafx.h"
#include "mfcgui.h"
#include "ffmpegdlg.h"


BEGIN_MESSAGE_MAP(CFFmpegdlg, CDialog)
    //{{AFX_MSG_MAP(CMFCGUIDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CFFmpegdlg::CFFmpegdlg(CWnd* pParent /*=NULL*/)
    : CDialog(CFFmpegdlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFFmpegdlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BOOL CFFmpegdlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFFmpegdlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void CFFmpegdlg::OnPaint()
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
    {
        CDC* pDC = GetDC();        //HDC hdc = ::GetDC(m_hWnd);
        ////直接画在dc上
        //CQrcode *qrcode = CQrcode::getMinimumQRCode(_T("xhamigua"), ErrorCorrectLevel_H);
        //qrcode->Draw(pDC, 8, 8);
        CDialog::OnPaint();
    }
}

HCURSOR CFFmpegdlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CFFmpegdlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); //实现客户区拖动
    CDialog::OnLButtonDown(nFlags, point);
}



#include "stdafx.h"
#include "mfcgui.h"
#include "vlcdlg.h"

BEGIN_MESSAGE_MAP(CVLCdlg, CDialog)
    //{{AFX_MSG_MAP(CMFCGUIDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_WM_HSCROLL()
    ON_WM_SHOWWINDOW()
    ON_WM_GETMINMAXINFO()
    ON_BN_CLICKED(IDC_BUTTON_PLAY, OnBnClickedButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnBnClickedButtonPause)
    ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
    ON_BN_CLICKED(IDC_BUTTON_MUTE, OnBnClickedButtonMute)
    ON_WM_SIZE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

static void HandleVLCEvents(const VLCEvent* pEvt, void* pUserData)
{
    CVLCdlg* pDlg = reinterpret_cast<CVLCdlg*>(pUserData);

    switch (pEvt->type)
    {
    case libvlc_MediaPlayerTimeChanged:
        TRACE("VLC_EVT_TIME_CHANGED: new_time %d[ms]\n", pEvt->u.media_player_time_changed.new_time);
        if (pDlg)
            pDlg->UpdatePosition();
        break;
    }
}

CVLCdlg::CVLCdlg(CWnd* pParent /*=NULL*/)
    : CDialog(CVLCdlg::IDD, pParent)
    , muteFlag_(false)
    , length_(0)
    , created_(false)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVLCdlg::UpdatePosition()
{
    length_ = vlcPlayer_.GetLength();
    int64_t newPosition = vlcPlayer_.GetTime();

    CTimeSpan length(static_cast<time_t>(length_ / 1000));
    CTimeSpan actualPosition(static_cast<time_t>(newPosition / 1000));

    CString lengthString;
    lengthString.Format(_T("%02d:%02d:%02d/%02d:%02d:%02d"),
        actualPosition.GetHours(), actualPosition.GetMinutes(), actualPosition.GetSeconds(),
        length.GetHours(), length.GetMinutes(), length.GetSeconds());

    mediaPosition_.SetWindowText(lengthString);

    int newSliderPos = length_ ? static_cast<int>((static_cast<double>(newPosition) / static_cast<double>(length_)* 1000)) : 0;
    mediaSlider_.SetPos(newSliderPos);
}

void CVLCdlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_MEDIA_CONTROL, mediaControlGroup_);
    DDX_Control(pDX, IDC_BUTTON_PLAY, buttonPlay_);
    DDX_Control(pDX, IDC_BUTTON_PAUSE, buttonPause_);
    DDX_Control(pDX, IDC_BUTTON_STOP, buttonStop_);
    DDX_Control(pDX, IDC_BUTTON_MUTE, buttonMute_);
    DDX_Control(pDX, IDC_BUTTON_LOAD, buttonOpen_);
    DDX_Control(pDX, IDC_STATIC_VLC, vlcControl_);
    DDX_Control(pDX, IDC_SLIDER_MEDIA, mediaSlider_);
    DDX_Control(pDX, IDC_STATIC_POSITION, mediaPosition_);
    DDX_Control(pDX, IDC_STATIC_VOLUME_TEXT, volumeText_);
    DDX_Control(pDX, IDC_SLIDER_VOLUME, volumeSlider_);
    DDX_Control(pDX, IDC_STATIC_VOLUME, volumeLevel_);
}

BOOL CVLCdlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    // ADD+++++++++++++++++++++++++++++++++++++++++++++

    vlcPlayer_.SetOutputWindow((void*)vlcControl_.GetSafeHwnd());
    vlcPlayer_.SetEventHandler(&HandleVLCEvents, this);
    volumeSlider_.SetRange(0, 100);
    mediaSlider_.SetRange(0, 1000);
    int volume = vlcPlayer_.GetVolume();
    volumeSlider_.SetPos(volume);
    CString volumeString;
    volumeString.Format(_T("%d"), volume);
    volumeLevel_.SetWindowText(volumeString);

    //设置按钮图片
    buttonPlay_.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PLAY), IMAGE_ICON, 16, 16, LR_SHARED));
    buttonPause_.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAUSE), IMAGE_ICON, 16, 16, LR_SHARED));
    buttonStop_.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_STOP), IMAGE_ICON, 16, 16, LR_SHARED));
    noMuteIcon_ = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MUTE), IMAGE_ICON, 16, 16, LR_SHARED);
    muteIcon_ = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_MUTE2), IMAGE_ICON, 16, 16, LR_SHARED);
    buttonMute_.SetIcon(noMuteIcon_);
    buttonOpen_.SetIcon((HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_EJECT), IMAGE_ICON, 16, 16, LR_SHARED));

    listDlgItems_.AddTail(&mediaControlGroup_);
    listDlgItems_.AddTail(&mediaSlider_);
    listDlgItems_.AddTail(&mediaPosition_);
    listDlgItems_.AddTail(&buttonPlay_);
    listDlgItems_.AddTail(&buttonPause_);
    listDlgItems_.AddTail(&buttonStop_);
    listDlgItems_.AddTail(&buttonMute_);
    listDlgItems_.AddTail(&buttonOpen_);
    listDlgItems_.AddTail(&volumeText_);
    listDlgItems_.AddTail(&volumeSlider_);
    listDlgItems_.AddTail(&volumeLevel_);

    created_ = true;

    CRect myRect;
    GetClientRect(&myRect);
    currentDlgSize_.cx = myRect.Width();
    currentDlgSize_.cy = myRect.Height();

    GetWindowRect(&myRect);
    minDlgSize_.cx = myRect.Width();
    minDlgSize_.cy = myRect.Height();
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVLCdlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    CDialog::OnSysCommand(nID, lParam);
}

void CVLCdlg::OnPaint()
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
        CDialog::OnPaint();
        vlcControl_.RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
    }

}

HCURSOR CVLCdlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CVLCdlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y)); //实现客户区拖动
    CDialog::OnLButtonDown(nFlags, point);
}

void CVLCdlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CSliderCtrl* pSlider = reinterpret_cast<CSliderCtrl*>(pScrollBar);

    if (*pSlider == volumeSlider_)
    {
        int pos = volumeSlider_.GetPos();
        vlcPlayer_.SetVolume(pos);
        CString volume;
        volume.Format(_T("%d"), pos);
        volumeLevel_.SetWindowText(volume);
        muteFlag_ = false;
        buttonMute_.SetIcon(noMuteIcon_);
    }

    if (*pSlider == mediaSlider_)
    {
        int pos = mediaSlider_.GetPos();
        vlcPlayer_.SetTime(length_ / 1000 * pos);
    }

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CVLCdlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CDialog::OnShowWindow(bShow, nStatus);
}

void CVLCdlg::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);
    RecalcLayout(cx, cy);
    Invalidate();
}

void CVLCdlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
    if (created_)
    {
        lpMMI->ptMinTrackSize.x = minDlgSize_.cx;
        lpMMI->ptMinTrackSize.y = minDlgSize_.cy;
    }
}

void CVLCdlg::OnBnClickedButtonPlay()
{
    vlcPlayer_.Play();
}

void CVLCdlg::OnBnClickedButtonStop()
{
    vlcPlayer_.Stop();
    length_ = 0;
    mediaSlider_.SetPos(0);
    CString length;
    mediaPosition_.GetWindowText(length);
    length = CString("[00:00:00") + length.Right(10);
    mediaPosition_.SetWindowText(length);
}

void CVLCdlg::OnBnClickedButtonPause()
{
    vlcPlayer_.Pause();
}

void CVLCdlg::OnBnClickedButtonMute()
{
    muteFlag_ = !vlcPlayer_.GetMute();
    (muteFlag_) ? buttonMute_.SetIcon(muteIcon_) : buttonMute_.SetIcon(noMuteIcon_);
    vlcPlayer_.Mute(muteFlag_);
}

void CVLCdlg::OnBnClickedButtonLoad()
{
    CFileDialog dlgFile(TRUE);
    if (dlgFile.DoModal() == IDOK)
    {
        CString file = dlgFile.GetPathName();
        //++++
        char str[1024];
        wsprintfA(str, "%S", file);
        vlcPlayer_.OpenMedia(str);
        //++++
        //vlcPlayer_.OpenMedia((LPCTSTR)file);
        vlcPlayer_.Play();

        //显示播放文件绝对路径
        if (file.GetLength() > 80)
            file = CString("...") + file.Right(80);
        mediaControlGroup_.SetWindowText(file);
    }
}

void CVLCdlg::RecalcLayout(int cx, int cy)
{
    if (!created_)
        return;

    if (cx == 0 && cy == 0)
    {
        CRect rect;
        GetClientRect(&rect);

        cx = rect.Width();
        cy = rect.Height();
    }

    int controlsHeight = 120;
    int newHeight = cy - controlsHeight - 7;

    // Resize the VLC static control
    //vlcControl_.MoveWindow(CRect(7, 7, cx - 7, newHeight));
    vlcControl_.MoveWindow(CRect(0, 0, cx , newHeight));

    // calculate the displacement values
    RECT rect;
    mediaControlGroup_.GetWindowRect(&rect);
    ScreenToClient(&rect);
    int offsetX = rect.left;
    int groupWidth = rect.right - rect.left;
    int moveX = (cx - groupWidth) / 2 - offsetX;
    int moveY = cy - currentDlgSize_.cy;

    // Move all controls in list
    POSITION pos = listDlgItems_.GetHeadPosition();
    for (int i = 0; i < listDlgItems_.GetCount(); i++)
    {
        CWnd* pMyWnd = listDlgItems_.GetNext(pos);
        if (pMyWnd)
        {
            CRect myPos;
            pMyWnd->GetWindowRect(&myPos);
            ScreenToClient(myPos);
            pMyWnd->SetWindowPos(0, myPos.left + moveX, myPos.top + moveY, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
        }
    }
    currentDlgSize_.cx = cx;
    currentDlgSize_.cy = cy;
}


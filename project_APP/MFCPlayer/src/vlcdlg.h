
#ifndef VLCDLG_H
#define VLCDLG_H
#include "VLCWrapper.h"

class CVLCdlg : public CDialog
{
    VLCWrapper vlcPlayer_;
    bool muteFlag_;
    int64_t length_;
    CList<CWnd*, CWnd*> listDlgItems_;
    CSize currentDlgSize_;
    CSize minDlgSize_;
    HICON noMuteIcon_;
    HICON muteIcon_;
    bool created_;

    //{{AFX_DATA(CVlcDialogDlg)
    CButton mediaControlGroup_;
    CButton buttonPlay_;
    CButton buttonPause_;
    CButton buttonStop_;
    CButton buttonMute_;
    CButton buttonOpen_;
    CStatic vlcControl_;
    CStatic volumeText_;
    CStatic volumeLevel_;
    CStatic mediaPosition_;
    CSliderCtrl mediaSlider_;
    CSliderCtrl volumeSlider_;
    //}}AFX_DATA
public:
    CVLCdlg(CWnd* pParent = NULL);   // standard constructor

    void UpdatePosition();

    enum { IDD = IDD_MFCGUI_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
    HICON m_hIcon;

    // Generated message map functions
    //{{AFX_MSG(CMFCGUIDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg HCURSOR OnQueryDragIcon();

    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
    afx_msg void OnBnClickedButtonPlay();
    afx_msg void OnBnClickedButtonStop();
    afx_msg void OnBnClickedButtonPause();
    afx_msg void OnBnClickedButtonMute();
    afx_msg void OnBnClickedButtonLoad();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
    void RecalcLayout(int cx, int cy);
public:
    afx_msg void OnStnClickedStaticVlc();
};

#endif


#ifndef MFCGUIDLG_H
#define MFCGUIDLG_H

#include "Game2048.h"

class CMFCGUIDlg : public CDialogEx
{
public:
    CMFCGUIDlg(CWnd* pParent = NULL);
    // 对话框数据
    enum { IDD = IDD_MFCGUI_DIALOG };
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV支持
protected:
    HICON m_hIcon;
    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    DECLARE_MESSAGE_MAP()
private:
    Game2048 m_game;
public:

};

#endif
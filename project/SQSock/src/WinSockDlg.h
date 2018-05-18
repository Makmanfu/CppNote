//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------
#ifndef WINSOCKDLG_H
#define WINSOCKDLG_H

#include "WinGUI.h"
#include "CopyDATA.h"

class WinSockDlg : public XqWindowDlg
{
public:
    //主程序入口
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    WinSockDlg();
public:
    //初始化
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //消息事件
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗口COPYDATA
    virtual void WMCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //处理通信模式下拉
    void CBoxTypeSelect(ModeCommunication id);
    //处理运行模式下拉
    void CBoxCSSelect(ModeRunWork id);
private:
    HWND hWCBoxType, hWCEdit, hWCEditLOG, hWEdtIP, hWCBoxCS, hWSVRINFO, hWCENTIT;
    //CopyDATA通信模式
    CopyDATA Cdata;
    char sendip[256];       //发送ip
};




#endif

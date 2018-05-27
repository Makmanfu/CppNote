
#ifndef WSOCKFILEDLG_H
#define WSOCKFILEDLG_H

#include "WinGUI.h"
#include "CopyDATA.h"

class WSockFileDlg : public XqWindowDlg
{
public:
    //主程序入口
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    WSockFileDlg();
public:
    //初始化
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //消息事件
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};




#endif

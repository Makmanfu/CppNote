
#ifndef WSOCKFILEDLG_H
#define WSOCKFILEDLG_H

#include "WinGUI.h"
#include "CopyDATA.h"

class WSockFileDlg : public XqWindowDlg
{
public:
    //���������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    WSockFileDlg();
public:
    //��ʼ��
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��Ϣ�¼�
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};




#endif

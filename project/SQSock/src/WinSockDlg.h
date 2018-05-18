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
    //���������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    WinSockDlg();
public:
    //��ʼ��
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��Ϣ�¼�
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //����COPYDATA
    virtual void WMCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //����ͨ��ģʽ����
    void CBoxTypeSelect(ModeCommunication id);
    //��������ģʽ����
    void CBoxCSSelect(ModeRunWork id);
private:
    HWND hWCBoxType, hWCEdit, hWCEditLOG, hWEdtIP, hWCBoxCS, hWSVRINFO, hWCENTIT;
    //CopyDATAͨ��ģʽ
    CopyDATA Cdata;
    char sendip[256];       //����ip
};




#endif

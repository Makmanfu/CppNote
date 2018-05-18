

#include "stdafx.h"
#include "WSockFileDlg.h"
#include <CommCtrl.h>

int WSockFileDlg::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    WSockFileDlg MsgDlg;
    DialogBoxA(hInstance, MAKEINTRESOURCEA(IDD_DLGFILE), NULL, WSockFileDlg::Main_Proc);
    return 0;
}

WSockFileDlg::WSockFileDlg() : XqWindowDlg(IDD_DLGFILE)
{
    WSockFileDlg::pDlg = this;
}

void WSockFileDlg::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowDlg::WMINITDIALOG(hWnd, uMsg, wParam, lParam);
    InitTitIcon("文件服务器");
    //设置IP显示
    int nIP = MAKEIPADDRESS(127, 0, 0, 1);
    SendMessage(GetDlgItem(hWnd, IDC_IP_SVR_ADDRESS), IPM_SETADDRESS, 0, nIP);
    SendMessage(GetDlgItem(hWnd, IDC_IP_CEN_ADDRESS), IPM_SETADDRESS, 0, nIP);
}

void WSockFileDlg::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);
    // 分析菜单选择:
    switch (ctrlID)
    {
        case IDC_BTNSVR:
            MessageBoxA(NULL, "启动服务器！", "TITLE", MB_OK);
            break;
        case IDC_BTNCEN:
            MessageBoxA(NULL, "连接服务器！", "TITLE", MB_OK); 
            break;
        default:
            break;
    }
}






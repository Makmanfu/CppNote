

#include "stdafx.h"
#include "WinSockDlg.h"

int WinSockDlg::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    WinSockDlg MsgDlg;
    DialogBoxA(hInstance, MAKEINTRESOURCEA(IDD_DLGSOCK), NULL, WinSockDlg::Main_Proc);
    return 0;
}

WinSockDlg::WinSockDlg() : XqWindowDlg(IDD_DLGSOCK)
{
    WinSockDlg::pDlg = this;
}

void WinSockDlg::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowDlg::WMINITDIALOG(hWnd, uMsg, wParam, lParam);
    InitTitIcon("SQSock");
    //获取句柄
    hWCBoxType = GetDlgItem(hWnd, IDC_CBOXTYPE);
    hWCEdit = GetDlgItem(hWnd, IDC_EDITMSG);
    hWCEditLOG = GetDlgItem(hWnd, IDC_EDITLOG);
    hWEdtIP = GetDlgItem(hWnd, IDC_EDITIP);
    hWCBoxCS = GetDlgItem(hWnd, IDC_CBOXCS);
    hWSVRINFO = GetDlgItem(hWnd, IDC_SVRINFO);
    hWCENTIT = GetDlgItem(hWnd, IDC_CENTIT);
    //设置通信类型框
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"COPYDATA");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"IPC");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"HTTP");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"TCP");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"UDP");
    ComboBox_SetCurSel(hWCBoxType, -1);          //选中第一个
    //设置运行模式框
    SendMessageA(hWCBoxCS, CB_ADDSTRING, 0, (LPARAM)"服务端");
    SendMessageA(hWCBoxCS, CB_ADDSTRING, 0, (LPARAM)"客户端");
    ComboBox_SetCurSel(hWCBoxCS, -1);            //选中第一个
    //默认设置标识框
    SetWindowTextA(hWEdtIP, (LPCSTR)"192.168.168.001:8080");
    SetWindowTextA(hWSVRINFO, (LPCSTR)"当前运行于服务器模式!");
    SetWindowTextA(hWCENTIT, (LPCSTR)"对方IP地址:");
}

void WinSockDlg::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);
    // 分析菜单选择:
    switch (ctrlID)
    {
        case IDC_CBOXTYPE:
            if (wmEvent == CBN_SELCHANGE)
                CBoxTypeSelect((ModeCommunication)ComboBox_GetCurSel(hWCBoxType));
            break;
        case IDC_CBOXCS:
            if (wmEvent == CBN_SELCHANGE)
                CBoxCSSelect((ModeRunWork)ComboBox_GetCurSel(hWCBoxCS));
            break;
        case IDC_BTNSENT:
        {
            MsgCDATA ds;
            GetDlgItemTextA(hWnd, IDC_EDITMSG, ds.data, 256);
            GetDlgItemTextA(hWnd, IDC_EDITIP, sendip, 256);
            Cdata.SendRecv(sendip, ds);
        }
        break;
        default:
            break;
    }
}

void WinSockDlg::WMCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    MsgCDATA ds;
    Cdata.RecvCOPYDATA(hWnd, uMsg, wParam, lParam, ds);    //MessageBoxA(NULL, ds.data, "BOX", MB_OK);
    SendMessageA(hWCEditLOG, EM_SETSEL, (WPARAM) - 1, (LPARAM) - 1); // (0, -1)表示全选, (-1，任意)表示全不选
    SendMessageA(hWCEditLOG, EM_REPLACESEL, FALSE, (LPARAM)ds.data); //在编辑框中追加数据
    SendMessageA(hWCEditLOG, EM_REPLACESEL, FALSE, (LPARAM)"\n");
    SendMessageA(hWCEditLOG, EM_SCROLLCARET, 0, 0);
}

void WinSockDlg::CBoxTypeSelect(ModeCommunication id)
{
    switch (id)
    {
        case EnumCOPYDATA:
        default:
            SetWindowTextA(hWCENTIT, (LPCSTR)"对方窗口名称:");
            break;
        case EnumIPC:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"当前运行于IPC!");
            break;
        case EnumHTTP:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"当前运行于HTTP!");
            break;
        case EnumTCP:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"当前运行于TCP!");
            break;
        case EnumUDP:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"当前运行于UDP!");
            break;
    }
}

void WinSockDlg::CBoxCSSelect(ModeRunWork id)
{
    char tmpchar[256];
    switch (id)
    {
        case 0:
        default:
            InitTitIcon("SQSockSvr");
            GetWindowTextA(GetDlgHWND(), tmpchar, 256);
            SetWindowTextA(hWEdtIP, (LPCSTR)tmpchar);
            break;
        case 1:
            InitTitIcon("SQSockCen");
            GetWindowTextA(GetDlgHWND(), tmpchar, 256);
            SetWindowTextA(hWEdtIP, (LPCSTR)tmpchar);
            break;
    }
}






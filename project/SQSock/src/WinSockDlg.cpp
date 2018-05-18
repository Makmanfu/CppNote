

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
    //��ȡ���
    hWCBoxType = GetDlgItem(hWnd, IDC_CBOXTYPE);
    hWCEdit = GetDlgItem(hWnd, IDC_EDITMSG);
    hWCEditLOG = GetDlgItem(hWnd, IDC_EDITLOG);
    hWEdtIP = GetDlgItem(hWnd, IDC_EDITIP);
    hWCBoxCS = GetDlgItem(hWnd, IDC_CBOXCS);
    hWSVRINFO = GetDlgItem(hWnd, IDC_SVRINFO);
    hWCENTIT = GetDlgItem(hWnd, IDC_CENTIT);
    //����ͨ�����Ϳ�
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"COPYDATA");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"IPC");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"HTTP");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"TCP");
    SendMessageA(hWCBoxType, CB_ADDSTRING, 0, (LPARAM)"UDP");
    ComboBox_SetCurSel(hWCBoxType, -1);          //ѡ�е�һ��
    //��������ģʽ��
    SendMessageA(hWCBoxCS, CB_ADDSTRING, 0, (LPARAM)"�����");
    SendMessageA(hWCBoxCS, CB_ADDSTRING, 0, (LPARAM)"�ͻ���");
    ComboBox_SetCurSel(hWCBoxCS, -1);            //ѡ�е�һ��
    //Ĭ�����ñ�ʶ��
    SetWindowTextA(hWEdtIP, (LPCSTR)"192.168.168.001:8080");
    SetWindowTextA(hWSVRINFO, (LPCSTR)"��ǰ�����ڷ�����ģʽ!");
    SetWindowTextA(hWCENTIT, (LPCSTR)"�Է�IP��ַ:");
}

void WinSockDlg::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);
    // �����˵�ѡ��:
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
    SendMessageA(hWCEditLOG, EM_SETSEL, (WPARAM) - 1, (LPARAM) - 1); // (0, -1)��ʾȫѡ, (-1������)��ʾȫ��ѡ
    SendMessageA(hWCEditLOG, EM_REPLACESEL, FALSE, (LPARAM)ds.data); //�ڱ༭����׷������
    SendMessageA(hWCEditLOG, EM_REPLACESEL, FALSE, (LPARAM)"\n");
    SendMessageA(hWCEditLOG, EM_SCROLLCARET, 0, 0);
}

void WinSockDlg::CBoxTypeSelect(ModeCommunication id)
{
    switch (id)
    {
        case EnumCOPYDATA:
        default:
            SetWindowTextA(hWCENTIT, (LPCSTR)"�Է���������:");
            break;
        case EnumIPC:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"��ǰ������IPC!");
            break;
        case EnumHTTP:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"��ǰ������HTTP!");
            break;
        case EnumTCP:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"��ǰ������TCP!");
            break;
        case EnumUDP:
            SetWindowTextA(hWSVRINFO, (LPCSTR)"��ǰ������UDP!");
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






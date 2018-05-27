

#include "stdafx.h"
#include "BootLoad.h"
#include <windowsx.h>
#include <CommCtrl.h>
#include "FunAll.h"

CWINParam* BootLoad::mWIN = new CWINParam();
CCMDParam* BootLoad::mCMD = new CCMDParam();
WINProc BootLoad::WINfp = NULL;
CMDProc BootLoad::CMDfp = NULL;
int BootLoad::cmdargc = 0;
char** BootLoad::cmdargv = NULL;
HINSTANCE BootLoad::guihInst = NULL;
HINSTANCE BootLoad::guihPrevInst = NULL;
char* BootLoad::guilpCmd = NULL;
int BootLoad::guinCmd = 0;

int BootLoad::WIN32MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    //���²���
    guihInst = hInstance;
    guihPrevInst = hPrevInstance;
    guilpCmd = lpCmdLine;
    guinCmd = nCmdShow;
    //Ĭ��GUI��
    mWIN->DefaultFunAdd(&BootLoad::WINMAIN);
    //ȫ������ָ��ע���
    FunAddWINAll();
    //���г���
    mWIN->GetCmdFunProc(string(lpCmdLine), &WINfp);
    if (WINfp != NULL)
    {
        return WINfp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    }
    //��Ϣѭ��
    MSG msg;
    return MSGLOOP(msg);
}

int BootLoad::CWinMain(int argc, char** argv)
{
    //����windows���淽��
    static HINSTANCE hInstance = GetModuleHandle(NULL);
    //ע��*argv ȱʧ�ж�
    return WIN32MAIN(hInstance, NULL, *argv, argc);
}

int BootLoad::CMDMAIN(int argc, char** argv)
{
    //���²���
    cmdargc = argc;
    cmdargv = argv;
    mCMD->SetExeName(argv[0]);
    //��ӽ����ϵĳ���
    mCMD->DefaultFunAdd(&BootLoad::CWinMain);
    //ȫ������ָ��ע���
    FunAddCMDAll();
    //���г���
    string strcmd = argc > 1 ? argv[1] : "default";
    mCMD->GetCmdFunProc(strcmd, &CMDfp);
    int ret(0);
    if (CMDfp != NULL)
        ret = CMDfp(argc, argv);
    //��Ϣѭ��
    if (ret)
    {
        MSG msg;
        return MSGLOOP(msg);
    }
}

int BootLoad::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    BootLoad MsgDlg;
    DialogBoxA(hInstance, MAKEINTRESOURCEA(XqWindowDlg::ResDlgID), NULL, Main_Proc);
    return 1;
}

BootLoad::BootLoad() : XqWindowDlg(IDD_RUNDLG)
{
    BootLoad::pDlg = this;
}

int BootLoad::MSGLOOP(MSG& msg)
{
    HACCEL hAccelTable = NULL;
    printf("message loop!\n");
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 1;
}

void BootLoad::FunAddCMDAll(void)
{
    FunCMDAll(mCMD);
}

void BootLoad::FunAddWINAll(void)
{
    FunWINAll(mWIN);
}

void BootLoad::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowDlg::WMINITDIALOG(hWnd, uMsg, wParam, lParam);
    //��ȡ���
    hWCBoxMode = GetDlgItem(hWnd, IDC_CBOXMODE);
    hWLISTEXE = GetDlgItem(hWnd, IDC_LISTEXE);
    hWCBoxLINK = GetDlgItem(hWnd, IDC_CBOXLINK);
    //����ģʽ���Ϳ�
#ifndef GUIRUN
    SendMessageA(hWCBoxMode, CB_ADDSTRING, 0, (LPARAM)"����ģʽ");
#endif
    SendMessageA(hWCBoxMode, CB_ADDSTRING, 0, (LPARAM)"����ģʽ");
    ComboBox_SetCurSel(hWCBoxMode, 0);
    //�������ɷ�ʽ��
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"����ģʽ");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"����ȫ�����");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"����ȫ���ű�");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"ȫ�����");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"���ɽӿ�");
    ComboBox_SetCurSel(hWCBoxLINK, 0);
    //IDC_LISTEXE
    LV_COLUMNA colmn;
    colmn.pszText = "����";
    colmn.cxMin = 100;
    SendMessageA(hWLISTEXE, LVM_INSERTCOLUMNA, 0, (LPARAM)&colmn); //LBS_MULTICOLUMN

    //Ĭ�ϴ���ģʽ
    modenum = EnumWIN;
    CBoxModeSelect(modenum);
}

void BootLoad::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowDlg::WMCLOSE(hWnd, uMsg, wParam, lParam);
    //�رճ���
    PostQuitMessage(0);
}

void BootLoad::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);
    // �����˵�ѡ��:
    switch (ctrlID)
    {
    case IDC_CBOXLINK:
        if (wmEvent == CBN_SELCHANGE)
        {
            switch ((int)ComboBox_GetCurSel(hWCBoxLINK))
            {
            case 0:
            default:        //��
                break;
            case 1:         //
                //ShortLink("C:/windows/notepad.exe", "C:/���±�.lnk");
                break;
            case 2:         //ȫ��bat
                switch (modenum)
                {
                case EnumWIN:
                default:
                    mWIN->CreateBat();
                    break;
                case EnumCMD:
                    mCMD->CreateBat();
                    break;
                }
                break;
            case 3:         //ȫ�����
                switch (modenum)
                {
                case EnumWIN:
                default:
                    mWIN->DeleteBat();
                    break;
                case EnumCMD:
                    mCMD->DeleteBat();
                    break;
                }
                break;
            case 4:
                MessageBoxA(NULL, "���ɽӿ�", "BOX", MB_OK);
                break;
            }
        }
        break;
    case IDC_CBOXMODE:      //ģʽ������
        if (wmEvent == CBN_SELCHANGE)
        {
            modenum = (ModeRunWork)ComboBox_GetCurSel(hWCBoxMode);
            CBoxModeSelect(modenum);
        }
        break;
    case IDC_LISTEXE:
    {
        char exename[512];
        if (HIWORD(wParam) == LBN_SELCHANGE)            //��굥��
        {
            ListCurrGetText(exename);
            CWINParam::FunProcInfo winfo;
            CCMDParam::FunProcInfo cinfo;
            switch (modenum)
            {
            case EnumWIN:
            default:
                mWIN->GetCmdFunProc(string(exename), winfo);
                if (winfo.func != NULL)
                    SetWindowTextA(GetDlgItem(hWnd, IDC_EDITTEXT), static_cast<const char*>(winfo.info));
                break;
            case EnumCMD:
                mCMD->GetCmdFunProc(string(exename), cinfo);
                if (cinfo.info != NULL)
                    SetWindowTextA(GetDlgItem(hWnd, IDC_EDITTEXT), static_cast<const char*>(cinfo.info));
                break;
            }
        }
        else if (HIWORD(wParam) == LBN_DBLCLK)          //���˫��
        {
            system("cls");                              //��ˢһ����
            ListCurrGetText(exename);
            switch (modenum)
            {
            case EnumWIN:
            default:                                    //��ʾ���ڳ����嵥
                mWIN->GetCmdFunProc(string(exename), &WINfp);
                if (WINfp != NULL)
                    WINfp(guihInst, guihPrevInst, guilpCmd, guinCmd);
                break;
            case EnumCMD:                               //��ʾ�����г����嵥
                mCMD->GetCmdFunProc(string(exename), &CMDfp);
                if (CMDfp != NULL)
                    CMDfp(cmdargc, cmdargv);
                break;
            }
        }
    }
    break;
    default:
        break;
    }
}

void BootLoad::CBoxModeSelect(ModeRunWork id)
{
    vector<string> cmdstr;
    switch (id)
    {
    case EnumWIN:
    default:            //��ʾ���ڳ����嵥
        mWIN->GetAllCmdName(cmdstr);
        ListViewShow(cmdstr);
        break;
    case EnumCMD:       //��ʾ�����г����嵥
        mCMD->GetAllCmdName(cmdstr);
        ListViewShow(cmdstr);
        break;
    }
}

void BootLoad::ListViewShow(vector<string>& lst)
{
    //�����
    int Count = ListBox_GetCount(hWLISTEXE), index(0);
    for (int i = 0; i < Count; i++)
        ListBox_DeleteString(hWLISTEXE, i);
    ListBox_ResetContent(hWLISTEXE);
    //�������
    for (vector<string>::const_iterator it = lst.begin(); it != lst.end(); ++it)
    {
        //������ʾ����
        index = (int)SendDlgItemMessageA(GetDlgHWND(), IDC_LISTEXE, LB_INSERTSTRING, WPARAM(-1), (LPARAM)it->c_str());
        //index = (int)SendDlgItemMessageA(GetDlgHWND(), IDC_LISTEXE, LB_INSERTSTRING, WPARAM(-1), (LPARAM)"sdds");
        //����ѡ������
        ListBox_SetItemData(hWLISTEXE, index, (LPARAM)it->c_str());
    }
}

void BootLoad::ListCurrGetText(char* exename)
{
    int index = LB_ERR;
    //��õ��ID
    index = ListBox_GetCurSel(hWLISTEXE);
    //��ȡlist�������
    TCHAR Texename[256];
    //Texename = (TCHAR*)ListBox_GetItemData(hWLISTEXE, index);
    ListBox_GetText(hWLISTEXE, index, Texename);
    WideCharToMultiByte(CP_ACP, 0, Texename, -1, exename,
        WideCharToMultiByte(CP_ACP, 0, Texename, -1, NULL, 0, NULL, NULL), NULL, NULL);
    //MessageBoxA(NULL, exename, "BOX", MB_OK);
}




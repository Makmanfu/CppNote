

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
    //记下参数
    guihInst = hInstance;
    guihPrevInst = hPrevInstance;
    guilpCmd = lpCmdLine;
    guinCmd = nCmdShow;
    //默认GUI绑定
    mWIN->DefaultFunAdd(&BootLoad::WINMAIN);
    //全部函数指针注册绑定
    FunAddWINAll();
    //运行程序
    mWIN->GetCmdFunProc(string(lpCmdLine), &WINfp);
    if (WINfp != NULL)
    {
        return WINfp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    }
    //消息循环
    MSG msg;
    return MSGLOOP(msg);
}

int BootLoad::CWinMain(int argc, char** argv)
{
    //调用windows界面方法
    static HINSTANCE hInstance = GetModuleHandle(NULL);
    //注意*argv 缺失判断
    return WIN32MAIN(hInstance, NULL, *argv, argc);
}

int BootLoad::CMDMAIN(int argc, char** argv)
{
    //记下参数
    cmdargc = argc;
    cmdargv = argv;
    mCMD->SetExeName(argv[0]);
    //添加界面上的程序
    mCMD->DefaultFunAdd(&BootLoad::CWinMain);
    //全部函数指针注册绑定
    FunAddCMDAll();
    //运行程序
    string strcmd = argc > 1 ? argv[1] : "default";
    mCMD->GetCmdFunProc(strcmd, &CMDfp);
    int ret(0);
    if (CMDfp != NULL)
        ret = CMDfp(argc, argv);
    //消息循环
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
    //获取句柄
    hWCBoxMode = GetDlgItem(hWnd, IDC_CBOXMODE);
    hWLISTEXE = GetDlgItem(hWnd, IDC_LISTEXE);
    hWCBoxLINK = GetDlgItem(hWnd, IDC_CBOXLINK);
    //设置模式类型框
#ifndef GUIRUN
    SendMessageA(hWCBoxMode, CB_ADDSTRING, 0, (LPARAM)"命令模式");
#endif
    SendMessageA(hWCBoxMode, CB_ADDSTRING, 0, (LPARAM)"窗口模式");
    ComboBox_SetCurSel(hWCBoxMode, 0);
    //设置生成方式框
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"生成模式");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"生成全部快捷");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"生成全部脚本");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"全部清除");
    SendMessageA(hWCBoxLINK, CB_ADDSTRING, 0, (LPARAM)"生成接口");
    ComboBox_SetCurSel(hWCBoxLINK, 0);
    //IDC_LISTEXE
    LV_COLUMNA colmn;
    colmn.pszText = "名称";
    colmn.cxMin = 100;
    SendMessageA(hWLISTEXE, LVM_INSERTCOLUMNA, 0, (LPARAM)&colmn); //LBS_MULTICOLUMN

    //默认窗口模式
    modenum = EnumWIN;
    CBoxModeSelect(modenum);
}

void BootLoad::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowDlg::WMCLOSE(hWnd, uMsg, wParam, lParam);
    //关闭程序
    PostQuitMessage(0);
}

void BootLoad::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);
    // 分析菜单选择:
    switch (ctrlID)
    {
    case IDC_CBOXLINK:
        if (wmEvent == CBN_SELCHANGE)
        {
            switch ((int)ComboBox_GetCurSel(hWCBoxLINK))
            {
            case 0:
            default:        //无
                break;
            case 1:         //
                //ShortLink("C:/windows/notepad.exe", "C:/记事本.lnk");
                break;
            case 2:         //全部bat
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
            case 3:         //全部清除
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
                MessageBoxA(NULL, "生成接口", "BOX", MB_OK);
                break;
            }
        }
        break;
    case IDC_CBOXMODE:      //模式下拉框
        if (wmEvent == CBN_SELCHANGE)
        {
            modenum = (ModeRunWork)ComboBox_GetCurSel(hWCBoxMode);
            CBoxModeSelect(modenum);
        }
        break;
    case IDC_LISTEXE:
    {
        char exename[512];
        if (HIWORD(wParam) == LBN_SELCHANGE)            //鼠标单击
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
        else if (HIWORD(wParam) == LBN_DBLCLK)          //鼠标双击
        {
            system("cls");                              //先刷一次屏
            ListCurrGetText(exename);
            switch (modenum)
            {
            case EnumWIN:
            default:                                    //显示窗口程序清单
                mWIN->GetCmdFunProc(string(exename), &WINfp);
                if (WINfp != NULL)
                    WINfp(guihInst, guihPrevInst, guilpCmd, guinCmd);
                break;
            case EnumCMD:                               //显示命令行程序清单
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
    default:            //显示窗口程序清单
        mWIN->GetAllCmdName(cmdstr);
        ListViewShow(cmdstr);
        break;
    case EnumCMD:       //显示命令行程序清单
        mCMD->GetAllCmdName(cmdstr);
        ListViewShow(cmdstr);
        break;
    }
}

void BootLoad::ListViewShow(vector<string>& lst)
{
    //先清空
    int Count = ListBox_GetCount(hWLISTEXE), index(0);
    for (int i = 0; i < Count; i++)
        ListBox_DeleteString(hWLISTEXE, i);
    ListBox_ResetContent(hWLISTEXE);
    //添加名称
    for (vector<string>::const_iterator it = lst.begin(); it != lst.end(); ++it)
    {
        //插入显示名称
        index = (int)SendDlgItemMessageA(GetDlgHWND(), IDC_LISTEXE, LB_INSERTSTRING, WPARAM(-1), (LPARAM)it->c_str());
        //index = (int)SendDlgItemMessageA(GetDlgHWND(), IDC_LISTEXE, LB_INSERTSTRING, WPARAM(-1), (LPARAM)"sdds");
        //插入选项名称
        ListBox_SetItemData(hWLISTEXE, index, (LPARAM)it->c_str());
    }
}

void BootLoad::ListCurrGetText(char* exename)
{
    int index = LB_ERR;
    //获得点击ID
    index = ListBox_GetCurSel(hWLISTEXE);
    //获取list点击名称
    TCHAR Texename[256];
    //Texename = (TCHAR*)ListBox_GetItemData(hWLISTEXE, index);
    ListBox_GetText(hWLISTEXE, index, Texename);
    WideCharToMultiByte(CP_ACP, 0, Texename, -1, exename,
        WideCharToMultiByte(CP_ACP, 0, Texename, -1, NULL, 0, NULL, NULL), NULL, NULL);
    //MessageBoxA(NULL, exename, "BOX", MB_OK);
}




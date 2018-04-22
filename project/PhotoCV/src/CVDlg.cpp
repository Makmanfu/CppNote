

#include "stdafx.h"
#include "../../../../CppNote/project/SQCore/src/WinAPI/WinGUI.CPP"
#include "../../../../CppNote/project/SQCore/src/WinAPI/GDIView.CPP"
#include "CVDlg.h"


int CVDlg::WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    CVDlg MsgDlg;
    DialogBoxA(hInstance, MAKEINTRESOURCEA(IDD_DLGCV), NULL, CVDlg::Main_Proc);
    return 0;
}

CVDlg::CVDlg() : XqWindowDlg(IDD_DLGCV)
{
    CVDlg::pDlg = this;
}

CVDlg::~CVDlg()
{
}

void CVDlg::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowDlg::WMINITDIALOG(hWnd, uMsg, wParam, lParam);
    InitTitIcon("CV");
    hWPICIN = GetDlgItem(hWnd, IDC_PICIN);
    hWPICOUT = GetDlgItem(hWnd, IDC_PICOUT);
    hWCBOXCMD = GetDlgItem(hWnd, IDC_COMBOXCMD);

    mPicView1.SetGDIHWND(hWPICIN);
    mPicView2.SetGDIHWND(hWPICOUT);
}

void CVDlg::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    int wmEvent = HIWORD(wParam);
    //�����˵�ѡ��:
    switch (ctrlID)
    {
        case IDC_COMBOXCMD:
            break;
        case IDC_BTNOPEN:
            OpenDlg(hWnd);
            //RECT rect;
            //GetClientRect(hWPICIN, &rect);
            //InvalidateRect(hWPICIN, &rect, false);
            //UpdateWindow(hWnd);
            break;
        default:
            break;
    }
}

void CVDlg::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    mPicView1.GDIBMPFill(pinname1.c_str());
    mPicView2.GDIBMPFill("../Resource/lena.bmp");
    //mPicView2.GDIBMPFill("../Resource/lena.bmp");
    //?????
}

void CVDlg::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowDlg::WMCLOSE(hWnd, uMsg, wParam, lParam);
}

void CVDlg::OpenDlg(HWND hWnd)
{
    char szOpenFileName[50 * MAX_PATH];
    OPENFILENAMEA ofn;
    //��ʼ��ofn
    ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
    ofn.lStructSize = sizeof(OPENFILENAMEA);
    ofn.lpstrFile = szOpenFileName;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szOpenFileName);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrFilter = "all\0*.*\0����ͼƬ(jpg,jpeg,bmp,png)\0 *.jpg;*.jpeg;*.bmp;*.png\0";
    ofn.nFilterIndex = 2;
    ofn.lpstrTitle = "��ͼƬ";
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;
    if (GetOpenFileNameA(&ofn))
    {
        char szMusicName[MAX_PATH];                             //������
        LPSTR PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);    //����ȫ·��(����������)
        size_t len = strlen(szOpenFileName), index, i = len;
        //��ѡ
        if (szOpenFileName[len + 1] == '\0')
        {
            char* pszMusicName = strrchr(szOpenFileName, '\\');
            lstrcpyA(PszFilePathName, szOpenFileName);
            pinname1 = szOpenFileName;  //�ļ���
            //::MessageBoxA(NULL, pinname1.c_str(), "TEST", MB_OK);
            //SendMessageA(hWndList, LB_SETITEMDATA, WPARAM(index), (LPARAM)PszFilePathName);
        }
        //��ѡ
        //while (szOpenFileName[i + 1] != '\0')
        //{
        //    for (int j = 0;; j++)
        //    {
        //        i++;
        //        szMusicName[j] = szOpenFileName[i];
        //        if (szOpenFileName[i] == '\0')
        //        {
        //            PszFilePathName = (LPSTR)malloc(2 * MAX_PATH);
        //            wsprintfA(PszFilePathName, (szOpenFileName[len - 1] == '\\'
        //                ) ? "%s%s" : "%s\\%s", szOpenFileName, szMusicName);
        //            index = (int)SendMessageA(hWndList, LB_INSERTSTRING, -1, (LPARAM)szMusicName);
        //            SendMessage(hWndList, LB_SETITEMDATA, index, (LPARAM)PszFilePathName);
        //            //��ListBox_SetItemData(hWndlist, index, (LPARAM)PszFilePathName);
        //            break;
        //        }
        //    }
        //}
    }
}









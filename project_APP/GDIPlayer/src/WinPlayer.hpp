
#ifndef WINPLAYER_HPP
#define WINPLAYER_HPP
#include "stdafx.h"
#include "MainDlg.h"
//#include "SQ/WinSysInfo.h"

//��Ϣ�ص�����ָ��
MainMsg* MP3 = GetMP3();

//�����ڻص�
#ifdef _WIN64
    INT_PTR
#else
    BOOL
#endif
CALLBACK Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_INITDIALOG:
            MP3->WMINITDIALOG(hWnd, uMsg, wParam, lParam);
            break;
        case WM_CLOSE:      //���Xʱִ��  //WM_DESTROY���ִ��
            MP3->WMCLOSE(hWnd, uMsg, wParam, lParam);
            break;
        case WM_COMMAND:
            MP3->WMCOMMAND(hWnd, uMsg, wParam, lParam);
            break;
        case WM_PAINT:
            MP3->WMPAINT(hWnd, uMsg, wParam, lParam);
            break;
        case WM_HSCROLL:
            MP3->WMHSCROLL(hWnd, uMsg, wParam, lParam);
            break;
        case WM_DROPFILES:
            MP3->WMDROPFILES(hWnd, uMsg, wParam, lParam);
            break;
        case WM_SHOWTASK:
            MP3->WMSHOWTASK(hWnd, uMsg, wParam, lParam);
            break;
        case WM_SIZING:
            MP3->WMSIZING(hWnd, uMsg, wParam, lParam);
            break;
        case WM_CONTEXTMENU:    //�Ҽ����
            MP3->WMCONTEXTMENU(hWnd, uMsg, wParam, lParam);
            break;
        default:
            break;
    }
    MP3->WMOTHER(hWnd, uMsg, wParam, lParam);
    return FALSE;
}

//���������
 int WinPlayer(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    //����Ψһ������
    //if (RunCheckOnly("GDIPlayer"))
    DialogBoxA(hInstance, MAKEINTRESOURCEA(IDD_MAIN), NULL, Main_Proc);
    return 0;
}






#endif


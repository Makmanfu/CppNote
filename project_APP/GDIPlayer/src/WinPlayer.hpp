
#ifndef WINPLAYER_HPP
#define WINPLAYER_HPP
#include "stdafx.h"
#include "MainDlg.h"
//#include "SQ/WinSysInfo.h"

//消息回调管理指针
MainMsg* MP3 = GetMP3();

//主窗口回调
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
        case WM_CLOSE:      //点击X时执行  //WM_DESTROY最后执行
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
        case WM_CONTEXTMENU:    //右键点击
            MP3->WMCONTEXTMENU(hWnd, uMsg, wParam, lParam);
            break;
        default:
            break;
    }
    MP3->WMOTHER(hWnd, uMsg, wParam, lParam);
    return FALSE;
}

//播放器入口
 int WinPlayer(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    //程序唯一性运行
    //if (RunCheckOnly("GDIPlayer"))
    DialogBoxA(hInstance, MAKEINTRESOURCEA(IDD_MAIN), NULL, Main_Proc);
    return 0;
}






#endif


//====================================================
//
//  WinMP4 对播放视频的显示测试
//
//====================================================
#ifndef WINMP4_HPP
#define WINMP4_HPP

#include "stdafx.h"
#include "CMP4.h"

//消息回调管理
CMP4Msg* mp4 = GetMP4();

//主窗口消息回调
LRESULT CALLBACK WndProcDemo(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            mp4->WMCREATE(hWnd, uMsg, wParam, lParam);
            break;
        case WM_LBUTTONDOWN:
            mp4->WMLBUTTONDOWN(hWnd, uMsg, wParam, lParam);
            break;
        case WM_SIZE:
            mp4->WMSIZE(hWnd, uMsg, wParam, lParam);
            break;
        case WM_PAINT:
            mp4->WMPAINT(hWnd, uMsg, wParam, lParam);
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_CONTEXTMENU://右键点击
            mp4->WMCONTEXTMENU(hWnd, uMsg, wParam, lParam);
            break;
        case WM_COMMAND:
            mp4->WMCOMMAND(hWnd, uMsg, wParam, lParam);
            break;
        case WM_SIZING:
            mp4->WMSIZING(hWnd, uMsg, wParam, lParam);
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    mp4->WMOTHER(hWnd, uMsg, wParam, lParam);
    return 0;
}

//主函数
 int WinMP4(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* /*LPTSTR*/ lpCmdLine, int nCmdShow)
{
    // 将实例句柄存储在全局变量中
    mp4->InitData(hInstance);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProcDemo;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));        //图标
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCE(IDC_MYGUI10);            //菜单
    wcex.lpszClassName = mp4->szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(MAINICON));
    //注册窗口
    RegisterClassEx(&wcex);
    //创建窗口
    HWND hWnd = CreateWindow(mp4->szWindowClass, mp4->szTitle,
                             WS_OVERLAPPEDWINDOW | WS_EX_TOOLWINDOW,
                             0, 0, 800, 600, NULL, NULL, hInstance, NULL);
    if (!hWnd)
        return FALSE;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    //主消息循环:
    MSG msg;
    HACCEL hAccelTable = LoadAcceleratorsA(hInstance, "WINMP4");
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}


#endif


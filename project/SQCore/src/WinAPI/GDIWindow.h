//-----------------------------------------------------------------------------
//
//          窗口绘制大全
//
//-----------------------------------------------------------------------------
#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "GDIView.h"
#include "GDIPlusView.h"
#include "WinGUI.h"

//GDI绘制
class GDIDraw : public XqWindowEx
{
public:
    GDIView mGDI;
public:
    //主函数
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//GDI+绘制
class GDIPlusDraw : public XqWindowEx
{
public:
    GDIPlusView mGDIp;
public:
    //主函数
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};





#endif

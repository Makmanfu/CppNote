

#include "stdafx.h"
#include "GDIWindow.h"


int GDIDraw::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    GDIDraw GPlay;
    GPlay.InitData(hInstance);
    GPlay.Create(L"GDIPlayer");
    return GPlay.ShowDLG();
}

void GDIDraw::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    mGDI.SetGDIHWND(hWnd);      //注意实际情况用这个中初始化就与对应句柄绑定
    //mGDI.GDIDrawTest();       //GDI绘制文字
    //mGDI.GDIDrawLine();       //GDI绘制线
    //mGDI.GDIPaint();          //GDI绘制logo
    mGDI.GDIBMPPaint();       //GDI贴图BMP演示
    //mGDI.GDIBMPFill();     //GDI绘制BMP拉伸
}

int GDIPlusDraw::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    GDIPlusDraw GDIP;
    GDIP.InitData(hInstance);
    GDIP.Create(L"GDI+Draw");
    return GDIP.ShowDLG();
}

void GDIPlusDraw::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

}

void GDIPlusDraw::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    mGDIp.GDIPlusPaint(hWnd);
}




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
    mGDI.SetGDIHWND(hWnd);      //ע��ʵ�����������г�ʼ�������Ӧ�����
    //mGDI.GDIDrawTest();       //GDI��������
    //mGDI.GDIDrawLine();       //GDI������
    //mGDI.GDIPaint();          //GDI����logo
    mGDI.GDIBMPPaint();       //GDI��ͼBMP��ʾ
    //mGDI.GDIBMPFill();     //GDI����BMP����
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


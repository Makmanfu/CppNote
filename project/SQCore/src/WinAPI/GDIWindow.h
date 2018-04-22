//-----------------------------------------------------------------------------
//
//          ���ڻ��ƴ�ȫ
//
//-----------------------------------------------------------------------------
#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "GDIView.h"
#include "GDIPlusView.h"
#include "WinGUI.h"

//GDI����
class GDIDraw : public XqWindowEx
{
public:
    GDIView mGDI;
public:
    //������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//GDI+����
class GDIPlusDraw : public XqWindowEx
{
public:
    GDIPlusView mGDIp;
public:
    //������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};





#endif

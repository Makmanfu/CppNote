//-----------------------------------------------------------------------------
//
//      windows GDI+ԭʼAPIѧϰ�ܽ�
//  #define WIN32_LEAN_AND_MEAN  GDI+ʱ��Ҫ��������� Ҳ���Լ�#include <comdef.h>
//
//-----------------------------------------------------------------------------

#ifndef GDIPLUSVIEW_H
#define GDIPLUSVIEW_H

#include <windows.h>
#define ULONG_PTR ULONG
#include <comdef.h>
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib,"GdiPlus.lib")
#include "CTemplate.hpp"

//GDI+����ѧϰ
class GDIPlusView : public noncopyable
{
public:
    GDIPlusView();
    ~GDIPlusView();
public:
    //GPI+����jpg
    void GDIPlusPaint(HWND hWnd);
    //GPI����MAT
    void GDIPlusMAT(HWND hWnd);
private:
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};










#endif



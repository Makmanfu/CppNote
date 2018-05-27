//-----------------------------------------------------------------------------
//
//      windows GDI+原始API学习总结
//  #define WIN32_LEAN_AND_MEAN  GDI+时不要定义这个宏 也可以加#include <comdef.h>
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

//GDI+绘制学习
class GDIPlusView : public noncopyable
{
public:
    GDIPlusView();
    ~GDIPlusView();
public:
    //GPI+绘制jpg
    void GDIPlusPaint(HWND hWnd);
    //GPI绘制MAT
    void GDIPlusMAT(HWND hWnd);
private:
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
};










#endif



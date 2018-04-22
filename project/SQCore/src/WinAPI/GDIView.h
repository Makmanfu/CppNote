//-----------------------------------------------------------------------------
//
//                 windows GDI原始API学习总结
//                                          @2017-03-10阿甘整理
//-----------------------------------------------------------------------------

#ifndef GDIVIEW_H
#define GDIVIEW_H

#include <windows.h>
#include <WinGDI.h>
#pragma comment(lib,"gdi32.lib")
#include "CTemplate.hpp"

//GDI绘制
class GDIView: public noncopyable
{
private:
    HWND DrawhWnd;      //绘制句柄
    HDC Canvas;         //绘制DC
    RECT rect;          //绘制区大小
public:
    GDIView();
    GDIView(HWND hWnd);
    ~GDIView();
public:
    //设置绘制句柄
    void SetGDIHWND(HWND& hWnd);
public:
    //GPI绘制文字
    void GDIDrawTest(void);
    //GPI绘制线
    void GDIDrawLine(void);
    //GPI绘制播放器logo
    void GDIPaint(void);
    //GDI直接贴图BMP
    void GDIBMPPaint(const char* BMPname = "../Resource/lena.bmp");
    //GDI拉伸贴图BMP拉伸
    void GDIBMPFill(const char* BMPname = "../Resource/lena.bmp");
};










#endif



//-----------------------------------------------------------------------------
//
//                 windows GDIԭʼAPIѧϰ�ܽ�
//                                          @2017-03-10��������
//-----------------------------------------------------------------------------

#ifndef GDIVIEW_H
#define GDIVIEW_H

#include <windows.h>
#include <WinGDI.h>
#pragma comment(lib,"gdi32.lib")
#include "CTemplate.hpp"

//GDI����
class GDIView: public noncopyable
{
private:
    HWND DrawhWnd;      //���ƾ��
    HDC Canvas;         //����DC
    RECT rect;          //��������С
public:
    GDIView();
    GDIView(HWND hWnd);
    ~GDIView();
public:
    //���û��ƾ��
    void SetGDIHWND(HWND& hWnd);
public:
    //GPI��������
    void GDIDrawTest(void);
    //GPI������
    void GDIDrawLine(void);
    //GPI���Ʋ�����logo
    void GDIPaint(void);
    //GDIֱ����ͼBMP
    void GDIBMPPaint(const char* BMPname = "../Resource/lena.bmp");
    //GDI������ͼBMP����
    void GDIBMPFill(const char* BMPname = "../Resource/lena.bmp");
};










#endif



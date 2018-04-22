
#include "stdafx.h"
#include "GDIView.h"

GDIView::GDIView() :DrawhWnd(NULL)
{
}

GDIView::GDIView(HWND hWnd) : DrawhWnd(hWnd)
{
}

GDIView::~GDIView()
{
}

void GDIView::SetGDIHWND(HWND& hWnd)
{
    DrawhWnd = hWnd;
}

void GDIView::GDIDrawTest(void)
{
    if (DrawhWnd == NULL)
        return;
    PAINTSTRUCT ps;
    //ZeroMemory(&ps, sizeof(PAINTSTRUCT));
    //memset(&ps, 0, sizeof(PAINTSTRUCT));
    Canvas = BeginPaint(DrawhWnd, &ps);
    SetTextColor(ps.hdc, RGB(10, 0, 255));//设置文本颜色
    DrawText(ps.hdc, TEXT("朋友，你好。"), int(wcslen(L"朋友，你好。")), &(ps.rcPaint), DT_CENTER);
    EndPaint(DrawhWnd, &ps);
}

void GDIView::GDIDrawLine(void)
{
    if (DrawhWnd == NULL)
        return;
    PAINTSTRUCT ps;                         //绘图结构体，存储目标窗口可以绘图的客户端区域
    //HDC hdc;  换成Canvas                  //DC(可画图的内存对象)的句柄
    HPEN hpen;                              //画笔
    int Penstyle[] = {                      // 画笔类型
        PS_SOLID,       /*   直线   */
        PS_DASH,        /* -------  */
        PS_DOT,         /* .......  */
        PS_DASHDOT,     /* _._._._  */
        PS_DASHDOTDOT,  /* _.._.._  */
        PS_NULL         /*          */
    };
    Canvas = BeginPaint(DrawhWnd, &ps);            //通过窗口句柄获取该窗口的 DC
    for (int i = 0; i < sizeof(Penstyle) / sizeof(Penstyle[0]); i++)
    {
        hpen = CreatePen(Penstyle[i], 1, RGB(0, 0, 0)); //创建画笔
        //hpen = ExtCreatePen(Penstyle[i],3,)
        SelectObject(Canvas, hpen);                        //设备选择画笔
        MoveToEx(Canvas, 30 * i + 80, 50, NULL);           //画一条从(30*i+80, 50) 到 (30*i+80, 200) 的线
        LineTo(Canvas, 30 * i + 80, 200);
    }
    DeleteObject(hpen);
    EndPaint(DrawhWnd, &ps);
}

void GDIView::GDIPaint(void)
{
    if (DrawhWnd == NULL)
        return;
    PAINTSTRUCT ps;
    Canvas = BeginPaint(DrawhWnd, &ps);
    GetClientRect(DrawhWnd, &rect); //获得绘制区域
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    //创建画刷
    HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
    //画刷选择到当前DC中
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
    //填充矩形图形区为黑色背景
    Rectangle(ps.hdc, 0, 0, w, h);
    //绘制中心播放器三角
    POINT aptTriangle[4] = { { (w /= 2) - 10, (h /= 2) - 15 }, { w - 10, h + 15 }, { w + 10, h }, { w - 10, h - 15 } };
    int num[1] = { 4 };
    HBRUSH hbTriangle = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(Canvas, hbTriangle);
    PolyPolygon(Canvas, aptTriangle, num, 1);
    //选回原先的画刷
    SelectObject(ps.hdc, orgBrs);
    //清理对象
    DeleteObject(hb);
    EndPaint(DrawhWnd, &ps);
}

void GDIView::GDIBMPPaint(const char* BMPname)
{
    if (DrawhWnd == NULL)
        return;
    HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, BMPname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hBmp == NULL)
        return;         //没有图片还玩个溜溜球啊
    PAINTSTRUCT ps;
    Canvas = BeginPaint(DrawhWnd, &ps);
    HDC memDc = CreateCompatibleDC(Canvas);
    //下面是读取图片方法
    //HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMPLOGO));    //资源加载图片
    //HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, "../Resource/lena.bmp", IMAGE_BITMAP, 512, 512, LR_LOADFROMFILE);
    //HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, "../Resource/lena.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SelectObject(memDc, hBmp);
    GetClientRect(DrawhWnd, &rect);
    //在rect里绘制图片
    BitBlt(Canvas, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDc, 0, 0, SRCCOPY);
    DeleteObject(hBmp);
    DeleteDC(memDc);
    EndPaint(DrawhWnd, &ps);
}

void GDIView::GDIBMPFill(const char* BMPname)
{
    if (DrawhWnd == NULL)
        return;
    HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, BMPname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hBmp == NULL)
        return;         //没有图片还玩个溜溜球啊
    PAINTSTRUCT ps;
    //创建DC和内存DC
    Canvas = BeginPaint(DrawhWnd, &ps);
    HDC memDc = CreateCompatibleDC(Canvas);
    //获得绘制区域
    GetClientRect(DrawhWnd, &rect);
    //图片放入内存DC
    SelectObject(memDc, hBmp);
    //设置在指定设备内容中的伸展模式。解决失真问题。
    SetStretchBltMode(Canvas, COLORONCOLOR);
    //获得图片尺寸
    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);

    //内存DC拷贝到Dc
    StretchBlt(Canvas, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
               memDc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    DeleteDC(memDc);
    DeleteObject(hBmp);
    EndPaint(DrawhWnd, &ps);
    
}




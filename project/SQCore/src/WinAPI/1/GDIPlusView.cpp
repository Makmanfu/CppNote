
#include "stdafx.h"
#include "GDIPlusView.h"

GDIPlusView::GDIPlusView()
{
    //初始化gdi+
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

GDIPlusView::~GDIPlusView()
{
    //释放gdi+
    GdiplusShutdown(gdiplusToken);
}

void GDIPlusView::GDIPlusPaint(HWND hWnd)
{
    PAINTSTRUCT ps;                         // 存放绘制客户区信息
    HDC hdc = BeginPaint(hWnd, &ps);        // 存放设备环境句柄
    Image image(_T("../Resource/lena.jpg"));
    //GDI+
    Graphics graph(hdc);
    graph.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight()); //按图像大小显示
    ReleaseDC(hWnd, hdc);
    EndPaint(hWnd, &ps);
}

void GDIPlusView::GDIPlusMAT(HWND hWnd)
{
    ////载入图像
    //IplImage* srcImage = cvLoadImage("lena.jpg");
    ////创建Bitmap对象
    //Gdiplus::Bitmap bitmap(srcImage->width, srcImage->height, srcImage->widthStep,
    //    PixelFormat24bppRGB, (BYTE*)srcImage->imageData);
    //Gdiplus::Graphics graphics(pDC->GetSafeHdc());
    //graphics.DrawImage(&bitmap, 0, 0);
    ////这里使用的是IplImage类型的图片，试验表明Mat也是可以的，因为具体传参是传的数据部分，如果是Mat类型，则上面创建Bitmap对象的时候改为
    //Gdiplus::Bitmap bitmap(rgbImage.cols, rgbImage.rows, rgbImage.cols * rgbImage.channels(), //每一行的字节数
    //PixelFormat24bppRGB, (BYTE*)rgbImage.data);
    ////其中rgbImage为已经有的Mat图像。顺便提一下，如果Mat是16位的，则最好用imread和imwrite来读取和保存。
}





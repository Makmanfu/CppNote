
#include "stdafx.h"
#include "GDIPlusView.h"

GDIPlusView::GDIPlusView()
{
    //��ʼ��gdi+
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

GDIPlusView::~GDIPlusView()
{
    //�ͷ�gdi+
    GdiplusShutdown(gdiplusToken);
}

void GDIPlusView::GDIPlusPaint(HWND hWnd)
{
    PAINTSTRUCT ps;                         // ��Ż��ƿͻ�����Ϣ
    HDC hdc = BeginPaint(hWnd, &ps);        // ����豸�������
    Image image(_T("../Resource/lena.jpg"));
    //GDI+
    Graphics graph(hdc);
    graph.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight()); //��ͼ���С��ʾ
    ReleaseDC(hWnd, hdc);
    EndPaint(hWnd, &ps);
}

void GDIPlusView::GDIPlusMAT(HWND hWnd)
{
    ////����ͼ��
    //IplImage* srcImage = cvLoadImage("lena.jpg");
    ////����Bitmap����
    //Gdiplus::Bitmap bitmap(srcImage->width, srcImage->height, srcImage->widthStep,
    //    PixelFormat24bppRGB, (BYTE*)srcImage->imageData);
    //Gdiplus::Graphics graphics(pDC->GetSafeHdc());
    //graphics.DrawImage(&bitmap, 0, 0);
    ////����ʹ�õ���IplImage���͵�ͼƬ���������MatҲ�ǿ��Եģ���Ϊ���崫���Ǵ������ݲ��֣������Mat���ͣ������洴��Bitmap�����ʱ���Ϊ
    //Gdiplus::Bitmap bitmap(rgbImage.cols, rgbImage.rows, rgbImage.cols * rgbImage.channels(), //ÿһ�е��ֽ���
    //PixelFormat24bppRGB, (BYTE*)rgbImage.data);
    ////����rgbImageΪ�Ѿ��е�Matͼ��˳����һ�£����Mat��16λ�ģ��������imread��imwrite����ȡ�ͱ��档
}





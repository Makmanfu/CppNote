
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
    SetTextColor(ps.hdc, RGB(10, 0, 255));//�����ı���ɫ
    DrawText(ps.hdc, TEXT("���ѣ���á�"), int(wcslen(L"���ѣ���á�")), &(ps.rcPaint), DT_CENTER);
    EndPaint(DrawhWnd, &ps);
}

void GDIView::GDIDrawLine(void)
{
    if (DrawhWnd == NULL)
        return;
    PAINTSTRUCT ps;                         //��ͼ�ṹ�壬�洢Ŀ�괰�ڿ��Ի�ͼ�Ŀͻ�������
    //HDC hdc;  ����Canvas                  //DC(�ɻ�ͼ���ڴ����)�ľ��
    HPEN hpen;                              //����
    int Penstyle[] = {                      // ��������
        PS_SOLID,       /*   ֱ��   */
        PS_DASH,        /* -------  */
        PS_DOT,         /* .......  */
        PS_DASHDOT,     /* _._._._  */
        PS_DASHDOTDOT,  /* _.._.._  */
        PS_NULL         /*          */
    };
    Canvas = BeginPaint(DrawhWnd, &ps);            //ͨ�����ھ����ȡ�ô��ڵ� DC
    for (int i = 0; i < sizeof(Penstyle) / sizeof(Penstyle[0]); i++)
    {
        hpen = CreatePen(Penstyle[i], 1, RGB(0, 0, 0)); //��������
        //hpen = ExtCreatePen(Penstyle[i],3,)
        SelectObject(Canvas, hpen);                        //�豸ѡ�񻭱�
        MoveToEx(Canvas, 30 * i + 80, 50, NULL);           //��һ����(30*i+80, 50) �� (30*i+80, 200) ����
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
    GetClientRect(DrawhWnd, &rect); //��û�������
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    //������ˢ
    HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
    //��ˢѡ�񵽵�ǰDC��
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
    //������ͼ����Ϊ��ɫ����
    Rectangle(ps.hdc, 0, 0, w, h);
    //�������Ĳ���������
    POINT aptTriangle[4] = { { (w /= 2) - 10, (h /= 2) - 15 }, { w - 10, h + 15 }, { w + 10, h }, { w - 10, h - 15 } };
    int num[1] = { 4 };
    HBRUSH hbTriangle = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(Canvas, hbTriangle);
    PolyPolygon(Canvas, aptTriangle, num, 1);
    //ѡ��ԭ�ȵĻ�ˢ
    SelectObject(ps.hdc, orgBrs);
    //�������
    DeleteObject(hb);
    EndPaint(DrawhWnd, &ps);
}

void GDIView::GDIBMPPaint(const char* BMPname)
{
    if (DrawhWnd == NULL)
        return;
    HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, BMPname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    if (hBmp == NULL)
        return;         //û��ͼƬ�����������
    PAINTSTRUCT ps;
    Canvas = BeginPaint(DrawhWnd, &ps);
    HDC memDc = CreateCompatibleDC(Canvas);
    //�����Ƕ�ȡͼƬ����
    //HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMPLOGO));    //��Դ����ͼƬ
    //HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, "../Resource/lena.bmp", IMAGE_BITMAP, 512, 512, LR_LOADFROMFILE);
    //HBITMAP hBmp = (HBITMAP)LoadImageA(NULL, "../Resource/lena.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SelectObject(memDc, hBmp);
    GetClientRect(DrawhWnd, &rect);
    //��rect�����ͼƬ
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
        return;         //û��ͼƬ�����������
    PAINTSTRUCT ps;
    //����DC���ڴ�DC
    Canvas = BeginPaint(DrawhWnd, &ps);
    HDC memDc = CreateCompatibleDC(Canvas);
    //��û�������
    GetClientRect(DrawhWnd, &rect);
    //ͼƬ�����ڴ�DC
    SelectObject(memDc, hBmp);
    //������ָ���豸�����е���չģʽ�����ʧ�����⡣
    SetStretchBltMode(Canvas, COLORONCOLOR);
    //���ͼƬ�ߴ�
    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);

    //�ڴ�DC������Dc
    StretchBlt(Canvas, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
               memDc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    DeleteDC(memDc);
    DeleteObject(hBmp);
    EndPaint(DrawhWnd, &ps);
    
}




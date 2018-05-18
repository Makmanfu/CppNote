
#include "stdafx.h"
#include "CMP4.h"

CMP4Msg::CMP4Msg()
{
}

CMP4Msg::~CMP4Msg()
{
}

void CMP4Msg::InitData(HINSTANCE hInstance)
{
    hInst = hInstance;
    //��ʼ��ȫ���ַ���
    //LoadString(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadString(hInst, IDC_MYGUI, szWindowClass, MAX_LOADSTRING);
    wcscpy(szTitle, TEXT("WINMP4"));
    wcscpy(szWindowClass, TEXT("MYGUI"));
}

void CMP4Msg::WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //+++++++++++++++++++++++++++++++����ؼ�+++++++++++++++++++++++++++++++
    //������ť
    //ButtonAndProc ( hWnd, uMsg, wParam, hInst );
    //�����Ի���
    //CreateDlg ( hWnd, message, hInst );
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //opengl����
    //OpenGlProc(hWnd, uMsg, wParam, lParam);
}

#define LEFT_LISTBOX    1

void CMP4Msg::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //��ȡ����ߴ�
    GetWindowRect(hWnd, &rect);
    //�����Ļ�ߴ� ��������ʾ
    rect.left = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
    rect.top = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
    //���ô���λ��
    SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
    //��ȡ�˵�
    //HMENU hMainMenu = LoadMenuA(hInst, MAKEINTRESOURCEA(IDR_MENU));
    //SetMenu(hWnd, hMainMenu);
}

void CMP4Msg::WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //ģ���϶�����
    SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    return;
    //openFileDialog(hWnd);
    //InvalidateRect(hWnd, NULL, TRUE);
    //mciSendStringA("Play movie repeat", NULL, 0, 0);
}

void CMP4Msg::WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //������avi�ļ��Ĵ��ھ���
    //if (hVideoALL)
    //    MoveWindow(hVideoALL, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
}

void CMP4Msg::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //DrawGDITest(hWnd);
    GDIPaint(hWnd);
    //GDIBMPPaint(hWnd);
}

void CMP4Msg::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
        case IDC_ADDFILE:
            break;
        case IDC_EXIT:
            PostQuitMessage(0);
        default:
            break;
    }
}

void CMP4Msg::WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (LPRECT(lParam)->right - LPRECT(lParam)->left < 500) LPRECT(lParam)->right = LPRECT(lParam)->left + 500;
    if (LPRECT(lParam)->bottom - LPRECT(lParam)->top < 558) LPRECT(lParam)->bottom = LPRECT(lParam)->top + 558;
}

void CMP4Msg::DrawGDITest(HWND hWnd)
{
    PAINTSTRUCT ps;     //ZeroMemory(&ps, sizeof(PAINTSTRUCT));     //memset(&ps, 0, sizeof(PAINTSTRUCT));
    HDC hDrawDC = BeginPaint(hWnd, &ps);
    SetTextColor(ps.hdc, RGB(10, 0, 255));//�����ı���ɫ
    DrawText(ps.hdc, TEXT("���ѣ���á�"), int(wcslen(L"���ѣ���á�")), &(ps.rcPaint), DT_CENTER);
    EndPaint(hWnd, &ps);
}

void CMP4Msg::GDIPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rect); //��û�������
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    // ������ˢ
    HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
    // ��ˢѡ�񵽵�ǰDC��
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
    // ������ͼ����Ϊ��ɫ����
    Rectangle(ps.hdc, 0, 0, w, h);
    // �������Ĳ���������
    POINT aptTriangle[4] = { { (w /= 2) - 10, (h /= 2) - 15 }, { w - 10, h + 15 }, { w + 10, h }, { w - 10, h - 15 } };
    int num[1] = { 4 };
    HBRUSH hbTriangle = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(hDrawDC, hbTriangle);
    PolyPolygon(hDrawDC, aptTriangle, num, 1);
    // ѡ��ԭ�ȵĻ�ˢ
    SelectObject(ps.hdc, orgBrs);
    //�������
    DeleteObject(hb);
    EndPaint(hWnd, &ps);
}

void CMP4Msg::GDIBMPPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    //����DC���ڴ�DC
    HDC hDrawDC = BeginPaint(hWnd, &ps), memDc = CreateCompatibleDC(NULL);
    //��û�������
    GetClientRect(hWnd, &rect);
    //����ͼƬ
    //HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMPLOGO));
    HBITMAP hBmp = LoadBitmapA(NULL, "bk.bmp");
    //ͼƬ�����ڴ�DC
    SelectObject(memDc, hBmp);
    //������ָ���豸�����е���չģʽ�����ʧ�����⡣
    SetStretchBltMode(hDrawDC, COLORONCOLOR);
    //���ͼƬ�ߴ�
    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);
    //�ڴ�DC������Dc
    StretchBlt(hDrawDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
               memDc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    DeleteDC(memDc);
    DeleteObject(hBmp);
    EndPaint(hWnd, &ps);
}

void CMP4Msg::WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //��ȡ�˵�
    HMENU hPopMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDR_LISTMENU));
    hPopMenu = GetSubMenu(hPopMenu, 0);
    //DestroyMenu(hTrayMenu);
    TrackPopupMenu(hPopMenu, TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
}

IMPLEMENT_SINGLETON(CMP4Msg);

extern "C" CMP4Msg* GetMP4()
{
    return CMP4Msg::Instance();
}

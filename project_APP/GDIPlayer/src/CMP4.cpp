
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
    //初始化全局字符串
    //LoadString(hInst, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    //LoadString(hInst, IDC_MYGUI, szWindowClass, MAX_LOADSTRING);
    wcscpy(szTitle, TEXT("WINMP4"));
    wcscpy(szWindowClass, TEXT("MYGUI"));
}

void CMP4Msg::WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //+++++++++++++++++++++++++++++++处理控件+++++++++++++++++++++++++++++++
    //创建按钮
    //ButtonAndProc ( hWnd, uMsg, wParam, hInst );
    //创建对话框
    //CreateDlg ( hWnd, message, hInst );
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    //opengl绘制
    //OpenGlProc(hWnd, uMsg, wParam, lParam);
}

#define LEFT_LISTBOX    1

void CMP4Msg::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //获取窗体尺寸
    GetWindowRect(hWnd, &rect);
    //获得屏幕尺寸 并居中显示
    rect.left = (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2;
    rect.top = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2;
    //设置窗体位置
    SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
    //获取菜单
    //HMENU hMainMenu = LoadMenuA(hInst, MAKEINTRESOURCEA(IDR_MENU));
    //SetMenu(hWnd, hMainMenu);
}

void CMP4Msg::WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //模拟拖动窗体
    SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
    return;
    //openFileDialog(hWnd);
    //InvalidateRect(hWnd, NULL, TRUE);
    //mciSendStringA("Play movie repeat", NULL, 0, 0);
}

void CMP4Msg::WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //将播放avi文件的窗口居中
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
    SetTextColor(ps.hdc, RGB(10, 0, 255));//设置文本颜色
    DrawText(ps.hdc, TEXT("朋友，你好。"), int(wcslen(L"朋友，你好。")), &(ps.rcPaint), DT_CENTER);
    EndPaint(hWnd, &ps);
}

void CMP4Msg::GDIPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rect); //获得绘制区域
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    // 创建画刷
    HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
    // 画刷选择到当前DC中
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
    // 填充矩形图形区为黑色背景
    Rectangle(ps.hdc, 0, 0, w, h);
    // 绘制中心播放器三角
    POINT aptTriangle[4] = { { (w /= 2) - 10, (h /= 2) - 15 }, { w - 10, h + 15 }, { w + 10, h }, { w - 10, h - 15 } };
    int num[1] = { 4 };
    HBRUSH hbTriangle = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(hDrawDC, hbTriangle);
    PolyPolygon(hDrawDC, aptTriangle, num, 1);
    // 选回原先的画刷
    SelectObject(ps.hdc, orgBrs);
    //清理对象
    DeleteObject(hb);
    EndPaint(hWnd, &ps);
}

void CMP4Msg::GDIBMPPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    //创建DC和内存DC
    HDC hDrawDC = BeginPaint(hWnd, &ps), memDc = CreateCompatibleDC(NULL);
    //获得绘制区域
    GetClientRect(hWnd, &rect);
    //加载图片
    //HBITMAP hBmp = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMPLOGO));
    HBITMAP hBmp = LoadBitmapA(NULL, "bk.bmp");
    //图片放入内存DC
    SelectObject(memDc, hBmp);
    //设置在指定设备内容中的伸展模式。解决失真问题。
    SetStretchBltMode(hDrawDC, COLORONCOLOR);
    //获得图片尺寸
    BITMAP bmp;
    GetObject(hBmp, sizeof(BITMAP), &bmp);
    //内存DC拷贝到Dc
    StretchBlt(hDrawDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
               memDc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    DeleteDC(memDc);
    DeleteObject(hBmp);
    EndPaint(hWnd, &ps);
}

void CMP4Msg::WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //获取菜单
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

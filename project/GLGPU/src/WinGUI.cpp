
#include "StdAfx.h"
#include "WinGUI.h"

//#include <CommCtrl.h>
//#pragma comment(lib,"comctl32")
//InitCommonControls();

bool CenterWindow(HWND hwndWindow)
{
    HWND hwndParent;
    RECT rectWindow, rectParent;
    // make the window relative to its parent
    if ((hwndParent = ::GetParent(hwndWindow)) != NULL)
    {
        ::GetWindowRect(hwndWindow, &rectWindow);
        ::GetWindowRect(hwndParent, &rectParent);
        int nWidth = rectWindow.right - rectWindow.left, nHeight = rectWindow.bottom - rectWindow.top;
        int nX = ((rectParent.right - rectParent.left) - nWidth) / 2 + rectParent.left;
        int nY = ((rectParent.bottom - rectParent.top) - nHeight) / 2 + rectParent.top;
        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN), nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
        // make sure that the dialog box never moves outside of the screen
        if (nX < 0) nX = 0;
        if (nY < 0) nY = 0;
        if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
        if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;
        ::MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);
        return true;
    }
    return false;
}

bool CenterWindowDesk(HWND hwndWindow)
{
    HWND hwndDesk;
    RECT rectWindow, rectDesk;
    // make the window relative to its parent
    if ((hwndDesk = ::GetDesktopWindow()) != NULL)
    {
        ::GetWindowRect(hwndWindow, &rectWindow);
        ::GetWindowRect(hwndDesk, &rectDesk);
        int nWidth = rectWindow.right - rectWindow.left, nHeight = rectWindow.bottom - rectWindow.top;
        int nX = ((rectDesk.right - rectDesk.left) - nWidth) / 2 + rectDesk.left;
        int nY = ((rectDesk.bottom - rectDesk.top) - nHeight) / 2 + rectDesk.top;
        int nScreenWidth = GetSystemMetrics(SM_CXSCREEN), nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
        // make sure that the dialog box never moves outside of the screen
        if (nX < 0) nX = 0;
        if (nY < 0) nY = 0;
        if (nX + nWidth > nScreenWidth) nX = nScreenWidth - nWidth;
        if (nY + nHeight > nScreenHeight) nY = nScreenHeight - nHeight;
        ::MoveWindow(hwndWindow, nX, nY, nWidth, nHeight, FALSE);
        return true;
    }
    return false;
}

XqWindowDlg* XqWindowDlg::pDlg = NULL;
int XqWindowDlg::ResDlgID = 0;

INT_PTR CALLBACK XqWindowDlg::Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //XqWindowDlg* pDlg = static_cast<XqWindowDlg*>((void*)(lParam));
    switch (uMsg)
    {
        case WM_INITDIALOG:
            pDlg->GethInstance(hWnd);
            pDlg->WMINITDIALOG(hWnd, uMsg, wParam, lParam);
            break;
        case WM_CLOSE:      //点击X时执行  //WM_DESTROY最后执行
            pDlg->WMCLOSE(hWnd, uMsg, wParam, lParam);
            break;
        case WM_LBUTTONDOWN:
            pDlg->WMLBUTTONDOWN(hWnd, uMsg, wParam, lParam);
            break;
        case WM_COMMAND:
            pDlg->WMCOMMAND(hWnd, uMsg, wParam, lParam);
            break;
        case WM_COPYDATA:
            pDlg->WMCOPYDATA(hWnd, uMsg, wParam, lParam);
            break;
        case WM_PAINT:
            pDlg->WMPAINT(hWnd, uMsg, wParam, lParam);
            break;
        case WM_SIZING:
            pDlg->WMSIZING(hWnd, uMsg, wParam, lParam);
            break;
        default:
            pDlg->WMOTHER(hWnd, uMsg, wParam, lParam);
            break;
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return FALSE;
}

XqWindowDlg::XqWindowDlg(int tResDlgID)
{
    XqWindowDlg::pDlg = this;
    XqWindowDlg::ResDlgID = tResDlgID;
}

XqWindowDlg::~XqWindowDlg()
{
}

void XqWindowDlg::InitTitIcon(const char* titname)
{
    char tmpchar[256];
#ifdef _WIN64
    sprintf(tmpchar, "%s64", titname);
#else
    sprintf(tmpchar, "%s", titname);
#endif
    //界面标题
    SetWindowTextA(hWndDlg, tmpchar);
    //SetWindowText(hWndDlg, TEXT("SQCore64"));
}

void XqWindowDlg::SetIcon(int iconid)
{
    //界面图标
    HICON hIcon = LoadIconA(hInstance, MAKEINTRESOURCEA(iconid));
    SendMessage(hWndDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
}

HWND XqWindowDlg::GetDlgHWND(void)
{
    return hWndDlg;
}

void XqWindowDlg::WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

#ifndef MAINICON
    #define MAINICON 200
#endif

void XqWindowDlg::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    InitTitIcon("SQCore");
    SetIcon(MAINICON);
}

void XqWindowDlg::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    EndDialog(hWnd, 0);
    hWndDlg = NULL;
    //PostQuitMessage(0);
}

void XqWindowDlg::WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowDlg::WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
}

void XqWindowDlg::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowDlg::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    switch (ctrlID)
    {
        default:
            break;
    }
}

void XqWindowDlg::WMCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //PCOPYDATASTRUCT pCopyData = (PCOPYDATASTRUCT)lParam;
    //CMDCHAR cmdtmp = ((PSDATA *)(pCopyData->lpData))->cmdname;
    //char* pathtmp = ((PSDATA *)(pCopyData->lpData))->filepath;
    switch (0)
    {
        case 0:
            break;
    }
}

void XqWindowDlg::GethInstance(HWND hWnd)
{
    //获得句柄
#ifdef _WIN64
    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
#else
    hInstance = (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE);
#endif
    hWndDlg = hWnd;
}

LRESULT XqWindowDlg::ListViewAndProc(HWND hWnd, UINT message, WPARAM wParam, HINSTANCE hInst)
{
    // 用于测试的结构体
    struct STUDENTINFO
    {
        CHAR Name[15];
        CHAR Age[3];
        CHAR Address[50];
    };
    switch (message)
    {
        case WM_CREATE:
            break;
        case WM_INITDIALOG:
            //// 获取ListView控件的句柄
            //HWND hListview = GetDlgItem ( hWnd, IDC_LIST1 );
            //// 设置ListView的列
            //LVCOLUMNA vcl;
            //vcl.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            //// 第一列
            //vcl.pszText = "姓名";//列标题
            //vcl.cx = 90;//列宽
            //vcl.iSubItem = 0;//子项索引，第一列无子项
            //ListView_InsertColumn ( hListview, 0, &vcl );
            //// 第二列
            //vcl.pszText = "年龄";
            //vcl.cx = 90;
            //vcl.iSubItem = 1;//子项索引
            //ListView_InsertColumn ( hListview, 1, &vcl );
            //// 第三列
            //vcl.pszText = "地址";
            //vcl.cx = 200;
            //vcl.iSubItem = 2;
            //ListView_InsertColumn ( hListview, 2, &vcl );
            break;
        case WM_COMMAND:    //消息处理
            //if (LOWORD(wParam) == IDC_BTNADD)
            //{
            //    STUDENTINFO stu[ ] = {
            //        { "小刘", "20", "火星" },
            //        { "老赵", "21", "木星" },
            //        { "小胡", "30", "水星" },
            //        { "老高", "32", "山沟一号" },
            //        { "黄牛", "24", "不知哪个星球来的" },
            //        { "王七", "28", "超人之乡" }
            //    };
            //    //求出数组中元素的个数
            //    int arrCount = (int)(sizeof(stu) / sizeof(stu[0]));
            //    LVITEM vitem;
            //    vitem.mask = LVIF_TEXT;
            //    for (int i = 0; i < arrCount; i++)
            //    {
            //        /*
            //        策略：
            //        先添加项再设置子项内容
            //        */
            //        vitem.pszText = stu[i].Name;
            //        vitem.iItem = i;
            //        vitem.iSubItem = 0;
            //        ListView_InsertItem(hListview, &vitem);
            //        // 设置子项
            //        vitem.iSubItem = 1;
            //        vitem.pszText = stu[i].Age;
            //        ListView_SetItem( hListview, &vitem);
            //        vitem.iSubItem = 2;
            //        vitem.pszText = stu[i].Address;
            //        ListView_SetItem(hListview, &vitem);
            //    }
            //}
            //else if(LOWORD(wParam) == IDC_BTNCLEAR)
            //{
            //    // 清除ListView中的所有项
            //    ListView_DeleteAllItems(hListview);
            //}
            break;
    }
    return 0;
}



XqWindow::XqWindow(void) : hWnd(NULL)
, hInst(NULL)
{
}

XqWindow::~XqWindow(void)
{
    if (this->hWnd != NULL && ::IsWindow(this->hWnd)) // C++对象被销毁之前，销毁窗口对象
    {
        ::DestroyWindow(this->hWnd);  // Tell system to destroy hWnd and Send WM_DESTROY to wndproc
        this->hWnd = NULL;  //此时，窗口对象已经销毁，通过设置hWnd=NULL，来通知C++对象
    }
}

HWND XqWindow::GetHandle(void)
{
    return this->hWnd;
}

void XqWindow::SetHandle(HWND hwnd)
{
    this->hWnd = hwnd;
}

RECT* XqWindow::GetRECT(void)
{
    GetClientRect(GetHandle(), &rect);
    return &rect;
}

HINSTANCE XqWindow::GethInst(void)
{
    return hInst;
}

bool XqWindow::Create(wchar_t* szTitleName, int w, int h, WNDPROC lWndProc)
{
    if (this->hInst == NULL)
        return 1;
    wchar_t szClassName[255];       //主窗口类名
    wchar_t szTitle[255];           //标题栏文本
    //注册窗口类
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    if (lWndProc != NULL)
        wcex.lpfnWndProc = (WNDPROC)lWndProc;
    else
        wcex.lpfnWndProc = XqWindow::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = this->hInst;
    wcex.hIcon = ::LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
    wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;           //菜单
    wcex.lpszClassName = szClassName;
    wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
    //注册窗口
    if (0 == ::RegisterClassExW(&wcex)) 
        return 1;
    // 创建窗口
    if (this->hWnd == NULL)
    {
        ::wsprintfW(szTitle, L"_XqWindow");
        RECT rectDesk;
        ::GetWindowRect(::GetDesktopWindow(), &rectDesk);
        int nX = ((rectDesk.right - rectDesk.left) - w) / 2;
        int nY = ((rectDesk.bottom - rectDesk.top) - h) / 2;
        this->hWnd = ::CreateWindowW(szClassName, wcscmp(szTitleName, L"WINGUI") ? szTitleName : szTitle,
            WS_OVERLAPPEDWINDOW /*| WS_EX_TOOLWINDOW*/, nX, nY, w, h, NULL, NULL, this->hInst, (LPVOID)this);
        if (this->hWnd == NULL)
        {
            this->hWnd = NULL;
            wchar_t msg[100];
            ::wsprintfW(msg, L"CreateWindow()失败:%ld", ::GetLastError());
            ::MessageBoxW(NULL, msg, L"错误", MB_OK);
            return 1;
        }
    }
    return 0;
}

int XqWindow::ShowDLG()
{
    if (this->hWnd == NULL)
        return 0;
    ShowWindow(this->hWnd, SW_SHOW);
    UpdateWindow(this->hWnd);
    //主消息循环:
    MSG msg = { 0 };
    HACCEL hAccelTable = NULL;  //LoadAcceleratorsA(this->hInst, "WINMP4");
    printf("message loop!\n");
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}

void XqWindow::InitData(HINSTANCE hInstance)
{
    this->hInst = hInstance;
}

LRESULT CALLBACK XqWindow::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;
    switch (uMsg)
    {
    case WM_CREATE:
        //锁定size
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &~WS_MAXIMIZEBOX &~WS_SIZEBOX);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        //模拟拖动窗体
        SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}



#define GWL_USERDATA        (-21)

std::vector<void*> XqWindowEx::registeredClassArray;

LRESULT CALLBACK XqWindowEx::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowEx* pObj = NULL;
    //在此消息收到时，把窗口对象句柄赋给C++对象成员，同时把C++对象地址赋给窗口对象成员
    if (uMsg == WM_CREATE)
    {
        pObj = (XqWindowEx*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        pObj->SetHandle(hWnd);                                  //在此处获取HWND，此时CreateWindow()尚未返回。
        ::SetWindowLong(hWnd, GWL_USERDATA, (LONG)pObj);        //通过USERDATA把HWND和C++对象关联起来
        pObj->WMCREATE(hWnd, uMsg, wParam, lParam);
    }
    pObj = (XqWindowEx*)::GetWindowLong(hWnd, GWL_USERDATA);
    switch (uMsg)
    {
        case WM_DESTROY:
            pObj->WMDESTROY(hWnd, uMsg, wParam, lParam);
            break;
        case WM_INITDIALOG:
            MessageBoxA(NULL, "有BUG", "WIN32", MB_OK);
            pObj->WMINITDIALOG(hWnd, uMsg, wParam, lParam);
            break;
        case WM_LBUTTONDOWN:
            pObj->WMLBUTTONDOWN(hWnd, uMsg, wParam, lParam);
            break;
        case WM_RBUTTONDOWN:
            pObj->WMRBUTTONDOWN(hWnd, uMsg, wParam, lParam);
            break;
        case WM_LBUTTONUP:
            pObj->WMLBUTTONUP(hWnd, uMsg, wParam, lParam);
            break;
        case WM_RBUTTONUP:
            pObj->WMRBUTTONUP(hWnd, uMsg, wParam, lParam);
            break;
        case WM_MOUSEWHEEL:
            pObj->WMMOUSEWHEEL(hWnd, uMsg, wParam, lParam);
            break;
        case WM_SIZE:
            pObj->WMSIZE(hWnd, uMsg, wParam, lParam);
            break;
        case WM_PAINT:
            pObj->WMPAINT(hWnd, uMsg, wParam, lParam);
            break;
        case WM_CLOSE:
            pObj->WMCLOSE(hWnd, uMsg, wParam, lParam);
            break;
        case WM_MOUSEMOVE:
            pObj->WMMOUSEMOVE(hWnd, uMsg, wParam, lParam);
            break;
        case WM_COMMAND:
            pObj->WMCOMMAND(hWnd, uMsg, wParam, lParam);
            break;
        case WM_SIZING:
            pObj->WMSIZING(hWnd, uMsg, wParam, lParam);
            break;
        case WM_TIMER:
            pObj->WMTIMER(hWnd, uMsg, wParam, lParam);
            break;
        case WM_KEYDOWN:
            pObj->WMKEYDOWN(hWnd, uMsg, wParam, lParam);
            break;
        default:
            //if (!pObj->WMOTHER(hWnd, uMsg, wParam, lParam))
            //    break;
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    //需要后续默认处理则需要返回0；停止该消息后续处理，则返回1
    return 0;
}

XqWindowEx::XqWindowEx() :XqWindow()
{
}

XqWindowEx::XqWindowEx(HINSTANCE hInst) : XqWindowEx()
{
    XqWindow::InitData(hInst);
}

XqWindowEx::~XqWindowEx()
{
}

bool XqWindowEx::Create(wchar_t* szTitleName, int w, int h)
{
    if (this->GethInst() == NULL)
        return false;
    wchar_t szClassName[255];       //主窗口类名
    wchar_t szTitle[255];           //标题栏文本
    void* _vPtr = *((void**)this);
    ::wsprintfW(szClassName, L"%p", _vPtr);
    std::vector<void*>::iterator it;
    // 判断对象的类是否注册过
    for (it = registeredClassArray.begin(); it != registeredClassArray.end(); it++)
    {
        if ((*it) == _vPtr)
            break;
    }
    if (it == registeredClassArray.end())       // 如果没注册过，则进行注册
    {
        //注册窗口类
        WNDCLASSEXW wcex;
        wcex.cbSize = sizeof(WNDCLASSEXW);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = XqWindowEx::WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = this->GethInst();
        wcex.hIcon = ::LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
        wcex.hCursor = ::LoadCursorW(NULL, MAKEINTRESOURCEW(IDC_ARROW));
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;           //菜单
        wcex.lpszClassName = szClassName;
        wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
        //注册窗口
        if (0 != ::RegisterClassExW(&wcex))     // 把注册成功的类加入链表
            registeredClassArray.push_back(_vPtr);
    }
    // 创建窗口
    if (this->GetHandle() == NULL)
    {
        ::wsprintfW(szTitle, L"窗口类名(C++类虚表指针)：%p", _vPtr);
        RECT rectDesk;
        ::GetWindowRect(::GetDesktopWindow(), &rectDesk);
        int nX = ((rectDesk.right - rectDesk.left) - w) / 2;
        int nY = ((rectDesk.bottom - rectDesk.top) - h) / 2;
        this->SetHandle(::CreateWindowW(szClassName, wcscmp(szTitleName, L"WINGUI") ? szTitleName : szTitle,
                                    WS_OVERLAPPEDWINDOW /*| WS_EX_TOOLWINDOW*/, nX, nY, w, h, NULL, NULL, this->GethInst(), (LPVOID)this));
        if (this->GetHandle() == NULL)
        {
            this->SetHandle(NULL);
            wchar_t msg[100];
            ::wsprintfW(msg, L"CreateWindow()失败:%ld", ::GetLastError());
            ::MessageBoxW(NULL, msg, L"错误", MB_OK);
            return false;
        }
    }
    return true;
}

bool XqWindowEx::CreateEX(wchar_t* szTitleName /*= L"WINGUI"*/, int w /*= 600*/, int h /*= 600*/)
{
    if (this->GethInst() == NULL)
        return 1;
    wchar_t szClassName[255];       //主窗口类名
    ::wsprintfW(szClassName, L"WINGUI");
    //注册窗口类
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = XqWindowEx::WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = this->GethInst();
    wcex.hIcon = ::LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
    wcex.hCursor = ::LoadCursorW(NULL, MAKEINTRESOURCEW(IDC_ARROW));
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;           //菜单
    wcex.lpszClassName = szClassName;
    wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
    //注册窗口
    if (0 == ::RegisterClassExW(&wcex))     // 把注册成功的类加入链表
    {
        ::MessageBoxW(NULL, L"注册窗口错误", L"错误", MB_OK);
        return 1;
    }
    // 创建窗口
    if (this->GetHandle() == NULL)
    {
        wchar_t szTitle[255];           //标题栏文本
        ::wsprintfW(szTitle, L"XqWindow");
        RECT rectDesk;
        ::GetWindowRect(::GetDesktopWindow(), &rectDesk);
        int nX = ((rectDesk.right - rectDesk.left) - w) / 2;
        int nY = ((rectDesk.bottom - rectDesk.top) - h) / 2;
        this->SetHandle(::CreateWindowW(szClassName, wcscmp(szTitleName, L"WINGUI") ? szTitleName : szTitle,
                                    WS_OVERLAPPEDWINDOW /*| WS_EX_TOOLWINDOW*/, nX, nY, w, h, NULL, NULL, this->GethInst(), (LPVOID)this));
        if (this->GetHandle() == NULL)
        {
            this->SetHandle(NULL);
            wchar_t msg[100];
            ::wsprintfW(msg, L"CreateWindow()失败:%ld", ::GetLastError());
            ::MessageBoxW(NULL, msg, L"错误", MB_OK);
            return 1;
        }
    }
    return 0;
}

int XqWindowEx::ShowDLG(void)
{
    ShowWindow(this->GetHandle(), SW_SHOW);
    UpdateWindow(this->GetHandle());
    //主消息循环:
    MSG msg = { 0 };
    HACCEL hAccelTable = NULL;  //LoadAcceleratorsA(this->hInst, "WINMP4");
    printf("message loop!\n");
    while (GetMessage(&msg, NULL, 0, 0))
    {
        MsgLoopFun();
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    MsgLoopLast();
    return (int)msg.wParam;
}

int XqWindowEx::MsgLoopFun(void)
{
    return 1;
}

int XqWindowEx::MsgLoopLast(void)
{
    return 1;
}

bool XqWindowEx::WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //MessageBoxA(NULL, "有BUG", "s", MB_OK);
    //switch (uMsg)
    //{

    //default:
        return true;
    //}
    //return false;
}

void XqWindowEx::WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //例如创建三个按钮
    //CreateWindowA("Button", "Button", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    //    10, 10, 80, 30, hWnd, (HMENU)IDB_ONE, hInst, NULL);
    //CreateWindowA("Button", "Radio", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
    //    100, 10, 80, 30, hWnd, (HMENU)IDB_TWO, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
    //CreateWindowA("Button", "对话框", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    //    200, 10, 80, 30, hWnd, (HMENU)IDB_THREE, hInst, NULL);
}

void XqWindowEx::WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
}

void XqWindowEx::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //销毁窗口
    ::DestroyWindow(hWnd);
}

void XqWindowEx::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int ctrlID = LOWORD(wParam);
    switch (ctrlID)
    {
    default:
        break;
    }
}

void XqWindowEx::WMMOUSEWHEEL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMMOUSEMOVE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //模拟拖动窗体
    SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
}

void XqWindowEx::WMRBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMLBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMRBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(hWnd, &ps);

    //绘画
    //::Rectangle(hdc, 10, 10, 300, 200);



    ::EndPaint(hWnd, &ps);
}

void XqWindowEx::WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMTIMER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}

void XqWindowEx::WMKEYDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
}








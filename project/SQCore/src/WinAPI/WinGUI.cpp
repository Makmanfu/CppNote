
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
        case WM_CLOSE:      //���Xʱִ��  //WM_DESTROY���ִ��
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
    //�������
    SetWindowTextA(hWndDlg, tmpchar);
    //SetWindowText(hWndDlg, TEXT("SQCore64"));
}

void XqWindowDlg::SetIcon(int iconid)
{
    //����ͼ��
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
    //��þ��
#ifdef _WIN64
    hInstance = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);
#else
    hInstance = (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE);
#endif
    hWndDlg = hWnd;
}

LRESULT XqWindowDlg::ListViewAndProc(HWND hWnd, UINT message, WPARAM wParam, HINSTANCE hInst)
{
    // ���ڲ��ԵĽṹ��
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
            //// ��ȡListView�ؼ��ľ��
            //HWND hListview = GetDlgItem ( hWnd, IDC_LIST1 );
            //// ����ListView����
            //LVCOLUMNA vcl;
            //vcl.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
            //// ��һ��
            //vcl.pszText = "����";//�б���
            //vcl.cx = 90;//�п�
            //vcl.iSubItem = 0;//������������һ��������
            //ListView_InsertColumn ( hListview, 0, &vcl );
            //// �ڶ���
            //vcl.pszText = "����";
            //vcl.cx = 90;
            //vcl.iSubItem = 1;//��������
            //ListView_InsertColumn ( hListview, 1, &vcl );
            //// ������
            //vcl.pszText = "��ַ";
            //vcl.cx = 200;
            //vcl.iSubItem = 2;
            //ListView_InsertColumn ( hListview, 2, &vcl );
            break;
        case WM_COMMAND:    //��Ϣ����
            //if (LOWORD(wParam) == IDC_BTNADD)
            //{
            //    STUDENTINFO stu[ ] = {
            //        { "С��", "20", "����" },
            //        { "����", "21", "ľ��" },
            //        { "С��", "30", "ˮ��" },
            //        { "�ϸ�", "32", "ɽ��һ��" },
            //        { "��ţ", "24", "��֪�ĸ���������" },
            //        { "����", "28", "����֮��" }
            //    };
            //    //���������Ԫ�صĸ���
            //    int arrCount = (int)(sizeof(stu) / sizeof(stu[0]));
            //    LVITEM vitem;
            //    vitem.mask = LVIF_TEXT;
            //    for (int i = 0; i < arrCount; i++)
            //    {
            //        /*
            //        ���ԣ�
            //        ���������������������
            //        */
            //        vitem.pszText = stu[i].Name;
            //        vitem.iItem = i;
            //        vitem.iSubItem = 0;
            //        ListView_InsertItem(hListview, &vitem);
            //        // ��������
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
            //    // ���ListView�е�������
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
    if (this->hWnd != NULL && ::IsWindow(this->hWnd)) // C++��������֮ǰ�����ٴ��ڶ���
    {
        ::DestroyWindow(this->hWnd);  // Tell system to destroy hWnd and Send WM_DESTROY to wndproc
        this->hWnd = NULL;  //��ʱ�����ڶ����Ѿ����٣�ͨ������hWnd=NULL����֪ͨC++����
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
    wchar_t szClassName[255];       //����������
    wchar_t szTitle[255];           //�������ı�
    //ע�ᴰ����
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
    wcex.lpszMenuName = NULL;           //�˵�
    wcex.lpszClassName = szClassName;
    wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
    //ע�ᴰ��
    if (0 == ::RegisterClassExW(&wcex)) 
        return 1;
    // ��������
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
            ::wsprintfW(msg, L"CreateWindow()ʧ��:%ld", ::GetLastError());
            ::MessageBoxW(NULL, msg, L"����", MB_OK);
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
    //����Ϣѭ��:
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
        //����size
        ::SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) &~WS_MAXIMIZEBOX &~WS_SIZEBOX);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN:
        //ģ���϶�����
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
    //�ڴ���Ϣ�յ�ʱ���Ѵ��ڶ���������C++�����Ա��ͬʱ��C++�����ַ�������ڶ����Ա
    if (uMsg == WM_CREATE)
    {
        pObj = (XqWindowEx*)(((LPCREATESTRUCT)lParam)->lpCreateParams);
        pObj->SetHandle(hWnd);                                  //�ڴ˴���ȡHWND����ʱCreateWindow()��δ���ء�
        ::SetWindowLong(hWnd, GWL_USERDATA, (LONG)pObj);        //ͨ��USERDATA��HWND��C++�����������
        pObj->WMCREATE(hWnd, uMsg, wParam, lParam);
    }
    pObj = (XqWindowEx*)::GetWindowLong(hWnd, GWL_USERDATA);
    switch (uMsg)
    {
        case WM_DESTROY:
            pObj->WMDESTROY(hWnd, uMsg, wParam, lParam);
            break;
        case WM_INITDIALOG:
            MessageBoxA(NULL, "��BUG", "WIN32", MB_OK);
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
    //��Ҫ����Ĭ�ϴ�������Ҫ����0��ֹͣ����Ϣ���������򷵻�1
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
    wchar_t szClassName[255];       //����������
    wchar_t szTitle[255];           //�������ı�
    void* _vPtr = *((void**)this);
    ::wsprintfW(szClassName, L"%p", _vPtr);
    std::vector<void*>::iterator it;
    // �ж϶�������Ƿ�ע���
    for (it = registeredClassArray.begin(); it != registeredClassArray.end(); it++)
    {
        if ((*it) == _vPtr)
            break;
    }
    if (it == registeredClassArray.end())       // ���ûע����������ע��
    {
        //ע�ᴰ����
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
        wcex.lpszMenuName = NULL;           //�˵�
        wcex.lpszClassName = szClassName;
        wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
        //ע�ᴰ��
        if (0 != ::RegisterClassExW(&wcex))     // ��ע��ɹ������������
            registeredClassArray.push_back(_vPtr);
    }
    // ��������
    if (this->GetHandle() == NULL)
    {
        ::wsprintfW(szTitle, L"��������(C++�����ָ��)��%p", _vPtr);
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
            ::wsprintfW(msg, L"CreateWindow()ʧ��:%ld", ::GetLastError());
            ::MessageBoxW(NULL, msg, L"����", MB_OK);
            return false;
        }
    }
    return true;
}

bool XqWindowEx::CreateEX(wchar_t* szTitleName /*= L"WINGUI"*/, int w /*= 600*/, int h /*= 600*/)
{
    if (this->GethInst() == NULL)
        return 1;
    wchar_t szClassName[255];       //����������
    ::wsprintfW(szClassName, L"WINGUI");
    //ע�ᴰ����
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
    wcex.lpszMenuName = NULL;           //�˵�
    wcex.lpszClassName = szClassName;
    wcex.hIconSm = LoadIconW(wcex.hInstance, MAKEINTRESOURCEW(MAINICON));
    //ע�ᴰ��
    if (0 == ::RegisterClassExW(&wcex))     // ��ע��ɹ������������
    {
        ::MessageBoxW(NULL, L"ע�ᴰ�ڴ���", L"����", MB_OK);
        return 1;
    }
    // ��������
    if (this->GetHandle() == NULL)
    {
        wchar_t szTitle[255];           //�������ı�
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
            ::wsprintfW(msg, L"CreateWindow()ʧ��:%ld", ::GetLastError());
            ::MessageBoxW(NULL, msg, L"����", MB_OK);
            return 1;
        }
    }
    return 0;
}

int XqWindowEx::ShowDLG(void)
{
    ShowWindow(this->GetHandle(), SW_SHOW);
    UpdateWindow(this->GetHandle());
    //����Ϣѭ��:
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
    //MessageBoxA(NULL, "��BUG", "s", MB_OK);
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
    //���紴��������ť
    //CreateWindowA("Button", "Button", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    //    10, 10, 80, 30, hWnd, (HMENU)IDB_ONE, hInst, NULL);
    //CreateWindowA("Button", "Radio", BS_AUTORADIOBUTTON | WS_CHILD | WS_VISIBLE,
    //    100, 10, 80, 30, hWnd, (HMENU)IDB_TWO, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
    //CreateWindowA("Button", "�Ի���", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
    //    200, 10, 80, 30, hWnd, (HMENU)IDB_THREE, hInst, NULL);
}

void XqWindowEx::WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PostQuitMessage(0);
}

void XqWindowEx::WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //���ٴ���
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
    //ģ���϶�����
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

    //�滭
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








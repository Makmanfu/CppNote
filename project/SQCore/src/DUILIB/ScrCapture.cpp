
#include "StdAfx.h"
#include "ScrCapture.h"

HBITMAP CreateDesktopBitmap(HWND hWnd)
{
    HWND hWndDesktop = ::GetDesktopWindow();
    RECT rect;
    GetWindowRect(hWndDesktop, &rect);
    rect.right -= rect.left;
    rect.bottom -= rect.top;
    rect.left = rect.top = 0;
    HDC hDcDesktop = GetDC(hWndDesktop);
    HDC hDcMem = CreateCompatibleDC(hDcDesktop);
    HBITMAP hBmp = CreateCompatibleBitmap(hDcDesktop, rect.right, rect.bottom);
    HGDIOBJ hOld = SelectObject(hDcMem, hBmp);
    ::BitBlt(hDcMem, 0, 0, rect.right, rect.bottom, hDcDesktop, 0, 0, SRCCOPY | CAPTUREBLT);
    ::ReleaseDC(hWndDesktop, hDcDesktop);
    ::DeleteDC(hDcMem);
    return hBmp;
}

HBITMAP CreateDesktopMaskBitmap(HWND hWnd)
{
    HDC hDCMem = CreateCompatibleDC(NULL);
    RECT rect;
    GetWindowRect(::GetDesktopWindow(), &rect);
    rect.right -= rect.left;
    rect.bottom -= rect.top;
    rect.left = rect.top = 0;
    HBITMAP hBmp = NULL;
    HDC hDC = GetDC(hWnd);
    hBmp = CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
    ReleaseDC(hWnd, hDC);
    HGDIOBJ hOld = SelectObject(hDCMem, hBmp);
    CRenderEngine::DrawColor(hDCMem, rect, 0x4F000000);
    SelectObject(hDCMem, hOld);
    DeleteObject(hDCMem);
    return hBmp;
}

HWND SmallestWindowFromCursor(RECT& rcWindow)
{
    HWND hWnd, hTemp;
    POINT pt;
    ::GetCursorPos(&pt);
    hWnd = ::ChildWindowFromPointEx(::GetDesktopWindow(), pt, CWP_SKIPDISABLED | CWP_SKIPINVISIBLE);
    if (hWnd != NULL)
    {
        hTemp = hWnd;
        while (true)
        {
            ::GetCursorPos(&pt);
            ::ScreenToClient(hTemp, &pt);
            hTemp = ::ChildWindowFromPointEx(hTemp, pt, CWP_SKIPINVISIBLE);
            if (hTemp == NULL || hTemp == hWnd)
                break;
            hWnd = hTemp;
        }
        ::GetWindowRect(hWnd, &rcWindow);
    }
    return hWnd;
}


CCanvasUI::CCanvasUI()
{
}

CCanvasUI::~CCanvasUI()
{
}

LPCTSTR CCanvasUI::GetClass() const
{
    return DUI_CTR_CANVAS;
}

LPVOID CCanvasUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_CANVAS) == 0) return static_cast<CCanvasUI*>(this);
    return CControlUI::GetInterface(pstrName);
}

void CCanvasUI::DoEvent(TEventUI& event)
{
    // if m_rcItem.left < 20 左边可以修改
}


CCanvasContainerUI::CCanvasContainerUI() : m_iCursor(0), m_uButtonState(0)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    m_ptLastMouse.x = m_ptLastMouse.y = 0;
}

LPCTSTR CCanvasContainerUI::GetClass() const
{
    return DUI_CTR_CANVASCONTAINER;
}

LPVOID CCanvasContainerUI::GetInterface(LPCTSTR pstrName)
{
    if (_tcscmp(pstrName, DUI_CTR_CANVASCONTAINER) == 0) return static_cast<CCanvasContainerUI*>(this);
    return CContainerUI::GetInterface(pstrName);
}

UINT CCanvasContainerUI::GetControlFlags() const
{
    return UIFLAG_SETCURSOR;
}

RECT CCanvasContainerUI::GetSizerRect(int iIndex)
{
    LONG lMiddleX = (m_rcItem.left + m_rcItem.right) / 2;
    LONG lMiddleY = (m_rcItem.top + m_rcItem.bottom) / 2;
    LONG SIZER_WIDTH = m_rcInset.left * 2;
    switch (iIndex)
    {
    case 0:
        return CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left + SIZER_WIDTH, m_rcItem.top + SIZER_WIDTH);
    case 1:
        return CDuiRect(lMiddleX - SIZER_WIDTH / 2, m_rcItem.top, lMiddleX + SIZER_WIDTH / 2, m_rcItem.top + SIZER_WIDTH);
    case 2:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.top, m_rcItem.right, m_rcItem.top + SIZER_WIDTH);
    case 3:
        return CDuiRect(m_rcItem.left, lMiddleY - SIZER_WIDTH / 2, m_rcItem.left + SIZER_WIDTH, lMiddleY + SIZER_WIDTH / 2);
    case 4:
        break;
    case 5:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, lMiddleY - SIZER_WIDTH / 2, m_rcItem.right, lMiddleY + SIZER_WIDTH / 2);
    case 6:
        return CDuiRect(m_rcItem.left, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.left + SIZER_WIDTH, m_rcItem.bottom);
    case 7:
        return CDuiRect(lMiddleX - SIZER_WIDTH / 2, m_rcItem.bottom - SIZER_WIDTH, lMiddleX + SIZER_WIDTH / 2, m_rcItem.bottom);
    case 8:
        return CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.right, m_rcItem.bottom);
    default:
        break;
    }
    return CDuiRect();
}

LPTSTR CCanvasContainerUI::GetSizerCursor(POINT& pt, int& iCursor)
{
    LONG SIZER_WIDTH = m_rcInset.left * 2;
    LONG SIZER_TO_ROOT = 20;
    RECT rcRoot = m_pManager->GetRoot()->GetPos();
    iCursor = -1;
    for (int i = 8; i >= 0; --i)
    {
        RECT rc = GetSizerRect(i);
        if (::PtInRect(&rc, pt))
        {
            iCursor = i;
            break;
        }
    }
    if (iCursor < 0 && rcRoot.bottom - m_rcItem.bottom < SIZER_TO_ROOT)
    {
        RECT rc = CDuiRect(m_rcItem.left, m_rcItem.bottom - SIZER_WIDTH, m_rcItem.right, m_rcItem.bottom);
        if (::PtInRect(&rc, pt))
            iCursor = 7;
    }
    if (iCursor < 0 && rcRoot.right - m_rcItem.right < SIZER_TO_ROOT)
    {
        RECT rc = CDuiRect(m_rcItem.right - SIZER_WIDTH, m_rcItem.top, m_rcItem.right, m_rcItem.bottom);
        if (::PtInRect(&rc, pt))
            iCursor = 5;
    }
    if (iCursor < 0 && m_rcItem.left - rcRoot.left < SIZER_TO_ROOT)
    {
        RECT rc = CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.left + SIZER_WIDTH, m_rcItem.bottom);
        if (::PtInRect(&rc, pt))
            iCursor = 3;
    }
    if (iCursor < 0 && m_rcItem.top - rcRoot.top < SIZER_TO_ROOT)
    {
        RECT rc = CDuiRect(m_rcItem.left, m_rcItem.top, m_rcItem.right, m_rcItem.top + SIZER_WIDTH);
        if (::PtInRect(&rc, pt))
            iCursor = 1;
    }
    if (iCursor < 0) iCursor = 4;
    switch (iCursor)
    {
    case 0:
        return IDC_SIZENWSE;
    case 1:
        return IDC_SIZENS;
    case 2:
        return IDC_SIZENESW;
    case 3:
        return IDC_SIZEWE;
    case 4:
        return IDC_SIZEALL;
    case 5:
        return IDC_SIZEWE;
    case 6:
        return IDC_SIZENESW;
    case 7:
        return IDC_SIZENS;
    case 8:
        return IDC_SIZENWSE;
    default:
        return IDC_SIZEALL;
    }
}

void CCanvasContainerUI::PaintBorder(HDC hDC)
{
    for (int i = 0; i < 9; ++i)
        CRenderEngine::DrawColor(hDC, GetSizerRect(i), m_dwBorderColor);
    RECT rc = m_rcItem;
    rc.left += m_rcInset.left;
    rc.top += m_rcInset.left;
    rc.right -= m_rcInset.left;
    rc.bottom -= m_rcInset.left;
    CRenderEngine::DrawRect(hDC, rc, m_rcBorderSize.left, m_dwBorderColor);
}

void CCanvasContainerUI::DoEvent(TEventUI& event)
{
    if (event.Type == UIEVENT_SETCURSOR && IsEnabled())
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0) return;
        ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetSizerCursor(event.ptMouse, m_iCursor))));
        return;
    }
    if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
    {
        m_uButtonState |= UISTATE_CAPTURED;
        m_ptLastMouse = event.ptMouse;
        RECT rcWindow = CScrCaptureWnd::Instance()->GetWindowRect();
        RECT rcClipPadding = CScrCaptureWnd::Instance()->GetClipPadding();
        switch (m_iCursor)
        {
        case 0:
        case 1:
        case 3:
            m_ptClipBasePoint.x = rcWindow.right - rcClipPadding.right;
            m_ptClipBasePoint.y = rcWindow.bottom - rcClipPadding.bottom;
            break;
        case 2:
        case 5:
            m_ptClipBasePoint.x = rcWindow.left + rcClipPadding.left;
            m_ptClipBasePoint.y = rcWindow.bottom - rcClipPadding.bottom;
            break;
        case 6:
        case 7:
            m_ptClipBasePoint.x = rcWindow.right - rcClipPadding.right;
            m_ptClipBasePoint.y = rcWindow.top + rcClipPadding.top;
            break;
        case 8:
        default:
            m_ptClipBasePoint.x = rcWindow.left + rcClipPadding.left;
            m_ptClipBasePoint.y = rcWindow.top + rcClipPadding.top;
            break;
        }
        return;
    }
    if (event.Type == UIEVENT_BUTTONUP)
    {
        if ((m_uButtonState & UISTATE_CAPTURED) != 0)
            m_uButtonState &= ~UISTATE_CAPTURED;
        return;
    }
    else if (event.Type == UIEVENT_MOUSEMOVE)
    {
        if ((m_uButtonState & UISTATE_CAPTURED) == 0) return;
        RECT rcWindow = CScrCaptureWnd::Instance()->GetWindowRect();
        RECT rcClipPadding = CScrCaptureWnd::Instance()->GetClipPadding();
        switch (m_iCursor)
        {
        case 0:
        case 2:
        case 6:
        case 8:
            rcClipPadding.left = min(event.ptMouse.x, m_ptClipBasePoint.x) - rcWindow.left;
            if (rcClipPadding.left < 0) rcClipPadding.left = 0;
            rcClipPadding.top = min(event.ptMouse.y, m_ptClipBasePoint.y) - rcWindow.top;
            if (rcClipPadding.top < 0) rcClipPadding.top = 0;
            rcClipPadding.right = rcWindow.right - max(event.ptMouse.x, m_ptClipBasePoint.x);
            if (rcClipPadding.right < 0) rcClipPadding.right = 0;
            rcClipPadding.bottom = rcWindow.bottom - max(event.ptMouse.y, m_ptClipBasePoint.y);
            if (rcClipPadding.bottom < 0) rcClipPadding.bottom = 0;
            break;
        case 1:
        case 7:
            rcClipPadding.top = min(event.ptMouse.y, m_ptClipBasePoint.y) - rcWindow.top;
            if (rcClipPadding.top < 0) rcClipPadding.top = 0;
            rcClipPadding.bottom = rcWindow.bottom - max(event.ptMouse.y, m_ptClipBasePoint.y);
            if (rcClipPadding.bottom < 0) rcClipPadding.bottom = 0;
            break;
        case 3:
        case 5:
            rcClipPadding.left = min(event.ptMouse.x, m_ptClipBasePoint.x) - rcWindow.left;
            if (rcClipPadding.left < 0) rcClipPadding.left = 0;
            rcClipPadding.right = rcWindow.right - max(event.ptMouse.x, m_ptClipBasePoint.x);
            if (rcClipPadding.right < 0) rcClipPadding.right = 0;
            break;
        default:
        {
            LONG cx = event.ptMouse.x - m_ptLastMouse.x;
            LONG cy = event.ptMouse.y - m_ptLastMouse.y;
            if (rcClipPadding.left + cx >= 0 && rcClipPadding.right - cx >= 0)
            {
                rcClipPadding.left += cx;
                rcClipPadding.right -= cx;
            }
            if (rcClipPadding.top + cy >= 0 && rcClipPadding.bottom - cy >= 0)
            {
                rcClipPadding.top += cy;
                rcClipPadding.bottom -= cy;
            }
        }
        break;
        }
        m_ptLastMouse = event.ptMouse;
        CScrCaptureWnd::Instance()->SetClipPadding(rcClipPadding);
    }
    CContainerUI::DoEvent(event);
}


CScrCaptureWnd* CScrCaptureWnd::m_pInstance = NULL;

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
    CControlUI* CreateControl(LPCTSTR pstrClass)
    {
        if (_tcscmp(pstrClass, DUI_CTR_CANVAS) == 0) return new CCanvasUI;
        else if (_tcscmp(pstrClass, DUI_CTR_CANVASCONTAINER) == 0) return new CCanvasContainerUI;
        return NULL;
    }
};

CScrCaptureWnd::CScrCaptureWnd() : m_hEditMenu(NULL), m_bClipChoiced(false)
{
    m_ptClipBasePoint.x = m_ptClipBasePoint.y = 0;
    ::ZeroMemory(&m_rcWindow, sizeof(m_rcWindow));
    ::ZeroMemory(&m_rcClipPadding, sizeof(m_rcClipPadding));
    ::ZeroMemory(m_sClipDrawStringBuf, sizeof(m_sClipDrawStringBuf));
};

CScrCaptureWnd::~CScrCaptureWnd()
{
    ::DestroyMenu(m_hEditMenu);
}

CScrCaptureWnd* CScrCaptureWnd::Instance()
{
    if (m_pInstance == NULL) m_pInstance = new CScrCaptureWnd;
    return m_pInstance;
}

LPCTSTR CScrCaptureWnd::GetWindowClassName() const
{
    return _T("UIScrCaptureFrame");
}

UINT CScrCaptureWnd::GetClassStyle() const
{
    return UI_CLASSSTYLE_DIALOG | CS_DBLCLKS;
}

void CScrCaptureWnd::OnFinalMessage(HWND /*hWnd*/)
{
    delete this;
};

void CScrCaptureWnd::Init()
{
    m_hEditMenu = ::CreatePopupMenu();
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_ADDRECTANGE, _T("添加矩形"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_ADDELLIPSE, _T("添加椭圆"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_ADDARROW, _T("添加箭头"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_ADDBRUSH, _T("添加画刷工具"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_ADDTEXT, _T("添加文字"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_UNDO, _T("撤销编辑"));
    ::AppendMenu(m_hEditMenu, MF_SEPARATOR, 0, NULL);
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_RECHOICE, _T("重新选择截图区域"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_SAVEANDQUIT, _T("复制并退出截图"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_SAVE, _T("保存"));
    ::AppendMenu(m_hEditMenu, MF_SEPARATOR, 0, NULL);
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_HIDETOOLBAR, _T("隐藏编辑工具栏"));
    ::AppendMenu(m_hEditMenu, MF_STRING, MENUCMD_QUIT, _T("退出截屏"));
    m_pDesktopImage = m_pm.FindControl(_T("desktopimage"));
    m_pDesktopMask = m_pm.FindControl(_T("desktopmask"));
    m_pDesktopMaskBorder = m_pm.FindControl(_T("desktopmaskborder"));
    m_pDesktopCanvasContainer = static_cast<CContainerUI*>(m_pm.FindControl(_T("desktopcanvascontainer")));
    if (m_pDesktopImage == NULL || m_pDesktopMask == NULL ||
        m_pDesktopMaskBorder == NULL || m_pDesktopCanvasContainer == NULL) 
        ::PostQuitMessage(0L);
    ::GetWindowRect(::GetDesktopWindow(), &m_rcWindow);
    ::SetWindowPos(GetHWND(), NULL, m_rcWindow.left, m_rcWindow.top, m_rcWindow.right - m_rcWindow.left, m_rcWindow.bottom - m_rcWindow.top, SWP_NOZORDER);
    HBITMAP hDesktopBmp = CreateDesktopBitmap(GetHWND());
    m_pm.AddImage(_T("desktopimage"), hDesktopBmp, m_rcWindow.right - m_rcWindow.left, m_rcWindow.bottom - m_rcWindow.top, false);
    HBITMAP hDesktopBmpMask = CreateDesktopMaskBitmap(GetHWND());
    m_pm.AddImage(_T("desktopimagemask"), hDesktopBmpMask, m_rcWindow.right - m_rcWindow.left, m_rcWindow.bottom - m_rcWindow.top, true);
    m_pDesktopImage->SetBkImage(_T("desktopimage"));
    m_pDesktopMask->SetBkImage(_T("desktopimagemask"));
}

void CScrCaptureWnd::OnPrepare()
{
}

void CScrCaptureWnd::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("windowinit"))
        OnPrepare();
    else if (msg.sType == _T("click"))
    {
    }
}

RECT CScrCaptureWnd::GetWindowRect() const
{
    return m_rcWindow;
}

RECT CScrCaptureWnd::GetClipPadding() const
{
    return m_rcClipPadding;
}

void CScrCaptureWnd::SetClipPadding(RECT rc)
{
    m_rcClipPadding = rc;
    _stprintf_s(m_sClipDrawStringBuf, MAX_PATH - 1, _T("file='desktopimagemask' scale9='%d,%d,%d,%d' hole='true'"),
                m_rcClipPadding.left, m_rcClipPadding.top, m_rcClipPadding.right, m_rcClipPadding.bottom);
    m_pDesktopMask->SetBkImage(m_sClipDrawStringBuf);
    m_pDesktopMaskBorder->SetPos(GetClipRect());
    m_pDesktopCanvasContainer->SetPos(GetCanvasContainerRect());
}

RECT CScrCaptureWnd::GetClipRect() const
{
    RECT rc = m_rcClipPadding;
    rc.left = rc.left + m_rcWindow.left;
    rc.top = rc.top + m_rcWindow.top;
    rc.right = m_rcWindow.right - rc.right;
    rc.bottom = m_rcWindow.bottom - rc.bottom;
    return rc;
}

RECT CScrCaptureWnd::GetCanvasContainerRect() const
{
    RECT rcInset = m_pDesktopCanvasContainer->GetInset();
    RECT rc = m_rcClipPadding;
    rc.left = rc.left + m_rcWindow.left - rcInset.left;
    rc.top = rc.top + m_rcWindow.top - rcInset.left;
    rc.right = m_rcWindow.right - rc.right + rcInset.left;
    rc.bottom = m_rcWindow.bottom - rc.bottom + rcInset.left;
    return rc;
}

LRESULT CScrCaptureWnd::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
    if (uMsg == WM_KEYDOWN)
    {
        if (wParam == VK_ESCAPE)
        {
            PostQuitMessage(0);
            return true;
        }
    }
    return false;
}

LRESULT CScrCaptureWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        m_pm.Init(m_hWnd);
        m_pm.AddPreMessageFilter(this);
        CDialogBuilder builder;
        CDialogBuilderCallbackEx cb;
        CControlUI* pRoot = builder.Create(_T("ScrCapture.xml"), (UINT)0, &cb, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        Init();
        return 0;
    }
    else if (uMsg == WM_DESTROY)
        ::PostQuitMessage(0L);
    else if (uMsg == WM_LBUTTONDOWN)
    {
        if (!m_bClipChoiced)
        {
            m_pDesktopCanvasContainer->SetEnabled(false);
            m_ptClipBasePoint.x = GET_X_LPARAM(lParam);
            m_ptClipBasePoint.y = GET_Y_LPARAM(lParam);
            return 0;
        }
    }
    else if (uMsg == WM_LBUTTONUP)
    {
        if (!m_bClipChoiced)
        {
            m_bClipChoiced = true;
            m_pDesktopMaskBorder->SetVisible(false);
            m_pDesktopCanvasContainer->SetVisible(true);
            m_pDesktopCanvasContainer->SetEnabled(true);
            SetClipPadding(m_rcClipPadding);
            return 0;
        }
        else
        {
        }
    }
    else if (uMsg == WM_LBUTTONDBLCLK)
    {
        // todo:完成截图，保存到剪贴板
        m_bClipChoiced = true;
        ::PostQuitMessage(0L);
        return 0;
    }
    else if (uMsg == WM_RBUTTONUP)
    {
        if (m_bClipChoiced)
        {
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            RECT rcClipRect = GetClipRect();
            if (::PtInRect(&rcClipRect, pt))
            {
                ::ClientToScreen(m_hWnd, &pt);
                ::TrackPopupMenu(m_hEditMenu, TPM_TOPALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
                ::PostMessage(m_hWnd, WM_NULL, 0, 0);
            }
            else
            {
                m_bClipChoiced = false;
                m_pDesktopCanvasContainer->SetEnabled(false);
                ::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
            }
        }
        else
        {
            // 退出截图
            ::PostQuitMessage(0L);
        }
        return 0;
    }
    else if (uMsg == WM_MOUSEMOVE)
    {
        if (!m_bClipChoiced)
        {
            if (::GetKeyState(VK_LBUTTON) < 0)    // 左键按下状态
            {
                if (m_ptClipBasePoint.x != GET_X_LPARAM(lParam) || m_ptClipBasePoint.y != GET_Y_LPARAM(lParam))
                {
                    m_rcClipPadding.left = min(GET_X_LPARAM(lParam), m_ptClipBasePoint.x) - m_rcWindow.left;
                    if (m_rcClipPadding.left < 0) m_rcClipPadding.left = 0;
                    m_rcClipPadding.top = min(GET_Y_LPARAM(lParam), m_ptClipBasePoint.y) - m_rcWindow.top;
                    if (m_rcClipPadding.top < 0) m_rcClipPadding.top = 0;
                    m_rcClipPadding.right = m_rcWindow.right - max(GET_X_LPARAM(lParam), m_ptClipBasePoint.x);
                    if (m_rcClipPadding.right < 0) m_rcClipPadding.right = 0;
                    m_rcClipPadding.bottom = m_rcWindow.bottom - max(GET_Y_LPARAM(lParam), m_ptClipBasePoint.y);
                    if (m_rcClipPadding.bottom < 0) m_rcClipPadding.bottom = 0;
                    m_pDesktopMaskBorder->SetVisible(false);
                    m_pDesktopCanvasContainer->SetVisible(true);
                    SetClipPadding(m_rcClipPadding);
                }
            }
            else
            {
                RECT rc;
                HWND hWnd;
                ::EnableWindow(m_hWnd, FALSE);
                hWnd = SmallestWindowFromCursor(rc);
                ::EnableWindow(m_hWnd, TRUE);
                ::SetFocus(m_hWnd);
                rc.left = rc.left - m_rcWindow.left;
                if (rc.left < 0) rc.left = 0;
                rc.top = rc.top - m_rcWindow.top;
                if (rc.top < 0) rc.top = 0;
                rc.right = m_rcWindow.right - rc.right;
                if (rc.right < 0) rc.right = 0;
                rc.bottom = m_rcWindow.bottom - rc.bottom;
                if (rc.bottom < 0) rc.bottom = 0;
                if (rc.left != m_rcClipPadding.left || rc.right != m_rcClipPadding.right || rc.top != m_rcClipPadding.top || rc.bottom != m_rcClipPadding.bottom)
                {
                    SetClipPadding(rc);
                    m_pDesktopMaskBorder->SetVisible(true);
                    m_pDesktopCanvasContainer->SetVisible(false);
                }
            }
        }
        else
        {
        }
    }
    else if (uMsg == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
            case MENUCMD_ADDRECTANGE:
                return 0;
            case MENUCMD_ADDELLIPSE:
                return 0;
            case MENUCMD_ADDARROW:
                return 0;
            case MENUCMD_ADDBRUSH:
                return 0;
            case MENUCMD_ADDTEXT:
                return 0;
            case MENUCMD_UNDO:
                return 0;
            case MENUCMD_RECHOICE:
            {
                m_bClipChoiced = false;
                m_pDesktopCanvasContainer->SetEnabled(false);
                POINT pt;
                ::GetCursorPos(&pt);
                ::SendMessage(m_hWnd, WM_MOUSEMOVE, 0, MAKELPARAM(pt.x, pt.y));
            }
            return 0;
            case MENUCMD_SAVEANDQUIT:
            {
                // todo:
                PostQuitMessage(0L);
            }
            return 0;
            case MENUCMD_SAVE:
            {
                // todo:弹出保存对话框
            }
            return 0;
            case MENUCMD_HIDETOOLBAR:
                return 0;
            case MENUCMD_QUIT:
                PostQuitMessage(0L);
                return 0;
        }
    }
    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

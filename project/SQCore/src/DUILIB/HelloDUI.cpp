
#include "stdafx.h"
#include "HelloDUI.h"




LPCTSTR DuiTest01::GetWindowClassName() const
{
    return _T("DuiMainFrame");
};

void DuiTest01::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("click"))
    {
        if (msg.pSender->GetName() == _T("btnHello"))
            ::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
    }
};

LRESULT DuiTest01::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    if (uMsg == WM_CREATE)
    {
        CControlUI* pWnd = new CButtonUI;           //CControlUI是所有控件的基类
        pWnd->SetName(_T("btnHello"));              //设置控件ID的名称必须唯一
        pWnd->SetText(_T("HelloWorld"));            //设置文字
        pWnd->SetBkColor(0xFF00FF00);               //设置背景
        m_PaintManager.Init(m_hWnd);
        m_PaintManager.AttachDialog(pWnd);
        m_PaintManager.AddNotifier(this);   // 添加控件等消息响应，这样消息就会传达到duilib的消息循环，我们可以在Notify函数里做消息处理
        return lRes;
    }
    if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
        return lRes;
    return __super::HandleMessage(uMsg, wParam, lParam);
};



DuiTest02::DuiTest02() : m_pWndShadow(NULL) 
{
}

LPCTSTR DuiTest02::GetWindowClassName() const
{
    return _T("UIMainFrame");
}

UINT DuiTest02::GetClassStyle() const
{
    return UI_CLASSSTYLE_FRAME | CS_DBLCLKS;
}

void DuiTest02::OnFinalMessage(HWND hWnd)
{
    delete this;
}

void DuiTest02::Init()
{
}

bool DuiTest02::OnHChanged(void* param)
{
    TNotifyUI* pMsg = (TNotifyUI*)param;
    if (pMsg->sType == _T("valuechanged"))
    {
        short H, S, L;
        CPaintManagerUI::GetHSL(&H, &S, &L);
        CPaintManagerUI::SetHSL(true, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue(), S, L);
    }
    return true;
}

bool DuiTest02::OnSChanged(void* param)
{
    TNotifyUI* pMsg = (TNotifyUI*)param;
    if (pMsg->sType == _T("valuechanged"))
    {
        short H, S, L;
        CPaintManagerUI::GetHSL(&H, &S, &L);
        CPaintManagerUI::SetHSL(true, H, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue(), L);
    }
    return true;
}

bool DuiTest02::OnLChanged(void* param)
{
    TNotifyUI* pMsg = (TNotifyUI*)param;
    if (pMsg->sType == _T("valuechanged"))
    {
        short H, S, L;
        CPaintManagerUI::GetHSL(&H, &S, &L);
        CPaintManagerUI::SetHSL(true, H, S, (static_cast<CSliderUI*>(pMsg->pSender))->GetValue());
    }
    return true;
}

bool DuiTest02::OnAlphaChanged(void* param)
{
    TNotifyUI* pMsg = (TNotifyUI*)param;
    if (pMsg->sType == _T("valuechanged"))
        m_pm.SetOpacity((static_cast<CSliderUI*>(pMsg->pSender))->GetValue());
    return true;
}

void DuiTest02::OnPrepare()
{
    CSliderUI* pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("alpha_controlor")));
    if (pSilder) pSilder->OnNotify += MakeDelegate(this, &DuiTest02::OnAlphaChanged);
    pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("h_controlor")));
    if (pSilder) pSilder->OnNotify += MakeDelegate(this, &DuiTest02::OnHChanged);
    pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("s_controlor")));
    if (pSilder) pSilder->OnNotify += MakeDelegate(this, &DuiTest02::OnSChanged);
    pSilder = static_cast<CSliderUI*>(m_pm.FindControl(_T("l_controlor")));
    if (pSilder) pSilder->OnNotify += MakeDelegate(this, &DuiTest02::OnLChanged);
}

void DuiTest02::Notify(TNotifyUI& msg)
{
    if (msg.sType == _T("windowinit")) OnPrepare();
    else if (msg.sType == _T("click"))
    {
        if (msg.pSender->GetName() == _T("insertimagebtn"))
        {
            CRichEditUI* pRich = static_cast<CRichEditUI*>(m_pm.FindControl(_T("testrichedit")));
            if (pRich)
                pRich->RemoveAll();
        }
        else if (msg.pSender->GetName() == _T("changeskinbtn"))
        {
            if (CPaintManagerUI::GetResourcePath() == CPaintManagerUI::GetInstancePath())
                CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\FlashRes"));
            else
                CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
            CPaintManagerUI::ReloadSkin();
        }
    }
}

LRESULT DuiTest02::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == WM_CREATE)
    {
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("test1.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        m_pWndShadow = new CWndShadow;
        m_pWndShadow->Create(m_hWnd);
        RECT rcCorner = { 3, 3, 4, 4 };
        RECT rcHoleOffset = { 0, 0, 0, 0 };
        m_pWndShadow->SetImage(_T("LeftWithFill.png"), rcCorner, rcHoleOffset);
        DWMNCRENDERINGPOLICY ncrp = DWMNCRP_ENABLED;
        SetWindowAttribute(m_hWnd, DWMWA_TRANSITIONS_FORCEDISABLED, &ncrp, sizeof(ncrp));
        //DWM_BLURBEHIND bb = {0};
        //bb.dwFlags = DWM_BB_ENABLE;
        //bb.fEnable = true;
        //bb.hRgnBlur = NULL;
        //EnableBlurBehindWindow(m_hWnd, bb);
        //DWM_MARGINS margins = {-1}/*{0,0,0,25}*/;
        //ExtendFrameIntoClientArea(m_hWnd, margins);
        Init();
        return 0;
    }
    else if (uMsg == WM_DESTROY)
        ::PostQuitMessage(0L);
    else if (uMsg == WM_NCACTIVATE)
    {
        if (!::IsIconic(*this)) return (wParam == 0) ? TRUE : FALSE;
    }
    LRESULT lRes = 0;
    if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}









#include <objbase.h>
#include <zmouse.h>
#include <exdisp.h>
#include <comdef.h>
#include <vector>
#include <sstream>

#define WM_ADDLISTITEM WM_USER + 50

std::vector<std::string> domain;        //存放第二列数据
std::vector<std::string> desc;          //存放第三列数据

//线程函数中传入的结构体变量，使用线程为了使界面线程立即返回，防止卡住，你们懂得。
struct Prama
{
    HWND hWnd;
    CListUI* pList;
    CButtonUI* pSearch;
    CDuiString tDomain;
};

inline HBITMAP CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
{
    LPBITMAPINFO lpbiSrc = NULL;
    lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
    if (lpbiSrc == NULL) return NULL;
    lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    lpbiSrc->bmiHeader.biWidth = cx;
    lpbiSrc->bmiHeader.biHeight = cy;
    lpbiSrc->bmiHeader.biPlanes = 1;
    lpbiSrc->bmiHeader.biBitCount = 32;
    lpbiSrc->bmiHeader.biCompression = BI_RGB;
    lpbiSrc->bmiHeader.biSizeImage = cx * cy;
    lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
    lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
    lpbiSrc->bmiHeader.biClrUsed = 0;
    lpbiSrc->bmiHeader.biClrImportant = 0;
    HBITMAP hBitmap = CreateDIBSection(hDC, lpbiSrc, DIB_RGB_COLORS, (void**)pBits, NULL, NULL);
    delete[] lpbiSrc;
    return hBitmap;
}

class CShadowWnd : public CWindowWnd
{
private:
    bool m_bNeedUpdate;
    TDrawInfo m_di;
    CPaintManagerUI m_pm;

public:
    CShadowWnd()
    {
        m_di.sDrawString = _T("file='menu_bk.png' corner='40,8,8,8'");
    };
    LPCTSTR GetWindowClassName() const
    {
        return _T("UIShadow");
    };
    UINT GetClassStyle() const
    {
        return UI_CLASSSTYLE_FRAME;
    };
    void OnFinalMessage(HWND /*hWnd*/)
    {
        delete this;
    };

    void RePaint()
    {
        RECT rcClient = { 0 };
        ::GetClientRect(m_hWnd, &rcClient);
        DWORD dwWidth = rcClient.right - rcClient.left;
        DWORD dwHeight = rcClient.bottom - rcClient.top;
        HDC hDcPaint = ::GetDC(m_hWnd);
        HDC hDcBackground = ::CreateCompatibleDC(hDcPaint);
        COLORREF* pBackgroundBits;
        HBITMAP hbmpBackground = CreateMyBitmap(hDcPaint, dwWidth, dwHeight, &pBackgroundBits);
        ::ZeroMemory(pBackgroundBits, dwWidth * dwHeight * 4);
        HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hDcBackground, hbmpBackground);
        CRenderEngine::DrawImage(hDcBackground, &m_pm, rcClient, rcClient, m_di);
        RECT rcWnd = { 0 };
        ::GetWindowRect(m_hWnd, &rcWnd);
        BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
        POINT ptPos = { rcWnd.left, rcWnd.top };
        SIZE sizeWnd = { dwWidth, dwHeight };
        POINT ptSrc = { 0, 0 };
        UpdateLayeredWindow(m_hWnd, hDcPaint, &ptPos, &sizeWnd, hDcBackground, &ptSrc, 0, &bf, ULW_ALPHA);
        ::SelectObject(hDcBackground, hOldBitmap);
        if (hDcBackground != NULL) ::DeleteDC(hDcBackground);
        if (hbmpBackground != NULL) ::DeleteObject(hbmpBackground);
        ::ReleaseDC(m_hWnd, hDcPaint);
        m_bNeedUpdate = false;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (uMsg == WM_CREATE) SetTimer(m_hWnd, 9000, 10, NULL);
        else if (uMsg == WM_SIZE) m_bNeedUpdate = true;
        else if (uMsg == WM_CLOSE) KillTimer(m_hWnd, 9000);
        else if (uMsg == WM_TIMER)
        {
            if (LOWORD(wParam) == 9000 && m_bNeedUpdate == true)
            {
                if (!::IsIconic(m_hWnd)) RePaint();
            }
        }
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }
};

class CMenuWnd : public CWindowWnd, public INotifyUI
{
public:
    CMenuWnd() : m_pOwner(NULL), m_pShadowWnd(NULL) { };

    void Init(CControlUI* pOwner, POINT pt)
    {
        if (pOwner == NULL) return;
        m_pOwner = pOwner;
        m_ptPos = pt;
        Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
        HWND hWndParent = m_hWnd;
        while (::GetParent(hWndParent) != NULL) hWndParent = ::GetParent(hWndParent);
        ::ShowWindow(m_hWnd, SW_SHOW);
        ::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
    }

    void AdjustPostion()
    {
        CDuiRect rcWnd;
        GetWindowRect(m_hWnd, &rcWnd);
        int nWidth = rcWnd.GetWidth();
        int nHeight = rcWnd.GetHeight();
        rcWnd.left = m_ptPos.x;
        rcWnd.top = m_ptPos.y;
        rcWnd.right = rcWnd.left + nWidth;
        rcWnd.bottom = rcWnd.top + nHeight;
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
        if (rcWnd.bottom > rcWork.bottom)
        {
            if (nHeight >= rcWork.GetHeight())
            {
                rcWnd.top = 0;
                rcWnd.bottom = nHeight;
            }
            else
            {
                rcWnd.bottom = rcWork.bottom;
                rcWnd.top = rcWnd.bottom - nHeight;
            }
        }
        if (rcWnd.right > rcWork.right)
        {
            if (nWidth >= rcWork.GetWidth())
            {
                rcWnd.left = 0;
                rcWnd.right = nWidth;
            }
            else
            {
                rcWnd.right = rcWork.right;
                rcWnd.left = rcWnd.right - nWidth;
            }
        }
        ::SetWindowPos(m_hWnd, NULL, rcWnd.left, rcWnd.top, rcWnd.GetWidth(), rcWnd.GetHeight(), SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
    }

    LPCTSTR GetWindowClassName() const
    {
        return _T("MenuWnd");
    };
    void OnFinalMessage(HWND /*hWnd*/)
    {
        delete this;
    };

    void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("itemselect"))
            Close();
        else if (msg.sType == _T("itemclick"))
        {
            if (msg.pSender->GetName() == _T("menu_Delete"))
            {
                if (m_pOwner) m_pOwner->GetManager()->SendNotify(m_pOwner, _T("menu_Delete"), 0, 0, true);
            }
        }
    }

    HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL)
    {
        if (m_pShadowWnd == NULL) m_pShadowWnd = new CShadowWnd;
        m_pShadowWnd->Create(hwndParent, _T(""), WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS, WS_EX_LAYERED | WS_EX_TOOLWINDOW, x, y, cx, cy, NULL);
        dwExStyle |= WS_EX_TOOLWINDOW;
        return CWindowWnd::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy, hMenu);
    }

    void ShowWindow(bool bShow = true, bool bTakeFocus = true)
    {
        if (m_pShadowWnd != NULL) m_pShadowWnd->ShowWindow(bShow, false);
        CWindowWnd::ShowWindow(bShow, bTakeFocus);
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("menu.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        m_pm.SetRoundCorner(3, 3);
        CListUI* pList = static_cast<CListUI*>(m_pOwner);
        int nSel = pList->GetCurSel();
        if (nSel < 0)
        {
            CControlUI* pControl = m_pm.FindControl(_T("menu_Delete"));
            if (pControl) pControl->SetEnabled(false);
        }
        AdjustPostion();
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (m_pShadowWnd != NULL) m_pShadowWnd->Close();
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if ((HWND)wParam == m_hWnd) bHandled = TRUE;
        else if (m_pShadowWnd != NULL && (HWND)wParam == m_pShadowWnd->GetHWND())
        {
            CWindowWnd::HandleMessage(uMsg, wParam, lParam);
            ::SetFocus(m_hWnd);
            bHandled = TRUE;
        }
        else
        {
            Close();
            bHandled = FALSE;
        }
        return 0;
    }

    LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (wParam == VK_ESCAPE) Close();
        return 0;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (!::IsIconic(*this))
        {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            HRGN hRgn = ::CreateRectRgn(rcWnd.left + 8, rcWnd.top + 8, rcWnd.right - 8, rcWnd.bottom - 8);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }
        if (m_pShadowWnd != NULL)
        {
            RECT rcWnd = { 0 };
            ::GetWindowRect(m_hWnd, &rcWnd);
            ::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left,
                rcWnd.bottom - rcWnd.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (m_pShadowWnd != NULL)
        {
            RECT rcWnd = { 0 };
            ::GetWindowRect(m_hWnd, &rcWnd);
            ::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left,
                rcWnd.bottom - rcWnd.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch (uMsg)
        {
        case WM_CREATE:
            lRes = OnCreate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_CLOSE:
            lRes = OnClose(uMsg, wParam, lParam, bHandled);
            break;
        case WM_KILLFOCUS:
            lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
            break;
        case WM_KEYDOWN:
            lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
            break;
        case WM_MOUSEWHEEL:
            break;
        case WM_SIZE:
            lRes = OnSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_MOVE:
            lRes = OnMove(uMsg, wParam, lParam, bHandled);
            break;
        default:
            bHandled = FALSE;
        }
        if (bHandled) return lRes;
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

public:
    CPaintManagerUI m_pm;
    CControlUI* m_pOwner;
    POINT m_ptPos;
    CShadowWnd* m_pShadowWnd;
};

class ListMainForm : public CWindowWnd, public INotifyUI, public IListCallbackUI
{
public:
    ListMainForm()
    {
    };

    LPCTSTR GetWindowClassName() const
    {
        return _T("ScanMainForm");
    };

    UINT GetClassStyle() const
    {
        return CS_DBLCLKS;
    };

    void OnFinalMessage(HWND /*hWnd*/)
    {
        delete this;
    };

    void Init()
    {
        m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
        m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
        m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
        m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
        m_pSearch = static_cast<CButtonUI*>(m_pm.FindControl(_T("btn")));
    }

    void OnPrepare(TNotifyUI& msg)
    {
    }

    static DWORD WINAPI Search(LPVOID lpParameter)
    {
        try
        {
            struct Prama* prama = (struct Prama*)lpParameter;
            CListUI* pList = prama->pList;
            CButtonUI* pSearch = prama->pSearch;
            CDuiString tDomain = prama->tDomain;
            //-------------------------------------
            /*
            添加数据循环
            */
            for (int i = 0; i < 100; i++)
            {
                std::stringstream ss;
                ss << "www." << i << ".com";
                domain.push_back(ss.str());
                ss.clear();
                ss << "it's " << i;
                desc.push_back(ss.str());
                CListTextElementUI* pListElement = new CListTextElementUI;
                pListElement->SetTag(i);
                if (pListElement != NULL)
                    ::PostMessage(prama->hWnd, WM_ADDLISTITEM, 0L, (LPARAM)pListElement);
                /*
                Sleep 为了展示添加的动态效果，故放慢了添加速度，同时可以看到添加过程中界面仍然可以响应
                */
                ::Sleep(100);
            }
            //------------------------------------------
            delete prama;
            pSearch->SetEnabled(true);
            return 0;
        }
        catch (...)
        {
            return 0;
        }
    }

    void OnSearch()
    {
        struct Prama* prama = new Prama;
        CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("domainlist")));
        CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("input")));
        pEdit->SetEnabled(false);
        CDuiString input = pEdit->GetText();
        m_pSearch->SetEnabled(false);
        pList->RemoveAll();
        domain.empty();
        domain.resize(0);
        desc.empty();
        desc.resize(0);
        DWORD dwThreadID = 0;
        pList->SetTextCallback(this);//[1]
        prama->hWnd = GetHWND();
        prama->pList = pList;
        prama->pSearch = m_pSearch;
        prama->tDomain = input;
        HANDLE hThread = CreateThread(NULL, 0, &ListMainForm::Search, (LPVOID)prama, 0, &dwThreadID);
    }

    //关键的回调函数，IListCallbackUI 中的一个虚函数，渲染时候会调用,在[1]中设置了回调对象
    LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem)
    {
        TCHAR szBuf[MAX_PATH] = { 0 };
        switch (iSubItem)
        {
        case 0:
            _stprintf(szBuf, _T("%d"), iIndex);
            break;
        case 1:
        {
#ifdef _UNICODE
            int iLen = domain[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen);
            _stprintf(szBuf, lpText);
            delete[] lpText;
#else
            _stprintf(szBuf, domain[iIndex].c_str());
#endif
        }
        break;
        case 2:
        {
#ifdef _UNICODE
            int iLen = desc[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, desc[iIndex].c_str(), -1, (LPWSTR)lpText, iLen);
            _stprintf(szBuf, lpText);
            delete[] lpText;
#else
            _stprintf(szBuf, desc[iIndex].c_str());
#endif
        }
        break;
        }
        pControl->SetUserData(szBuf);
        return pControl->GetUserData();
    }

    void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("windowinit"))
            OnPrepare(msg);
        else if (msg.sType == _T("click"))
        {
            if (msg.pSender == m_pCloseBtn)
            {
                PostQuitMessage(0);
                return;
            }
            else if (msg.pSender == m_pMinBtn)
            {
                SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
                return;
            }
            else if (msg.pSender == m_pMaxBtn)
            {
                SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
                return;
            }
            else if (msg.pSender == m_pRestoreBtn)
            {
                SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
                return;
            }
            else if (msg.pSender == m_pSearch)
                OnSearch();
        }
        else if (msg.sType == _T("setfocus"))
        {
        }
        else if (msg.sType == _T("itemclick"))
        {
        }
        else if (msg.sType == _T("itemactivate"))
        {
            int iIndex = msg.pSender->GetTag();
            CDuiString sMessage = _T("Click: ");;
#ifdef _UNICODE
            int iLen = domain[iIndex].length();
            LPWSTR lpText = new WCHAR[iLen + 1];
            ::ZeroMemory(lpText, (iLen + 1) * sizeof(WCHAR));
            ::MultiByteToWideChar(CP_ACP, 0, domain[iIndex].c_str(), -1, (LPWSTR)lpText, iLen);
            sMessage += lpText;
            delete[] lpText;
#else
            sMessage += domain[iIndex].c_str();
#endif
            ::MessageBox(NULL, sMessage.GetData(), _T("提示(by tojen)"), MB_OK);
        }
        else if (msg.sType == _T("menu"))
        {
            if (msg.pSender->GetName() != _T("domainlist")) return;
            CMenuWnd* pMenu = new CMenuWnd();
            if (pMenu == NULL)
                return;
            POINT pt = { msg.ptMouse.x, msg.ptMouse.y };
            ::ClientToScreen(*this, &pt);
            pMenu->Init(msg.pSender, pt);
        }
        else if (msg.sType == _T("menu_Delete"))
        {
            CListUI* pList = static_cast<CListUI*>(msg.pSender);
            int nSel = pList->GetCurSel();
            if (nSel < 0) return;
            pList->RemoveAt(nSel);
            domain.erase(domain.begin() + nSel);
            desc.erase(desc.begin() + nSel);
        }
    }

    LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        CListTextElementUI* pListElement = (CListTextElementUI*)lParam;
        CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("domainlist")));
        if (pList) pList->Add(pListElement);
        return 0;
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        m_pm.Init(m_hWnd);
        //m_pm.SetTransparent(100);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        Init();
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        ::PostQuitMessage(0L);
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (::IsIconic(*this)) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);
        RECT rcClient;
        ::GetClientRect(*this, &rcClient);
        if (!::IsZoomed(*this))
        {
            RECT rcSizeBox = m_pm.GetSizeBox();
            if (pt.y < rcClient.top + rcSizeBox.top)
            {
                if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
                if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
                return HTTOP;
            }
            else if (pt.y > rcClient.bottom - rcSizeBox.bottom)
            {
                if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
                if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
            if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
        }
        RECT rcCaption = m_pm.GetCaptionRect();
        if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom)
        {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
                _tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
                _tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
                return HTCAPTION;
        }
        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0))
        {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++;
            rcWnd.bottom++;
            RECT rc = { rcWnd.left, rcWnd.top + szRoundCorner.cx, rcWnd.right, rcWnd.bottom };
            HRGN hRgn1 = ::CreateRectRgnIndirect(&rc);
            HRGN hRgn2 = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
            ::CombineRgn(hRgn1, hRgn1, hRgn2, RGN_OR);
            ::SetWindowRgn(*this, hRgn1, TRUE);
            ::DeleteObject(hRgn1);
            ::DeleteObject(hRgn2);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
        rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMaxPosition.x = rcWork.left;
        lpMMI->ptMaxPosition.y = rcWork.top;
        lpMMI->ptMaxSize.x = rcWork.right;
        lpMMI->ptMaxSize.y = rcWork.bottom;
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if (wParam == SC_CLOSE)
        {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        if (::IsZoomed(*this) != bZoomed)
        {
            if (!bZoomed)
            {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(true);
            }
            else
            {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(false);
            }
        }
        return lRes;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch (uMsg)
        {
        case WM_ADDLISTITEM:
            lRes = OnAddListItem(uMsg, wParam, lParam, bHandled);
            break;
        case WM_CREATE:
            lRes = OnCreate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_CLOSE:
            lRes = OnClose(uMsg, wParam, lParam, bHandled);
            break;
        case WM_DESTROY:
            lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCACTIVATE:
            lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCCALCSIZE:
            lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCPAINT:
            lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCHITTEST:
            lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
            break;
        case WM_SIZE:
            lRes = OnSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_GETMINMAXINFO:
            lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
            break;
        case WM_SYSCOMMAND:
            lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);
            break;
        default:
            bHandled = FALSE;
        }
        if (bHandled) return lRes;
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }
public:
    CPaintManagerUI m_pm;
private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
    CButtonUI* m_pSearch;
    //...
};

int DuiListForm(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    //if (1)
    //    return 1;
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
    CPaintManagerUI::SetResourceZip(_T("ListRes.zip"));
    ListMainForm* pFrame = new ListMainForm();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("ListDemo"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 320);
    pFrame->CenterWindow();
    ::ShowWindow(*pFrame, SW_SHOW);
    CPaintManagerUI::MessageLoop();
    return 0;
}


class ComputerExamineUI : public CContainerUI
{
public:
    ComputerExamineUI()
    {
        CDialogBuilder builder;
        CContainerUI* pComputerExamine = static_cast<CContainerUI*>(builder.Create(_T("ComputerExamine.xml"), (UINT)0));
        if (pComputerExamine)
            this->Add(pComputerExamine);
        else
        {
            this->RemoveAll();
            return;
        }
    }
};

class CDialogBuilderCallbackEx : public IDialogBuilderCallback
{
public:
    CControlUI* CreateControl(LPCTSTR pstrClass)
    {
        if (_tcscmp(pstrClass, _T("ComputerExamine")) == 0) return new ComputerExamineUI;
        return NULL;
    }
};

class C360SafeFrameWnd : public CWindowWnd, public INotifyUI
{
public:
    C360SafeFrameWnd() { };
    LPCTSTR GetWindowClassName() const
    {
        return _T("UIMainFrame");
    };
    UINT GetClassStyle() const
    {
        return CS_DBLCLKS;
    };
    void OnFinalMessage(HWND /*hWnd*/)
    {
        delete this;
    };

    void Init()
    {
        m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
        m_pMaxBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("maxbtn")));
        m_pRestoreBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("restorebtn")));
        m_pMinBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("minbtn")));
    }

    void OnPrepare()
    {
    }

    void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("windowinit")) OnPrepare();
        else if (msg.sType == _T("click"))
        {
            if (msg.pSender == m_pCloseBtn)
            {
                PostQuitMessage(0);
                return;
            }
            else if (msg.pSender == m_pMinBtn)
            {
                SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
                return;
            }
            else if (msg.pSender == m_pMaxBtn)
            {
                SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
                return;
            }
            else if (msg.pSender == m_pRestoreBtn)
            {
                SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
                return;
            }
        }
        else if (msg.sType == _T("selectchanged"))
        {
            CDuiString name = msg.pSender->GetName();
            CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_pm.FindControl(_T("switch")));
            if (name == _T("examine"))
                pControl->SelectItem(0);
            else if (name == _T("trojan"))
                pControl->SelectItem(1);
            else if (name == _T("plugins"))
                pControl->SelectItem(2);
            else if (name == _T("vulnerability"))
                pControl->SelectItem(3);
            else if (name == _T("rubbish"))
                pControl->SelectItem(4);
            else if (name == _T("cleanup"))
                pControl->SelectItem(5);
            else if (name == _T("fix"))
                pControl->SelectItem(6);
            else if (name == _T("tool"))
                pControl->SelectItem(7);
        }
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CDialogBuilderCallbackEx cb;
        CControlUI* pRoot = builder.Create(_T("skin.xml"), (UINT)0, &cb, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);
        Init();
        return 0;
    }

    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        //::PostQuitMessage(0L);
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (::IsIconic(*this)) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);
        RECT rcClient;
        ::GetClientRect(*this, &rcClient);
        //      if( !::IsZoomed(*this) ) {
        //          RECT rcSizeBox = m_pm.GetSizeBox();
        //          if( pt.y < rcClient.top + rcSizeBox.top ) {
        //              if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
        //              if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
        //              return HTTOP;
        //          }
        //          else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
        //              if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
        //              if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
        //              return HTBOTTOM;
        //          }
        //          if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
        //          if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
        //      }
        RECT rcCaption = m_pm.GetCaptionRect();
        if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom)
        {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0 &&
                _tcscmp(pControl->GetClass(), DUI_CTR_OPTION) != 0 &&
                _tcscmp(pControl->GetClass(), DUI_CTR_TEXT) != 0)
                return HTCAPTION;
        }
        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0))
        {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++;
            rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        MONITORINFO oMonitor = {};
        oMonitor.cbSize = sizeof(oMonitor);
        ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
        CDuiRect rcWork = oMonitor.rcWork;
        rcWork.Offset(-oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
        LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
        lpMMI->ptMaxPosition.x = rcWork.left;
        lpMMI->ptMaxPosition.y = rcWork.top;
        lpMMI->ptMaxSize.x = rcWork.right;
        lpMMI->ptMaxSize.y = rcWork.bottom;
        bHandled = FALSE;
        return 0;
    }

    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
        if (wParam == SC_CLOSE)
        {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
        BOOL bZoomed = ::IsZoomed(*this);
        LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
        if (::IsZoomed(*this) != bZoomed)
        {
            if (!bZoomed)
            {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(false);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(true);
            }
            else
            {
                CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
                if (pControl) pControl->SetVisible(true);
                pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
                if (pControl) pControl->SetVisible(false);
            }
        }
        return lRes;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch (uMsg)
        {
        case WM_CREATE:
            lRes = OnCreate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_CLOSE:
            lRes = OnClose(uMsg, wParam, lParam, bHandled);
            break;
        case WM_DESTROY:
            lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCACTIVATE:
            lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCCALCSIZE:
            lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCPAINT:
            lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCHITTEST:
            lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
            break;
        case WM_SIZE:
            lRes = OnSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_GETMINMAXINFO:
            lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
            break;
        case WM_SYSCOMMAND:
            lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);
            break;
        default:
            bHandled = FALSE;
        }
        if (bHandled) return lRes;
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }

public:
    CPaintManagerUI m_pm;

private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
    //...
};

int Dui360Safe(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    //if (1)
    //    return 1;
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
    CPaintManagerUI::SetResourceZip(_T("360SafeRes.zip"));
    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr)) return 0;
    C360SafeFrameWnd* pFrame = new C360SafeFrameWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, _T("360安全卫士"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
    pFrame->CenterWindow();
    ::ShowWindow(*pFrame, SW_SHOW);
    CPaintManagerUI::MessageLoop();
    ::CoUninitialize();
    return 0;
}

#include "flash10a.tlh"

class CFrameWnd : public CWindowWnd, public INotifyUI
{
public:
    CFrameWnd() { };
    LPCTSTR GetWindowClassName() const { return _T("UIFrame"); };
    UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
    void OnFinalMessage(HWND /*hWnd*/) { delete this; };

    void Init()
    {
    }

    void Notify(TNotifyUI& msg)
    {
        if (msg.sType == _T("click")) {
            if (msg.pSender->GetName() == _T("closebtn") || msg.pSender->GetName() == _T("closebtn2")) {
                PostQuitMessage(0);
                return;
            }
        }
        else if (msg.sType == _T("showactivex")) {
            if (msg.pSender->GetName() != _T("flash")) return;
            IShockwaveFlash* pFlash = NULL;
            CActiveXUI* pActiveX = static_cast<CActiveXUI*>(msg.pSender);
            pActiveX->GetControl(IID_IUnknown, (void**)&pFlash);
            if (pFlash != NULL) {
                pFlash->put_WMode(_bstr_t(_T("Transparent")));
                pFlash->put_Movie(_bstr_t(CPaintManagerUI::GetInstancePath() + _T("\\skin\\FlashRes\\test.swf")));
                pFlash->DisableLocalSecurity();
                pFlash->put_AllowScriptAccess(L"always");
                BSTR response;
                pFlash->CallFunction(L"<invoke name=\"setButtonText\" returntype=\"xml\"><arguments><string>Click me!</string></arguments></invoke>", &response);
                pFlash->Release();
            }
        }
    }

    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

        m_pm.Init(m_hWnd);
        CDialogBuilder builder;
        CControlUI* pRoot = builder.Create(_T("ui.xml"), (UINT)0, NULL, &m_pm);
        ASSERT(pRoot && "Failed to parse XML");
        m_pm.AttachDialog(pRoot);
        m_pm.AddNotifier(this);

        Init();
        return 0;
    }

    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        ::PostQuitMessage(0L);

        bHandled = FALSE;
        return 0;
    }

    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        if (::IsIconic(*this)) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
    }

    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        return 0;
    }

    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        RECT rcCaption = m_pm.GetCaptionRect();
        if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
            CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
            if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0)
                return HTCAPTION;
        }

        return HTCLIENT;
    }

    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        SIZE szRoundCorner = m_pm.GetRoundCorner();
        if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
            CDuiRect rcWnd;
            ::GetWindowRect(*this, &rcWnd);
            rcWnd.Offset(-rcWnd.left, -rcWnd.top);
            rcWnd.right++; rcWnd.bottom++;
            HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
            ::SetWindowRgn(*this, hRgn, TRUE);
            ::DeleteObject(hRgn);
        }

        bHandled = FALSE;
        return 0;
    }

    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        LRESULT lRes = 0;
        BOOL bHandled = TRUE;
        switch (uMsg) {
        case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
        case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
        case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
        case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
        case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
        case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
        case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
        default:
            bHandled = FALSE;
        }
        if (bHandled) return lRes;
        if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
        return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    }
public:
    CPaintManagerUI m_pm;
};

int DuiFlash(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\FlashRes"));

    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr)) return 0;

    CFrameWnd* pFrame = new CFrameWnd();
    if (pFrame == NULL) return 0;
    pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}

#include "ScrCapture.h"

int DuiScrCap(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

    HRESULT Hr = ::CoInitialize(NULL);
    if (FAILED(Hr))
        return 0;

    CScrCaptureWnd* pFrame = CScrCaptureWnd::Instance();
    if (pFrame == NULL)
        return 0;
    pFrame->Create(NULL, _T("ScrCapture"), WS_VISIBLE | WS_POPUP, /*WS_EX_TOOLWINDOW|WS_EX_TOPMOST*/0);
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();

    ::CoUninitialize();
    return 0;
}



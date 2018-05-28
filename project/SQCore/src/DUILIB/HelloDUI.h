
#ifndef HELLODUI_H
#define HELLODUI_H
#include <UIlib.h>
using namespace DuiLib;
#include "HelloDUI_def.h"
#include "ScrCapture.h"

class DuiTest01 : public CWindowWnd, public INotifyUI
{
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        CPaintManagerUI::SetInstance(hInstance);
        DuiTest01 duiFrame;
        duiFrame.Create(nullptr, _T("DuiWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
        duiFrame.ResizeClient(400, 400);
        duiFrame.ShowModal();
        return 0;
    }
protected:
    CPaintManagerUI m_PaintManager;
public:
    virtual LPCTSTR GetWindowClassName() const;
    //处理事件(类似事件处理回调)
    virtual void Notify(TNotifyUI& msg);
    //类似消息处理回调
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class DuiTest02 : public CWindowWnd, public INotifyUI, public CDwm, public CDPI
{
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        CPaintManagerUI::SetInstance(hInstance);
        CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
        HRESULT Hr = ::CoInitialize(NULL);
        if (FAILED(Hr)) return 0;
        CWndShadow::Initialize(hInstance);
        DuiTest02* pFrame = new DuiTest02();
        if (pFrame == NULL) return 0;
        pFrame->Create(NULL, _T("这是一个最简单的测试用exe，修改test1.xml就可以看到效果"), UI_WNDSTYLE_FRAME | WS_CLIPCHILDREN, WS_EX_WINDOWEDGE);
        pFrame->CenterWindow();
        pFrame->ShowWindow(true);
        CPaintManagerUI::MessageLoop();
        ::CoUninitialize();
        return 0;
    }
public:
    CPaintManagerUI m_pm;
    CWndShadow* m_pWndShadow;
public:
    DuiTest02();
    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND hWnd);
    void Init();
    bool OnHChanged(void* param);
    bool OnSChanged(void* param);
    bool OnLChanged(void* param);
    bool OnAlphaChanged(void* param);
    void OnPrepare();
    void Notify(TNotifyUI& msg);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class DuiTest03 : public CWindowWnd, public INotifyUI, public IListCallbackUI
{
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        CPaintManagerUI::SetInstance(hInstance);
        CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
        CPaintManagerUI::SetResourceZip(_T("ListRes.zip"));
        DuiTest03* pFrame = new DuiTest03();
        if (pFrame == NULL) return 0;
        pFrame->Create(NULL, _T("ListDemo"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 320);
        pFrame->CenterWindow();
        ::ShowWindow(*pFrame, SW_SHOW);
        CPaintManagerUI::MessageLoop();
        return 0;
    }
public:
    static std::vector<std::string> domain;        //存放第二列数据
    static std::vector<std::string> desc;          //存放第三列数据
    static DWORD WINAPI Search(LPVOID lpParameter);
public:
    DuiTest03();

    LPCTSTR GetWindowClassName(void) const;
    UINT GetClassStyle(void) const;
    void OnFinalMessage(HWND hWnd);
    void OnPrepare(TNotifyUI& msg);
    void Init(void);
    void OnSearch(void);
    //关键的回调函数，IListCallbackUI 中的一个虚函数，渲染时候会调用,在[1]中设置了回调对象
    LPCTSTR GetItemText(CControlUI* pControl, int iIndex, int iSubItem);
    void Notify(TNotifyUI& msg);
    LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    CPaintManagerUI m_pm;
private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
    CButtonUI* m_pSearch;
};

class DuiTest04 : public CWindowWnd, public INotifyUI
{
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        CPaintManagerUI::SetInstance(hInstance);
        CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
        CPaintManagerUI::SetResourceZip(_T("360SafeRes.zip"));
        HRESULT Hr = ::CoInitialize(NULL);
        if (FAILED(Hr)) return 0;
        DuiTest04* pFrame = new DuiTest04();
        if (pFrame == NULL) return 0;
        pFrame->Create(NULL, _T("360安全卫士"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 800, 572);
        pFrame->CenterWindow();
        ::ShowWindow(*pFrame, SW_SHOW);
        CPaintManagerUI::MessageLoop();
        ::CoUninitialize();
        return 0;
    };
public:
    DuiTest04(void);
    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND hWnd);
    void Init(void);
    void OnPrepare(void);
    void Notify(TNotifyUI& msg);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    CPaintManagerUI m_pm;
private:
    CButtonUI* m_pCloseBtn;
    CButtonUI* m_pMaxBtn;
    CButtonUI* m_pRestoreBtn;
    CButtonUI* m_pMinBtn;
};

class DuiTest05 : public CWindowWnd, public INotifyUI
{
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        CPaintManagerUI::SetInstance(hInstance);
        CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin\\FlashRes"));

        HRESULT Hr = ::CoInitialize(NULL);
        if (FAILED(Hr)) 
            return 0;

        DuiTest05* pFrame = new DuiTest05();
        if (pFrame == NULL) return 0;
        pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, 0);
        pFrame->CenterWindow();
        pFrame->ShowWindow(true);
        CPaintManagerUI::MessageLoop();

        ::CoUninitialize();
        return 0;
    }
public:
    DuiTest05(void);
    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const; 
    void OnFinalMessage(HWND hWnd);
    void Init(void);
    void Notify(TNotifyUI& msg);
    LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    CPaintManagerUI m_pm;
};

class DuiTest06
{
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        return 0;
    }
};





#endif



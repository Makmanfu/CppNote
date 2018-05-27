
#ifndef HELLODUI_H
#define HELLODUI_H
#include <UIlib.h>
using namespace DuiLib;
#include "HelloDUI_def.h"

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


int DuiListForm(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);

int Dui360Safe(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);

int DuiFlash(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);

int DuiScrCap(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);




#endif



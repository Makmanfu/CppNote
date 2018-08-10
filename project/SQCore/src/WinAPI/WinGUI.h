//-----------------------------------------------------------------------------
//
//                      win32 消息机制界面学习
//                                              @2015-01-19 阿甘
//-----------------------------------------------------------------------------

#pragma once
#include <vector>
using namespace std;
#include <Windows.h>

//窗口局部居中
SQC_API bool CenterWindow(HWND hwndWindow);
//窗口桌面居中
SQC_API bool CenterWindowDesk(HWND hwndWindow);

//模态窗口(模态窗口用 DialogBox)
class SQCPP_API XqWindowDlg
{
public:
    //测试演示 主程序入口
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
#ifndef IDD_DLLDLG
#define IDD_DLLDLG 99999
#endif
        XqWindowDlg MsgDlg(IDD_DLLDLG);     //模态窗口 消息回调DialogBoxParam
        //DialogBoxParamA(hInstance, MAKEINTRESOURCEA(IDD_DLLDLG), NULL, (DLGPROC)Main_Proc, (LPARAM)&MsgDlg);
        DialogBoxA(hInstance, MAKEINTRESOURCEA(XqWindowDlg::ResDlgID), NULL, XqWindowDlg::Main_Proc);
        return 0;
    };
public:
    static XqWindowDlg* pDlg;           //窗口对象指针
    static int ResDlgID;                //资源ID
    //主窗口回调
    static INT_PTR CALLBACK Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    XqWindowDlg(int tResDlgID = 0);     //IDD_DLLDLG
    virtual ~XqWindowDlg();
private:
    XqWindowDlg(const XqWindowDlg&);
    XqWindowDlg& operator=(const XqWindowDlg&);
private:
    HINSTANCE hInstance;                //本程序实例
    HWND hWndDlg;                       //本窗口句柄保存
public:
    void InitTitIcon(const char* titname);
    void SetIcon(int iconid /*= MAINICON*/);
    HWND GetDlgHWND(void);
    HINSTANCE GetDlgInst(void);
public:
    //其他消息
    virtual void WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //初始化
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //关闭
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗口改变时
    virtual void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗口点击事件
    virtual void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //重绘制
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //消息事件
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗口COPYDATA消息
    virtual void WMCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //水平滚动条
    virtual void WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //拖放文件
    virtual void WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //listbox右键点击
    virtual void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //获得Instance和其他句柄
    void GethInstance(HWND hWnd);
    //ListView例子
    LRESULT ListViewAndProc(HWND hWnd, UINT message, WPARAM wParam, HINSTANCE hInst);
};

//非模态窗口(非模态窗口用 CreateDialog)
class SQCPP_API XqWindow
{
public:
    //测试演示 主程序入口
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        XqWindow WND;
        WND.InitData(hInstance);
        WND.Create();
        return WND.ShowDLG();
    };
public:
    XqWindow(void);
    virtual ~XqWindow(void);
public:
    //创建窗口 OK返回0
    bool Create(wchar_t* szTitleName = L"WINGUI", int w = 600, int h = 600, WNDPROC lWndProc = NULL);
    //窗口显示
    virtual int ShowDLG(void);
public:
    //属性处理
    HWND GetHandle(void);
    void SetHandle(HWND hwnd);
    RECT* GetRECT(void);
    HINSTANCE GethInst(void);
    //初始化
    virtual void InitData(HINSTANCE hInstance);
private:
    HWND hWnd;
    RECT rect;
    HINSTANCE hInst;
private:
    //消息回调函数
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //屏蔽 你懂的
    XqWindow(const XqWindow &);
    XqWindow &operator=(const XqWindow &);
};

//非模态窗口消息处理
class SQCPP_API XqWindowEx: public XqWindow
{
public:
    //测试演示 主程序入口
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        XqWindowEx WND(hInstance);
        WND.Create();
        return WND.ShowDLG();
    };
private:
    //已注册过的类集合
    static std::vector<void*> registeredClassArray;
    //原始窗口过程
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    XqWindowEx();
    XqWindowEx(HINSTANCE hInst);
    virtual ~XqWindowEx();
public:
    //创建窗口
    virtual bool Create(wchar_t* szTitleName = L"WINGUI", int w = 600, int h = 600);
    //创建窗口
    bool CreateEX(wchar_t* szTitleName = L"WINGUI", int w = 600, int h = 600);
    //阻塞式消息循环
    virtual int ShowDLG(void);
public:
    //消息循环处理事件
    virtual int MsgLoopFun(void);
    //消息循环处理结尾事件
    virtual int MsgLoopLast(void);
    //自定义消息处理
    virtual bool WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMMOUSEWHEEL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMMOUSEMOVE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMRBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMLBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMRBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMTIMER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMKEYDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};








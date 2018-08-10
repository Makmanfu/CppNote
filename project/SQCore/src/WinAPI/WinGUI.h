//-----------------------------------------------------------------------------
//
//                      win32 ��Ϣ���ƽ���ѧϰ
//                                              @2015-01-19 ����
//-----------------------------------------------------------------------------

#pragma once
#include <vector>
using namespace std;
#include <Windows.h>

//���ھֲ�����
SQC_API bool CenterWindow(HWND hwndWindow);
//�����������
SQC_API bool CenterWindowDesk(HWND hwndWindow);

//ģ̬����(ģ̬������ DialogBox)
class SQCPP_API XqWindowDlg
{
public:
    //������ʾ ���������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
#ifndef IDD_DLLDLG
#define IDD_DLLDLG 99999
#endif
        XqWindowDlg MsgDlg(IDD_DLLDLG);     //ģ̬���� ��Ϣ�ص�DialogBoxParam
        //DialogBoxParamA(hInstance, MAKEINTRESOURCEA(IDD_DLLDLG), NULL, (DLGPROC)Main_Proc, (LPARAM)&MsgDlg);
        DialogBoxA(hInstance, MAKEINTRESOURCEA(XqWindowDlg::ResDlgID), NULL, XqWindowDlg::Main_Proc);
        return 0;
    };
public:
    static XqWindowDlg* pDlg;           //���ڶ���ָ��
    static int ResDlgID;                //��ԴID
    //�����ڻص�
    static INT_PTR CALLBACK Main_Proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    XqWindowDlg(int tResDlgID = 0);     //IDD_DLLDLG
    virtual ~XqWindowDlg();
private:
    XqWindowDlg(const XqWindowDlg&);
    XqWindowDlg& operator=(const XqWindowDlg&);
private:
    HINSTANCE hInstance;                //������ʵ��
    HWND hWndDlg;                       //�����ھ������
public:
    void InitTitIcon(const char* titname);
    void SetIcon(int iconid /*= MAINICON*/);
    HWND GetDlgHWND(void);
    HINSTANCE GetDlgInst(void);
public:
    //������Ϣ
    virtual void WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��ʼ��
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ر�
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //���ڸı�ʱ
    virtual void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //���ڵ���¼�
    virtual void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ػ���
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��Ϣ�¼�
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //����COPYDATA��Ϣ
    virtual void WMCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //ˮƽ������
    virtual void WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�Ϸ��ļ�
    virtual void WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //listbox�Ҽ����
    virtual void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //���Instance���������
    void GethInstance(HWND hWnd);
    //ListView����
    LRESULT ListViewAndProc(HWND hWnd, UINT message, WPARAM wParam, HINSTANCE hInst);
};

//��ģ̬����(��ģ̬������ CreateDialog)
class SQCPP_API XqWindow
{
public:
    //������ʾ ���������
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
    //�������� OK����0
    bool Create(wchar_t* szTitleName = L"WINGUI", int w = 600, int h = 600, WNDPROC lWndProc = NULL);
    //������ʾ
    virtual int ShowDLG(void);
public:
    //���Դ���
    HWND GetHandle(void);
    void SetHandle(HWND hwnd);
    RECT* GetRECT(void);
    HINSTANCE GethInst(void);
    //��ʼ��
    virtual void InitData(HINSTANCE hInstance);
private:
    HWND hWnd;
    RECT rect;
    HINSTANCE hInst;
private:
    //��Ϣ�ص�����
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //���� �㶮��
    XqWindow(const XqWindow &);
    XqWindow &operator=(const XqWindow &);
};

//��ģ̬������Ϣ����
class SQCPP_API XqWindowEx: public XqWindow
{
public:
    //������ʾ ���������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        XqWindowEx WND(hInstance);
        WND.Create();
        return WND.ShowDLG();
    };
private:
    //��ע������༯��
    static std::vector<void*> registeredClassArray;
    //ԭʼ���ڹ���
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
public:
    XqWindowEx();
    XqWindowEx(HINSTANCE hInst);
    virtual ~XqWindowEx();
public:
    //��������
    virtual bool Create(wchar_t* szTitleName = L"WINGUI", int w = 600, int h = 600);
    //��������
    bool CreateEX(wchar_t* szTitleName = L"WINGUI", int w = 600, int h = 600);
    //����ʽ��Ϣѭ��
    virtual int ShowDLG(void);
public:
    //��Ϣѭ�������¼�
    virtual int MsgLoopFun(void);
    //��Ϣѭ�������β�¼�
    virtual int MsgLoopLast(void);
    //�Զ�����Ϣ����
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








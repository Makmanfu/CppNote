//-----------------------------------------------------------------------------
//
//          封装proc过程
//
//-----------------------------------------------------------------------------

#ifndef CMP4_H
#define CMP4_H

#include "CTemplate.hpp"
#include "CSingleton.hpp"


//管理消息回调
class CMP4Msg: public noncopyable
{
    DEFINE_SINGLETON(CMP4Msg);
#define MAX_LOADSTRING 100
public:
    HINSTANCE hInst;                            //当前实例
    TCHAR szTitle[MAX_LOADSTRING];              //标题栏文本
    TCHAR szWindowClass[MAX_LOADSTRING];        //主窗口类名
    RECT rect;                                  //绘制区大小
public:
    CMP4Msg();
    ~CMP4Msg();
private:
    void DrawGDITest(HWND hWnd);
    //GPI绘制播放器logo
    void GDIPaint(HWND hWnd);
    //GDI贴图BMP
    void GDIBMPPaint(HWND hWnd);
public:
    void InitData(HINSTANCE hInstance);
    //其他消息
    void WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗口创建时候的消息.
    void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //点击消息
    void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //右键事件
    void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //改变尺寸时
    void WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //重绘制
    void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //命令事件
    void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗口改变时
    void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//单例指针
extern "C" CMP4Msg* GetMP4();




#endif
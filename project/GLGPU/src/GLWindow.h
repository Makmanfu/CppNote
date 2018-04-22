//-----------------------------------------------------------------------------
//
//          窗口绘制大全
//
//-----------------------------------------------------------------------------
#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "GLView.h"
#include "DXView.h"
#define SQC_API
#define SQCPP_API
#include "../../SQCore/src/WinAPI/WinGUI.h"

//GL绘制
class GLWnd : public XqWindowEx
{
public:
    GLView mGlView;
public:
    //主函数
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    //消息
    virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//TrackBall绘制正方体例子
class TrackBallWnd : public XqWindowEx
{
public:
    CubeGL mGlRender;
    //TriangleGL mGlRender;                   //GL绘制
    CameraEye mCamera;                        //相机
    TrackBallInteractor mInteractor;          //计算TrackBall点模块
public:
    //主函数
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //消息
    virtual void WMMOUSEWHEEL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMLBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMMOUSEMOVE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //消息循环处理
    virtual int MsgLoopFun(void);
};




#endif

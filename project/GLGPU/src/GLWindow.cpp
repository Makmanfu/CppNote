

#include "stdafx.h"
#include "GLWindow.h"
#include "../../SQCore/src/WinAPI/WinGUI.cpp"


int GLWnd::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    GLWnd WndGl;
    WndGl.InitData(hInstance);
    WndGl.Create(L"OpenGL");
    return WndGl.ShowDLG();
}

void GLWnd::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    mGlView.SetDC(GetDC(hWnd));
    mGlView.GLInit();
    RECT rect;
    GetWindowRect(hWnd, &rect);
    mGlView.ChangeSize(rect.right - rect.left, rect.bottom - rect.top);
    XqWindowEx::WMCREATE(hWnd, uMsg, wParam, lParam);
}

void GLWnd::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = GetDC(hWnd);
    hDrawDC = BeginPaint(hWnd, &ps);
    mGlView.RenderScene();
    SwapBuffers(hDrawDC);
    EndPaint(hWnd, &ps);
}

void GLWnd::WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT* rect = GetRECT();
    mGlView.ChangeSize(rect->right - rect->left, rect->bottom - rect->top);
}



int TrackBallWnd::WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    TrackBallWnd GlWnd;
    GlWnd.InitData(hInstance);
    GlWnd.Create(L"TrackBallWnd");
    return GlWnd.ShowDLG();
}

void TrackBallWnd::WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    XqWindowEx::WMCREATE(hWnd, uMsg, wParam, lParam);
    //绘制模块
    mGlRender.SetDC(GetDC(hWnd));
    mGlRender.setCamera(&mCamera);
    mGlRender.GLInit();
    //操作控制模块
    mInteractor.setCamera(&mCamera);
    //因为只有Create完后才能GetRECT
    RECT* rect = GetRECT();
    GetWindowRect(hWnd, rect);
    mInteractor.setScreenSize(rect->right - rect->left, rect->bottom - rect->top);
    mGlRender.resize(rect->right - rect->left, rect->bottom - rect->top);
}

void TrackBallWnd::WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT* rect = GetRECT();
    mInteractor.setScreenSize(rect->right - rect->left, rect->bottom - rect->top);
    mGlRender.resize(rect->right - rect->left, rect->bottom - rect->top);
}

void TrackBallWnd::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = GetDC(hWnd);
    hDrawDC = BeginPaint(hWnd, &ps);
    //opengl绘制
    mGlRender.render();
    SwapBuffers(hDrawDC);
    EndPaint(hWnd, &ps);
}

void TrackBallWnd::WMMOUSEWHEEL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    //正数往下负数往上
    mInteractor.setScrollDirection(zDelta > 0 ? true : false);
    PostMessage(hWnd, WM_PAINT, 0, 0);
}

void TrackBallWnd::WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    mInteractor.setLeftClicked(true);
    int xpos = LOWORD(lParam);
    int ypos = HIWORD(lParam);
    mInteractor.setClickPoint(xpos, ypos);
}

void TrackBallWnd::WMLBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    mInteractor.setLeftClicked(false);
}

void TrackBallWnd::WMMOUSEMOVE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int xpos = LOWORD(lParam);
    int ypos = HIWORD(lParam);
    mInteractor.setClickPoint(xpos, ypos);
    //触发重绘
    PostMessage(hWnd, WM_PAINT, 0, 0);
}

int TrackBallWnd::MsgLoopFun(void)
{
    mInteractor.update();
    return 1;
}



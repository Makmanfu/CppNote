//-----------------------------------------------------------------------------
//
//          ���ڻ��ƴ�ȫ
//
//-----------------------------------------------------------------------------
#ifndef GLWINDOW_H
#define GLWINDOW_H

#include "GLView.h"
#include "DXView.h"
#define SQC_API
#define SQCPP_API
#include "../../SQCore/src/WinAPI/WinGUI.h"

//GL����
class GLWnd : public XqWindowEx
{
public:
    GLView mGlView;
public:
    //������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    //��Ϣ
    virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//TrackBall��������������
class TrackBallWnd : public XqWindowEx
{
public:
    CubeGL mGlRender;
    //TriangleGL mGlRender;                   //GL����
    CameraEye mCamera;                        //���
    TrackBallInteractor mInteractor;          //����TrackBall��ģ��
public:
    //������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��Ϣ
    virtual void WMMOUSEWHEEL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMLBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMMOUSEMOVE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��Ϣѭ������
    virtual int MsgLoopFun(void);
};




#endif

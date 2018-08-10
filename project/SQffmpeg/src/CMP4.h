//-----------------------------------------------------------------------------
//
//          封装proc过程
//
//-----------------------------------------------------------------------------

#ifndef CMP4_H
#define CMP4_H

#include "WinGUI.h"

class CMP4;

struct  tagMediaParam
{
    AVFormatContext* pFormatCtx;
    AVCodecContext*  pCodecCtx;
    AVCodec*         pCodec;
    int              videoindex;
    SwsContext*      img_convert_ctx;
    RECT             screenRect;
    HANDLE           thread;
    CMP4*            gui;
};

//非模态窗口消息处理
class CMP4 : public XqWindowEx
{
public:
    //测试演示 主程序入口
    static int WIN_MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        CMP4 WND(hInstance);
        WND.CreateEX(L"ffmpeg_player");
        return WND.ShowDLG();     //消息循环
    };
    static unsigned int __stdcall VideoRenderThread(LPVOID p);
public:
    CMP4(HINSTANCE hInst);
    ~CMP4() {};
public:
    BOOL bOffThreadFlag = FALSE;        //是否关闭当前的播放视频的线程
    bool playstate;
    tagMediaParam mParam;
    HWND m_hWnd;
    RECT rect;
public:
    virtual void WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMRBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void play(void);
    void freeall(void);
};









#endif

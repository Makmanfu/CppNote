//-----------------------------------------------------------------------------
//
//          ��װproc����
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

//��ģ̬������Ϣ����
class CMP4 : public XqWindowEx
{
public:
    //������ʾ ���������
    static int WIN_MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        CMP4 WND(hInstance);
        WND.CreateEX(L"ffmpeg_player");
        return WND.ShowDLG();     //��Ϣѭ��
    };
    static unsigned int __stdcall VideoRenderThread(LPVOID p);
public:
    CMP4(HINSTANCE hInst);
    ~CMP4() {};
public:
    BOOL bOffThreadFlag = FALSE;        //�Ƿ�رյ�ǰ�Ĳ�����Ƶ���߳�
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

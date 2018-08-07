//====================================================
//
//         WinMP4 对播放视频的显示测试
//
//====================================================
#ifndef WINMP4_HPP
#define WINMP4_HPP

#include "stdafx.h"
#include "CMP4.h"
#include <atlstr.h>

//消息回调管理
CMP4Msg* mp4 = GetMP4();

BOOL bOffThreadFlag = FALSE;        //是否关闭当前的播放视频的线程

HWND m_hWnd;

struct  tagMediaParam
{
    AVFormatContext* pFormatCtx;
    AVCodecContext*  pCodecCtx;
    AVCodec*         pCodec;
    int              videoindex;
    SwsContext*      img_convert_ctx;
    RECT             screenRect;
    HANDLE           thread;
};

tagMediaParam mParam;

CString GetFilePath(HWND* phWnd)
{
    OPENFILENAME ofn;      // 公共对话框结构。
    TCHAR szFile[MAX_PATH]; // 保存获取文件名称的缓冲区。
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = *phWnd;
    ofn.lpstrFile = szFile;
    //
    //
    ofn.lpstrFile[0] = _T('\0');
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = _T("视频(*.*)\0*.*\0\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    ofn.lpstrFileTitle = (LPTSTR)_T("打开");
    // 显示打开选择文件对话框。
    BOOL bRet = GetOpenFileName(&ofn);
    if (bRet)
        return szFile;
    return _T("");
}

unsigned int __stdcall VideoRenderThread(LPVOID p)
{
    tagMediaParam* pmParam = (tagMediaParam*)p;
    AVPacket* packet = NULL;
    AVFrame* pFrame = NULL;
    AVFrame* pFrameRGB = NULL;
    PVOID pbit = NULL;
    BITMAP bmp = { 0 };
    HBITMAP hBitmap = NULL;
    BITMAPINFO bmpinfo = { 0 };
    HDC hdcSrc = NULL;
    HDC hdcDes = NULL;
    unsigned char* out_bufRgb;
    RECT rc;
    packet = av_packet_alloc();
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    GetClientRect(m_hWnd, &pmParam->screenRect);
    bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpinfo.bmiHeader.biWidth = pmParam->screenRect.right - pmParam->screenRect.left;
    bmpinfo.bmiHeader.biHeight = -pmParam->screenRect.bottom + pmParam->screenRect.top;
    bmpinfo.bmiHeader.biPlanes = 1;
    bmpinfo.bmiHeader.biBitCount = 32;
    bmpinfo.bmiHeader.biCompression = BI_RGB;
    pmParam->img_convert_ctx = sws_getContext(pmParam->pCodecCtx->width, pmParam->pCodecCtx->height
                               , pmParam->pCodecCtx->pix_fmt, pmParam->screenRect.right - pmParam->screenRect.left
                               , pmParam->screenRect.bottom - pmParam->screenRect.top
                               , AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
    out_bufRgb = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32
                                           , pmParam->screenRect.right - pmParam->screenRect.left
                                           , pmParam->screenRect.bottom - pmParam->screenRect.top
                                           , 1));
    hdcDes = GetDC(m_hWnd);
    hdcSrc = CreateCompatibleDC(hdcDes);
    av_image_fill_arrays(pFrameRGB->data
                         , pFrameRGB->linesize, out_bufRgb
                         , AV_PIX_FMT_RGB32, pmParam->screenRect.right - pmParam->screenRect.left
                         , pmParam->screenRect.bottom - pmParam->screenRect.top, 1);
    while (true)
    {
        if (bOffThreadFlag)
        {
            ReleaseDC(m_hWnd, hdcSrc);
            ReleaseDC(m_hWnd, hdcDes);
            bOffThreadFlag = FALSE;
            avformat_close_input(&mParam.pFormatCtx);
            av_packet_unref(packet);
            return 0;
        }
        if (packet == NULL)
        {
            packet = av_packet_alloc();
            av_usleep(20 * 1000);
            continue;
        }
        if (av_read_frame(pmParam->pFormatCtx, packet) >= 0)
        {
            if (packet->stream_index == pmParam->videoindex)
            {
                GetClientRect(m_hWnd, &rc);
                if (rc.left != pmParam->screenRect.left
                        || rc.right != pmParam->screenRect.right
                        || rc.top != pmParam->screenRect.top
                        || rc.bottom != pmParam->screenRect.bottom)
                {
                    pmParam->screenRect = rc;
                    pmParam->img_convert_ctx = sws_getContext(pmParam->pCodecCtx->width, pmParam->pCodecCtx->height
                                               , pmParam->pCodecCtx->pix_fmt, pmParam->screenRect.right - pmParam->screenRect.left
                                               , pmParam->screenRect.bottom - pmParam->screenRect.top
                                               , AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);
                    av_free(out_bufRgb);
                    out_bufRgb = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32
                                                           , pmParam->screenRect.right - pmParam->screenRect.left
                                                           , pmParam->screenRect.bottom - pmParam->screenRect.top
                                                           , 1));
                    av_frame_unref(pFrameRGB);
                    av_image_fill_arrays(pFrameRGB->data
                                         , pFrameRGB->linesize, out_bufRgb
                                         , AV_PIX_FMT_RGB32, pmParam->screenRect.right - pmParam->screenRect.left
                                         , pmParam->screenRect.bottom - pmParam->screenRect.top, 1);
                    bmpinfo.bmiHeader.biWidth = pmParam->screenRect.right - pmParam->screenRect.left;
                    bmpinfo.bmiHeader.biHeight = -pmParam->screenRect.bottom + pmParam->screenRect.top;
                }
                if (avcodec_send_packet(pmParam->pCodecCtx, packet) == 0)
                {
                    if (avcodec_receive_frame(pmParam->pCodecCtx, pFrame) == 0)
                    {
                        sws_scale(pmParam->img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize
                                  , 0, pmParam->pCodecCtx->height,
                                  pFrameRGB->data, pFrameRGB->linesize);
                        hBitmap = CreateDIBSection(hdcSrc, &bmpinfo, DIB_RGB_COLORS, &pbit, NULL, 0);
                        GetObject(hBitmap, sizeof(BITMAP), &bmp);
                        memcpy(pbit, out_bufRgb
                               , (pmParam->screenRect.right - pmParam->screenRect.left) * (pmParam->screenRect.bottom - pmParam->screenRect.top) * 4);
                        SelectObject(hdcSrc, hBitmap);
                        BitBlt(hdcDes, 0, 0, pmParam->screenRect.right - pmParam->screenRect.left, pmParam->screenRect.bottom - pmParam->screenRect.top, hdcSrc, 0, 0, SRCCOPY);
                        DeleteObject(hBitmap);
                        av_usleep(40 * 1000);
                    }
                    else
                        continue;
                }
                else
                    break;
            }
        }
        av_packet_unref(packet);
    }
    ReleaseDC(m_hWnd, hdcSrc);
    ReleaseDC(m_hWnd, hdcDes);
    bOffThreadFlag = FALSE;
    avformat_close_input(&mParam.pFormatCtx);
    return 0;
}

void play(void)
{
    //播放之前清理相关参数‘
    bOffThreadFlag = FALSE;
    if (mParam.thread != NULL)
        bOffThreadFlag = TRUE;
    WaitForSingleObject(mParam.thread, INFINITE);
    avcodec_close(mParam.pCodecCtx);
    avcodec_free_context(&mParam.pCodecCtx);
    avformat_free_context(mParam.pFormatCtx);
    sws_freeContext(mParam.img_convert_ctx);
    CloseHandle(mParam.thread);
    mParam.thread = NULL;
    av_register_all();
    mParam.pFormatCtx = avformat_alloc_context();
    CStringA strPath(GetFilePath(&m_hWnd));
    if (avformat_open_input(&mParam.pFormatCtx, strPath, NULL, NULL) < 0)
        return;
    if (avformat_find_stream_info(mParam.pFormatCtx, NULL) < 0)
        return;
    for (int i = 0; i < (int)mParam.pFormatCtx->nb_streams; i++)
    {
        if (mParam.pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            mParam.videoindex = i;
    }
    mParam.pCodecCtx = avcodec_alloc_context3(NULL);
    if (avcodec_parameters_to_context(mParam.pCodecCtx
                                      , mParam.pFormatCtx->streams[mParam.videoindex]->codecpar) < 0)
        av_log(NULL, AV_LOG_ERROR, "video avcodec_parameters_to_context faild");
    mParam.pCodec = avcodec_find_decoder(mParam.pCodecCtx->codec_id);
    if (mParam.pCodec == NULL)
        return;
    if (avcodec_open2(mParam.pCodecCtx, mParam.pCodec, NULL) < 0)
        return;
    mParam.thread = (HANDLE)_beginthreadex(NULL, 0, VideoRenderThread, &mParam, 0, NULL);
}

//主窗口消息回调
LRESULT CALLBACK WndProcDemo(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        mp4->WMCREATE(hWnd, uMsg, wParam, lParam);
        break;
    case WM_LBUTTONDOWN:
        mp4->WMLBUTTONDOWN(hWnd, uMsg, wParam, lParam);
        break;
    case WM_SIZE:
        mp4->WMSIZE(hWnd, uMsg, wParam, lParam);
        break;
    case WM_PAINT:
        mp4->WMPAINT(hWnd, uMsg, wParam, lParam);
        break;
    case WM_DESTROY:
        //播放之前清理相关参数
        bOffThreadFlag = TRUE;
        WaitForSingleObject(mParam.thread, INFINITE);
        avcodec_close(mParam.pCodecCtx);
        avcodec_free_context(&mParam.pCodecCtx);
        avformat_free_context(mParam.pFormatCtx);
        sws_freeContext(mParam.img_convert_ctx);
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    case WM_CONTEXTMENU://右键点击
        mp4->WMCONTEXTMENU(hWnd, uMsg, wParam, lParam);
        break;
    case WM_COMMAND:
        mp4->WMCOMMAND(hWnd, uMsg, wParam, lParam);
        break;
    case WM_SIZING:
        mp4->WMSIZING(hWnd, uMsg, wParam, lParam);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    mp4->WMOTHER(hWnd, uMsg, wParam, lParam);
    return 0;
}


class WinMP4
{
public:
    //测试 主程序入口
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        // 将实例句柄存储在全局变量中
        mp4->InitData(hInstance);
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProcDemo;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MAINICON));        //图标
        wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = NULL;//MAKEINTRESOURCE(IDC_MYGUI10);            //菜单
        wcex.lpszClassName = mp4->szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(MAINICON));
        //注册窗口
        RegisterClassEx(&wcex);
        //创建窗口
        /*HWND hWnd*/m_hWnd = CreateWindow(mp4->szWindowClass, mp4->szTitle,
                                 WS_OVERLAPPEDWINDOW | WS_EX_TOOLWINDOW,
                                 0, 0, 800, 600, NULL, NULL, hInstance, NULL);
        if (!m_hWnd)
            return FALSE;
        ShowWindow(m_hWnd, nCmdShow);
        UpdateWindow(m_hWnd);

        //主消息循环:
        MSG msg;
        HACCEL hAccelTable = LoadAcceleratorsA(hInstance, "WINMP4");
        while (GetMessage(&msg, NULL, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        return (int)msg.wParam;
    };
};


#endif





#include "stdafx.h"
//#include "gdiplay.h"
#include <atlstr.h>
extern "C" {
#include "libavcodec\avcodec.h"
#include "libavformat\avformat.h"
#include "libavutil\channel_layout.h"
#include "libavutil\common.h"
#include "libavutil\imgutils.h"
#include "libswscale\swscale.h"
#include "libavutil\imgutils.h"
#include "libavutil\opt.h"
#include "libavutil\mathematics.h"
#include "libavutil\samplefmt.h"
#include "libavutil\timestamp.h"
#include "libavutil\log.h"
#include "libavutil\time.h"
#include "libswresample\swresample.h"
}

#define MAX_LOADSTRING 100

BOOL bOffThreadFlag = FALSE;        //是否关闭当前的播放视频的线程

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
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

VOID play()
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

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,_In_ LPWSTR lpCmdLine,_In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    // TODO: 在此放置代码。
    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GDIPLAY, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    // 执行应用程序初始化:
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GDIPLAY));
    play();
    MSG msg;
    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int)msg.wParam;
}
//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GDIPLAY));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GDIPLAY);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    if (!hWnd)
        return FALSE;
    m_hWnd = hWnd;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // 分析菜单选择:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_OPEN:
            play();
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 在此处添加使用 hdc 的任何绘图代码...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {
        //播放之前清理相关参数
        bOffThreadFlag = TRUE;
        WaitForSingleObject(mParam.thread, INFINITE);
        avcodec_close(mParam.pCodecCtx);
        avcodec_free_context(&mParam.pCodecCtx);
        avformat_free_context(mParam.pFormatCtx);
        sws_freeContext(mParam.img_convert_ctx);
        PostQuitMessage(0);
    }
    break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
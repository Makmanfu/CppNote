
#include "stdafx.h"
#include "CMP4.h"
#include <atlstr.h>

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

unsigned int __stdcall CMP4::VideoRenderThread(LPVOID p)
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

    GetClientRect(pmParam->gui->m_hWnd, &pmParam->screenRect);
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
    hdcDes = GetDC(pmParam->gui->m_hWnd);
    hdcSrc = CreateCompatibleDC(hdcDes);
    av_image_fill_arrays(pFrameRGB->data
        , pFrameRGB->linesize, out_bufRgb
        , AV_PIX_FMT_RGB32, pmParam->screenRect.right - pmParam->screenRect.left
        , pmParam->screenRect.bottom - pmParam->screenRect.top, 1);
    while (true)
    {
        if (pmParam->gui->bOffThreadFlag)
        {
            ReleaseDC(pmParam->gui->m_hWnd, hdcSrc);
            ReleaseDC(pmParam->gui->m_hWnd, hdcDes);
            pmParam->gui->bOffThreadFlag = FALSE;
            avformat_close_input(&pmParam->gui->mParam.pFormatCtx);
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
                GetClientRect(pmParam->gui->m_hWnd, &rc);
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
    ReleaseDC(pmParam->gui->m_hWnd, hdcSrc);
    ReleaseDC(pmParam->gui->m_hWnd, hdcDes);
    pmParam->gui->bOffThreadFlag = FALSE;
    avformat_close_input(&pmParam->gui->mParam.pFormatCtx);
    return 0;
}

CMP4::CMP4(HINSTANCE hInst) :XqWindowEx(hInst)
{
    m_hWnd = this->GetHandle();
    mParam.gui = this;

    playstate = false;
}

void CMP4::WMDESTROY(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //播放之前清理相关参数
    freeall();
    PostQuitMessage(0);
}

void CMP4::WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = BeginPaint(hWnd, &ps);
    GetClientRect(hWnd, &rect); //获得绘制区域
    int w = rect.right - rect.left, h = rect.bottom - rect.top;
    // 创建画刷
    HBRUSH hb = CreateSolidBrush(RGB(0, 0, 0));
    // 画刷选择到当前DC中
    HBRUSH orgBrs = (HBRUSH)SelectObject(ps.hdc, hb);
    // 填充矩形图形区为黑色背景
    Rectangle(ps.hdc, 0, 0, w, h);
    // 绘制中心播放器三角
    POINT aptTriangle[4] = { { (w /= 2) - 10, (h /= 2) - 15 }, { w - 10, h + 15 }, { w + 10, h }, { w - 10, h - 15 } };
    int num[1] = { 4 };
    HBRUSH hbTriangle = CreateSolidBrush(RGB(250, 250, 250));
    SelectObject(hDrawDC, hbTriangle);
    PolyPolygon(hDrawDC, aptTriangle, num, 1);
    // 选回原先的画刷
    SelectObject(ps.hdc, orgBrs);
    //清理对象
    DeleteObject(hb);
    EndPaint(hWnd, &ps);


    //PAINTSTRUCT ps;
    ////创建DC和内存DC
    //HDC hDrawDC = BeginPaint(hWnd, &ps), memDc = CreateCompatibleDC(NULL);
    ////获得绘制区域
    //GetClientRect(hWnd, &rect);
    ////加载图片
    //HBITMAP hBmp = LoadBitmap(GethInst(), MAKEINTRESOURCE(IDB_BMPLOGO));
    ////HBITMAP hBmp = LoadBitmapA(NULL, "bk.bmp");
    ////图片放入内存DC
    //SelectObject(memDc, hBmp);
    ////设置在指定设备内容中的伸展模式。解决失真问题。
    //SetStretchBltMode(hDrawDC, COLORONCOLOR);
    ////获得图片尺寸
    //BITMAP bmp;
    //GetObject(hBmp, sizeof(BITMAP), &bmp);
    ////内存DC拷贝到Dc
    //StretchBlt(hDrawDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
    //    memDc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
    //DeleteDC(memDc);
    //DeleteObject(hBmp);
    //EndPaint(hWnd, &ps);
}

void CMP4::WMRBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINT pt;
    pt.x = LOWORD(lParam);
    pt.y = HIWORD(lParam);
    ClientToScreen(hWnd, &pt);
    HMENU hPopMenu = LoadMenu(this->GethInst(), MAKEINTRESOURCEW(IDR_FFMPEGMENU));
    hPopMenu = GetSubMenu(hPopMenu, 0);
    TrackPopupMenu(hPopMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hPopMenu);
}

void CMP4::WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case ID_OpenFile:
        play();
        break;
    case IDC_FFMPEGEXIT:
        PostQuitMessage(0);
    default:
        break;
    }
}

void CMP4::play(void)
{
    //播放之前清理相关参数‘
    bOffThreadFlag = FALSE;
    freeall();

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
    mParam.thread = (HANDLE)_beginthreadex(NULL, 0, &VideoRenderThread, &mParam, 0, NULL);
    playstate = true;
}

void CMP4::freeall(void)
{
    if (playstate)
    {
        avcodec_close(mParam.pCodecCtx);
        avcodec_free_context(&mParam.pCodecCtx);
        avformat_free_context(mParam.pFormatCtx);
        sws_freeContext(mParam.img_convert_ctx);

        if (mParam.thread != NULL)
            bOffThreadFlag = TRUE;
        WaitForSingleObject(mParam.thread, INFINITE);
        CloseHandle(mParam.thread);
        mParam.thread = NULL;

        playstate = false;
    }
}



#ifndef GLPP_HPP
#define GLPP_HPP

#include "stdafx.h"
#define MAX_LOADSTRING 100
// 全局变量:
//++++++++++++++++++++++++++++++++++++++++++
HDC         hDC = NULL;             // OpenGL渲染描述表句柄
HGLRC       hRC = NULL;             // Permanent Rendering Context
HWND        hWnd = NULL;            // 保存我们的窗口句柄
//++++++++++++++++++++++++++++++++++++++++++
HINSTANCE hInst;                                // 当前实例
TCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
bool  fullscreen = true;                        // 是否全屏

bool keyPressed[256];                           // holds true for keys that are pressed

//告诉窗口我们所希望的东东，即窗口使用的像素格式
void SetDCPixelFormat(HDC hDC)
{
    static PIXELFORMATDESCRIPTOR pfd =          // 告诉窗口我们所希望的东东，即窗口使用的像素格式
    {
        sizeof(PIXELFORMATDESCRIPTOR),          // 上述格式描述符的大小
        1,                                      // 版本号
        PFD_DRAW_TO_WINDOW |                    // 格式支持窗口
        PFD_SUPPORT_OPENGL |                    // 格式必须支持OpenGL
        PFD_DOUBLEBUFFER,                       // 必须支持双缓冲
        PFD_TYPE_RGBA,                          // 申请 RGBA 格式
        32,                                     // 选定色彩深度
        0, 0, 0, 0, 0, 0,                       // 忽略的色彩位
        0,                                      // 无Alpha缓存
        0,                                      // 忽略Shift Bit
        0,                                      // 无累加缓存
        0, 0, 0, 0,                             // 忽略聚集位
        16,                                     // 16位 Z-缓存 (深度缓存)
        0,                                      // 无蒙板缓存
        0,                                      // 无辅助缓存
        PFD_MAIN_PLANE,                         // 主绘图层
        0,                                      // Reserved
        0, 0, 0                                 // 忽略层遮罩
    };
    // Choose a pixel format that best matches that described in pfd
    int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    // Set the pixel format for the device context
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}

//此处开始对OpenGL进行所有设置
void InitOpenGL()
{
    if (hDC != NULL)
    {
        SetDCPixelFormat(hDC);
        hRC = wglCreateContext(hDC);
    }
    if (hRC != NULL)
        wglMakeCurrent(hDC, hRC);
}

void UnInitOpenGL()
{
    if (fullscreen)                             // Are We In Fullscreen Mode?
    {
        ChangeDisplaySettings(NULL, 0);         // If So Switch Back To The Desktop
        ShowCursor(TRUE);                       // Show Mouse Pointer
    }
    if (hRC)                                    // Do We Have A Rendering Context?
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);
        hRC = NULL;                             // Set RC To NULL
    }
    if (hDC && !ReleaseDC(hWnd, hDC))           // Are We Able To Release The DC
        hDC = NULL;                             // Set DC To NULL
    if (hWnd && !DestroyWindow(hWnd))           // Are We Able To Destroy The Window?
        hWnd = NULL;                            // Set hWnd To NULL
    if (!UnregisterClassA("OpenGL", hInst))      // Are We Able To Unregister Class
        hInst = NULL;                           // Set hInstance To NULL
    //if (NULL != hDC)
    //    wglMakeCurrent(NULL,NULL);
    //if (NULL != hRC)
    //    wglDeleteContext(hRC);
    //if (NULL != hDC)
    //    ::ReleaseDC(hWnd,hDC);
}

bool BitmapWriteFile(char* filename, int width, int height, unsigned char* bitmapData)
{
    //WriteBitmapFile
    //根据bitmapData的（RGB）数据，保存bitmap
    //filename是要保存到物理硬盘的文件名（包括路径）
    //填充BITMAPFILEHEADER
    BITMAPFILEHEADER bitmapFileHeader;
    memset(&bitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
    bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
    bitmapFileHeader.bfType = 0x4d42;   //BM
    bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    //填充BITMAPINFOHEADER
    BITMAPINFOHEADER bitmapInfoHeader;
    memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
    bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfoHeader.biWidth = width;
    bitmapInfoHeader.biHeight = height;
    bitmapInfoHeader.biPlanes = 1;
    bitmapInfoHeader.biBitCount = 24;
    bitmapInfoHeader.biCompression = BI_RGB;
    bitmapInfoHeader.biSizeImage = width * abs(height) * 3;
    //////////////////////////////////////////////////////////////////////////
    FILE* filePtr;          //连接要保存的bitmap文件用
    unsigned char tempRGB;  //临时色素
    int imageIdx;
    //交换R、B的像素位置,bitmap的文件放置的是BGR,内存的是RGB
    for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3)
    {
        tempRGB = bitmapData[imageIdx];
        bitmapData[imageIdx] = bitmapData[imageIdx + 2];
        bitmapData[imageIdx + 2] = tempRGB;
    }
    filePtr = fopen(filename, "wb");
    if (NULL == filePtr)
        return false;
    fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, filePtr);
    fclose(filePtr);
    return true;
}

//保存窗口客户端的截图 窗口大小
void SaveScreenShot()
{
    int clnWidth(800), clnHeight(600);       //client width and height
    //RECT rect;
    //rect.left=rect.top=rect.right=rect.bottom = 0;
    //GetWindowRect(hWnd,&rect);
    //clnWidth = rect.right;
    //clnHeight = rect.bottom;
    static void* screenData;
    RECT rc;
    int len = clnWidth * clnHeight * 3;
    screenData = malloc(len);
    memset(screenData, 0, len);
    glReadPixels(0, 0, clnWidth, clnHeight, GL_RGB, GL_UNSIGNED_BYTE, screenData);
    //生成文件名字符串，以时间命名
    time_t tm = 0;
    tm = time(NULL);
    char lpstrFilename[256] = {0};
    sprintf_s(lpstrFilename, sizeof(lpstrFilename), "%d.bmp", tm);
    BitmapWriteFile(lpstrFilename, clnWidth, clnHeight, (unsigned char*) screenData);
    //Mat f_img = Mat(image_height, image_width, CV_8UC3, screenData);
    //imshow("f_img",f_img);
    //waitKey(0);
    free(screenData);
}










#endif
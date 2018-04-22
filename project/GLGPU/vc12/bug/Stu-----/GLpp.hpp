
#ifndef GLPP_HPP
#define GLPP_HPP

#include "stdafx.h"
#define MAX_LOADSTRING 100
// ȫ�ֱ���:
//++++++++++++++++++++++++++++++++++++++++++
HDC         hDC = NULL;             // OpenGL��Ⱦ��������
HGLRC       hRC = NULL;             // Permanent Rendering Context
HWND        hWnd = NULL;            // �������ǵĴ��ھ��
//++++++++++++++++++++++++++++++++++++++++++
HINSTANCE hInst;                                // ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];            // ����������
bool  fullscreen = true;                        // �Ƿ�ȫ��

bool keyPressed[256];                           // holds true for keys that are pressed

//���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
void SetDCPixelFormat(HDC hDC)
{
    static PIXELFORMATDESCRIPTOR pfd =          // ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
    {
        sizeof(PIXELFORMATDESCRIPTOR),          // ������ʽ�������Ĵ�С
        1,                                      // �汾��
        PFD_DRAW_TO_WINDOW |                    // ��ʽ֧�ִ���
        PFD_SUPPORT_OPENGL |                    // ��ʽ����֧��OpenGL
        PFD_DOUBLEBUFFER,                       // ����֧��˫����
        PFD_TYPE_RGBA,                          // ���� RGBA ��ʽ
        32,                                     // ѡ��ɫ�����
        0, 0, 0, 0, 0, 0,                       // ���Ե�ɫ��λ
        0,                                      // ��Alpha����
        0,                                      // ����Shift Bit
        0,                                      // ���ۼӻ���
        0, 0, 0, 0,                             // ���Ծۼ�λ
        16,                                     // 16λ Z-���� (��Ȼ���)
        0,                                      // ���ɰ建��
        0,                                      // �޸�������
        PFD_MAIN_PLANE,                         // ����ͼ��
        0,                                      // Reserved
        0, 0, 0                                 // ���Բ�����
    };
    // Choose a pixel format that best matches that described in pfd
    int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    // Set the pixel format for the device context
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}

//�˴���ʼ��OpenGL������������
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
    //����bitmapData�ģ�RGB�����ݣ�����bitmap
    //filename��Ҫ���浽����Ӳ�̵��ļ���������·����
    //���BITMAPFILEHEADER
    BITMAPFILEHEADER bitmapFileHeader;
    memset(&bitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
    bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
    bitmapFileHeader.bfType = 0x4d42;   //BM
    bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    //���BITMAPINFOHEADER
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
    FILE* filePtr;          //����Ҫ�����bitmap�ļ���
    unsigned char tempRGB;  //��ʱɫ��
    int imageIdx;
    //����R��B������λ��,bitmap���ļ����õ���BGR,�ڴ����RGB
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

//���洰�ڿͻ��˵Ľ�ͼ ���ڴ�С
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
    //�����ļ����ַ�������ʱ������
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
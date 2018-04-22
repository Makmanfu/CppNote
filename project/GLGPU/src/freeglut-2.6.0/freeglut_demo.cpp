//-----------------------------------------------------------------------------
//
//          注意链接freeglut时 dll不能编译成exe 静态可以直接链接
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <time.h>
//#include <GLTools.h>                    // OpenGL toolkit
//#include <GLShaderManager.h>            // Shader Manager Class
#include <gl/glut.h>                       // Windows FreeGlut equivalent

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);           //设置白色背景
    glMatrixMode(GL_PROJECTION);                //正投影将世界坐标系映射到屏幕
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);         //
}

void lineSegment(void)
{
    glClear(GL_COLOR_BUFFER_BIT);               //刷新显示窗口
    glColor3f(1.0, 0.0, 0.0);                   //设置红色线
    glBegin(GL_LINES);
    {
        glVertex2i(180, 15);
        glVertex2i(10, 145);
    }
    glEnd();
    glFlush();
}

bool BmpWriteFile(char* filename, int width, int height, unsigned char* bitmapData)
{
    //WriteBitmapFile  //根据bitmapData的（RGB）数据，保存bitmap  //filename是要保存到物理硬盘的文件名（包括路径）
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

void SaveCutScreenShot()
{
    int clnWidth(800), clnHeight(600);       //client width and height
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
    BmpWriteFile(lpstrFilename, clnWidth, clnHeight, (unsigned char*) screenData);
    free(screenData);
}

void GLMenu(int value)
{
    switch (value)
    {
        case 1:
            SaveCutScreenShot();
            break;
        case 2:
            break;
        case 3:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case 4:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case 5:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
    }
    glutPostRedisplay();
}

int GL_freeglut_demo(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 400) / 2, (GetSystemMetrics(SM_CYSCREEN) - 300) / 2);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OGL");
    init();
    glutDisplayFunc(lineSegment);
    //创建右键菜单
    glutCreateMenu(GLMenu);
    glutAddMenuEntry("Cut Screen", 1);
    glutAddMenuEntry("Toggle cull backface", 2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set Line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutMainLoop();
    return 1;
}






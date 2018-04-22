//-----------------------------------------------------------------------------
//
//          ע������freeglutʱ dll���ܱ����exe ��̬����ֱ������
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <time.h>
//#include <GLTools.h>                    // OpenGL toolkit
//#include <GLShaderManager.h>            // Shader Manager Class
#include <gl/glut.h>                       // Windows FreeGlut equivalent

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);           //���ð�ɫ����
    glMatrixMode(GL_PROJECTION);                //��ͶӰ����������ϵӳ�䵽��Ļ
    gluOrtho2D(0.0, 800.0, 0.0, 600.0);         //
}

void lineSegment(void)
{
    glClear(GL_COLOR_BUFFER_BIT);               //ˢ����ʾ����
    glColor3f(1.0, 0.0, 0.0);                   //���ú�ɫ��
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
    //WriteBitmapFile  //����bitmapData�ģ�RGB�����ݣ�����bitmap  //filename��Ҫ���浽����Ӳ�̵��ļ���������·����
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

void SaveCutScreenShot()
{
    int clnWidth(800), clnHeight(600);       //client width and height
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
    //�����Ҽ��˵�
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






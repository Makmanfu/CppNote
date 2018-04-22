
#include "stdafx.h"
#include "StuOpenGL.h"
#include <GLTools.h>                    // OpenGL toolkit
#include <GLShaderManager.h>    // Shader Manager Class
#include <GL/glut.h>                     // Windows FreeGlut equivalent

GLBatch triangleBatch;
//��ɫ��������
GLShaderManager shaderManager;
int wl, hl;

GLfloat blockSize = 0.1f;
GLfloat vVerts[] = { -blockSize - 0.5f, -blockSize, 0.0f,
                     blockSize - 0.5f, -blockSize, 0.0f,
                     blockSize - 0.5f,  blockSize, 0.0f,
                     -blockSize - 0.5f,  blockSize, 0.0f
                   };

bool BitmapWriteFile(char* filename, int width, int height, unsigned char* bitmapData)
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

void SaveScreenShot()
{
    int clnWidth(wl), clnHeight(hl);       //client width and height
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
    free(screenData);
}

void ProcessMenu(int value)
{
    switch (value)
    {
        case 1:
            SaveScreenShot();
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

void ChangeSize(int w, int h)
{
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    wl = w;
    hl = h;
}

void SetupRC()
{
    // ������
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    //GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,   0.5f, 0.0f, 0.0f,   0.0f, 0.5f, 0.0f };
    //triangleBatch.Begin(GL_TRIANGLES, 3);
    // ��������
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}

void SpecialKeys(int key, int x, int y)
{
    //GLfloat stepSize = 0.025f;
    //GLfloat blockX = vVerts[0];   // Upper left X
    //GLfloat blockY = vVerts[7];  // Upper left Y
    //if(key == GLUT_KEY_UP)
    //    blockY += stepSize;
    //if(key == GLUT_KEY_DOWN)
    //    blockY -= stepSize;
    //if(key == GLUT_KEY_LEFT)
    //    blockX -= stepSize;
    //if(key == GLUT_KEY_RIGHT)
    //    blockX += stepSize;
    //// Collision detection
    //if(blockX < -1.0f) blockX = -1.0f;
    //if(blockX > (1.0f - blockSize * 2))  blockX = 1.0f - blockSize * 2;;
    //if(blockY < -1.0f + blockSize * 2)  blockY = -1.0f + blockSize * 2;
    //if(blockY > 1.0f) blockY = 1.0f;
    //// Recalculate vertex positions
    //vVerts[0] = blockX;
    //vVerts[1] = blockY - blockSize*2;
    //vVerts[3] = blockX + blockSize*2;
    //vVerts[4] = blockY - blockSize*2;
    //vVerts[6] = blockX + blockSize*2;
    //vVerts[7] = blockY;
    //vVerts[9] = blockX;
    //vVerts[10] = blockY;
    //squareBatch.CopyVertexData3f(vVerts);
    glutPostRedisplay();
}

void RenderScene(void)
{
    // ������ڵ�ǰ��ɫ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GLfloat vRed[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    //�ύ��ɫ��
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    triangleBatch.Draw();
    // ִ�л�������������ʾ��̨������ �����˸����
    glutSwapBuffers();
}

int Triangle(int argc, char** argv)
{
    //GetWindowRect(hWnd,&rect);
    gltSetWorkingDirectory(argv[0]);
    //��ʼ��
    glutInit(&argc, argv);
    //���ó�ʼ��ʾģʽ
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    //���д�������
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 800) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2);
    //�ֱ���
    glutInitWindowSize(800, 600);
    glutCreateWindow("������");
    //����ά�ȸı�ص� (���������ӿ�����)
    glutReshapeFunc(ChangeSize);
    //���ð����¼�
    glutSpecialFunc(SpecialKeys);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    SetupRC();
    //ѭ���������ص� ����Ϣѭ��ʱ��Ⱦ
    glutDisplayFunc(RenderScene);
    //�����Ҽ��˵�
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Cut Screen", 1);
    glutAddMenuEntry("Toggle cull backface", 2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set Line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    //��Ϣѭ��
    glutMainLoop();
    return 0;
}



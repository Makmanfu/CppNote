
#include "stdafx.h"
#include "StuOpenGL.h"
#include <GLTools.h>                    // OpenGL toolkit
#include <GLShaderManager.h>    // Shader Manager Class
#include <GL/glut.h>                     // Windows FreeGlut equivalent

GLBatch triangleBatch;
//着色器管理器
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

void SaveScreenShot()
{
    int clnWidth(wl), clnHeight(hl);       //client width and height
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
    // 蓝背景
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    shaderManager.InitializeStockShaders();
    triangleBatch.Begin(GL_TRIANGLE_FAN, 4);
    //GLfloat vVerts[] = { -0.5f, 0.0f, 0.0f,   0.5f, 0.0f, 0.0f,   0.0f, 0.5f, 0.0f };
    //triangleBatch.Begin(GL_TRIANGLES, 3);
    // 画三角形
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
    // 清除窗口当前颜色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    GLfloat vRed[] = { 0.0f, 1.0f, 0.0f, 1.0f };
    //提交着色器
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    triangleBatch.Draw();
    // 执行缓冲区交换来显示后台缓冲区 解决闪烁问题
    glutSwapBuffers();
}

int Triangle(int argc, char** argv)
{
    //GetWindowRect(hWnd,&rect);
    gltSetWorkingDirectory(argv[0]);
    //初始化
    glutInit(&argc, argv);
    //设置初始显示模式
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
    //居中创建窗口
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 800) / 2, (GetSystemMetrics(SM_CYSCREEN) - 600) / 2);
    //分辨率
    glutInitWindowSize(800, 600);
    glutCreateWindow("三角形");
    //窗口维度改变回调 (可以缩放视口区域)
    glutReshapeFunc(ChangeSize);
    //设置按键事件
    glutSpecialFunc(SpecialKeys);
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
        return 1;
    }
    SetupRC();
    //循环缓冲区回调 在消息循环时渲染
    glutDisplayFunc(RenderScene);
    //创建右键菜单
    glutCreateMenu(ProcessMenu);
    glutAddMenuEntry("Cut Screen", 1);
    glutAddMenuEntry("Toggle cull backface", 2);
    glutAddMenuEntry("Set Fill Mode", 3);
    glutAddMenuEntry("Set Line Mode", 4);
    glutAddMenuEntry("Set Point Mode", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    //消息循环
    glutMainLoop();
    return 0;
}



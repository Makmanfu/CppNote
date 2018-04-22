//-----------------------------------------------------------------------------
//
//          ע������GLEWʱ dll���ܱ����exe ��̬����ֱ������
//
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include <stdlib.h>
#include "gpucl.h"
#include <glew.h>
#include <glut.h>
//========================================================
//ע��FreeImage����Ҫ�������
//#include "FreeImage.h"
//#if defined(_WIN64)
//#pragma comment(lib,"../src/FreeImage/FreeImage64.lib")
//#else
//#pragma comment(lib,"../src/FreeImage/FreeImage.lib")
//#endif
//========================================================
#include <glaux.h>
#pragma comment(lib,"opengl32.lib")
//#pragma comment(lib,"glu32.lib")
//#if defined(_WIN64)
//    #pragma comment(lib,"GlAux64.lib")
//#else
//    #pragma comment(lib,"GlAux.lib")
//#endif

AUX_RGBImageRec* LoadBMP(char* Filename)
{
    FILE* File = NULL;                                  // File Handle
    if (!Filename)                                      // Make Sure A Filename Was Given
        return NULL;                                    // If Not Return NULL
    //fopen_s ( &File, Filename, _T ( "r" ) );          // Check To See If The File Exists
    fopen_s(&File, Filename, "r");            // Check To See If The File Exists
    if (File)                                           // Does The File Exist?
    {
        fclose(File);                                   // Close The Handle
        return auxDIBImageLoadA(Filename);              // Load The Bitmap And Return A Pointer
    }
    return NULL;                                        // If Load Failed Return NULL
}
//========================================================

GPUCCL* gpu;

unsigned char* testImageData = 0;
int imageWidth = 0, imageHeight = 0;
GLuint previewTexID;

void GLSL_init()
{
    glewInit();
    //========================================================
    //���²���FreeImage����
    // Load test image from disk
    //FreeImage_Initialise();
    //FIBITMAP* bitmap = FreeImage_Load(FREE_IMAGE_FORMAT::FIF_PNG, "image.png");
    //BYTE* frameData = FreeImage_GetBits(bitmap);
    //imageWidth = FreeImage_GetWidth(bitmap);
    //imageHeight = FreeImage_GetHeight(bitmap);
    //testImageData = new unsigned char[imageWidth * imageHeight];
    //for (int y = 0; y < imageHeight; y++)
    //  for (int x = 0; x < imageWidth; x++)
    //      testImageData[y * imageWidth + x] = frameData[(y * imageWidth + x) * 4 + 0];
    //
    //FreeImage_Unload(bitmap);
    //========================================================
    AUX_RGBImageRec* TextureImage[1];                       // ��������Ĵ洢�ռ�
    memset(TextureImage, 0, sizeof(void*) * 1);             // Set The Pointer To NULL
    // ����λͼ��������޴������λͼû�ҵ����˳�
    //if (TextureImage[0] = LoadBMP(_T("image.bmp")))
    if (TextureImage[0] = LoadBMP("image.bmp"))
    {
        imageWidth = TextureImage[0]->sizeX;
        imageHeight = TextureImage[0]->sizeY;
        testImageData = new unsigned char[imageWidth * imageHeight];
        for (int y = 0; y < imageHeight; y++)
            for (int x = 0; x < imageWidth; x++)
                testImageData[y * imageWidth + x] = TextureImage[0]->data[(y * imageWidth + x) * 3 + 0];    //ע��BMP��PNG��ͬ������3
        //memcpy(testImageData,TextureImage[0]->data,sizeof(imageWidth * imageHeight*3));
    }
    if (TextureImage[0])                                    // �����Ƿ����
    {
        if (TextureImage[0]->data)                          // ����ͼ���Ƿ����
            free(TextureImage[0]->data);                    // �ͷ�����ͼ��ռ�õ��ڴ�
        free(TextureImage[0]);                              // �ͷ�ͼ��ṹ
    }
    //========================================================
    cout << "test.png " << imageWidth << "x" << imageHeight << endl;
    // Create preview texture
    glGenTextures(1, &previewTexID);
    glBindTexture(GL_TEXTURE_2D, previewTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, testImageData);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Initialize GPUCCL
    gpu = new GPUCCL();
    if (!gpu->init(imageWidth, imageHeight, 4))
    {
        cout << "Error initializing GPUCCL" << endl;
        system("pause");
        exit(1);
    }
}

void GLSL_display()
{
    gpu->begin(testImageData);
    gpu->createRuns();
    gpu->processColumns();
    gpu->postProcessColumns();
    gpu->computeBBoxes();
    gpu->stencilRoute();
    vector<GPUCCL::Component> c = gpu->results();
    gpu->end();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    int winW = glutGet(GLUT_WINDOW_WIDTH);
    int winH = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, winW, winH);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, winW, winH, 0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.375f, 0.375f, 0.0f);
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, previewTexID);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(0, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(imageWidth, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(imageWidth, imageHeight);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(0, imageHeight);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < c.size(); i++)
    {
        glVertex2i(c[i].minX, c[i].minY);
        glVertex2i(c[i].maxX, c[i].minY);
        glVertex2i(c[i].maxX, c[i].maxY);
        glVertex2i(c[i].minX, c[i].maxY);
    }
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glutSwapBuffers();
}

void reshape(int w, int h)
{
}

void idle()
{
    glutPostRedisplay();
}

void cleanup()
{
    delete gpu;
}

int GLSL_GPU_demo(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(512, 512);
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) - 512) / 2, (GetSystemMetrics(SM_CYSCREEN) - 512) / 2);
    //glutInitWindowPosition(600, 100);
    int winID = glutCreateWindow("GPU_GLSL");
    GLSL_init();
    glutDisplayFunc(&GLSL_display);
    glutReshapeFunc(&reshape);
    glutIdleFunc(&idle);
    glutMainLoop();
    cleanup();
    return 1;
}






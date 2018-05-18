
#include "stdafx.h"
#include "OpenGLView.h"
#include <Windows.h>

//opengl消息
void OpenGlProc(OpenGLView* m_OpenGL, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hDrawDC = GetDC(hWnd);
    RECT rect;
    GetClientRect(hWnd, &rect);//GetClientRect  GetWindowRect
    switch (uMsg)
    {
        //case WM_INITDIALOG:
        case WM_CREATE:
        {
            //opengl初始化
            m_OpenGL->SetDC(hDrawDC);
            m_OpenGL->InitOpenGL();
            m_OpenGL->ChangeSize(rect.right - rect.left, rect.bottom - rect.top);
        }
        break;
        case WM_PAINT:
        {
            //opengl绘图代码
            hDrawDC = BeginPaint(hWnd, &ps);
            m_OpenGL->RenderScene();
            SwapBuffers(hDrawDC);
            EndPaint(hWnd, &ps);
        }
        break;
        case WM_SIZE:
            m_OpenGL->ChangeSize(rect.right - rect.left, rect.bottom - rect.top);
            break;
    }
}

OpenGLView::OpenGLView(void) : hDC(NULL)
    , hRC(NULL)
    , hWnd(NULL)
{
    InitData();
}

OpenGLView::OpenGLView(HDC _hDC) : hDC(_hDC)
    , hRC(NULL)
    , hWnd(NULL)
{
    InitData();
}

OpenGLView::~OpenGLView(void)
{
}

void OpenGLView::SetDCPixelFormat(HDC hDC)
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

void OpenGLView::InitDC()
{
    if (hDC != NULL)
    {
        SetDCPixelFormat(hDC);
        hRC = wglCreateContext(hDC);
    }
    if (hRC != NULL)
        wglMakeCurrent(hDC, hRC);
}

void OpenGLView::InitScene(void)
{
    //纹理相关
    //if (!LoadGLTextures())
    //    return;
    // 加载背景纹理
    //Loadback();
    // 打开纹理
    //glEnable(GL_TEXTURE_2D);
    // 启用阴影平滑
    glShadeModel(GL_SMOOTH);
    // 背景色
    glClearColor(0.0f, 1.8f, 0.0f, 0.0f);
    // 设置深度缓存
    glClearDepth(1.0f);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 所作深度测试的类型
    glDepthFunc(GL_LEQUAL);
    // 告诉系统对透视进行修正
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //return;
    //glLightfv ( GL_LIGHT1, GL_AMBIENT, LightAmbient ); // Setup The Ambient Light
    //glLightfv ( GL_LIGHT1, GL_DIFFUSE, LightDiffuse ); // Setup The Diffuse Light
    //glLightfv ( GL_LIGHT1, GL_POSITION, LightPosition ); // Position The Light
    //glEnable ( GL_LIGHT1 );                          // Enable Light One
    //glColor4f ( 1.0f, 1.0f, 1.0f, 0.5 );             // 全亮度， 50% Alpha 混合
    //glBlendFunc ( GL_SRC_ALPHA, GL_ONE );            // 基于源象素alpha通道值的半透明混合函数
}

void OpenGLView::InitData(void)
{
    //LightAmbient[0] = 0.5f;
    //LightAmbient[1] = 0.5f;
    //LightAmbient[2] = 0.5f;
    //LightAmbient[3] = 1.0f;
    //LightDiffuse[0] = 1.0f;
    //LightDiffuse[1] = 1.0f;
    //LightDiffuse[2] = 1.0f;
    //LightDiffuse[3] = 1.0f;
    //LightPosition[0] = 0.0f;
    //LightPosition[1] = 0.0f;
    //LightPosition[2] = 2.0f;
    //LightPosition[3] = 1.0f;
}

void OpenGLView::InitOpenGL(void)
{
    InitDC();
    InitScene();
}

void OpenGLView::ChangeSize(GLsizei width, GLsizei height)
{
    // 防止被零除将Height设为1
    if (height == 0)
        height = 1;
    // 重置当前的视口
    glViewport(0, 0, width, height);
    // 选择投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 重置投影矩阵
    glLoadIdentity();
    // 设置视口的大小
    //gluPerspective ( 45.0f, ( GLfloat ) width / ( GLfloat ) height, 0.1f, 100.0f );
    // 选择模型观察矩阵
    glMatrixMode(GL_MODELVIEW);
    // 重置模型观察矩阵
    glLoadIdentity();
}

void OpenGLView::RenderScene()
{
    // 清除屏幕和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 重置当前的模型观察矩阵
    glLoadIdentity();
    // 左移 1.5 单位，并移入屏幕 6.0
    //glTranslatef ( 0.0f, -0.0f, -5.0f );
    //glTranslatef(0.0f,-10.4f,-26.0f);
    // 画线
    //DrawLine();
    // 画三角形
    DrawTriangle();
    // 画三角形
    //Draw3Triangle ( 1.0f );
    // 彩色正方体
    //DrawQuad(0.1f);
    // 纹理
    //DrawLightBMP(1.0f);
}

void OpenGLView::DrawLine(void)
{
    // 指定点的颜色
    glColor3f(1.0f, 0.0f, 0.0f);
    // 绘制直线
    glBegin(GL_LINES);
    {
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.3f, 0.3f, 0.0f);
        //glVertex3f ( 0.0f, 300.0f, 0.0f );
        //glVertex3f ( 0.0f, -300.0f, 0.0f );
        //glVertex3f ( -300.0f, 0.0f, 0.0f );
        //glVertex3f ( 300.0f, 0.0f, 0.0f );
        //glVertex3f ( 0.0f, 0.0f, -300.0f );
        //glVertex3f ( 0.0f, 0.0f, 300.0f );
    }
    glEnd();
}

void OpenGLView::DrawTriangle(float ff)
{
    // 绕Y轴旋转三角形
    //glRotatef(280.0f, 1.0f, 0.0f, 0.0f);
    // 旋转至当前所画星星的角度
    //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    // 绘制三角形
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);                 //红
        glVertex3f(0.0f, ff + 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);                 //绿
        glVertex3f(-ff, -ff + 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);                 //蓝
        glVertex3f(ff, -ff + 1.0f, 0.0f);
    }
    glEnd();
}

void OpenGLView::Draw3Triangle(float ff)
{
    //glBindTexture(GL_TEXTURE_2D, textBack[0]);
    // 绕Y轴旋转三角形
    glRotatef(290.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    {
        // glTexCoord2f都是生成纹理
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-2.8f, -2.0f, -0.01f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(2.8f, -2.0f, -0.01f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(2.8f, 2.0f, -0.01f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-2.8f, 2.0f, -0.01f);
    }
    glEnd();
    return;
    //float yy = 0.0f;
    //// 绕Y轴旋转三角形
    //glRotatef ( 280.0f, 1.0f, 0.0f, 0.0f );
    //// 旋转至当前所画星星的角度
    //glRotatef ( 25.0f, 0.0f, 0.0f, 1.0f );
    //// 绘制三角形
    //glBegin ( GL_TRIANGLES );
    //{
    //    glColor3f ( 0.0f, 1.0f, 0.0f );
    //    glVertex3f ( yy, 2 * ff, yy );
    //    glVertex3f ( -ff, yy, yy );
    //    glVertex3f ( ff, yy, yy );
    //    glVertex3f ( yy, 2 * ff + 3.0f, yy );
    //    glVertex3f ( -ff, yy + 3.0f, yy );
    //    glVertex3f ( ff, yy + 3.0f, yy );
    //    glVertex3f ( yy, 2 * ff + 3.0f * 2, yy );
    //    glVertex3f ( -ff, yy + 3.0f * 2, yy );
    //    glVertex3f ( ff, yy + 3.0f * 2, yy );
    //    glVertex3f ( yy, 2 * ff + 3.0f * 3, yy );
    //    glVertex3f ( -ff, yy + 3.0f * 3, yy );
    //    glVertex3f ( ff, yy + 3.0f * 3, yy );
    //}
    //glEnd();
}

void OpenGLView::DrawQuad(float ff)
{
    //glRotatef(m_rtri, 1.0f, 1.0f, 0.0f);             // 绕Y轴旋转三角形
    glBegin(GL_QUADS);
    {
        glColor3f(0.0f, 1.0f, 0.0f);                 // 颜色改为蓝色
        glVertex3f(ff, ff, -ff);                     // 四边形的右上顶点 (顶面)
        glVertex3f(-ff, ff, -ff);                    // 四边形的左上顶点 (顶面)
        glVertex3f(-ff, ff, ff);                     // 四边形的左下顶点 (顶面)
        glVertex3f(ff, ff, ff);                      // 四边形的右下顶点 (顶面)
        glColor3f(1.0f, 0.5f, 0.0f);                 // 颜色改成橙色
        glVertex3f(ff, -ff, ff);                     // 四边形的右上顶点(底面)
        glVertex3f(-ff, -ff, ff);                    // 四边形的左上顶点(底面)
        glVertex3f(-ff, -ff, -ff);                   // 四边形的左下顶点(底面)
        glVertex3f(ff, -ff, -ff);                    // 四边形的右下顶点(底面)
        glColor3f(1.0f, 0.0f, 0.0f);                 // 颜色改成红色
        glVertex3f(ff, ff, ff);                      // 四边形的右上顶点(前面)
        glVertex3f(-ff, ff, ff);                     // 四边形的左上顶点(前面)
        glVertex3f(-ff, -ff, ff);                    // 四边形的左下顶点(前面)
        glVertex3f(ff, -ff, ff);                     // 四边形的右下顶点(前面)
        glColor3f(1.0f, 1.0f, 0.0f);                 // 颜色改成黄色
        glVertex3f(ff, -ff, -ff);                    // 四边形的右上顶点(后面)
        glVertex3f(-ff, -ff, -ff);                   // 四边形的左上顶点(后面)
        glVertex3f(-ff, ff, -ff);                    // 四边形的左下顶点(后面)
        glVertex3f(ff, ff, -ff);                     // 四边形的右下顶点(后面)
        glColor3f(0.0f, 0.0f, 1.0f);                 // 颜色改成蓝色
        glVertex3f(-ff, ff, ff);                     // 四边形的右上顶点(左面)
        glVertex3f(-ff, ff, -ff);                    // 四边形的左上顶点(左面)
        glVertex3f(-ff, -ff, -ff);                   // 四边形的左下顶点(左面)
        glVertex3f(-ff, -ff, ff);                    // 四边形的右下顶点(左面)
        glColor3f(1.0f, 0.0f, 1.0f);                 // 颜色改成紫罗兰色
        glVertex3f(ff, ff, -ff);                     // 四边形的右上顶点(右面)
        glVertex3f(ff, ff, ff);                      // 四边形的左上顶点(右面)
        glVertex3f(ff, -ff, ff);                     // 四边形的左下顶点(右面)
        glVertex3f(ff, -ff, -ff);                    // 四边形的右下顶点(右面)
    }
    glEnd();
    //m_rtri += 1.0f;
}







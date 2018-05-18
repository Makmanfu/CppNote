//-----------------------------------------------------------------------------
//
//   windows OpenGl原始API学习总结
//
//-----------------------------------------------------------------------------

#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"GlU32.lib")

//WMPAINT重绘制过程
void OpenGlPaint(HWND hDrawWnd);

//消息过程OpenglProc
void OpenGlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//opengl
class OpenGLView
{
private:
    typedef struct _glPoint2f
    {
        float x, y;
        _glPoint2f(float a = 0.0f, float b = 0.0f)
        {
            x = a;
            y = b;
        }
    } glPoint2f;
    //GLfloat LightAmbient[4];
    //GLfloat LightDiffuse[4];
    //GLfloat LightPosition[4];
public:
    // OpenGL渲染描述表句柄
    HDC hDC = NULL;
    // Permanent Rendering Context
    HGLRC hRC = NULL;
    // 保存我们的窗口句柄
    HWND hWnd = NULL;
public:
    OpenGLView(void);
    OpenGLView(HDC _hDC);
    ~OpenGLView(void);
    void SetDC(HDC _hDC)
    {
        hDC = _hDC;
    };
public:
    //此处开始对OpenGL进行所有设置
    void InitOpenGL(void);
    void UnInitOpenGL();
    //重置OpenGL窗口大小
    void ChangeSize(GLsizei width, GLsizei height);
    //opengl绘制
    void RenderScene(void);
    // 绘制直线坐标系
    void DrawLine(void);
    //绘制三角形
    void DrawTriangle(float ff = 1.0f);
    //绘制投影三角形
    void Draw3Triangle(float ff = 1.0f);
    //绘制立方体
    void DrawQuad(float ff = 1.0f);
private:
    //告诉窗口我们所希望的东东，即窗口使用的像素格式
    void SetDCPixelFormat(HDC hDC);
    //初始化DC
    void InitDC();
    //初始化数据
    void InitData();
    //初始化Scene
    void InitScene();
};

#endif
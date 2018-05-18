//-----------------------------------------------------------------------------
//
//   windows OpenGlԭʼAPIѧϰ�ܽ�
//
//-----------------------------------------------------------------------------

#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H
#include <gl/gl.h>
#include <gl/glu.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"GlU32.lib")

//WMPAINT�ػ��ƹ���
void OpenGlPaint(HWND hDrawWnd);

//��Ϣ����OpenglProc
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
    // OpenGL��Ⱦ��������
    HDC hDC = NULL;
    // Permanent Rendering Context
    HGLRC hRC = NULL;
    // �������ǵĴ��ھ��
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
    //�˴���ʼ��OpenGL������������
    void InitOpenGL(void);
    void UnInitOpenGL();
    //����OpenGL���ڴ�С
    void ChangeSize(GLsizei width, GLsizei height);
    //opengl����
    void RenderScene(void);
    // ����ֱ������ϵ
    void DrawLine(void);
    //����������
    void DrawTriangle(float ff = 1.0f);
    //����ͶӰ������
    void Draw3Triangle(float ff = 1.0f);
    //����������
    void DrawQuad(float ff = 1.0f);
private:
    //���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
    void SetDCPixelFormat(HDC hDC);
    //��ʼ��DC
    void InitDC();
    //��ʼ������
    void InitData();
    //��ʼ��Scene
    void InitScene();
};

#endif
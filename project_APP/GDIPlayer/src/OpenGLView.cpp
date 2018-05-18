
#include "stdafx.h"
#include "OpenGLView.h"
#include <Windows.h>

//opengl��Ϣ
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
            //opengl��ʼ��
            m_OpenGL->SetDC(hDrawDC);
            m_OpenGL->InitOpenGL();
            m_OpenGL->ChangeSize(rect.right - rect.left, rect.bottom - rect.top);
        }
        break;
        case WM_PAINT:
        {
            //opengl��ͼ����
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
    //�������
    //if (!LoadGLTextures())
    //    return;
    // ���ر�������
    //Loadback();
    // ������
    //glEnable(GL_TEXTURE_2D);
    // ������Ӱƽ��
    glShadeModel(GL_SMOOTH);
    // ����ɫ
    glClearColor(0.0f, 1.8f, 0.0f, 0.0f);
    // ������Ȼ���
    glClearDepth(1.0f);
    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    // ������Ȳ��Ե�����
    glDepthFunc(GL_LEQUAL);
    // ����ϵͳ��͸�ӽ�������
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //return;
    //glLightfv ( GL_LIGHT1, GL_AMBIENT, LightAmbient ); // Setup The Ambient Light
    //glLightfv ( GL_LIGHT1, GL_DIFFUSE, LightDiffuse ); // Setup The Diffuse Light
    //glLightfv ( GL_LIGHT1, GL_POSITION, LightPosition ); // Position The Light
    //glEnable ( GL_LIGHT1 );                          // Enable Light One
    //glColor4f ( 1.0f, 1.0f, 1.0f, 0.5 );             // ȫ���ȣ� 50% Alpha ���
    //glBlendFunc ( GL_SRC_ALPHA, GL_ONE );            // ����Դ����alphaͨ��ֵ�İ�͸����Ϻ���
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
    // ��ֹ�������Height��Ϊ1
    if (height == 0)
        height = 1;
    // ���õ�ǰ���ӿ�
    glViewport(0, 0, width, height);
    // ѡ��ͶӰ����
    glMatrixMode(GL_PROJECTION);
    // ����ͶӰ����
    glLoadIdentity();
    // �����ӿڵĴ�С
    //gluPerspective ( 45.0f, ( GLfloat ) width / ( GLfloat ) height, 0.1f, 100.0f );
    // ѡ��ģ�͹۲����
    glMatrixMode(GL_MODELVIEW);
    // ����ģ�͹۲����
    glLoadIdentity();
}

void OpenGLView::RenderScene()
{
    // �����Ļ����Ȼ���
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // ���õ�ǰ��ģ�͹۲����
    glLoadIdentity();
    // ���� 1.5 ��λ����������Ļ 6.0
    //glTranslatef ( 0.0f, -0.0f, -5.0f );
    //glTranslatef(0.0f,-10.4f,-26.0f);
    // ����
    //DrawLine();
    // ��������
    DrawTriangle();
    // ��������
    //Draw3Triangle ( 1.0f );
    // ��ɫ������
    //DrawQuad(0.1f);
    // ����
    //DrawLightBMP(1.0f);
}

void OpenGLView::DrawLine(void)
{
    // ָ�������ɫ
    glColor3f(1.0f, 0.0f, 0.0f);
    // ����ֱ��
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
    // ��Y����ת������
    //glRotatef(280.0f, 1.0f, 0.0f, 0.0f);
    // ��ת����ǰ�������ǵĽǶ�
    //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
    // ����������
    glBegin(GL_TRIANGLES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);                 //��
        glVertex3f(0.0f, ff + 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);                 //��
        glVertex3f(-ff, -ff + 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);                 //��
        glVertex3f(ff, -ff + 1.0f, 0.0f);
    }
    glEnd();
}

void OpenGLView::Draw3Triangle(float ff)
{
    //glBindTexture(GL_TEXTURE_2D, textBack[0]);
    // ��Y����ת������
    glRotatef(290.0f, 1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    {
        // glTexCoord2f������������
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
    //// ��Y����ת������
    //glRotatef ( 280.0f, 1.0f, 0.0f, 0.0f );
    //// ��ת����ǰ�������ǵĽǶ�
    //glRotatef ( 25.0f, 0.0f, 0.0f, 1.0f );
    //// ����������
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
    //glRotatef(m_rtri, 1.0f, 1.0f, 0.0f);             // ��Y����ת������
    glBegin(GL_QUADS);
    {
        glColor3f(0.0f, 1.0f, 0.0f);                 // ��ɫ��Ϊ��ɫ
        glVertex3f(ff, ff, -ff);                     // �ı��ε����϶��� (����)
        glVertex3f(-ff, ff, -ff);                    // �ı��ε����϶��� (����)
        glVertex3f(-ff, ff, ff);                     // �ı��ε����¶��� (����)
        glVertex3f(ff, ff, ff);                      // �ı��ε����¶��� (����)
        glColor3f(1.0f, 0.5f, 0.0f);                 // ��ɫ�ĳɳ�ɫ
        glVertex3f(ff, -ff, ff);                     // �ı��ε����϶���(����)
        glVertex3f(-ff, -ff, ff);                    // �ı��ε����϶���(����)
        glVertex3f(-ff, -ff, -ff);                   // �ı��ε����¶���(����)
        glVertex3f(ff, -ff, -ff);                    // �ı��ε����¶���(����)
        glColor3f(1.0f, 0.0f, 0.0f);                 // ��ɫ�ĳɺ�ɫ
        glVertex3f(ff, ff, ff);                      // �ı��ε����϶���(ǰ��)
        glVertex3f(-ff, ff, ff);                     // �ı��ε����϶���(ǰ��)
        glVertex3f(-ff, -ff, ff);                    // �ı��ε����¶���(ǰ��)
        glVertex3f(ff, -ff, ff);                     // �ı��ε����¶���(ǰ��)
        glColor3f(1.0f, 1.0f, 0.0f);                 // ��ɫ�ĳɻ�ɫ
        glVertex3f(ff, -ff, -ff);                    // �ı��ε����϶���(����)
        glVertex3f(-ff, -ff, -ff);                   // �ı��ε����϶���(����)
        glVertex3f(-ff, ff, -ff);                    // �ı��ε����¶���(����)
        glVertex3f(ff, ff, -ff);                     // �ı��ε����¶���(����)
        glColor3f(0.0f, 0.0f, 1.0f);                 // ��ɫ�ĳ���ɫ
        glVertex3f(-ff, ff, ff);                     // �ı��ε����϶���(����)
        glVertex3f(-ff, ff, -ff);                    // �ı��ε����϶���(����)
        glVertex3f(-ff, -ff, -ff);                   // �ı��ε����¶���(����)
        glVertex3f(-ff, -ff, ff);                    // �ı��ε����¶���(����)
        glColor3f(1.0f, 0.0f, 1.0f);                 // ��ɫ�ĳ�������ɫ
        glVertex3f(ff, ff, -ff);                     // �ı��ε����϶���(����)
        glVertex3f(ff, ff, ff);                      // �ı��ε����϶���(����)
        glVertex3f(ff, -ff, ff);                     // �ı��ε����¶���(����)
        glVertex3f(ff, -ff, -ff);                    // �ı��ε����¶���(����)
    }
    glEnd();
    //m_rtri += 1.0f;
}







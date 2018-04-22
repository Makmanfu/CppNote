//-----------------------------------------------------------------------------
//
//          opengl ����ģ��
//
//-----------------------------------------------------------------------------
#ifndef GLVIEW_H
#define GLVIEW_H

#include "GLDraw.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "TrackBall.h"

//GL���������
class GLFram
{
public:
    GLFram(void);
    GLFram(HDC _hDC);
    virtual ~GLFram(void);
    void SetDC(HDC _hDC);
public:
    //��ʼ����������
    void GLInit(void);
    //�ͷ�GL
    void GLFree(void);
    //opengl����
    virtual void RenderScene(void);
    //��ʼ��opengl
    virtual bool InitOpenGL(void);
    //����OpenGL���ڴ�С
    virtual void ChangeSize(GLsizei width, GLsizei height);
private:
    HDC hDC;                        //OpenGL��Ⱦ��������
    HGLRC hRC;                      //Permanent Rendering Context
private:
    //���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
    void SetDCPixelFormat(HDC hDC);
    //��ʼ��DC
    void InitDC();
};

//GL��ʾ
class GLView : public GLFram
{
    GLDraw mDraw;
protected:
    //��ʼ��opengl
    virtual bool InitOpenGL(void);
public:
    //opengl����
    virtual void RenderScene(void);
    //����OpenGL���ڴ�С
    virtual void ChangeSize(GLsizei width, GLsizei height);
};

//�����β鿴
class TriangleGL : public RendererGL, public GLFram
{
public:
    //����ģ��
    virtual void renderObjects(void)
    {
        //�����Ļ����Ȼ���
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //���õ�ǰ��ģ�͹۲����
        //glLoadIdentity();
        //��������
        // ��Y����ת������
        glRotatef(280.0f, 1.0f, 0.0f, 0.0f);
        // ��ת����ǰ�������ǵĽǶ�
        //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
        float ff = 1.0f;
        // ����������
        glBegin(GL_TRIANGLES);
        {
            glColor3f(1.0f, 0.0f, 0.0f);              //��
            glVertex3f(0.0f, ff + 1.0f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f);              //��
            glVertex3f(-ff, -ff + 1.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f);              //��
            glVertex3f(ff, -ff + 1.0f, 0.0f);
        }
        glEnd();
    };
};

//������鿴
class CubeGL : public RendererGL, public GLFram
{
public:
    //GL��ʼ��ģ��
    virtual bool InitOpenGL(void)
    {
        RendererGL::init();
        return true;
    };
};

//3DT������ʾ
//class C3DTGL : public RendererGL, public GLFram
//{
//private:
//    std::vector<Node3DT>m_Nodes;            //3D���ݾͲ���˵��
//public:
//    //GL����ģ��
//    virtual void renderObjects(void)
//    {
//        if (m_Nodes.size() > 0)
//        {
//            for (int i = 0; i < m_Nodes.size(); i++)
//                DrawNode(m_Nodes[i], GL_RENDER);
//        }
//    };
//
//private:
//    //��������
//    GLuint ddsTexture(std::string path);
//    //3DT���ݽ���
//    void Load3DTModel(std::string path);
//    //����ģ��
//    bool DrawNode(Node3DT node, int mode);
//};





#endif
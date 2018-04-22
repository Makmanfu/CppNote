//-----------------------------------------------------------------------------
//
//          opengl 绘制模块
//
//-----------------------------------------------------------------------------
#ifndef GLVIEW_H
#define GLVIEW_H

#include "GLDraw.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "TrackBall.h"

//GL处理抽象框架
class GLFram
{
public:
    GLFram(void);
    GLFram(HDC _hDC);
    virtual ~GLFram(void);
    void SetDC(HDC _hDC);
public:
    //初始化所有设置
    void GLInit(void);
    //释放GL
    void GLFree(void);
    //opengl绘制
    virtual void RenderScene(void);
    //初始化opengl
    virtual bool InitOpenGL(void);
    //重置OpenGL窗口大小
    virtual void ChangeSize(GLsizei width, GLsizei height);
private:
    HDC hDC;                        //OpenGL渲染描述表句柄
    HGLRC hRC;                      //Permanent Rendering Context
private:
    //告诉窗口我们所希望的东东，即窗口使用的像素格式
    void SetDCPixelFormat(HDC hDC);
    //初始化DC
    void InitDC();
};

//GL显示
class GLView : public GLFram
{
    GLDraw mDraw;
protected:
    //初始化opengl
    virtual bool InitOpenGL(void);
public:
    //opengl绘制
    virtual void RenderScene(void);
    //重置OpenGL窗口大小
    virtual void ChangeSize(GLsizei width, GLsizei height);
};

//三角形查看
class TriangleGL : public RendererGL, public GLFram
{
public:
    //绘制模块
    virtual void renderObjects(void)
    {
        //清除屏幕和深度缓存
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //重置当前的模型观察矩阵
        //glLoadIdentity();
        //画三角形
        // 绕Y轴旋转三角形
        glRotatef(280.0f, 1.0f, 0.0f, 0.0f);
        // 旋转至当前所画星星的角度
        //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
        float ff = 1.0f;
        // 绘制三角形
        glBegin(GL_TRIANGLES);
        {
            glColor3f(1.0f, 0.0f, 0.0f);              //红
            glVertex3f(0.0f, ff + 1.0f, 0.0f);
            glColor3f(0.0f, 1.0f, 0.0f);              //绿
            glVertex3f(-ff, -ff + 1.0f, 0.0f);
            glColor3f(0.0f, 0.0f, 1.0f);              //蓝
            glVertex3f(ff, -ff + 1.0f, 0.0f);
        }
        glEnd();
    };
};

//正方体查看
class CubeGL : public RendererGL, public GLFram
{
public:
    //GL初始化模块
    virtual bool InitOpenGL(void)
    {
        RendererGL::init();
        return true;
    };
};

//3DT数据显示
//class C3DTGL : public RendererGL, public GLFram
//{
//private:
//    std::vector<Node3DT>m_Nodes;            //3D数据就不多说了
//public:
//    //GL绘制模块
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
//    //加载纹理
//    GLuint ddsTexture(std::string path);
//    //3DT数据解析
//    void Load3DTModel(std::string path);
//    //绘制模型
//    bool DrawNode(Node3DT node, int mode);
//};





#endif
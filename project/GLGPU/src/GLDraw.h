//-----------------------------------------------------------------------------
//
//          opengl 绘制数据结构
//
//-----------------------------------------------------------------------------
#ifndef GLDRAW_H
#define GLDRAW_H

#include <gl/gl.h>
#include <gl/glu.h>

typedef struct _glPoint2f
{
    float x, y;
    _glPoint2f(float a = 0.0f, float b = 0.0f)
    {
        x = a;
        y = b;
    }
} glPoint2f;

class GLDraw
{
protected:
    GLfloat m_rtri;
    GLfloat m_rquad;
    GLuint  texture[1];                   //存储一个纹理
    GLuint  textBack[1];                  //背景图纹理

    GLfloat LightAmbient[4];
    GLfloat LightDiffuse[4];
    GLfloat LightPosition[4];
public:
    GLDraw();
    virtual ~GLDraw();
public:
    // 绘制直线坐标系
    void DrawLine(void);
    //绘制三角形
    void DrawTriangle(float ff = 1.0f);
    //绘制投影三角形
    void Draw3Triangle(float ff = 1.0f);
    //绘制立方体
    void DrawQuad(float ff = 1.0f);
};





#endif
//-----------------------------------------------------------------------------
//
//          opengl �������ݽṹ
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
    GLuint  texture[1];                   //�洢һ������
    GLuint  textBack[1];                  //����ͼ����

    GLfloat LightAmbient[4];
    GLfloat LightDiffuse[4];
    GLfloat LightPosition[4];
public:
    GLDraw();
    virtual ~GLDraw();
public:
    // ����ֱ������ϵ
    void DrawLine(void);
    //����������
    void DrawTriangle(float ff = 1.0f);
    //����ͶӰ������
    void Draw3Triangle(float ff = 1.0f);
    //����������
    void DrawQuad(float ff = 1.0f);
};





#endif
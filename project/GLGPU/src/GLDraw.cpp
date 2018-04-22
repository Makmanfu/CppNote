

#include "stdafx.h"
#include "GLDraw.h"

GLDraw::GLDraw()
{
}

GLDraw::~GLDraw()
{
}

void GLDraw::DrawLine(void)
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

void GLDraw::DrawTriangle(float ff /*= 1.0f*/)
{
    // 绕Y轴旋转三角形
    //glRotatef(280.0f, 1.0f, 0.0f, 0.0f);
    // 旋转至当前所画星星的角度
    //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
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
}

void GLDraw::Draw3Triangle(float ff /*= 1.0f*/)
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

void GLDraw::DrawQuad(float ff /*= 1.0f*/)
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

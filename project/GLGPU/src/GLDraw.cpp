

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

void GLDraw::DrawTriangle(float ff /*= 1.0f*/)
{
    // ��Y����ת������
    //glRotatef(280.0f, 1.0f, 0.0f, 0.0f);
    // ��ת����ǰ�������ǵĽǶ�
    //glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
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
}

void GLDraw::Draw3Triangle(float ff /*= 1.0f*/)
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

void GLDraw::DrawQuad(float ff /*= 1.0f*/)
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

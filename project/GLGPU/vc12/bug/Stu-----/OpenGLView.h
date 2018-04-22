
#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H
#include "stdafx.h"
#include "Glconf.h"


//GL����ģ��
class COpenGLView
{
private:
    GLfloat m_rtri;
    GLfloat m_rquad;
    GLuint  texture[1];                   //�洢һ������
    GLuint  textBack[1];                  //����ͼ����
    typedef struct _glPoint2f
    {
        float x, y;
        _glPoint2f(float a = 0.0f, float b = 0.0f)
        {
            x = a;
            y = b;
        }
    } glPoint2f;

    GLfloat LightAmbient[4];
    GLfloat LightDiffuse[4];
    GLfloat LightPosition[4];
public:
    bool active;                //Window Active Flag Set To TRUE By Default
    bool light;                 //Lighting ON/OFF ( NEW )
    bool blend;                 //Blending OFF/ON? ( NEW )
    bool lp, bp;                //L Pressed? ( NEW )

    GLfloat xrot, yrot, zrot;   //��ת��
    GLfloat xspeed;             //X Rotation Speed
    GLfloat yspeed;             //Y Rotation Speed
    GLfloat z;                  //Depth Into The Screen
public:
    COpenGLView(void);
    ~COpenGLView(void);
private:
    //����λͼͼ��
    AUX_RGBImageRec* LoadBMP(wchar_t* Filename);
    //����Դ����λͼ
    AUX_RGBImageRec* LoadRes(WORD f_id);
    //����λͼ(��������Ĵ���)��ת��������
    int LoadGLTextures();
    int Loadback();
    //����ֱ������ϵ
    void DrawLine(void);
    //����������
    void DrawTriangle(float ff = 1.0f);
    //����ͶӰ������
    void Draw3Triangle(float ff = 1.0f);
    void DrawTriangleVec(vector<glPoint2f> pts);
    //����������
    void DrawQuad(float ff = 1.0f);
    //����
    void DrawBMP(float ff = 1.0f);
    //��͸���͹�Դ����
    void DrawLightBMP(float ff = 1.0f);
public:
    //��ʼ��
    void InitScene(void);
    //opengl����
    void RenderScene(void);
    //����OpenGL���ڴ�С
    void ChangeSize(GLsizei width, GLsizei height);
};




#endif
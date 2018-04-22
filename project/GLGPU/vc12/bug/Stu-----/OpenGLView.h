
#ifndef OPENGLVIEW_H
#define OPENGLVIEW_H
#include "stdafx.h"
#include "Glconf.h"


//GL绘制模型
class COpenGLView
{
private:
    GLfloat m_rtri;
    GLfloat m_rquad;
    GLuint  texture[1];                   //存储一个纹理
    GLuint  textBack[1];                  //背景图纹理
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

    GLfloat xrot, yrot, zrot;   //旋转量
    GLfloat xspeed;             //X Rotation Speed
    GLfloat yspeed;             //Y Rotation Speed
    GLfloat z;                  //Depth Into The Screen
public:
    COpenGLView(void);
    ~COpenGLView(void);
private:
    //载入位图图象
    AUX_RGBImageRec* LoadBMP(wchar_t* Filename);
    //从资源加载位图
    AUX_RGBImageRec* LoadRes(WORD f_id);
    //载入位图(调用上面的代码)并转换成纹理
    int LoadGLTextures();
    int Loadback();
    //绘制直线坐标系
    void DrawLine(void);
    //绘制三角形
    void DrawTriangle(float ff = 1.0f);
    //绘制投影三角形
    void Draw3Triangle(float ff = 1.0f);
    void DrawTriangleVec(vector<glPoint2f> pts);
    //绘制立方体
    void DrawQuad(float ff = 1.0f);
    //纹理
    void DrawBMP(float ff = 1.0f);
    //半透明和光源纹理
    void DrawLightBMP(float ff = 1.0f);
public:
    //初始化
    void InitScene(void);
    //opengl绘制
    void RenderScene(void);
    //重置OpenGL窗口大小
    void ChangeSize(GLsizei width, GLsizei height);
};




#endif
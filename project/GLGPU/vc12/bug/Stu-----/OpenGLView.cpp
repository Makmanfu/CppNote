

#include "stdafx.h"
#include "OpenGLView.h"


COpenGLView::COpenGLView(void)
{
    m_rtri = 0;
    m_rquad = 0;
    z = -5.0f;          // Depth Into The Screen
    active = true;
    LightAmbient[0] = 0.5f;
    LightAmbient[1] = 0.5f;
    LightAmbient[2] = 0.5f;
    LightAmbient[3] = 1.0f;
    LightDiffuse[0] = 1.0f;
    LightDiffuse[1] = 1.0f;
    LightDiffuse[2] = 1.0f;
    LightDiffuse[3] = 1.0f;
    LightPosition[0] = 0.0f;
    LightPosition[1] = 0.0f;
    LightPosition[2] = 2.0f;
    LightPosition[3] = 1.0f;
}

COpenGLView::~COpenGLView(void)
{
}

void COpenGLView::DrawLine(void)
{
}

void COpenGLView::DrawTriangle(float ff)
{
    glRotatef(280.0f, 1.0f, 0.0f, 0.0f);                // 绕Y轴旋转三角形
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);                 // 旋转至当前所画星星的角度
    glBegin(GL_TRIANGLES);                              // 绘制三角形
    {
        glColor3f(1.0f, 0.0f, 0.0f);                    //红
        glVertex3f(0.0f, ff + 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);                    //绿
        glVertex3f(-ff, -ff + 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);                    //蓝
        glVertex3f(ff, -ff + 1.0f, 0.0f);
    }
    glEnd();
}

void COpenGLView::Draw3Triangle(float ff)
{
    glBindTexture(GL_TEXTURE_2D, textBack[0]);
    glRotatef(290.0f, 1.0f, 0.0f, 0.0f);                // 绕Y轴旋转三角形
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
    //___________________________________________________________________
    float yy = 0.0f;
    glRotatef(280.0f, 1.0f, 0.0f, 0.0f);                // 绕Y轴旋转三角形
    glRotatef(25.0f, 0.0f, 0.0f, 1.0f);                 // 旋转至当前所画星星的角度
    glBegin(GL_TRIANGLES);                              // 绘制三角形
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(yy, 2 * ff, yy);
        glVertex3f(-ff, yy, yy);
        glVertex3f(ff, yy, yy);
        glVertex3f(yy, 2 * ff + 3.0f, yy);
        glVertex3f(-ff, yy + 3.0f, yy);
        glVertex3f(ff, yy + 3.0f, yy);
        glVertex3f(yy, 2 * ff + 3.0f * 2, yy);
        glVertex3f(-ff, yy + 3.0f * 2, yy);
        glVertex3f(ff, yy + 3.0f * 2, yy);
        glVertex3f(yy, 2 * ff + 3.0f * 3, yy);
        glVertex3f(-ff, yy + 3.0f * 3, yy);
        glVertex3f(ff, yy + 3.0f * 3, yy);
    }
    glEnd();
}

void COpenGLView::DrawTriangleVec(vector<glPoint2f> pts)
{
    //三角形的边长
    float Trianglel = 10.0f;
    float xl, yl, xr, yr;
    double ql, qr;                                      //左角, 右角
    //glPoint2f pt = glPoint2f(0.0f,1.0f);
    //结尾点虚拟 此处需要好好思考
    pts.push_back(glPoint2f(pts.rbegin()->x, pts.rbegin()->y + 1));
    //沿着轨迹绘制并旋转图像
    glPoint2f t_py;
    for (vector<glPoint2f>::const_iterator it = pts.begin(); it != pts.end(); ++it)
    {
        if (it == pts.begin())
        {
            t_py = *it;
            continue;
        }
        else
        {
            //glRotatef(280.0f,1.0f,0.0f,0.0f);         //绕Y轴旋转三角形
            glBegin(GL_TRIANGLES);                      //绘制三角形
            {
                ql = 0.1666667 - atan((double)(it->x - t_py.x) / (it->y - t_py.y));
                qr = 0.1666667 + atan((double)(it->x - t_py.x) / (it->y - t_py.y));
                xl = it->x - sin(ql) * Trianglel;
                yl = it->y - cos(ql) * Trianglel;
                xr = it->x - sin(qr) * Trianglel;
                yr = it->y - cos(qr) * Trianglel;
                glColor3f(0.0f, 1.0f, 0.0f);            //颜色
                glVertex3f(it->x, it->y, 0.0);          //第一个点
                glVertex3f(xl, yl, 0.0);
                glVertex3f(xr, yr, 0.0);
            }
            glEnd();
            //DrawLoopArrow(f_img,t_py,40,15,1.0/3,atan((double)(it->x-t_py.x)/(it->y-t_py.y)) /3.1415926 );
            t_py = *it;
        }
    }
}

void COpenGLView::DrawQuad(float ff)
{
    glRotatef(m_rtri, 1.0f, 1.0f, 0.0f);                //绕Y轴旋转三角形
    glBegin(GL_QUADS);
    {
        glColor3f(0.0f, 1.0f, 0.0f);                    //颜色改为蓝色
        glVertex3f(ff, ff, -ff);                        //四边形的右上顶点 (顶面)
        glVertex3f(-ff, ff, -ff);                       //四边形的左上顶点 (顶面)
        glVertex3f(-ff, ff, ff);                        //四边形的左下顶点 (顶面)
        glVertex3f(ff, ff, ff);                         //四边形的右下顶点 (顶面)
        glColor3f(1.0f, 0.5f, 0.0f);                    //颜色改成橙色
        glVertex3f(ff, -ff, ff);                        //四边形的右上顶点(底面)
        glVertex3f(-ff, -ff, ff);                       //四边形的左上顶点(底面)
        glVertex3f(-ff, -ff, -ff);                      //四边形的左下顶点(底面)
        glVertex3f(ff, -ff, -ff);                       //四边形的右下顶点(底面)
        glColor3f(1.0f, 0.0f, 0.0f);                    //颜色改成红色
        glVertex3f(ff, ff, ff);                         //四边形的右上顶点(前面)
        glVertex3f(-ff, ff, ff);                        //四边形的左上顶点(前面)
        glVertex3f(-ff, -ff, ff);                       //四边形的左下顶点(前面)
        glVertex3f(ff, -ff, ff);                        //四边形的右下顶点(前面)
        glColor3f(1.0f, 1.0f, 0.0f);                    //颜色改成黄色
        glVertex3f(ff, -ff, -ff);                       //四边形的右上顶点(后面)
        glVertex3f(-ff, -ff, -ff);                      //四边形的左上顶点(后面)
        glVertex3f(-ff, ff, -ff);                       //四边形的左下顶点(后面)
        glVertex3f(ff, ff, -ff);                        //四边形的右下顶点(后面)
        glColor3f(0.0f, 0.0f, 1.0f);                    //颜色改成蓝色
        glVertex3f(-ff, ff, ff);                        //四边形的右上顶点(左面)
        glVertex3f(-ff, ff, -ff);                       //四边形的左上顶点(左面)
        glVertex3f(-ff, -ff, -ff);                      //四边形的左下顶点(左面)
        glVertex3f(-ff, -ff, ff);                       //四边形的右下顶点(左面)
        glColor3f(1.0f, 0.0f, 1.0f);                    //颜色改成紫罗兰色
        glVertex3f(ff, ff, -ff);                        //四边形的右上顶点(右面)
        glVertex3f(ff, ff, ff);                         //四边形的左上顶点(右面)
        glVertex3f(ff, -ff, ff);                        //四边形的左下顶点(右面)
        glVertex3f(ff, -ff, -ff);                       //四边形的右下顶点(右面)
    }
    glEnd();
    m_rtri += 1.0f;
}

void COpenGLView::DrawBMP(float ff)
{
    //创建背景---------------------------------------------------------
    //glBindTexture(GL_TEXTURE_2D, textBack[0]);
    //glBegin(GL_QUADS);
    //{
    //    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.8f, -2.0f,  1.0f);  // glTexCoord2f都是生成纹理
    //    glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.8f, -2.0f,  1.0f);
    //    glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.8f,  2.0f,  1.0f);
    //    glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.8f,  2.0f,  1.0f);
    //}
    //glEnd();
    //return ;
    //-----------------------------------------------------------------
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);
    // 使用来自位图数据生成 的典型纹理
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    {
        // 前侧面
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);    // glTexCoord2f都是生成纹理
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, ff);
        // 后侧面
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, -ff);
        // 顶面
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, ff, ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        // 底面
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, -ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);
        // Right face
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, -ff, -ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        // Left Face
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, ff, ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
    }
    glEnd();
    xrot += 0.6f * 2;
    yrot += 0.4f * 2;
    zrot += 0.8f * 2;
}

void COpenGLView::DrawLightBMP(float ff/*=1.0f*/)
{
    glTranslatef(0.0f, 0.0f, z);                        //平移
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);
    // 使用来自位图数据生成 的典型纹理
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    {
        // 前侧面
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);                       //glTexCoord2f都是生成纹理
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, ff);
        // 后侧面
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, -ff);
        // 顶面
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, ff, ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        // 底面
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, -ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);
        // Right face
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, -ff, -ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        // Left Face
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, ff, ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
    }
    glEnd();
    //xrot+=xspeed;
    //yrot+=yspeed;
    xrot += 0.6f * 2;
    yrot += 0.4f * 2;
    zrot += 0.8f * 2;
}

void COpenGLView::InitScene()
{
    if (!LoadGLTextures())
        return;
    Loadback();                                         //加载背景纹理
    glEnable(GL_TEXTURE_2D);                            //打开纹理
    glShadeModel(GL_SMOOTH);                            //启用阴影平滑
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               //黑色背景
    glClearDepth(1.0f);                                 //设置深度缓存
    glEnable(GL_DEPTH_TEST);                            //启用深度测试
    glDepthFunc(GL_LEQUAL);                             //所作深度测试的类型
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //告诉系统对透视进行修正
    return;
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //Position The Light
    glEnable(GL_LIGHT1);                                //Enable Light One
    glColor4f(1.0f, 1.0f, 1.0f, 0.5);                   //全亮度， 50% Alpha 混合
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);                  //基于源象素alpha通道值的半透明混合函数
}

void COpenGLView::RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存
    glLoadIdentity();                                   //重置当前的模型观察矩阵
    glTranslatef(0.0f, -0.0f, -5.0f);                   //左移 1.5 单位，并移入屏幕 6.0        //glTranslatef(0.0f,-10.4f,-26.0f);
    DrawLine();                                         //画线
    //DrawTriangle();
    //Draw3Triangle(1.0f);                              //画三角形
    //DrawQuad(0.1f);                                   //彩色正方体
    DrawLightBMP(1.0f);                                 //纹理
}

void COpenGLView::ChangeSize(GLsizei width, GLsizei height)
{
}

AUX_RGBImageRec* COpenGLView::LoadBMP(wchar_t* Filename)
{
    //暂时屏蔽
    FILE* File = NULL;                                  //File Handle
    if (!Filename)                                      //Make Sure A Filename Was Given
        return NULL;                                    //If Not Return NULL
    _wfopen_s(&File, Filename, L"r"/*_T("r")*/);        //Check To See If The File Exists
    if (File)                                           //Does The File Exist?
    {
        fclose(File);                                   //Close The Handle
        return auxDIBImageLoadW(Filename);              //Load The Bitmap And Return A Pointer
    }
    return NULL;                                        //If Load Failed Return NULL
}

int COpenGLView::LoadGLTextures()
{
    int Status = FALSE;                                 // 状态指示器
    AUX_RGBImageRec* TextureImage[1];                   // 创建纹理的存储空间
    memset(TextureImage, 0, sizeof(void*) * 1);         // Set The Pointer To NULL
    // 载入位图，检查有无错误，如果位图没找到则退出
    if (TextureImage[0] = LoadBMP(L"../Resource/Crate.bmp" /*_T("Crate.bmp")*/))
    {
        Status = TRUE;                                  // Set The Status To TRUE
        glGenTextures(1, &texture[0]);                  // 创建纹理
        // 使用来自位图数据生成 的典型纹理
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        // 生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // 线形滤波
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if (TextureImage[0])                                // 纹理是否存在
    {
        if (TextureImage[0]->data)                      // 纹理图像是否存在
            free(TextureImage[0]->data);                // 释放纹理图像占用的内存
        free(TextureImage[0]);                          // 释放图像结构
    }
    return Status;                                      // 返回 Status
}

//???????????????????????????????????????????????????????????????????????????????
int COpenGLView::Loadback()
{
    int Status = FALSE;                                 //状态指示器
    AUX_RGBImageRec* TextureImage[1];                   //创建纹理的存储空间
    memset(TextureImage, 0, sizeof(void*) * 1);         //Set The Pointer To NULL
    //载入位图，检查有无错误，如果位图没找到则退出
    if (TextureImage[0] = LoadBMP(L"../Resource/Crate.bmp"/*_T("Crate.bmp")*/))
    {
        Status = TRUE;                                  //Set The Status To TRUE
        glGenTextures(1, &textBack[0]);                 //创建纹理
        //使用来自位图数据生成 的典型纹理
        glBindTexture(GL_TEXTURE_2D, textBack[0]);
        //生成纹理
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    //线形滤波
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if (TextureImage[0])                                //纹理是否存在
    {
        if (TextureImage[0]->data)                      //纹理图像是否存在
            free(TextureImage[0]->data);                //释放纹理图像占用的内存
        free(TextureImage[0]);                          //释放图像结构
    }
    return Status;                                      //返回 Status
}

bool UpPackExeRes(char* f_strFileName, WORD f_wResID, char* f_strFileType)
{
    //if (!_access(f_strFileName,0))  return false;
    // 资源大小
    DWORD dwWrite = 0;
    // 创建文件
    HANDLE hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)    return FALSE;
    // 查找资源文件中、加载资源到内存、得到资源大小
    HRSRC hrsc = FindResourceA(NULL, MAKEINTRESOURCEA(f_wResID), f_strFileType);
    HGLOBAL hG = LoadResource(NULL, hrsc);
    DWORD   dwSize = SizeofResource(NULL, hrsc);
    // 写入文件
    WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
    CloseHandle(hFile);
    return TRUE;
}

AUX_RGBImageRec* COpenGLView::LoadRes(WORD f_id)
{
    AUX_RGBImageRec* ret = NULL;
    //从资源读取句柄
    BITMAP bmp;
    HBITMAP hBitmap = HBITMAP();
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    //计算参数
    int f_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    ret->sizeX = bmp.bmWidth;
    ret->sizeY = bmp.bmHeight;
    ret->data = new BYTE[ret->sizeX * ret->sizeY * f_nchannels];
    GetBitmapBits(hBitmap, ret->sizeX * ret->sizeY * f_nchannels, ret->data);
    return ret;
}









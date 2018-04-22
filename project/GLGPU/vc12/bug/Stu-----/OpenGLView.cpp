

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
    glRotatef(280.0f, 1.0f, 0.0f, 0.0f);                // ��Y����ת������
    glRotatef(30.0f, 0.0f, 0.0f, 1.0f);                 // ��ת����ǰ�������ǵĽǶ�
    glBegin(GL_TRIANGLES);                              // ����������
    {
        glColor3f(1.0f, 0.0f, 0.0f);                    //��
        glVertex3f(0.0f, ff + 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);                    //��
        glVertex3f(-ff, -ff + 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);                    //��
        glVertex3f(ff, -ff + 1.0f, 0.0f);
    }
    glEnd();
}

void COpenGLView::Draw3Triangle(float ff)
{
    glBindTexture(GL_TEXTURE_2D, textBack[0]);
    glRotatef(290.0f, 1.0f, 0.0f, 0.0f);                // ��Y����ת������
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
    //___________________________________________________________________
    float yy = 0.0f;
    glRotatef(280.0f, 1.0f, 0.0f, 0.0f);                // ��Y����ת������
    glRotatef(25.0f, 0.0f, 0.0f, 1.0f);                 // ��ת����ǰ�������ǵĽǶ�
    glBegin(GL_TRIANGLES);                              // ����������
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
    //�����εı߳�
    float Trianglel = 10.0f;
    float xl, yl, xr, yr;
    double ql, qr;                                      //���, �ҽ�
    //glPoint2f pt = glPoint2f(0.0f,1.0f);
    //��β������ �˴���Ҫ�ú�˼��
    pts.push_back(glPoint2f(pts.rbegin()->x, pts.rbegin()->y + 1));
    //���Ź켣���Ʋ���תͼ��
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
            //glRotatef(280.0f,1.0f,0.0f,0.0f);         //��Y����ת������
            glBegin(GL_TRIANGLES);                      //����������
            {
                ql = 0.1666667 - atan((double)(it->x - t_py.x) / (it->y - t_py.y));
                qr = 0.1666667 + atan((double)(it->x - t_py.x) / (it->y - t_py.y));
                xl = it->x - sin(ql) * Trianglel;
                yl = it->y - cos(ql) * Trianglel;
                xr = it->x - sin(qr) * Trianglel;
                yr = it->y - cos(qr) * Trianglel;
                glColor3f(0.0f, 1.0f, 0.0f);            //��ɫ
                glVertex3f(it->x, it->y, 0.0);          //��һ����
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
    glRotatef(m_rtri, 1.0f, 1.0f, 0.0f);                //��Y����ת������
    glBegin(GL_QUADS);
    {
        glColor3f(0.0f, 1.0f, 0.0f);                    //��ɫ��Ϊ��ɫ
        glVertex3f(ff, ff, -ff);                        //�ı��ε����϶��� (����)
        glVertex3f(-ff, ff, -ff);                       //�ı��ε����϶��� (����)
        glVertex3f(-ff, ff, ff);                        //�ı��ε����¶��� (����)
        glVertex3f(ff, ff, ff);                         //�ı��ε����¶��� (����)
        glColor3f(1.0f, 0.5f, 0.0f);                    //��ɫ�ĳɳ�ɫ
        glVertex3f(ff, -ff, ff);                        //�ı��ε����϶���(����)
        glVertex3f(-ff, -ff, ff);                       //�ı��ε����϶���(����)
        glVertex3f(-ff, -ff, -ff);                      //�ı��ε����¶���(����)
        glVertex3f(ff, -ff, -ff);                       //�ı��ε����¶���(����)
        glColor3f(1.0f, 0.0f, 0.0f);                    //��ɫ�ĳɺ�ɫ
        glVertex3f(ff, ff, ff);                         //�ı��ε����϶���(ǰ��)
        glVertex3f(-ff, ff, ff);                        //�ı��ε����϶���(ǰ��)
        glVertex3f(-ff, -ff, ff);                       //�ı��ε����¶���(ǰ��)
        glVertex3f(ff, -ff, ff);                        //�ı��ε����¶���(ǰ��)
        glColor3f(1.0f, 1.0f, 0.0f);                    //��ɫ�ĳɻ�ɫ
        glVertex3f(ff, -ff, -ff);                       //�ı��ε����϶���(����)
        glVertex3f(-ff, -ff, -ff);                      //�ı��ε����϶���(����)
        glVertex3f(-ff, ff, -ff);                       //�ı��ε����¶���(����)
        glVertex3f(ff, ff, -ff);                        //�ı��ε����¶���(����)
        glColor3f(0.0f, 0.0f, 1.0f);                    //��ɫ�ĳ���ɫ
        glVertex3f(-ff, ff, ff);                        //�ı��ε����϶���(����)
        glVertex3f(-ff, ff, -ff);                       //�ı��ε����϶���(����)
        glVertex3f(-ff, -ff, -ff);                      //�ı��ε����¶���(����)
        glVertex3f(-ff, -ff, ff);                       //�ı��ε����¶���(����)
        glColor3f(1.0f, 0.0f, 1.0f);                    //��ɫ�ĳ�������ɫ
        glVertex3f(ff, ff, -ff);                        //�ı��ε����϶���(����)
        glVertex3f(ff, ff, ff);                         //�ı��ε����϶���(����)
        glVertex3f(ff, -ff, ff);                        //�ı��ε����¶���(����)
        glVertex3f(ff, -ff, -ff);                       //�ı��ε����¶���(����)
    }
    glEnd();
    m_rtri += 1.0f;
}

void COpenGLView::DrawBMP(float ff)
{
    //��������---------------------------------------------------------
    //glBindTexture(GL_TEXTURE_2D, textBack[0]);
    //glBegin(GL_QUADS);
    //{
    //    glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.8f, -2.0f,  1.0f);  // glTexCoord2f������������
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
    // ʹ������λͼ�������� �ĵ�������
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    {
        // ǰ����
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);    // glTexCoord2f������������
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, ff);
        // �����
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, -ff);
        // ����
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, ff, ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        // ����
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
    glTranslatef(0.0f, 0.0f, z);                        //ƽ��
    glRotatef(xrot, 1.0f, 0.0f, 0.0f);
    glRotatef(yrot, 0.0f, 1.0f, 0.0f);
    glRotatef(zrot, 0.0f, 0.0f, 1.0f);
    // ʹ������λͼ�������� �ĵ�������
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    {
        // ǰ����
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, -ff, ff);                       //glTexCoord2f������������
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, -ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, ff);
        // �����
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(-ff, -ff, -ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(ff, -ff, -ff);
        // ����
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-ff, ff, -ff);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-ff, ff, ff);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(ff, ff, ff);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(ff, ff, -ff);
        // ����
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
    Loadback();                                         //���ر�������
    glEnable(GL_TEXTURE_2D);                            //������
    glShadeModel(GL_SMOOTH);                            //������Ӱƽ��
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);               //��ɫ����
    glClearDepth(1.0f);                                 //������Ȼ���
    glEnable(GL_DEPTH_TEST);                            //������Ȳ���
    glDepthFunc(GL_LEQUAL);                             //������Ȳ��Ե�����
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //����ϵͳ��͸�ӽ�������
    return;
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);     //Setup The Ambient Light
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);     //Setup The Diffuse Light
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);   //Position The Light
    glEnable(GL_LIGHT1);                                //Enable Light One
    glColor4f(1.0f, 1.0f, 1.0f, 0.5);                   //ȫ���ȣ� 50% Alpha ���
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);                  //����Դ����alphaͨ��ֵ�İ�͸����Ϻ���
}

void COpenGLView::RenderScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�����Ļ����Ȼ���
    glLoadIdentity();                                   //���õ�ǰ��ģ�͹۲����
    glTranslatef(0.0f, -0.0f, -5.0f);                   //���� 1.5 ��λ����������Ļ 6.0        //glTranslatef(0.0f,-10.4f,-26.0f);
    DrawLine();                                         //����
    //DrawTriangle();
    //Draw3Triangle(1.0f);                              //��������
    //DrawQuad(0.1f);                                   //��ɫ������
    DrawLightBMP(1.0f);                                 //����
}

void COpenGLView::ChangeSize(GLsizei width, GLsizei height)
{
}

AUX_RGBImageRec* COpenGLView::LoadBMP(wchar_t* Filename)
{
    //��ʱ����
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
    int Status = FALSE;                                 // ״ָ̬ʾ��
    AUX_RGBImageRec* TextureImage[1];                   // ��������Ĵ洢�ռ�
    memset(TextureImage, 0, sizeof(void*) * 1);         // Set The Pointer To NULL
    // ����λͼ��������޴������λͼû�ҵ����˳�
    if (TextureImage[0] = LoadBMP(L"../Resource/Crate.bmp" /*_T("Crate.bmp")*/))
    {
        Status = TRUE;                                  // Set The Status To TRUE
        glGenTextures(1, &texture[0]);                  // ��������
        // ʹ������λͼ�������� �ĵ�������
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        // ��������
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    // �����˲�
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if (TextureImage[0])                                // �����Ƿ����
    {
        if (TextureImage[0]->data)                      // ����ͼ���Ƿ����
            free(TextureImage[0]->data);                // �ͷ�����ͼ��ռ�õ��ڴ�
        free(TextureImage[0]);                          // �ͷ�ͼ��ṹ
    }
    return Status;                                      // ���� Status
}

//???????????????????????????????????????????????????????????????????????????????
int COpenGLView::Loadback()
{
    int Status = FALSE;                                 //״ָ̬ʾ��
    AUX_RGBImageRec* TextureImage[1];                   //��������Ĵ洢�ռ�
    memset(TextureImage, 0, sizeof(void*) * 1);         //Set The Pointer To NULL
    //����λͼ��������޴������λͼû�ҵ����˳�
    if (TextureImage[0] = LoadBMP(L"../Resource/Crate.bmp"/*_T("Crate.bmp")*/))
    {
        Status = TRUE;                                  //Set The Status To TRUE
        glGenTextures(1, &textBack[0]);                 //��������
        //ʹ������λͼ�������� �ĵ�������
        glBindTexture(GL_TEXTURE_2D, textBack[0]);
        //��������
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);    //�����˲�
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    if (TextureImage[0])                                //�����Ƿ����
    {
        if (TextureImage[0]->data)                      //����ͼ���Ƿ����
            free(TextureImage[0]->data);                //�ͷ�����ͼ��ռ�õ��ڴ�
        free(TextureImage[0]);                          //�ͷ�ͼ��ṹ
    }
    return Status;                                      //���� Status
}

bool UpPackExeRes(char* f_strFileName, WORD f_wResID, char* f_strFileType)
{
    //if (!_access(f_strFileName,0))  return false;
    // ��Դ��С
    DWORD dwWrite = 0;
    // �����ļ�
    HANDLE hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)    return FALSE;
    // ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
    HRSRC hrsc = FindResourceA(NULL, MAKEINTRESOURCEA(f_wResID), f_strFileType);
    HGLOBAL hG = LoadResource(NULL, hrsc);
    DWORD   dwSize = SizeofResource(NULL, hrsc);
    // д���ļ�
    WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
    CloseHandle(hFile);
    return TRUE;
}

AUX_RGBImageRec* COpenGLView::LoadRes(WORD f_id)
{
    AUX_RGBImageRec* ret = NULL;
    //����Դ��ȡ���
    BITMAP bmp;
    HBITMAP hBitmap = HBITMAP();
    GetObject(hBitmap, sizeof(BITMAP), &bmp);
    //�������
    int f_nchannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
    ret->sizeX = bmp.bmWidth;
    ret->sizeY = bmp.bmHeight;
    ret->data = new BYTE[ret->sizeX * ret->sizeY * f_nchannels];
    GetBitmapBits(hBitmap, ret->sizeX * ret->sizeY * f_nchannels, ret->data);
    return ret;
}









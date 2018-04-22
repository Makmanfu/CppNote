

#include "stdafx.h"
//#include "SQ/glew.h"
#include "GLView.h"



GLFram::GLFram(void) : hDC(NULL)
    , hRC(NULL)
{
}

GLFram::GLFram(HDC _hDC) : GLFram()
{
    SetDC(_hDC);
}

GLFram::~GLFram(void)
{
}

void GLFram::SetDC(HDC _hDC)
{
    hDC = _hDC;
}

void GLFram::GLInit(void)
{
    InitDC();
    InitOpenGL();
}

void GLFram::GLFree(void)
{
}

void GLFram::RenderScene(void)
{
}

bool GLFram::InitOpenGL(void)
{
    return true;
}

void GLFram::ChangeSize(GLsizei width, GLsizei height)
{
}

void GLFram::SetDCPixelFormat(HDC hDC)
{
    static PIXELFORMATDESCRIPTOR pfd =          // 告诉窗口我们所希望的东东，即窗口使用的像素格式
    {
        sizeof(PIXELFORMATDESCRIPTOR),          // 上述格式描述符的大小
        1,                                      // 版本号
        PFD_DRAW_TO_WINDOW |                    // 格式支持窗口
        PFD_SUPPORT_OPENGL |                    // 格式必须支持OpenGL
        PFD_DOUBLEBUFFER,                       // 必须支持双缓冲
        PFD_TYPE_RGBA,                          // 申请 RGBA 格式
        32,                                     // 选定色彩深度
        0, 0, 0, 0, 0, 0,                       // 忽略的色彩位
        0,                                      // 无Alpha缓存
        0,                                      // 忽略Shift Bit
        0,                                      // 无累加缓存
        0, 0, 0, 0,                             // 忽略聚集位
        16,                                     // 16位 Z-缓存 (深度缓存)
        0,                                      // 无蒙板缓存
        0,                                      // 无辅助缓存
        PFD_MAIN_PLANE,                         // 主绘图层
        0,                                      // Reserved
        0, 0, 0                                 // 忽略层遮罩
    };
    // Choose a pixel format that best matches that described in pfd
    int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
    // Set the pixel format for the device context
    SetPixelFormat(hDC, nPixelFormat, &pfd);
}

void GLFram::InitDC()
{
    if (hDC != NULL)
    {
        SetDCPixelFormat(hDC);
        hRC = wglCreateContext(hDC);
    }
    if (hRC != NULL)
        wglMakeCurrent(hDC, hRC);
}


bool GLView::InitOpenGL(void)
{
    // 启用阴影平滑
    glShadeModel(GL_SMOOTH);
    // 背景色
    glClearColor(0.0f, 1.8f, 0.0f, 0.0f);
    // 设置深度缓存
    glClearDepth(1.0f);
    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 所作深度测试的类型
    glDepthFunc(GL_LEQUAL);
    // 告诉系统对透视进行修正
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    return true;
}

void GLView::RenderScene(void)
{
    //清除屏幕和深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //重置当前的模型观察矩阵
    //glLoadIdentity();
    //画三角形
    mDraw.DrawTriangle();
}

void GLView::ChangeSize(GLsizei width, GLsizei height)
{
    // 防止被零除将Height设为1
    if (height == 0)
        height = 1;
    // 重置当前的视口
    glViewport(0, 0, width, height);
    // 选择投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 重置投影矩阵
    glLoadIdentity();
    // 设置视口的大小
    //gluPerspective ( 45.0f, ( GLfloat ) width / ( GLfloat ) height, 0.1f, 100.0f );
    // 选择模型观察矩阵
    glMatrixMode(GL_MODELVIEW);
    // 重置模型观察矩阵
    glLoadIdentity();
}






/*

GLuint C3DTGL::ddsTexture(std::string path)
{
    FILE* fp = fopen(path.c_str(), "rb");
    if (!fp)
        return 0;
    fseek(fp, 0, SEEK_END);
    int length = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* ref = new char[length];
    fread(ref, length, 1, fp);
    fclose(fp);
    //char* origin = ref ;
    GLuint tex = 0;
    std::string ext;//// = boost::filesystem::extension(path);
    if (ext == ".dds")
    {
        std::string sss;
        if (!snappy::Uncompress(ref, length, &sss))
            return 0;
        delete[]ref;
        ref = &sss[0];
        int width = *(int*)ref;
        ref += sizeof(int);
        int height = *(int*)ref;
        ref += sizeof(int);
        glEnable(GL_TEXTURE_2D);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        //????
        ////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        ////glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //glTexParameterf(GL_TEXTURE_2D ,GL_TEXTURE_MAX_ANISOTROPY_EXT ,16.0f) ;
        int texture_buf_size = ((width + 3) / 4) * ((height + 3) / 4) * 8;
        //int texture_buf_size = width * height / 2; //length - 8 ;//
        //int texture_buf_size = length - 8 ;
        ////glCompressedTexImage2DARB(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
        ////    width, height, 0, texture_buf_size, ref);
        //      gluBuild2DMipmaps(GL_TEXTURE_2D , GL_RGB ,
        //          width ,height ,GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ,GL_UNSIGNED_BYTE ,ref) ;
        //      delete []ref ;
        sss.clear();
        sss.resize(0);
        sss.shrink_to_fit();
        //delete []origin;
    }
    return tex;
}

void C3DTGL::Load3DTModel(std::string path)
{
    if (path.size() > 1)
    {
        //m_X3DEnv->makeCurrent();
        int pos = path.find_last_of("\\");
        std::string dir = path.substr(0, pos + 1);
        FILE* fp = fopen(path.c_str(), "rb");
        if (!fp)
            return;
        fseek(fp, 0, SEEK_END);
        int length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        char* strfileText = new char[length];
        fread(strfileText, 1, length, fp);
        fclose(fp);
        std::string ss;
        ss.resize(length);
        memcpy(&ss[0], strfileText, length);
        delete[]strfileText;
        strfileText = 0;
        lib3dt::x3dt file3dt;
        lib3dt::ss_to_3dt()(file3dt, ss);
        float boxMax[3] = { -1e30, -1e30, -1e30 };
        float boxMin[3] = { 1e30, 1e30, 1e30 };
        boxMin[0] = file3dt.bbox_[0];
        boxMin[1] = file3dt.bbox_[1];
        boxMin[2] = file3dt.bbox_[2];
        boxMax[0] = file3dt.bbox_[3];
        boxMax[1] = file3dt.bbox_[4];
        boxMax[2] = file3dt.bbox_[5];
        //m_Nodes
        int pointnum = file3dt.vertex_.size();
        //unsigned char* ref = model.GetPointsBuffer(&pointnum) ;
        unsigned char* ref = (unsigned char*)(&file3dt.vertex_[0].x_);
        Node3DT temp;
        temp.strName_ = "bug";////boost::filesystem::basename(path);
        //GL操作
        ////glGenBuffers(1, &temp.iPoint_);
        ////glBindBuffer(GL_ARRAY_BUFFER, temp.iPoint_);
        ////glBufferData(GL_ARRAY_BUFFER, pointnum * sizeof(lib3dt::vertex), ref, GL_STATIC_DRAW);
        ////glBindBuffer(GL_ARRAY_BUFFER, 0);
        for (int i = 0; i <file3dt.indices_.size(); i++)        //*model.GetNodesCount()*
        {
            GLuint indicesIds = 0;
            lib3dt::indices node = file3dt.indices_[i];
            int indicesnum = node.indexPt.size();
            unsigned char* refIndices = (unsigned char*)(&node.indexPt[0]);
            ////glGenBuffers(1, &indicesIds);
            ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesIds);
            ////glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesnum * sizeof(int), refIndices, GL_STATIC_DRAW);
            ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            unsigned char tex[64];
            memcpy(tex, node.texture_name_, 64);
            std::string ddsPath = dir;
            ddsPath += (char*)tex;
            GLuint texIds = 0;
            texIds = ddsTexture(ddsPath);
            Indices3DT tempIndices;
            tempIndices.iIndices_ = indicesIds;
            tempIndices.iTexture_ = texIds;
            tempIndices.use_blend = node.use_blend_;
            tempIndices.use_repeat = node.repeat_or_clamp;
            memcpy(tempIndices.ambient_, node.ambient_, 3 * sizeof(float));
            memcpy(tempIndices.diffuse_, node.diffuse_, 3 * sizeof(float));
            memcpy(tempIndices.specular_, node.specular_, 3 * sizeof(float));
            temp.iIndices.push_back(tempIndices);
        }
        m_Nodes.push_back(temp);
        ////m_bIsDrawing = true;
    }
}

bool C3DTGL::DrawNode(Node3DT node, int mode)
{
   
        if (!node.iPoint_ || !glIsBuffer(node.iPoint_))
        return (0);
        GLfloat ambient[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
        GLfloat lmodel_ambient[] = { 0.4, 0.4, 0.4, 1.0 };
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
        glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
        //glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glPushMatrix();
        if (mode == GL_SELECT)
        glPushName(node.iPoint_);
        if (m_pickedModel == node.iPoint_)
        {
        glUseProgram(shadowShaderId);
        glUniform1i(textureUniform, 0);
        }
        if (glIsBuffer(node.iPoint_))
        {
        glBindBuffer(GL_ARRAY_BUFFER, node.iPoint_);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(lib3dt::vertex), BUFFER_OFFSET(0));
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(2, GL_FLOAT, sizeof(lib3dt::vertex), BUFFER_OFFSET(12));
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_FLOAT, sizeof(lib3dt::vertex), BUFFER_OFFSET(20));
        glColor4f(149 / 255.0, 150 / 255.0, 163 / 255.0, 1.0);
        }
        for (int i = 0; i < node.iIndices.size(); i++)
        {
        Indices3DT Indices_Texture = node.iIndices[i];
        if (Indices_Texture.iTexture_
            && glIsTexture(Indices_Texture.iTexture_))
        {
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, Indices_Texture.iTexture_);
            GLenum tex_mode = GL_CLAMP_TO_EDGE;
            if (Indices_Texture.use_repeat)
                tex_mode = GL_REPEAT;
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            //          glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            //          glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_BASE_LEVEL ,0 ) ;
            //          glTexParameteri(GL_TEXTURE_2D , GL_TEXTURE_MAX_LEVEL ,9) ;
            float shinning[] = { 32.0 };
            float ambient[] = { 1.0, 1.0, 1.0, 1.0 };
            float diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
            float specular[] = { 0.1, 0.1, 0.1, 1.0 };
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &ambient[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &diffuse[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &specular[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shinning);
            if (Indices_Texture.use_blend)
            {
                //                  glEnable(GL_ALPHA_TEST) ;
                //                  glAlphaFunc(GL_GREATER ,.01f) ;
                //                  glEnable(GL_BLEND) ;
            }
        }
        else
        {
            glEnable(GL_NORMALIZE); //Automatically normalize normals
            float shinning[] = { 32.0 };
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &Indices_Texture.ambient_[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &Indices_Texture.diffuse_[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &Indices_Texture.specular_[0]);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shinning);
        }
        if (glIsBuffer(Indices_Texture.iIndices_))
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Indices_Texture.iIndices_);
            GLint indice_buf_size = 0;
            glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &indice_buf_size);
            Rq_restart_index index(0xffffffff);
            glDrawElements(
                GL_TRIANGLE_STRIP,
                (indice_buf_size / sizeof(int)),
                GL_UNSIGNED_INT,
                BUFFER_OFFSET(0));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        if (Indices_Texture.iTexture_)
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_TEXTURE_2D);
            if (Indices_Texture.use_blend)
            {
                //              glDisable(GL_ALPHA_TEST) ;
                //              glDisable(GL_BLEND) ;
            }
        }
        else
        {
            glDisable(GL_NORMALIZE); //Automatically normalize normals
        }
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        if (mode == GL_SELECT)
        glPopName();
        if (m_pickedModel == node.iPoint_)
        glUseProgram(0);
        glPopMatrix();
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHTING);
 
    return true;
}

*/

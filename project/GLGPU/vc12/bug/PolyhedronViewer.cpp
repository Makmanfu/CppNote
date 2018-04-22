
#include "stdafx.h"
#include "PolyhedronViewer.h"

extern "C" {
#include "glew.c"
}

Polyhedron_Viewer::Polyhedron_Viewer() : mHDC(NULL)
    , mHGLRC(NULL)
    , mSelected(-1)
    , mTextureUnitCount(0)
{
    mViewport = std::shared_ptr<Viewport>(new Viewport);
    mViewport->setClearColor(0.344f, 0.55f, 0.81f, 1.0f);
    mCamera = std::shared_ptr<Camera>(new Camera);
    mCamera->setViewport(mViewport);
    //texture_collection_ = new Polyhedron_texture_collection;
    //texture_collection_->clear();
    //texture_collection_->resize(0);
}

Polyhedron_Viewer::~Polyhedron_Viewer()
{
    //quitApplication();
}

bool Polyhedron_Viewer::init()
{
    mHDC = ::GetDC(hwnd());
    if (!mHDC)
        return false;
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
        1,                              // version number
        PFD_DRAW_TO_WINDOW |            // support window
        PFD_SUPPORT_OPENGL |            // support OpenGL
        PFD_DOUBLEBUFFER ,              // double buffered
        PFD_TYPE_RGBA,                  // RGBA type
        24,                             // 32-bit color depth
        0, 0, 0, 0, 0, 0,               // color bits ignored
        0,                              // no alpha buffer
        0,                              // shift bit ignored
        0,                              // no accumulation buffer
        0, 0, 0, 0,                     // accum bits ignored
        32,                             // 32-bit z-buffer
        24,                             // stencil buffer
        0,                              // no auxiliary buffer
        PFD_MAIN_PLANE,                 // main layer
        0,                              // reserved
        0, 0, 0                         // layer masks ignored
    };
    int pixel_format_index = ChoosePixelFormat(mHDC, &pfd);
    // if this is not supported we use the current 'pixel_format_index' returned by ChoosePixelFormat above.
    if (WGLEW_ARB_pixel_format)
    {
        float fAttributes[] = { 0, 0 };
        int iAttributes[] =
        {
            // multi sampling
            WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
            WGL_SAMPLES_ARB,        -1, // this is set below
            // generic
            WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
            WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
            // color buffer
            WGL_RED_BITS_ARB,         pfd.cRedBits,
            WGL_GREEN_BITS_ARB,       pfd.cGreenBits,
            WGL_BLUE_BITS_ARB,        pfd.cBlueBits,
            WGL_ALPHA_BITS_ARB,       pfd.cAlphaBits,
            // accumulation buffer
            WGL_ACCUM_RED_BITS_ARB,   pfd.cAccumRedBits,
            WGL_ACCUM_GREEN_BITS_ARB, pfd.cAccumGreenBits,
            WGL_ACCUM_BLUE_BITS_ARB,  pfd.cAccumBlueBits,
            WGL_ACCUM_ALPHA_BITS_ARB, pfd.cAccumAlphaBits,
            // depth buffer
            WGL_DEPTH_BITS_ARB,       pfd.cDepthBits,
            WGL_DOUBLE_BUFFER_ARB,    GL_TRUE,
            // stencil buffer
            WGL_STENCIL_BITS_ARB,     pfd.cStencilBits,
            // stereo
            WGL_STEREO_ARB,           GL_FALSE,
            0, 0
        };
        const int multisamples = 4;
        for (int samples = multisamples; samples > 1; samples /= 2)
        {
            // sets WGL_SAMPLES_ARB value
            iAttributes[3] = samples;
            pixel_format_index = -1;
            UINT num_formats = 0;
            if (wglChoosePixelFormatARB(mHDC, iAttributes, fAttributes, 1, &pixel_format_index, &num_formats) && num_formats >= 1)
                break;
            else
                pixel_format_index = -1;
        }
    }
    if (pixel_format_index == -1)
        return false;
    if (SetPixelFormat(mHDC, pixel_format_index, NULL) == FALSE)
        return false;
    // OpenGL Rendering Context
    mHGLRC = wglCreateContext(mHDC);
    if (!mHGLRC)
        return false;
    wglMakeCurrent(mHDC, mHGLRC);
    initExtensions();
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    //if (fmt.multisample() && !WGLEW_ARB_multisample)
    //  return false;
    //if (WGLEW_EXT_swap_control)
    //  wglSwapIntervalEXT( fmt.vSync() ? 1 : 0 );
    //ReleaseDC(hwnd(),mHDC);
    return true;
}

void Polyhedron_Viewer::adjustView(vec3 const& min_corner, vec3 const& max_corner)
{
    AABB aabb;
    aabb.setMaxCorner(max_corner);
    aabb.setMinCorner(min_corner);
    mCamera->setSceneBoundingSphere(aabb);
    mCamera->adjustView(aabb, vec3(0, 0, 1), vec3(0, 1, 0), 1.0f);
}

int Polyhedron_Viewer::select(int sx, int sy)
{
    this->makeCurrent();
    int sz = collection_.size() * 5;
    GLuint* selectBuf = new GLuint[sz];
    glSelectBuffer(sz, selectBuf);
    glRenderMode(GL_SELECT);
    glInitNames();
    /* Because LoadName() won't work with no names on the stack */
    glPushName(-1);
    double mp[16];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    glPushAttrib(GL_TRANSFORM_BIT);
    glMatrixMode(GL_PROJECTION);
    glGetDoublev(GL_PROJECTION_MATRIX, mp);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(sx, viewport[3] - sy, 4, 4, viewport);
    glMultMatrixd(mp);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    for (int k = 0; k < collection_.size(); k++)
    {
        Polyhedron_ptr polyhedron = collection_[k];
        //glPushName(k + 1);
        glLoadName(k + 1);
        for (int i = 0; i < polyhedron->facet_ds().size(); i++)
        {
            P_triangle_ptr tri = polyhedron->facet(i);
            P_vertex_ptr v0 = polyhedron->vertex(tri->i0_);
            P_vertex_ptr v1 = polyhedron->vertex(tri->i1_);
            P_vertex_ptr v2 = polyhedron->vertex(tri->i2_);
            glBegin(GL_TRIANGLES);
            glVertex3d(v0->position_.x(), v0->position_.y(), v0->position_.z());
            glVertex3d(v1->position_.x(), v1->position_.y(), v1->position_.z());
            glVertex3d(v2->position_.x(), v2->position_.y(), v2->position_.z());
            glEnd();
        }
        //glPopName();
    }
    long hits;
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    hits = glRenderMode(GL_RENDER);
    glPopAttrib();
    std::vector< std::pair<double, unsigned int> > H;
    for (long ii = 0; ii < hits; ii++)
        H.push_back(std::pair<double, unsigned int>(selectBuf[ii * 4 + 1] / 4294967295.0, selectBuf[ii * 4 + 3]));
    std::sort(H.begin(), H.end());
    delete[] selectBuf;
    if (hits == 0)
    {
        mSelected = -1;
        return -1;
    }
    mSelected = H.front().second - 1;
    return mSelected;
}

void Polyhedron_Viewer::quitApplication()
{
    wglDeleteContext(mHGLRC);
    mHGLRC = 0;
    ReleaseDC(hwnd(), mHDC);
    mHDC = 0;
    PostQuitMessage(0);
}

void Polyhedron_Viewer::shareOpenGLResources(HGLRC hGLRC)
{
    if (hwnd() && mHDC && mHGLRC)
        wglShareLists(hglrc(), hGLRC);
}

void Polyhedron_Viewer::initExtensions()
{
    makeCurrent();
    // init glew for each rendering context
    GLenum err = glewInit();
    if (GLEW_OK != err)
        fprintf(stderr, "Error calling glewInit(): %s\n", glewGetErrorString(err));
    // clears eventual OpenGL errors generated by GLEW
    glGetError();
    mTextureUnitCount = 1;
    if (GLEW_ARB_multitexture || GLEW_VERSION_1_3)
        glGetIntegerv(GL_MAX_TEXTURE_UNITS, &mTextureUnitCount);
#define MAX_TEXTURE_UNIT_COUNT 16
    mTextureUnitCount = mTextureUnitCount < MAX_TEXTURE_UNIT_COUNT ? mTextureUnitCount : MAX_TEXTURE_UNIT_COUNT;
}

bool Polyhedron_Viewer::isExtensionSupported(const char* ext_name)
{
    makeCurrent();
    size_t len = strlen(ext_name);
    const char* ext = (const char*)glGetString(GL_EXTENSIONS);
    const char* ext_end = ext + strlen(ext);
    for (const char* pos = strstr(ext, ext_name); pos && pos < ext_end; pos = strstr(pos, ext_name))
    {
        if (pos[len] == ' ' || pos[len] == 0)
            return true;
        else
            pos += len;
    }
    return false;
}

void* Polyhedron_Viewer::getProcAddress(const char* function_name)
{
    makeCurrent();
    return (void*)glewGetProcAddress((const unsigned char*)function_name);
}

void Polyhedron_Viewer::setVSyncEnabled(bool enable)
{
    makeCurrent();
    if (WGLEW_EXT_swap_control)
        wglSwapIntervalEXT(enable ? 1 : 0);
}

bool Polyhedron_Viewer::vsyncEnabled() const
{
    if (WGLEW_EXT_swap_control)
        return wglGetSwapIntervalEXT() != 0;
    else
        return false;
}

void Polyhedron_Viewer::update()
{
    if (hwnd())
        PostMessage(hwnd(), WM_PAINT, 0, 0);
}

void Polyhedron_Viewer::swapBuffers()
{
    if (hwnd() && hdc())
        SwapBuffers(hdc());
}

void Polyhedron_Viewer::makeCurrent()
{
    if (hwnd() && mHDC && mHGLRC)
        wglMakeCurrent(mHDC, mHGLRC);
}



HDC Polyhedron_Viewer::hdc() const
{
    return mHDC;
}

HGLRC Polyhedron_Viewer::hglrc() const
{
    return mHGLRC;
}

std::shared_ptr<Camera> Polyhedron_Viewer::camera()
{
    return mCamera;
}

std::shared_ptr<Viewport> Polyhedron_Viewer::viewport()
{
    return mViewport;
}

Polyhedron_collection& Polyhedron_Viewer::collection()
{
    return collection_;
}

Polyhedron_texture_collection& Polyhedron_Viewer::texture_collection()
{
    return texture_collection_;
}

int Polyhedron_Viewer::textureUnitCount() const
{
    return mTextureUnitCount;
}

int Polyhedron_Viewer::selected()const
{
    return mSelected;
}


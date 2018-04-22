
#pragma once

#include "Camera.h"
#include "Polyhedron.hpp"
#include "glew.h"

class Polyhedron_Viewer
{
private:
    int mTextureUnitCount;
    int mSelected;
    HDC mHDC;
    HGLRC mHGLRC;
    Polyhedron_collection collection_;
    Polyhedron_texture_collection texture_collection_;
    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<Viewport> mViewport;
public:
    Polyhedron_Viewer();
    virtual ~Polyhedron_Viewer();
public:
    virtual HWND hwnd() const = 0;
    //Initializes the supported OpenGL
    virtual bool init();
    //
    virtual void adjustView(vec3 const& min_corner, vec3 const& max_corner);
    //µã»÷view´¥·¢
    virtual int select(int sx, int sy);
    //! Asks to the windowing system that is managing the GLArea to quit the application.
    virtual void quitApplication();

    //! Use this function when you want two OpenGL contexts to share their resources (display lists, textures, shader objects, buffer objects etc.)
    //! Equivalent to wglShareLists(this->hglrc(), hGLRC)
    //! \remarks
    //! If you want to share resources among two or more OpenGL contexts, you must call this function
    //! before you start creating any resources.
    virtual void shareOpenGLResources(HGLRC hGLRC);
    //! Initializes the supported OpenGL extensions.
    virtual void initExtensions();
    //! Returns true if the given extension is supported.
    //! \note This is a relatively slow function, don't use it inside loops and similar.
    virtual bool isExtensionSupported(const char* ext_name);
    //! Returns the address of an OpenGL extension function
    virtual void* getProcAddress(const char* function_name);
    virtual void setVSyncEnabled(bool enable);
    virtual bool vsyncEnabled() const;
    virtual void update();
    virtual void swapBuffers();
    virtual void makeCurrent();

public:
    HDC hdc() const;
    HGLRC hglrc() const;
    std::shared_ptr<Camera> camera();
    std::shared_ptr<Viewport> viewport();
    Polyhedron_collection& collection();
    Polyhedron_texture_collection& texture_collection();
    // !this value is clamped to VL_MAX_TEXTURE_UNIT_COUNT
    int textureUnitCount() const;
    int selected() const;
};



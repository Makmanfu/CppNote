#ifndef RSMZ_RENDERER_H
#define RSMZ_RENDERER_H

#include <GL/gl.h>
#include "Camera.h"

namespace rsmz
{
    //GL���Ƴ�����
    class Renderer
    {
    public:
        Renderer();
        virtual ~Renderer();
        //GL���ƽӿ�
        virtual void render(void) = 0;

        void setCamera(Camera* c);
        const Camera* getCamera(void);
    protected:
        Camera* mCamera;
    };

}

#endif





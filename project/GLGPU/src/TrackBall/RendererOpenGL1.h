#ifndef RENDEREROPENGL1_H
#define RENDEREROPENGL1_H

#include "Renderer.h"
#include <GL/gl.h>

namespace rsmz
{
    //GL��ģ�ͻ���...
    class RendererOpenGL1 : public Renderer
    {
    public:
        RendererOpenGL1();
        virtual ~RendererOpenGL1();
    public:
        void init(void);
        //���Ƴ������ж���
        void renderObjects(void);
        virtual void render(void);
        void resize(int width, int height);
    private:
        //����������ο���
        void renderAxes(void);
        //�������ĵ�
        void renderCenter(void);
        //�������ĵ㶥��������
        void renderCube(void);
        //��������Ϊ���ĵ�������
        void renderCubeCenter(void);
        //��������
        void renderGEO(void);
        //���Ƹ�������
        void renderGrid(void);
        void renderClear(void);
    protected:
        //SIZE�����õ�����
        glm::mat4 mProjection;
    };
}

#endif





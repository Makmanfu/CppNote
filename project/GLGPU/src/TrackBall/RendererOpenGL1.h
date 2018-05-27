#ifndef RENDEREROPENGL1_H
#define RENDEREROPENGL1_H

#include "Renderer.h"
#include <GL/gl.h>

namespace rsmz
{
    //GL的模型绘制...
    class RendererOpenGL1 : public Renderer
    {
    public:
        RendererOpenGL1();
        virtual ~RendererOpenGL1();
    public:
        void init(void);
        //绘制场景所有对象
        void renderObjects(void);
        virtual void render(void);
        void resize(int width, int height);
    private:
        //绘制坐标轴参考线
        void renderAxes(void);
        //绘制球心点
        void renderCenter(void);
        //绘制球心点顶点正方体
        void renderCube(void);
        //绘制球心为中心点正方体
        void renderCubeCenter(void);
        //绘制球体
        void renderGEO(void);
        //绘制辅助网格
        void renderGrid(void);
        void renderClear(void);
    protected:
        //SIZE缩放用到矩阵
        glm::mat4 mProjection;
    };
}

#endif





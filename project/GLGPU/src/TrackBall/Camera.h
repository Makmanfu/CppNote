#ifndef CAMERA_H
#define CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace rsmz
{
    //相机类(转动视觉画面就靠这个了)
    class Camera
    {
    public:
        Camera();
        ~Camera();
    public:
        const glm::mat4& getMatrix(void);
        const float* getMatrixFlat(void);
        const glm::vec3& getCenter(void);
        const glm::vec3& getEye(void);
        const glm::vec3& getUp(void);
        //重置相机位置
        void reset();
        //设置相机转动的中心球心点
        void setCenter(float x, float y, float z);
        void setCenter(const glm::vec3& c);
        //设置相机位置视角点
        void setEye(float x, float y, float z);
        void setEye(const glm::vec3& e);
        //设置世界坐标中的方向
        void setUp(float x, float y, float z);
        void setUp(const glm::vec3& u);
        //刷新相机视图
        void update(void);
    private:
        glm::vec3 mCenter;          //中心点(相机镜头对准的物体在世界坐标的位置)
        glm::vec3 mEye;             //视角点(相机在世界坐标的位置)
        glm::mat4 mMatrix;          //视图矩阵
        glm::vec3 mUp;              //世界坐标中的方向
    };
}

#endif



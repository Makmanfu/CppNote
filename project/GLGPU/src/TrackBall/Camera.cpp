
#include "stdafx.h"
#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp> // lookAt
#include <glm/gtc/type_ptr.hpp> // value_ptr

namespace rsmz
{
    Camera::Camera()
    {
        reset();
    }

    Camera::~Camera()
    {
    }

    const glm::vec3& Camera::getCenter(void)
    {
        return mCenter;
    }

    const glm::vec3& Camera::getEye(void)
    {
        return mEye;
    }

    const glm::mat4& Camera::getMatrix(void)
    {
        return mMatrix;
    }

    const float* Camera::getMatrixFlat(void)
    {
        return glm::value_ptr(mMatrix);
    }

    const glm::vec3& Camera::getUp(void)
    {
        return mUp;
    }

    void Camera::reset(void)
    {
        mEye.x = 0.f;
        mEye.y = 0.f;
        mEye.z = 1.f;
        mCenter.x = 0.f;
        mCenter.y = 0.f;
        mCenter.z = 0.f;
        mUp.x = 0.f;
        mUp.y = 1.f;
        mUp.z = 0.f;
        update();
    }

    void Camera::setEye(float x, float y, float z)
    {
        mEye.x = x;
        mEye.y = y;
        mEye.z = z;
    }

    void Camera::setEye(const glm::vec3& e)
    {
        mEye = e;
    }

    void Camera::setCenter(float x, float y, float z)
    {
        mCenter.x = x;
        mCenter.y = y;
        mCenter.z = z;
    }

    void Camera::setCenter(const glm::vec3& c)
    {
        mCenter = c;
    }

    void Camera::setUp(float x, float y, float z)
    {
        mUp.x = x;
        mUp.y = y;
        mUp.z = z;
    }

    void Camera::setUp(const glm::vec3& u)
    {
        mUp = u;
    }

    void Camera::update(void)
    {
        //lookAt 移动照相机（使用试图变换）来观察这个立方体
        mMatrix = glm::lookAt(mEye, mCenter, mUp);
    }

}




#ifndef CAMERA4_H
#define CAMERA4_H

#include <memory>
#include "ViewPort.h"

//代表定义了视点的虚拟相机从中可以呈现场景
class Camera
{
protected:
    mat4 mViewMatrix;                           //视图矩阵
    mat4 mInverseViewMatrix;                    //反向视图矩阵
    mat4 mProjectionMatrix;                     //投影矩阵 (透视)
    Frustum mFrustum;                           //圆锥体
    std::shared_ptr<Viewport> mViewport;        //GL视口
    Real mFOV;                                  //??????
    Real mNearPlane;                            //近平面
    Real mFarPlane;                             //远平面
    bool mNearFarClippingPlanesOptimized;       //远近快速平面优化
private:
    Sphere mSceneBoundingSphere;                //球
public:
    Camera();
private:
    //提取平面算法 后面分析
    void extractPlanes(Plane* planes, const mat4& modelviewproj);
public:
    //根据初始化GL_PROJECTION和GL_MODELVIEW矩阵相机的投影和视图矩阵并计算平截头平面
    void activate(void);
    //view的宽高比
    Real aspectRatio() const;
    //在世界空间中计算相机的平截头体平面操作
    void computeFrustumPlanes();
    //通过图锥截体计算投影矩阵 (给定的平截头体构建相机的透视投影矩阵)
    void setProjectionAsFrustum(Real left, Real right, Real bottom, Real top, Real near, Real far);
    //通过透视变换计算投影矩阵 (基于相机和视口的设置构建相机的透视投影矩阵)
    void setProjectionAsPerspective();
    void setProjectionAsPerspective(Real fov, Real near, Real far);
    //计算变换要的视图矩阵
    void setViewMatrixAsLookAt(const vec3& eye, const vec3& center, const vec3& up);
    void getViewMatrixAsLookAt(vec3& eye, vec3& look, vec3& up, vec3& right) const;
    //设置正交投影(基于相机的近/远平面和其视口的设置为相机构建正交投影矩阵)
    void setProjectionAsOrtho(Real offset = -0.5f);
    //设置正交投影(基于像机的视口设置为摄像机构建正交投影矩阵)
    void setProjectionAsOrtho2D(Real offset = -0.5f);
    //应用视图矩阵到cam(加载GL_MODELVIEW 将视图矩阵设置到相机中)
    void applyViewMatrix() const;
    //应用模型视图矩阵(加载GL_MODELVIEW 视图矩阵乘以指定的模型矩阵 使用到相机中)
    void applyModelViewMatrix(const mat4& model_matrix) const;
    //????计算映射一个向量
    bool project(const vec4& in, vec4& out) const;
    //从窗口坐标到世界坐标的向量
    bool unproject(const vec3& win, vec4& out) const;
    //Unprojects a set of points. See unproject(const vec3& win, vec4& out) for more information.
    bool unproject(std::vector<vec3>& points) const;
    //调整相机位置，以便可以正确查看给定的vbox
    void adjustView(const VBOX& vbox, const vec3& dir, const vec3& up, Real bias = 1.0f);
public:
    //相机的参考框架矩阵，视图矩阵的倒数。(The camera's reference frame matrix, the inverse of the view matrix.)
    void setInverseViewMatrix(const mat4& mat);
    const mat4& inverseViewMatrix() const;

    //Sets the Camera's view matrix.
    void setViewMatrix(const mat4& mat);
    //The Camera's view matrix. This is what you would pass to OpenGL with "glMatrixMode(GL_MODELVIEW);glLoadMatrix(camera.viewMatrix().ptr());"
    const mat4& viewMatrix() const;
    //The bounding sphere of the scene used to optimize the near and far clipping planes.sa setNearFarClippingPlanesOptimized()
    void setSceneBoundingSphere(const Sphere& sphere);
    //The bounding sphere of the scene used to optimize the near and far clipping planes.sa setNearFarClippingPlanesOptimized()
    const Sphere& sceneBoundingSphere() const;
    /**
        Enables the optimization of the near and far clipping planes.
        If the optimization is enabled the near and far clipping planes are adjusted to fit the
        sphere returned by sceneBoundingSphere(). Optimizing the near and far clipping planes
        results in an optimized usage of the z-buffer with the consequence of minimizing possible
        z-fighting artifacts and enhancing the rendering quality.
        \sa setSceneBoundingSphere()
        \note Optimizing the near and far clipping planes can slightly slow down the rendering performances.
        \note At the moment the near and far clipping planes optimization is available only when using a
        perspective projection matrix and not for orthographic projection matrices.
    */
    void setNearFarClippingPlanesOptimized(bool enable);
    /**
        Set to true if the optimization of the near and far clipping planes is enabled.
        If the optimization is enabled the near and far clipping planes are adjusted to fit the
        sphere returned by sceneBoundingSphere(). Optimizing the near and far clipping planes
        results in an optimized usage of the z-buffer with the consequence of minimizing possible
        z-fighting artifacts and enhancing the rendering quality.
        \sa setSceneBoundingSphere()
        \note Optimizing the near and far clipping planes might slightly slow down the rendering performances if the scene contains several thousands of objects.
        \note At the moment the near and far clipping planes optimization is available only when using a
        perspective projection matrix set up by setProjectionAsPerspective().
    */
    bool nearFarClippingPlanesOptimized() const;
    //相机投影矩阵(The camera's projection matrix.)
    void setProjectionMatrix(const mat4& mat);
    const mat4& projectionMatrix() const;

    void setFOV(Real fov);
    Real fov() const;

    void setNearPlane(Real nearplane);
    Real nearPlane() const;

    void setFarPlane(Real farplane);
    Real farPlane() const;

    void setFrustum(const Frustum& frustum);
    const Frustum& frustum() const;

    void setViewport(std::shared_ptr<Viewport> viewport);
    Viewport* viewport();
    const Viewport* viewport() const;
};



//中心点,半径,视角点,平面点,
bool HitHyper(vec3 const& center, const float radius, const vec3 viewpoint, const vec3 vp, const vec3 hitplane, vec3& hit);

//堆
Real clamp(const Real x, const Real minval, const Real maxval);

vec3 ClosestPoint(const vec4 plane, const vec3 point);

//计算生成关系点[相机,中心点,半径,输入控制点]
vec3 HitSphere(Camera* camera, const vec3& pivot, const float radius, const vec3& p);

//计算与view关系点[camera,中心点，最后点]
vec3 HitViewPlane(Camera* camera, const vec3& pivot, const vec3& p);



#endif

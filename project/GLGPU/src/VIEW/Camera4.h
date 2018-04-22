
#ifndef CAMERA4_H
#define CAMERA4_H

#include <memory>
#include "ViewPort.h"

//���������ӵ������������п��Գ��ֳ���
class Camera
{
protected:
    mat4 mViewMatrix;                           //��ͼ����
    mat4 mInverseViewMatrix;                    //������ͼ����
    mat4 mProjectionMatrix;                     //ͶӰ���� (͸��)
    Frustum mFrustum;                           //Բ׶��
    std::shared_ptr<Viewport> mViewport;        //GL�ӿ�
    Real mFOV;                                  //??????
    Real mNearPlane;                            //��ƽ��
    Real mFarPlane;                             //Զƽ��
    bool mNearFarClippingPlanesOptimized;       //Զ������ƽ���Ż�
private:
    Sphere mSceneBoundingSphere;                //��
public:
    Camera();
private:
    //��ȡƽ���㷨 �������
    void extractPlanes(Plane* planes, const mat4& modelviewproj);
public:
    //���ݳ�ʼ��GL_PROJECTION��GL_MODELVIEW���������ͶӰ����ͼ���󲢼���ƽ��ͷƽ��
    void activate(void);
    //view�Ŀ�߱�
    Real aspectRatio() const;
    //������ռ��м��������ƽ��ͷ��ƽ�����
    void computeFrustumPlanes();
    //ͨ��ͼ׶�������ͶӰ���� (������ƽ��ͷ�幹�������͸��ͶӰ����)
    void setProjectionAsFrustum(Real left, Real right, Real bottom, Real top, Real near, Real far);
    //ͨ��͸�ӱ任����ͶӰ���� (����������ӿڵ����ù��������͸��ͶӰ����)
    void setProjectionAsPerspective();
    void setProjectionAsPerspective(Real fov, Real near, Real far);
    //����任Ҫ����ͼ����
    void setViewMatrixAsLookAt(const vec3& eye, const vec3& center, const vec3& up);
    void getViewMatrixAsLookAt(vec3& eye, vec3& look, vec3& up, vec3& right) const;
    //��������ͶӰ(��������Ľ�/Զƽ������ӿڵ�����Ϊ�����������ͶӰ����)
    void setProjectionAsOrtho(Real offset = -0.5f);
    //��������ͶӰ(����������ӿ�����Ϊ�������������ͶӰ����)
    void setProjectionAsOrtho2D(Real offset = -0.5f);
    //Ӧ����ͼ����cam(����GL_MODELVIEW ����ͼ�������õ������)
    void applyViewMatrix() const;
    //Ӧ��ģ����ͼ����(����GL_MODELVIEW ��ͼ�������ָ����ģ�;��� ʹ�õ������)
    void applyModelViewMatrix(const mat4& model_matrix) const;
    //????����ӳ��һ������
    bool project(const vec4& in, vec4& out) const;
    //�Ӵ������굽�������������
    bool unproject(const vec3& win, vec4& out) const;
    //Unprojects a set of points. See unproject(const vec3& win, vec4& out) for more information.
    bool unproject(std::vector<vec3>& points) const;
    //�������λ�ã��Ա������ȷ�鿴������vbox
    void adjustView(const VBOX& vbox, const vec3& dir, const vec3& up, Real bias = 1.0f);
public:
    //����Ĳο���ܾ�����ͼ����ĵ�����(The camera's reference frame matrix, the inverse of the view matrix.)
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
    //���ͶӰ����(The camera's projection matrix.)
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



//���ĵ�,�뾶,�ӽǵ�,ƽ���,
bool HitHyper(vec3 const& center, const float radius, const vec3 viewpoint, const vec3 vp, const vec3 hitplane, vec3& hit);

//��
Real clamp(const Real x, const Real minval, const Real maxval);

vec3 ClosestPoint(const vec4 plane, const vec3 point);

//�������ɹ�ϵ��[���,���ĵ�,�뾶,������Ƶ�]
vec3 HitSphere(Camera* camera, const vec3& pivot, const float radius, const vec3& p);

//������view��ϵ��[camera,���ĵ㣬����]
vec3 HitViewPlane(Camera* camera, const vec3& pivot, const vec3& p);



#endif

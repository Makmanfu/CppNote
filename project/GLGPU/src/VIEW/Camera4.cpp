
#include "stdafx.h"
#include "Camera4.h"
#include <gl/GL.h>
#include <assert.h>

Camera::Camera()
{
    mFrustum.planes().resize(6);
    mNearFarClippingPlanesOptimized = false;
    mFOV = 30;//45.0f;
    mNearPlane = (Real)1.0;//0.5;//0.05;
    mFarPlane = (Real)100000.0;
    mProjectionMatrix = mat4::perspective(fov(), 640.0f / 480.0f, nearPlane(), farPlane());
}

void Camera::extractPlanes(Plane* planes, const mat4& modelviewproj)
{
    // see also http://www2.ravensoft.com/users/ggribb/plane%20extraction.pdf
    // see also http://zach.in.tu-clausthal.de/teaching/cg_literatur/lighthouse3d_view_frustum_culling/index.html
    // the equation is a*x+b*y+x*z+d = 0
    // where <a b c> is the normal of the plane
    // the normals are pointing inside the viewfrustum
    // 1) we want the planes to point outward so we reverse them
    // 2) because of 1) "d" becomes the distance of the plane from the origin
    vec3 n;
    Real d;
    // left clipping plane
    n.x() = modelviewproj[0][3] + modelviewproj[0][0];
    n.y() = modelviewproj[1][3] + modelviewproj[1][0];
    n.z() = modelviewproj[2][3] + modelviewproj[2][0];
    d = modelviewproj[3][3] + modelviewproj[3][0];
    d /= n.length();
    n.normalize();
    planes[0] = Plane(d, -n);
    // right clipping plane
    n.x() = modelviewproj[0][3] - modelviewproj[0][0];
    n.y() = modelviewproj[1][3] - modelviewproj[1][0];
    n.z() = modelviewproj[2][3] - modelviewproj[2][0];
    d = modelviewproj[3][3] - modelviewproj[3][0];
    d /= n.length();
    n.normalize();
    planes[1] = Plane(d, -n);
    // top clipping plane
    n.x() = modelviewproj[0][3] - modelviewproj[0][1];
    n.y() = modelviewproj[1][3] - modelviewproj[1][1];
    n.z() = modelviewproj[2][3] - modelviewproj[2][1];
    d = modelviewproj[3][3] - modelviewproj[3][1];
    d /= n.length();
    n.normalize();
    planes[2] = Plane(d, -n);
    // bottom clipping plane
    n.x() = modelviewproj[0][3] + modelviewproj[0][1];
    n.y() = modelviewproj[1][3] + modelviewproj[1][1];
    n.z() = modelviewproj[2][3] + modelviewproj[2][1];
    d = modelviewproj[3][3] + modelviewproj[3][1];
    d /= n.length();
    n.normalize();
    planes[3] = Plane(d, -n);
    // near clipping plane
    n.x() = modelviewproj[0][3] + modelviewproj[0][2];
    n.y() = modelviewproj[1][3] + modelviewproj[1][2];
    n.z() = modelviewproj[2][3] + modelviewproj[2][2];
    d = modelviewproj[3][3] + modelviewproj[3][2];
    d /= n.length();
    n.normalize();
    planes[4] = Plane(d, -n);
    // far clipping plane
    n.x() = modelviewproj[0][3] - modelviewproj[0][2];
    n.y() = modelviewproj[1][3] - modelviewproj[1][2];
    n.z() = modelviewproj[2][3] - modelviewproj[2][2];
    d = modelviewproj[3][3] - modelviewproj[3][2];
    d /= n.length();
    n.normalize();
    planes[5] = Plane(d, -n);
}

void Camera::activate()
{
    // near/far clipping planes optimization
    if (nearFarClippingPlanesOptimized() && !sceneBoundingSphere().isNull())
    {
        // compute the sphere in camera coordinates
        Sphere camera_sphere;
        sceneBoundingSphere().transformed(camera_sphere, viewMatrix());
#undef far  // chi e' il marrano?
#undef near // chi e' il marrano?
        Real far  = -(camera_sphere.center().z() - camera_sphere.radius() * (Real)1.01);
        Real near = -(camera_sphere.center().z() + camera_sphere.radius() * (Real)1.01);
#if 0
        far  = max(far, (Real)1.0e-5);
        near = max(near, (Real)1.0e-6);
#else
        // prevents z-thrashing when very large objects are zoomed a lot
        Real ratio = camera_sphere.radius() * (Real)2.01 / (Real)2000.0;
        far  = max(far,  ratio * 2);
        near = max(near, ratio * 1);
#endif
        // supports only perspective projection matrices
        mat4 projm = mat4::perspective(fov(), aspectRatio(), near, far);
        setProjectionMatrix(projm);
        // printf("near -> far == %f -> %f\n", (float)near, (float)far);
    }
    // activation
    // projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(projectionMatrix().ptr());
    //glLoadMatrixd( projectionMatrix().ptr() );
    // view matrix
    applyViewMatrix();
}

Real Camera::aspectRatio() const
{
    if (viewport())
        return (Real)viewport()->width() / viewport()->height();
    else
        return 0;
}

void Camera::computeFrustumPlanes()
{
    //构建模型视图矩阵
    mat4 viewproj = projectionMatrix() * viewMatrix();
    //平截头体平面提取
    extractPlanes(&mFrustum.planes()[0], viewproj);
    //如果(近/远裁剪平面)优化是固定(近/远裁剪平面)时处理
    if (nearFarClippingPlanesOptimized())
    {
        Real onear = dot(inverseViewMatrix().getZ(), inverseViewMatrix().getT() - inverseViewMatrix().getZ() * nearPlane());
        Real ofar = dot(-inverseViewMatrix().getZ(), inverseViewMatrix().getT() - inverseViewMatrix().getZ() * farPlane());
        //近平面
        mFrustum.planes()[4] = Plane(onear, inverseViewMatrix().getZ());
        //远平面
        mFrustum.planes()[5] = Plane(ofar, -inverseViewMatrix().getZ());
    }
}

void Camera::setProjectionAsFrustum(Real left, Real right, Real bottom, Real top, Real near, Real far)
{
    setFOV(-1);
    setNearPlane(near);
    setFarPlane(far);
    //图锥截体
    mProjectionMatrix = mat4::frustum(left, right, bottom, top, near, far);
}

void Camera::setProjectionAsPerspective(Real fov, Real near, Real far)
{
    setFOV(fov);
    setNearPlane(near);
    setFarPlane(far);
    //通过透视得到 投影矩阵
    mProjectionMatrix = mat4::perspective(fov, aspectRatio(), near, far);
}

void Camera::setProjectionAsPerspective()
{
    //通过透视得到 投影矩阵
    mProjectionMatrix = mat4::perspective(fov(), aspectRatio(), nearPlane(), farPlane());
}

void Camera::setViewMatrixAsLookAt(const vec3& eye, const vec3& center, const vec3& up)
{
    //旋转计算
    mat4 m = mat4::lookAt(eye, center, up);
    //这设置了帧矩阵和视图矩阵
    setInverseViewMatrix(m);
}

void Camera::getViewMatrixAsLookAt(vec3& eye, vec3& look, vec3& up, vec3& right) const
{
    mInverseViewMatrix.getAsLookAt(eye, look, up, right);
}

void Camera::setProjectionAsOrtho(Real offset)
{
    setProjectionMatrix(mat4::ortho(
                            offset, (Real)mViewport->width() + offset,
                            offset, (Real)mViewport->height() + offset,
                            nearPlane(), farPlane())
                       );
}

void Camera::setProjectionAsOrtho2D(Real offset)
{
    setProjectionMatrix(mat4::ortho(
                            offset, (Real)mViewport->width() + offset,
                            offset, (Real)mViewport->height() + offset,
                            -1, +1)
                       );
}

void Camera::applyViewMatrix() const
{
    /* some OpenGL drivers (ATI) require this instead of the more general (and mathematically correct) viewMatrix() */
    mat4 viewm = viewMatrix();
    viewm.e(0, 3) = 0.0;
    viewm.e(1, 3) = 0.0;
    viewm.e(2, 3) = 0.0;
    viewm.e(3, 3) = 1.0;
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(viewm.ptr());
}

void Camera::applyModelViewMatrix(const mat4& model_matrix) const
{
    /* some OpenGL drivers (ATI) require this instead of the more general (and mathematically correct) viewMatrix() */
    mat4 viewm = viewMatrix();
    viewm.e(0, 3) = 0.0;
    viewm.e(1, 3) = 0.0;
    viewm.e(2, 3) = 0.0;
    viewm.e(3, 3) = 1.0;
    glMatrixMode(GL_MODELVIEW);
#if 0
    VL_glLoadMatrix(viewm.ptr());
    VL_glMultMatrix(matrix.ptr());
#elif 0
    viewm = viewm * matrix;
    VL_glLoadMatrix(viewm.ptr());
#else
    glLoadMatrixd((viewm * model_matrix).ptr());
#endif
}

bool Camera::project(const vec4& in, vec4& out) const
{
    out = mProjectionMatrix * mViewMatrix * in;
    if (out.w() == 0.0f)
        return false;
    out.x() /= out.w();
    out.y() /= out.w();
    out.z() /= out.w();
    // map to range 0-1
    out.x() = out.x() * 0.5f + 0.5f;
    out.y() = out.y() * 0.5f + 0.5f;
    out.z() = out.z() * 0.5f + 0.5f;
    // map to viewport
    out.x() = out.x() * mViewport->width() + mViewport->x();
    out.y() = out.y() * mViewport->height() + mViewport->y();
    return true;
}

bool Camera::unproject(const vec3& win, vec4& out) const
{
    //param win要投影的窗口坐标点。 注意:按照OpenGL约定,窗口坐标的Y轴向上,原点在窗口的左下角
    //param out投影点的世界坐标。
    //note  win的w坐标确定投影点 沿通过win的光线的位置。
    //如果 w等于0，则out将放在近剪裁平面上，
    //如果 w等于1，则out将放在远剪裁平面上。
    vec4 v(win.x(), win.y(), win.z(), 1.0);
    // map from viewport to 0-1
    v.x() = (v.x() - mViewport->x()) / mViewport->width();
    v.y() = (v.y() - mViewport->y()) / mViewport->height();
    // map to range -1 to 1
    v.x() = v.x() * 2.0f - 1.0f;
    v.y() = v.y() * 2.0f - 1.0f;
    v.z() = v.z() * 2.0f - 1.0f;
    bool invertible = true;
    mat4 inverse = (mProjectionMatrix * mViewMatrix).inverse(&invertible);
    if (!invertible)
        return false;
    v = inverse * v;
    if (v.w() == 0.0)
        return false;
    out = v / v.w();
    return true;
}

bool Camera::unproject(std::vector<vec3>& win) const
{
    bool invertible = true;
    mat4 inverse = (mProjectionMatrix * mViewMatrix).inverse(&invertible);
    if (!invertible)
        return false;
    bool ok = true;
    for (unsigned i = 0; i < win.size(); ++i)
    {
        vec4 v(win[i], 1.0);
        // map from viewport to 0-1
        v.x() = (v.x() - mViewport->x()) / mViewport->width();
        v.y() = (v.y() - mViewport->y()) / mViewport->height();
        // map to range -1 to 1
        v.x() = v.x() * 2.0f - 1.0f;
        v.y() = v.y() * 2.0f - 1.0f;
        v.z() = v.z() * 2.0f - 1.0f;
        v = inverse * v;
        if (v.w() == 0.0)
        {
            ok = false;
            continue;
        }
        v = v / v.w();
        win[i] = v.xyz();
    }
    return ok;
}

void Camera::adjustView(const VBOX& vbox, const vec3& dir, const vec3& up, Real bias)
{
    //vbox应该从新计算的摄像机位置可见的VBOX（在世界坐标系中）。
    //dir相机应该移动以查看给定VBOX的方向（在世界坐标系中）。
    //up定义向上的向量（在世界坐标系中）。 用于正确计算新的摄像机矩阵。
    //bias用于调整从给定VBOX计算的相机距离的偏置因子。 0到1之间的值使得摄像机靠近VBOX中心，大于1的值会使摄像机远离。
    assert(bias >= 0);
    vec3 center = vbox.center();
    Sphere sphere(vbox);
    const vec3& C = inverseViewMatrix().getT();
    const vec3& V = -inverseViewMatrix().getZ();
    const Real  R = sphere.radius();
    // extract the frustum planes based on the current view and projection matrices
    mat4 viewproj = projectionMatrix() * viewMatrix();
    Frustum frustum;
    frustum.planes().resize(6);
    extractPlanes(&frustum.planes()[0], viewproj);
    // iterate over left/right/top/bottom clipping planes. the planes are in world coords.
    Real max_t = 0;
    for (int i = 0; i < 4; ++i)
    {
        const vec3& O = frustum.plane(i).origin() * frustum.plane(i).normal();
        const vec3& N = frustum.plane(i).normal();
        Real t = - (R + dot(O, N) - dot(C, N)) / dot(N, V);
        if (t > max_t)
            max_t = t;
    }
    Real dist = max_t;
    mat4 m = mat4::lookAt(center + dir * dist * bias, center, up);
    setInverseViewMatrix(m);
}


void Camera::setInverseViewMatrix(const mat4& mat)
{
    mInverseViewMatrix = mat;
    mViewMatrix = mat;
    mViewMatrix.invert(); /*CameraPosChanged();*/
}

const mat4& Camera::inverseViewMatrix() const
{
    return mInverseViewMatrix;
}

void Camera::setViewMatrix(const mat4& mat)
{
    mViewMatrix = mat;
    mInverseViewMatrix = mat;
    mInverseViewMatrix.invert();
}

const mat4& Camera::viewMatrix() const
{
    return mViewMatrix;
}

void Camera::setSceneBoundingSphere(const Sphere& sphere)
{
    mSceneBoundingSphere = sphere;
}

const Sphere& Camera::sceneBoundingSphere() const
{
    return mSceneBoundingSphere;
}

void Camera::setNearFarClippingPlanesOptimized(bool enable)
{
    mNearFarClippingPlanesOptimized = enable;
}

bool Camera::nearFarClippingPlanesOptimized() const
{
    return mNearFarClippingPlanesOptimized;
}

void Camera::setProjectionMatrix(const mat4& mat)
{
    mProjectionMatrix = mat;
}

const mat4& Camera::projectionMatrix() const
{
    return mProjectionMatrix;
}

void Camera::setFOV(Real fov)
{
    mFOV = fov;
}

Real Camera::fov() const
{
    return mFOV;
}

void Camera::setNearPlane(Real nearplane)
{
    mNearPlane = nearplane;
}

Real Camera::nearPlane() const
{
    return mNearPlane;
}

void Camera::setFarPlane(Real farplane)
{
    mFarPlane = farplane;
}

Real Camera::farPlane() const
{
    return mFarPlane;
}

void Camera::setFrustum(const Frustum& frustum)
{
    mFrustum = frustum;
}

const Frustum& Camera::frustum() const
{
    return mFrustum;
}

void Camera::setViewport(std::shared_ptr<Viewport> viewport)
{
    mViewport = viewport;
}

Viewport* Camera::viewport()
{
    return mViewport.get();
}

const Viewport* Camera::viewport() const
{
    return mViewport.get();
}




//中心点,半径,视角点,平面点,
bool HitHyper(vec3 const& center, const float radius, const vec3 viewpoint, const vec3 vp, const vec3 hitplane, vec3& hit)
{
    float hitplaney = (center - hitplane).length();
    float viewpointx = (center - viewpoint).length();
    float a = hitplaney / viewpointx;
    float b = -hitplaney;
    float c = radius * radius / 2.0f;
    float delta = b * b - 4 * a * c;
    float x1, x2, xval, yval;
    if (delta > 0)
    {
        x1 = (-b - sqrt(delta)) / (2.0f * a);
        x2 = (-b + sqrt(delta)) / (2.0f * a);
        xval = x1;                  //always take the minimum value solution
        yval = c / xval;            //alternatively it also could be the other part of the equation yval=-(hitplaney/viewpointx)*xval+hitplaney;
    }
    else
        return false;
    // Computing the result in 3d space;
    vec3 dirRadial = (hitplane - center).normalize();
    vec3 dirView = vp;              //vp.normalize();
    hit = center + dirRadial * yval + dirView * xval;
    return true;
}

Real clamp(const Real x, const Real minval, const Real maxval)
{
    return min(max(x, minval), maxval);
}

vec3 ClosestPoint(const vec4 plane, const vec3 point)
{
    Real f0 = plane.xyz().lengthSquared();          //长度平方
    if (f0 == 0.0)
        return vec3(-9999.0, -9999.0, -9999.0);
    Real f1 = dot(plane.xyz(), point) + plane.w();
    Real t = -f1 / f0;
    return point + plane.xyz() * t;
}

//计算生成关系点[相机,中心点,半径,输入控制点]
vec3 HitSphere(Camera* camera, const vec3& pivot, const float radius, const vec3& p)
{
    vec4 p1;
    camera->unproject(p, p1);
    vec3 eye = camera->inverseViewMatrix().getT();
    vec3 wp = (p1.xyz() - eye).normalize();
    //vec3 center = camera->sceneBoundingSphere().center();
    //vec3 center = pivot;
    //float radius = camera->sceneBoundingSphere().radius();
    //vec3 dir = (eye - pivot).normalize();
    vec3 dir = (camera->inverseViewMatrix().getZ()).normalize();//m * vec3(0.0,0.0,1.0);//mat_rot * dir0 ;//(eye - center).normalize();
    vec4 vp = vec4(dir, -dot(dir, pivot));
    vec3 hit_plane, hit_sphere, hit_sphere_0, hit_sphere_1, hit_hyper;
    //????
    int pi = ray_plane_intersection(p1.xyz(), wp, vp, hit_plane);
    int si = ray_sphere_intersection(p1.xyz(), wp, pivot, radius, hit_sphere_0, hit_sphere_1);
    //????
    if (si)
    {
        if ((eye - hit_sphere_0).lengthSquared() < (eye - hit_sphere_1).lengthSquared())
            hit_sphere = hit_sphere_0;
        else
            hit_sphere = hit_sphere_1;
    }
    //????
    bool hi = HitHyper(pivot, radius, eye, vp.xyz(), hit_plane, hit_hyper);
    //TRACE("pi = %d\n",hi);
    //hi = false;
    if (!si && !hi)
    {
        // 退化线到球体和双曲面
        vec3 hit = ClosestPoint(vp, pivot);
        return hit;
        //(center -_ori).dot(_dir)
        //throw;
        return hit_hyper;
    }
    else if (si && !hi)
    {
        //TRACE("sphere\n");
        return hit_sphere;
    }
    else if (!si && hi)
    {
        //TRACE("hyper\n");
        return hit_hyper;
    }
    else
    {
        float angle = acos(clamp(dot(vec3(eye - pivot).normalize(),
                                     vec3(hit_sphere - pivot).normalize()), -1.0, +1.0)) * 180 / double(3.14159265358979323846);
        //float angle = acos(clamp(dot(vec3(eye - center).normalize(),vec3(hit_sphere - center).normalize()),-1.0,+1.0));
        //TRACE("angle = %f \n", angle);
        if (angle < 45)
            return hit_sphere;
        else
            return hit_hyper;
    }
}

//计算与view关系点[camera,中心点，最后点]
vec3 HitViewPlane(Camera* camera, const vec3& pivot, const vec3& p)
{
    vec4 p1;
    camera->unproject(p, p1);
    vec3 eye = camera->inverseViewMatrix().getT();
    vec3 wp = (p1.xyz() - eye).normalize();
    //vec3 center = camera->sceneBoundingSphere().center();   //相机中心点
    vec3 center = pivot;
    //?????
    vec3 dir = (-camera->inverseViewMatrix().getZ()).normalize();
    vec4 vp = vec4(dir, -dot(center, dir));
    vec3 hit_plane;
    int pi = ray_plane_intersection(p1.xyz(), wp, vp, hit_plane);
    if (!pi)
        return vec3(0.0, 0.0, 0.0);
    return hit_plane;
}

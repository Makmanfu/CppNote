

#include "stdafx.h"
#include "ViewPort.h"
#include <gl/gl.h>
#include "Math.hpp"
#include <assert.h>

Plane::Plane(Real o /*= 0.0f*/, vec3 n /*= vec3(0, 0, 0)*/) : mNormal(n), mOrigin(o)
{
}

Plane::Plane(const vec3& o, const vec3& n)
{
    mNormal = n;
    mOrigin = dot(o, n);    //算出两点间的距离 return o.x() * n.x() + o.y() * n.y() + o.z() * n.z();
}

Real Plane::distance(const vec3& v) const
{
    return dot(v, mNormal) - mOrigin;
}

int Plane::classify(const VBOX& vbox) const
{
    vec3 corner[] =
    {
        vec3(vbox.minCorner().x(), vbox.minCorner().y(), vbox.minCorner().z()),
        vec3(vbox.minCorner().x(), vbox.minCorner().y(), vbox.maxCorner().z()),
        vec3(vbox.minCorner().x(), vbox.maxCorner().y(), vbox.minCorner().z()),
        vec3(vbox.minCorner().x(), vbox.maxCorner().y(), vbox.maxCorner().z()),
        vec3(vbox.maxCorner().x(), vbox.minCorner().y(), vbox.minCorner().z()),
        vec3(vbox.maxCorner().x(), vbox.minCorner().y(), vbox.maxCorner().z()),
        vec3(vbox.maxCorner().x(), vbox.maxCorner().y(), vbox.minCorner().z()),
        vec3(vbox.maxCorner().x(), vbox.maxCorner().y(), vbox.maxCorner().z())
    };
    int left = 0;
    int right = 0;
    Real const NEPS = -0.0001f;
    Real const PEPS = +0.0001f;
    for (int i = 0; i < 8; ++i)
    {
        if (distance(corner[i]) < NEPS)         //为负
            left++;
        else if (distance(corner[i]) > PEPS)    //为正
            right++;
        // else -> we don't count the points on the plane
        if (left && right) // its clearly intersecting the plane
            return 0;
    }
    if (left)
        return -1;
    else if (right)
        return +1;
    else // all the points were on the plane
        return 0;
}



bool Plane::isOutside(const VBOX& vbox) const
{
    vec3 pt;
    pt.x() = mNormal.x() >= 0 ? vbox.minCorner().x() : vbox.maxCorner().x();
    pt.y() = mNormal.y() >= 0 ? vbox.minCorner().y() : vbox.maxCorner().y();
    pt.z() = mNormal.z() >= 0 ? vbox.minCorner().z() : vbox.maxCorner().z();
    return distance(pt) >= 0;
}

const vec3& Plane::normal() const
{
    return mNormal;
}

Real Plane::origin() const
{
    return mOrigin;
}

void Plane::setNormal(const vec3& normal)
{
    mNormal = normal;
}

void Plane::setOrigin(Real origin)
{
    mOrigin = origin;
}

Sphere::Sphere()
{
    mRadius = -1;
}

Sphere::Sphere(const VBOX& vbox)
{
    *this = vbox;
}

Sphere& Sphere::operator=(const VBOX& vbox)
{
    // center the sphere on the vbox center
    mCenter = vbox.center();
    // half of the maximum diagonal
    mRadius = (vbox.minCorner() - vbox.maxCorner()).length() / (Real)2.0;
    return *this;
}

Sphere Sphere::operator+(const Sphere& other)
{
    Sphere t = *this;
    return t += other;
}

const Sphere& Sphere::operator+=(const Sphere& other)
{
    if (this->isNull())
        *this = other;
    else if (other.includes(*this))
        *this = other;
    else if (!other.isNull() && !this->includes(other))
    {
        vec3 v = other.center() - this->center();
        if (v.isNull())
        {
            // the center remains the same
            // sets the maximum radius
            setRadius(radius() > other.radius() ? radius() : other.radius());
        }
        else
        {
            v.normalize();
            vec3 p0 = this->center() - v * this->radius();
            vec3 p1 = other.center() + v * other.radius();
            setCenter((p0 + p1) * (Real)0.5);
            setRadius((p0 - p1).length() * (Real)0.5);
        }
    }
    return *this;
}

void Sphere::setNull()
{
    mRadius = -1.0f;
    mCenter = vec3(0, 0, 0);
}

void Sphere::setPoint()
{
    mRadius = 0.0f; /*mCenter = vec3(0,0,0);*/
}

bool Sphere::isNull()  const
{
    return mRadius < 0.0f;
}

bool Sphere::isPoint() const
{
    return mRadius == 0.0f;
}

bool Sphere::includes(const Sphere& other) const
{
    if (isNull())
        return false;
    else if (other.isNull())
        return true;
    else
    {
        Real distance = (center() - other.center()).length();
        return radius() >= distance + other.radius();
    }
}

void Sphere::transformed(Sphere& sphere, const mat4& mat) const
{
    sphere.setNull();
    if (!isNull())
    {
        sphere.mCenter = mat * center();
        // vec3 p = center() + vec3( (Real)0.577350269189625840, (Real)0.577350269189625840, (Real)0.577350269189625840 ) * radius();
        // p = mat * p;
        // p = p - sphere.center();
        // sphere.setRadius(p.length());
        vec3 p0 = center() + vec3(radius(), 0, 0);
        vec3 p1 = center() + vec3(0, radius(), 0);
        vec3 p2 = center() + vec3(0, 0, radius());
        p0 = mat * p0;
        p1 = mat * p1;
        p2 = mat * p2;
        Real d0 = (p0 - sphere.mCenter).lengthSquared();
        Real d1 = (p1 - sphere.mCenter).lengthSquared();
        Real d2 = (p2 - sphere.mCenter).lengthSquared();
        sphere.mRadius = d0 > d1 ? (d0 > d2 ? d0 : d2) : (d1 > d2 ? d1 : d2);
        sphere.mRadius = ::sqrt(sphere.mRadius);
    }
}

Sphere Sphere::transformed(const mat4& mat) const
{
    Sphere sphere;
    transformed(sphere, mat);
    return sphere;
}

void Sphere::setCenter(const vec3& center)
{
    mCenter = center;
}

const vec3& Sphere::center() const
{
    return mCenter;
}

void Sphere::setRadius(Real radius)
{
    mRadius = radius;
}

Real Sphere::radius() const
{
    return mRadius;
}

void Frustum::setPlane(unsigned i, const Plane& plane)
{
    assert(i < mPlanes.size());
    mPlanes[i] = plane;
}

const Plane& Frustum::plane(unsigned i) const
{
    assert(i < mPlanes.size());
    return mPlanes[i];
}

bool Frustum::cull(const Sphere& sphere) const
{
    // null spheres are always visible
    if (sphere.isNull())
        return false;
    for (unsigned i = 0; i < planes().size(); ++i)
    {
        if (plane(i).distance(sphere.center()) > sphere.radius())
            return true;
    }
    return false;
}

bool Frustum::cull(const VBOX& vbox) const
{
    if (vbox.isNull())
        return false;
    for (unsigned i = 0; i < planes().size(); ++i)
    {
        if (plane(i).isOutside(vbox))
            return true;
    }
    return false;
}

bool Frustum::cull(const std::vector<fvec3>& points) const
{
    for (unsigned i = 0; i < planes().size(); ++i)
    {
        unsigned j = 0;
        for (; j < points.size(); ++j)
            if (plane(i).distance((vec3)points[j]) <= 0)
                break;
        if (j == points.size())
            return true;
    }
    return false;
}

std::vector<Plane>& Frustum::planes()
{
    return mPlanes;
}

const std::vector<Plane>& Frustum::planes() const
{
    return mPlanes;
}




Viewport::Viewport()
{
    mX = 0;
    mY = 0;
    mWidth = 0;
    mHeight = 0;
}

Viewport::~Viewport()
{
    mX = 0;
    mY = 0;
    mWidth = 0;
    mHeight = 0;
}

void Viewport::activate(void) const
{
    // viewport
    int x = mX, y = mY, w = mWidth, h = mHeight;
    if (w < 1)
        w = 1;
    if (h < 1)
        h = 1;
    //设置GL视口区域
    glViewport(x, y, w, h);
    // glScissor(x,y,w,h);
    //当然是设置颜色咯
    glClearColor(mClearColor.r(), mClearColor.g(), mClearColor.b(), mClearColor.a());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Viewport::null(void)
{
    return mWidth == 0 || mHeight == 0;
}

fvec2 Viewport::center(void) const
{
    return fvec2(mX + mWidth / 2.0f, mY + mHeight / 2.0f);
}

Viewport::Viewport(int x, int y, int w, int h)
{
    mX = x;
    mY = y;
    mWidth  = w;
    mHeight = h;
}

void Viewport::setX(int x)
{
    mX = x;
}

int Viewport::x(void) const
{
    return mX;
}

void Viewport::setY(int y)
{
    mY = y;
}

int Viewport::y(void) const
{
    return mY;
}

void Viewport::setWidth(int width)
{
    mWidth = width;
}

int Viewport::width() const
{
    return mWidth;
}

void Viewport::setHeight(int height)
{
    mHeight = height;
}

int Viewport::height() const
{
    return mHeight;
}

void Viewport::setClearColor(float r, float g, float b, float a)
{
    mClearColor = fvec4(r, g, b, a);
}

void Viewport::setClearColor(const fvec4& color)
{
    mClearColor = color;
}

void Viewport::set(int x, int y, int w, int h)
{
    mX = x;
    mY = y;
    mWidth = w;
    mHeight = h;
}

const fvec4& Viewport::clearColor() const
{
    return mClearColor;
}

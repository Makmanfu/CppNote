
#include "stdafx.h"
#include "VBOX.h"

VBOX::VBOX()
{
    setNull();
}

VBOX::VBOX(const vec3& center, Real radius)
{
    mMax = center + radius;
    mMin = center - radius;
}

VBOX::VBOX(const vec3& pt1, const vec3& pt2, Real displace)
{
    mMax = mMin = pt1;
    if (mMax.x() < pt2.x()) mMax.x() = pt2.x();
    if (mMax.y() < pt2.y()) mMax.y() = pt2.y();
    if (mMax.z() < pt2.z()) mMax.z() = pt2.z();
    if (mMin.x() > pt2.x()) mMin.x() = pt2.x();
    if (mMin.y() > pt2.y()) mMin.y() = pt2.y();
    if (mMin.z() > pt2.z()) mMin.z() = pt2.z();
    mMax = mMax + displace;
    mMin = mMin - displace;
}

VBOX VBOX::operator+(const VBOX& vbox) const
{
    if (isNull())
        return vbox;
    if (vbox.isNull())
        return *this;
    VBOX tmp = vbox;
    tmp.addPoint(this->mMin);
    tmp.addPoint(this->mMax);
    return tmp;
}

const VBOX& VBOX::operator+=(const VBOX& other)
{
    *this = *this + other;
    return *this;
}

VBOX VBOX::operator+(const vec3& p)
{
    VBOX vbox = *this;
    vbox += p;
    return vbox;
}

const VBOX& VBOX::operator+=(const vec3& p)
{
    addPoint(p);
    return *this;
}

void VBOX::setNull()
{
    mMin = 1;
    mMax = -1;
}

bool VBOX::isNull() const
{
    return mMin.x() > mMax.x() || mMin.y() > mMax.y() || mMin.z() > mMax.z();
}

bool VBOX::isPoint() const
{
    return mMin == mMax;
}

void VBOX::enlarge(Real displace)
{
    if (isNull())
        return;
    mMax = mMax + displace;
    mMin = mMin - displace;
}

void VBOX::addPoint(const vec3& v, Real radius)
{
    if (isNull())
    {
        mMax = v;
        mMin = v;
        return;
    }
    if (mMax.x() < v.x() + radius) mMax.x() = v.x() + radius;
    if (mMax.y() < v.y() + radius) mMax.y() = v.y() + radius;
    if (mMax.z() < v.z() + radius) mMax.z() = v.z() + radius;
    if (mMin.x() > v.x() - radius) mMin.x() = v.x() - radius;
    if (mMin.y() > v.y() - radius) mMin.y() = v.y() - radius;
    if (mMin.z() > v.z() - radius) mMin.z() = v.z() - radius;
}

void VBOX::addPoint(const vec3& v)
{
    if (isNull())
    {
        mMax = v;
        mMin = v;
        return;
    }
    if (mMax.x() < v.x()) mMax.x() = v.x();
    if (mMax.y() < v.y()) mMax.y() = v.y();
    if (mMax.z() < v.z()) mMax.z() = v.z();
    if (mMin.x() > v.x()) mMin.x() = v.x();
    if (mMin.y() > v.y()) mMin.y() = v.y();
    if (mMin.z() > v.z()) mMin.z() = v.z();
}

bool VBOX::intersects(const VBOX& bb) const
{
    if (isNull() || bb.isNull())
        return false;
    if (mMax.x() < bb.mMin.x())
        return false;
    if (mMax.z() < bb.mMin.z())
        return false;
    if (mMin.x() > bb.mMax.x())
        return false;
    if (mMin.z() > bb.mMax.z())
        return false;
    return true;
}

vec3 VBOX::clip(const vec3& v, bool clipx, bool clipy, bool clipz) const
{
    if (isNull())
        return v;
    vec3 tmp = v;
    if (clipx)
        tmp.x() = v.x() > mMax.x() ? mMax.x() : mMin.x();
    if (clipy)
        tmp.y() = v.y() > mMax.y() ? mMax.y() : mMin.y();
    if (clipz)
        tmp.z() = v.z() > mMax.z() ? mMax.z() : mMin.z();
    return tmp;
}

bool VBOX::isInside(const vec3& v, bool clipx, bool clipy, bool clipz) const
{
    vec3 t = v;
    return v == clip(t, clipx, clipy, clipz);
}

bool VBOX::isInside(const vec3& v) const
{
    return v.x() >= minCorner().x() && v.x() <= maxCorner().x() &&
           v.y() >= minCorner().y() && v.y() <= maxCorner().y() &&
           v.z() >= minCorner().z() && v.z() <= maxCorner().z() ;
}

Real VBOX::height() const
{
    if (isNull())
        return 0;
    else
        return mMax.y() - mMin.y();
}

Real VBOX::width() const
{
    if (isNull())
        return 0;
    else
        return mMax.x() - mMin.x();
}

Real VBOX::depth() const
{
    if (isNull())
        return 0;
    else
        return mMax.z() - mMin.z();
}

vec3 VBOX::center() const
{
    vec3 c = (minCorner() + maxCorner()) / 2.0f;
    return c;
}

Real VBOX::area() const
{
    if (isNull())
        return 0;
    else
        return width() * height() * depth();
}

Real VBOX::volume() const
{
    return width() * height() * depth();
}

Real VBOX::longestSideLength() const
{
    Real side = width();
    if (height() > side)
        side = height();
    if (depth() > side)
        side = depth();
    return side;
}

VBOX VBOX::transformed(const mat4& mat) const
{
    VBOX vbox;
    transformed(vbox, mat);
    return vbox;
}

void VBOX::transformed(VBOX& vbox, const mat4& mat) const
{
    vbox.setNull();
    if (!isNull())
    {
        vbox.addPoint(mat * vec3(minCorner().x(), minCorner().y(), minCorner().z()));
        vbox.addPoint(mat * vec3(minCorner().x(), maxCorner().y(), minCorner().z()));
        vbox.addPoint(mat * vec3(maxCorner().x(), maxCorner().y(), minCorner().z()));
        vbox.addPoint(mat * vec3(maxCorner().x(), minCorner().y(), minCorner().z()));
        vbox.addPoint(mat * vec3(minCorner().x(), minCorner().y(), maxCorner().z()));
        vbox.addPoint(mat * vec3(minCorner().x(), maxCorner().y(), maxCorner().z()));
        vbox.addPoint(mat * vec3(maxCorner().x(), maxCorner().y(), maxCorner().z()));
        vbox.addPoint(mat * vec3(maxCorner().x(), minCorner().y(), maxCorner().z()));
    }
}

const vec3& VBOX::minCorner() const
{
    return mMin;
}

const vec3& VBOX::maxCorner() const
{
    return mMax;
}

void VBOX::setMinCorner(Real x, Real y, Real z)
{
    mMin = vec3(x, y, z);
}

void VBOX::setMinCorner(const vec3& v)
{
    mMin = v;
}

void VBOX::setMaxCorner(Real x, Real y, Real z)
{
    mMax = vec3(x, y, z);
}

void VBOX::setMaxCorner(const vec3& v)
{
    mMax = v;
}

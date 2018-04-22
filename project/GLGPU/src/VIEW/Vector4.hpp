#pragma once

#include <cmath>
#include <assert.h>

/**
    The Vector2 class is a template class that implements a generic 2 components vector, see also fvec2, dvec2, uvec2, ivec2, svec2, usvec2, bvec2, ubvec2.
    \sa Vector4, Vector3, Matrix4, Matrix3, Matrix2
*/
template<typename scalar_type>
class Vector2
{
public:
    static const int scalar_count = 2;
    Vector2(const Vector2& other)
    {
        *this = other;
    }
    Vector2()
    {
        x() = y() = 0;
    }

    template<class T>
    explicit Vector2(const T& other)
    {
        x() = (scalar_type)other.x();
        y() = (scalar_type)other.y();
    }

    explicit Vector2(scalar_type x, scalar_type y)
    {
        mScalar[0] = x;
        mScalar[1] = y;
    }

    scalar_type* ptr()
    {
        return mScalar;
    }
    const scalar_type* ptr() const
    {
        return mScalar;
    }

    const scalar_type& x() const
    {
        return mScalar[0];
    }
    const scalar_type& y() const
    {
        return mScalar[1];
    }

    scalar_type& x()
    {
        return mScalar[0];
    }
    scalar_type& y()
    {
        return mScalar[1];
    }

    const scalar_type& r() const
    {
        return mScalar[0];
    }
    const scalar_type& g() const
    {
        return mScalar[1];
    }

    scalar_type& r()
    {
        return mScalar[0];
    }
    scalar_type& g()
    {
        return mScalar[1];
    }

    const scalar_type& s() const
    {
        return mScalar[0];
    }
    const scalar_type& t() const
    {
        return mScalar[1];
    }

    scalar_type& s()
    {
        return mScalar[0];
    }
    scalar_type& t()
    {
        return mScalar[1];
    }

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x() + other.x(), y() + other.y());
    }
    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x() - other.x(), y() - other.y());
    }
    Vector2 operator*(const Vector2& other) const
    {
        return Vector2(x() * other.x(), y() * other.y());
    }
    Vector2 operator/(const Vector2& other) const
    {
        return Vector2(x() / other.x(), y() / other.y());
    }
    Vector2 operator+(scalar_type val) const
    {
        return Vector2(x() + val, y() + val);
    }
    Vector2 operator-(scalar_type val) const
    {
        return Vector2(x() - val, y() - val);
    }
    Vector2 operator*(scalar_type val) const
    {
        return Vector2(x() * val, y() * val);
    }
    Vector2 operator/(scalar_type val) const
    {
        return Vector2(x() / val, y() / val);
    }
    Vector2 operator-() const
    {
        return Vector2(-x(), -y());
    }
    Vector2& operator+=(const Vector2& other)
    {
        *this = *this + other;
        return *this;
    }
    Vector2& operator-=(const Vector2& other)
    {
        *this = *this - other;
        return *this;
    }
    Vector2& operator*=(const Vector2& other)
    {
        *this = *this * other;
        return *this;
    }
    Vector2& operator/=(const Vector2& other)
    {
        *this = *this / other;
        return *this;
    }
    Vector2& operator+=(scalar_type val)
    {
        *this = *this + val;
        return *this;
    }
    Vector2& operator-=(scalar_type val)
    {
        *this = *this - val;
        return *this;
    }
    Vector2& operator*=(scalar_type val)
    {
        *this = *this * val;
        return *this;
    }
    Vector2& operator/=(scalar_type val)
    {
        *this = *this / val;
        return *this;
    }
    Vector2& operator=(const Vector2& other)
    {
        x() = other.x();
        y() = other.y();
        return *this;
    }
    Vector2& operator=(scalar_type val)
    {
        x() = y() = val;
        return *this;
    }
    bool operator==(const Vector2& other) const
    {
        return x() == other.x() && y() == other.y();
    }
    bool operator!=(const Vector2& other) const
    {
        return !operator==(other);
    }
    bool operator<(const Vector2& other) const
    {
        if (x() != other.x())
            return x() < other.x();
        else
            return y() < other.y();
    }
    scalar_type& operator[](unsigned i)
    {
        return mScalar[i];
    }
    const scalar_type& operator[](unsigned i) const
    {
        return mScalar[i];
    }
    scalar_type length() const
    {
        return ::sqrt(x() * x() + y() * y());
    }
    scalar_type lengthSquared() const
    {
        return x() * x() + y() * y();
    }
    bool isNull() const
    {
        return !x() && !y();
    }
    const Vector2& normalize()
    {
        scalar_type l = length();
        if (l)
            *this *= (scalar_type)(1.0 / l);
        return *this;
    }

protected:
    scalar_type mScalar[scalar_count];
};

template<typename T>
inline const Vector2<T> operator*(T val, const Vector2<T>& v)
{
    return v * val;
}

//! A 2 components vector with \p int precision.
typedef Vector2<int> ivec2;
//! A 2 components vector with \p unsigned int precision.
typedef Vector2<unsigned int> uvec2;
//! A 2 components vector with \p float precision.
typedef Vector2<float> fvec2;
//! A 2 components vector with \p double precision.
typedef Vector2<double> dvec2;
//! A 2 components vector with \p signed char precision.
typedef Vector2<signed char> bvec2;
//! A 2 components vector with \p unsigned char precision.
typedef Vector2<unsigned char> ubvec2;
//! A 2 components vector with \p short precision.
typedef Vector2<short> svec2;
//! A 2 components vector with \p unsigned short precision.
typedef Vector2<unsigned short> usvec2;

#ifndef PIPELINE_PRECISION_FLOAT
    typedef dvec2 vec2;
#else
    typedef fvec2 vec2;
#endif


/**
    The Vector3 class is a template class that implements a generic 3 components vector, see also fvec3, dvec3, uvec3, ivec3, svec3, usvec3, bvec3, ubvec3.
    \sa Vector4, Vector2, Matrix4, Matrix3, Matrix2
*/
template<typename scalar_type>
class Vector3
{
public:
    static const int scalar_count = 3;
    Vector3(const Vector3& other)
    {
        *this = other;
    }
    Vector3()
    {
        x() = y() = z() = 0;
    }

    template<class T>
    explicit Vector3(const T& other)
    {
        x() = (scalar_type)other.x();
        y() = (scalar_type)other.y();
        z() = (scalar_type)other.z();
    }

    explicit Vector3(scalar_type x, scalar_type y, scalar_type z)
    {
        mScalar[0] = x;
        mScalar[1] = y;
        mScalar[2] = z;
    }

    explicit Vector3(const Vector2<scalar_type>& v, scalar_type z)
    {
        mScalar[0] = v.x();
        mScalar[1] = v.y();
        mScalar[2] = z;
    }

    scalar_type* ptr()
    {
        return mScalar;
    }
    const scalar_type* ptr() const
    {
        return mScalar;
    }

    const scalar_type& x() const
    {
        return mScalar[0];
    }
    const scalar_type& y() const
    {
        return mScalar[1];
    }
    const scalar_type& z() const
    {
        return mScalar[2];
    }

    scalar_type& x()
    {
        return mScalar[0];
    }
    scalar_type& y()
    {
        return mScalar[1];
    }
    scalar_type& z()
    {
        return mScalar[2];
    }

    const scalar_type& r() const
    {
        return mScalar[0];
    }
    const scalar_type& g() const
    {
        return mScalar[1];
    }
    const scalar_type& b() const
    {
        return mScalar[2];
    }

    scalar_type& r()
    {
        return mScalar[0];
    }
    scalar_type& g()
    {
        return mScalar[1];
    }
    scalar_type& b()
    {
        return mScalar[2];
    }

    const scalar_type& s() const
    {
        return mScalar[0];
    }
    const scalar_type& t() const
    {
        return mScalar[1];
    }
    const scalar_type& p() const
    {
        return mScalar[2];
    }

    scalar_type& s()
    {
        return mScalar[0];
    }
    scalar_type& t()
    {
        return mScalar[1];
    }
    scalar_type& p()
    {
        return mScalar[2];
    }

    Vector2<scalar_type> xy() const
    {
        return Vector2<scalar_type>(x(), y());
    }
    Vector2<scalar_type> st() const
    {
        return Vector2<scalar_type>(x(), y());
    }

    Vector3 operator+(const Vector3& other) const
    {
        return Vector3(x() + other.x(), y() + other.y(), z() + other.z());
    }
    Vector3 operator-(const Vector3& other) const
    {
        return Vector3(x() - other.x(), y() - other.y(), z() - other.z());
    }
    Vector3 operator*(const Vector3& other) const
    {
        return Vector3(x() * other.x(), y() * other.y(), z() * other.z());
    }
    Vector3 operator/(const Vector3& other) const
    {
        return Vector3(x() / other.x(), y() / other.y(), z() / other.z());
    }
    Vector3 operator+(scalar_type val) const
    {
        return Vector3(x() + val, y() + val, z() + val);
    }
    Vector3 operator-(scalar_type val) const
    {
        return Vector3(x() - val, y() - val, z() - val);
    }
    Vector3 operator*(scalar_type val) const
    {
        return Vector3(x() * val, y() * val, z() * val);
    }
    Vector3 operator/(scalar_type val) const
    {
        return Vector3(x() / val, y() / val, z() / val);
    }
    Vector3 operator-() const
    {
        return Vector3(-x(), -y(), -z());
    }
    Vector3& operator+=(const Vector3& other)
    {
        *this = *this + other;
        return *this;
    }
    Vector3& operator-=(const Vector3& other)
    {
        *this = *this - other;
        return *this;
    }
    Vector3& operator*=(const Vector3& other)
    {
        *this = *this * other;
        return *this;
    }
    Vector3& operator/=(const Vector3& other)
    {
        *this = *this / other;
        return *this;
    }
    Vector3& operator+=(scalar_type val)
    {
        *this = *this + val;
        return *this;
    }
    Vector3& operator-=(scalar_type val)
    {
        *this = *this - val;
        return *this;
    }
    Vector3& operator*=(scalar_type val)
    {
        *this = *this * val;
        return *this;
    }
    Vector3& operator/=(scalar_type val)
    {
        *this = *this / val;
        return *this;
    }
    Vector3& operator=(const Vector3& other)
    {
        x() = other.x();
        y() = other.y();
        z() = other.z();
        return *this;
    }
    Vector3& operator=(scalar_type val)
    {
        x() = y() = z() = val;
        return *this;
    }
    bool operator==(const Vector3& other) const
    {
        if (fabs(x() - other.x()) < 1e-6)
            if (fabs(y() - other.y()) < 1e-6)
                if (fabs(z() - other.z()) < 1e-6)
                    return (1);
        return (0);
        return x() == other.x() && y() == other.y() && z() == other.z();
        //return x() == other.x() && y() == other.y() && z() == other.z();
    }
    bool operator!=(const Vector3& other) const
    {
        return !operator==(other);
    }
    bool operator<(const Vector3& other) const
    {
        if (x() != other.x())
            return x() < other.x();
        else if (y() != other.y())
            return y() < other.y();
        else
            return z() < other.z();
    }
    scalar_type& operator[](unsigned i)
    {
        return mScalar[i];
    }
    const scalar_type& operator[](unsigned i) const
    {
        return mScalar[i];
    }
    scalar_type length() const
    {
        return ::sqrt(x() * x() + y() * y() + z() * z());
    }
    scalar_type lengthSquared() const
    {
        return x() * x() + y() * y() + z() * z();
    }
    bool isNull() const
    {
        return !x() && !y() && !z();
    }
    const Vector3& normalize()
    {
        scalar_type l = length();
        if (l > 1e-6)
            *this *= (scalar_type)(1.0 / l);
        return *this;
    }

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("x", x()), cereal::make_nvp("y", y()), cereal::make_nvp("z", z()));
    }

protected:
    scalar_type mScalar[scalar_count];
};

template<typename scalar_type>
inline const Vector3<scalar_type> operator*(scalar_type val, const Vector3<scalar_type>& v)
{
    return v * val;
};

//! A 3 components vector with \p int precision.
typedef Vector3<int> ivec3;
//! A 3 components vector with \p unsigned int precision.
typedef Vector3<unsigned int> uvec3;
//! A 3 components vector with \p float precision.
typedef Vector3<float> fvec3;
//! A 3 components vector with \p double precision.
typedef Vector3<double> dvec3;
//! A 3 components vector with \p signed char precision.
typedef Vector3<signed char> bvec3;
//! A 3 components vector with \p unsigned char precision.
typedef Vector3<unsigned char> ubvec3;
//! A 3 components vector with \p short precision.
typedef Vector3<short> svec3;
//! A 3 components vector with \p unsigned short precision.
typedef Vector3<unsigned short> usvec3;

#ifndef PIPELINE_PRECISION_FLOAT
    typedef dvec3 vec3;
#else
    typedef fvec3 vec3;
#endif

/**
    The Vector4 class is a template class that implements a generic 4 components vector, see also fvec4, dvec4, uvec4, ivec4, svec4, usvec4, bvec4, ubvec4.
    \sa Vector3, Vector2, Matrix4, Matrix3, Matrix2
*/
template<typename scalar_type>
class Vector4
{
public:
    static const int scalar_count = 4;
    Vector4(const Vector4& other)
    {
        *this = other;
    }
    Vector4()
    {
        x() = y() = z() = w() = 0;
    }

    template<class T>
    explicit Vector4(const T& other)
    {
        x() = (scalar_type)other.x();
        y() = (scalar_type)other.y();
        z() = (scalar_type)other.z();
        w() = (scalar_type)other.w();
    }

    explicit Vector4(scalar_type x, scalar_type y, scalar_type z, scalar_type w)
    {
        mScalar[0] = x;
        mScalar[1] = y;
        mScalar[2] = z;
        mScalar[3] = w;
    }

    explicit Vector4(const Vector3<scalar_type>& v, scalar_type w)
    {
        mScalar[0] = v.x();
        mScalar[1] = v.y();
        mScalar[2] = v.z();
        mScalar[3] = w;
    }

    explicit Vector4(const Vector2<scalar_type>& u, const Vector2<scalar_type>& v)
    {
        mScalar[0] = u.x();
        mScalar[1] = u.y();
        mScalar[2] = v.x();
        mScalar[3] = v.y();
    }

    scalar_type* ptr()
    {
        return mScalar;
    }
    const scalar_type* ptr() const
    {
        return mScalar;
    }

    const scalar_type& x() const
    {
        return mScalar[0];
    }
    const scalar_type& y() const
    {
        return mScalar[1];
    }
    const scalar_type& z() const
    {
        return mScalar[2];
    }
    const scalar_type& w() const
    {
        return mScalar[3];
    }

    scalar_type& x()
    {
        return mScalar[0];
    }
    scalar_type& y()
    {
        return mScalar[1];
    }
    scalar_type& z()
    {
        return mScalar[2];
    }
    scalar_type& w()
    {
        return mScalar[3];
    }

    const scalar_type& r() const
    {
        return mScalar[0];
    }
    const scalar_type& g() const
    {
        return mScalar[1];
    }
    const scalar_type& b() const
    {
        return mScalar[2];
    }
    const scalar_type& a() const
    {
        return mScalar[3];
    }

    scalar_type& r()
    {
        return mScalar[0];
    }
    scalar_type& g()
    {
        return mScalar[1];
    }
    scalar_type& b()
    {
        return mScalar[2];
    }
    scalar_type& a()
    {
        return mScalar[3];
    }

    const scalar_type& s() const
    {
        return mScalar[0];
    }
    const scalar_type& t() const
    {
        return mScalar[1];
    }
    const scalar_type& p() const
    {
        return mScalar[2];
    }
    const scalar_type& q() const
    {
        return mScalar[3];
    }

    scalar_type& s()
    {
        return mScalar[0];
    }
    scalar_type& t()
    {
        return mScalar[1];
    }
    scalar_type& p()
    {
        return mScalar[2];
    }
    scalar_type& q()
    {
        return mScalar[3];
    }

    Vector3<scalar_type> xyz() const
    {
        return Vector3<scalar_type>(x(), y(), z());
    }
    Vector3<scalar_type> rgb() const
    {
        return Vector3<scalar_type>(x(), y(), z());
    }
    Vector3<scalar_type> stp() const
    {
        return Vector3<scalar_type>(x(), y(), z());
    }

    Vector2<scalar_type> xy() const
    {
        return Vector2<scalar_type>(x(), y());
    }
    Vector2<scalar_type> rg() const
    {
        return Vector2<scalar_type>(x(), y());
    }
    Vector2<scalar_type> st() const
    {
        return Vector2<scalar_type>(x(), y());
    }

    Vector4 operator+(const Vector4& other) const
    {
        return Vector4(x() + other.x(), y() + other.y(), z() + other.z(), w() + other.w());
    }
    Vector4 operator-(const Vector4& other) const
    {
        return Vector4(x() - other.x(), y() - other.y(), z() - other.z(), w() - other.w());
    }
    Vector4 operator*(const Vector4& other) const
    {
        return Vector4(x() * other.x(), y() * other.y(), z() * other.z(), w() * other.w());
    }
    Vector4 operator/(const Vector4& other) const
    {
        return Vector4(x() / other.x(), y() / other.y(), z() / other.z(), w() / other.w());
    }
    Vector4 operator+(scalar_type val) const
    {
        return Vector4(x() + val, y() + val, z() + val, w() + val);
    }
    Vector4 operator-(scalar_type val) const
    {
        return Vector4(x() - val, y() - val, z() - val, w() - val);
    }
    Vector4 operator*(scalar_type val) const
    {
        return Vector4(x() * val, y() * val, z() * val, w() * val);
    }
    Vector4 operator/(scalar_type val) const
    {
        return Vector4(x() / val, y() / val, z() / val, w() / val);
    }
    Vector4 operator-() const
    {
        return Vector4(-x(), -y(), -z(), -w());
    }
    Vector4& operator+=(const Vector4& other)
    {
        *this = *this + other;
        return *this;
    }
    Vector4& operator-=(const Vector4& other)
    {
        *this = *this - other;
        return *this;
    }
    Vector4& operator*=(const Vector4& other)
    {
        *this = *this * other;
        return *this;
    }
    Vector4& operator/=(const Vector4& other)
    {
        *this = *this / other;
        return *this;
    }
    Vector4& operator+=(scalar_type val)
    {
        *this = *this + val;
        return *this;
    }
    Vector4& operator-=(scalar_type val)
    {
        *this = *this - val;
        return *this;
    }
    Vector4& operator*=(scalar_type val)
    {
        *this = *this * val;
        return *this;
    }
    Vector4& operator/=(scalar_type val)
    {
        *this = *this / val;
        return *this;
    }
    Vector4& operator=(const Vector4& other)
    {
        x() = other.x();
        y() = other.y();
        z() = other.z();
        w() = other.w();
        return *this;
    }
    Vector4& operator=(scalar_type val)
    {
        x() = y() = z() = w() = val;
        return *this;
    }
    bool operator==(const Vector4& other) const
    {
        return x() == other.x() && y() == other.y() && z() == other.z() && w() == other.w() ;
    }
    bool operator!=(const Vector4& other) const
    {
        return !operator==(other);
    }
    bool operator<(const Vector4& other) const
    {
        if (x() != other.x())
            return x() < other.x();
        else if (y() != other.y())
            return y() < other.y();
        else if (z() != other.z())
            return z() < other.z();
        else
            return w() < other.w();
    }
    scalar_type& operator[](unsigned i)
    {
        return mScalar[i];
    }
    const scalar_type& operator[](unsigned i) const
    {
        return mScalar[i];
    }
    scalar_type length() const
    {
        return (scalar_type)::sqrt(x() * x() + y() * y() + z() * z() + w() * w());
    }
    scalar_type lengthSquared() const
    {
        return x() * x() + y() * y() + z() * z() + w() * w();
    }
    bool isNull() const
    {
        return !x() && !y() && !z() && !w();
    }
    const Vector4& normalize()
    {
        scalar_type l = length();
        if (l)
            *this *= (scalar_type)(1.0 / l);
        return *this;
    }

protected:
    scalar_type mScalar[scalar_count];
};

template<typename T>
inline const Vector4<T> operator*(T val, const Vector4<T>& v)
{
    return v * val;
}

//! A 4 components vector with \p int precision.
typedef Vector4<int> ivec4;
//! A 4 components vector with \p unsigned int precision.
typedef Vector4<unsigned int> uvec4;
//! A 4 components vector with \p float precision.
typedef Vector4<float> fvec4;
//! A 4 components vector with \p double precision.
typedef Vector4<double> dvec4;
//! A 4 components vector with \p signed char precision.
typedef Vector4<signed char> bvec4;
//! A 4 components vector with \p unsigned char precision.
typedef Vector4<unsigned char> ubvec4;
//! A 4 components vector with \p short precision.
typedef Vector4<short> svec4;
//! A 4 components vector with \p unsigned short precision.
typedef Vector4<unsigned short> usvec4;


#ifndef PIPELINE_PRECISION_FLOAT
    typedef dvec4 vec4;
    typedef double Real;
#else
    typedef fvec4 vec4;
    typedef float Real;
#endif

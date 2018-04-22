#pragma once

#include "Vector4.hpp"
#include "Matrix4.hpp"

template<typename scalar_type>
inline Vector3<scalar_type> cross(Vector3<scalar_type> const& u, Vector3<scalar_type> const& v)
{
    return Vector3<scalar_type>(u[1] * v[2] - v[1] * u[2], -u[0] * v[2] + v[0] * u[2], u[0] * v[1] - v[0] * u[1]);
};

template<typename scalar_type>
inline scalar_type dot(Vector3<scalar_type> const& u, Vector3<scalar_type> const& v)
{
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
};

template<typename scalar_type>
inline scalar_type dot(Vector2<scalar_type> const& u, Vector2<scalar_type> const& v)
{
    return u.x() * v.x() + u.y() * v.y();
};

template<typename scalar_type>
inline Vector3<scalar_type> triangle_raw_normal(Vector3<scalar_type> const& a, Vector3<scalar_type> const& b,
        Vector3<scalar_type> const& c)
{
    return cross(b - a, c - a);
};

template<typename scalar_type>
inline Vector3<scalar_type> triangle_normal(Vector3<scalar_type> const& a, Vector3<scalar_type> const& b,
        Vector3<scalar_type> const& c)
{
    return triangle_raw_normal(a, b, c).normalize();
};

template<typename scalar_type>
inline Vector3<scalar_type> triangle_incenter(Vector3<scalar_type> const& a , Vector3<scalar_type> const& b ,
        Vector3<scalar_type> const& c)
{
    Vector3<scalar_type> r0 = c - b;
    Vector3<scalar_type> r1 = c - a;
    Vector3<scalar_type> r2 = a - b;
    scalar_type a1 = r0.length() , b1 = r1.length() , c1 = r2.length() ;
    scalar_type t = a1 + b1 + c1 ;
    scalar_type x = (a1 * a.x() + b1 * b.x() + c1 * c.x()) / t ;
    scalar_type y = (a1 * a.y() + b1 * b.y() + c1 * c.y()) / t ;
    scalar_type z = (a1 * a.z() + b1 * b.z() + c1 * c.z()) / t ;
    return Vector3<scalar_type>(x, y, z);
}

template<typename scalar_type>
inline Vector4<scalar_type> triangle_plane(Vector3<scalar_type> const& a, Vector3<scalar_type> const& b,
        Vector3<scalar_type> const& c)
{
    //Ax + By + Cz + D = 0
    //dot(P,N) + D = 0
    Vector3<scalar_type> n = triangle_normal(a, b, c);
    return Vector4<scalar_type>(n, -dot(a, n));
};

template<typename scalar_type>
inline Vector4<scalar_type> triangle_raw_plane(Vector3<scalar_type> const& a, Vector3<scalar_type> const& b,
        Vector3<scalar_type> const& c)
{
    Vector3<scalar_type> n = triangle_raw_normal(a, b, c);
    return Vector4<scalar_type>(n, -dot(a, n));
};

template<typename scalar_type>
inline scalar_type triangle_area(Vector3<scalar_type> const& a, Vector3<scalar_type> const& b,
                                 Vector3<scalar_type> const& c)
{
    return 0.5 * triangle_raw_normal(a, b, c).length();
};

template<typename scalar_type>
inline int ray_plane_intersection(Vector3<scalar_type> const& p, Vector3<scalar_type> const& ray,
                                  Vector4<scalar_type> const& plane, Vector3<scalar_type>& intersection)
{
    //Ray   P = P0 + t*Ray
    //Plane dot(P,N) + d = 0
    // -> dot((P0 + t*V),N) + d = 0 -> t = - (dot(P0,N) + d) / dot(Ray,N)
    //
    scalar_type dot_Ray_n = dot(ray, plane.xyz());
    if (dot_Ray_n > -1e-12 && dot_Ray_n < 1e-12)
        return (0);
    scalar_type t  = -(dot(p, plane.xyz()) + plane.w()) / dot_Ray_n;
    intersection = p + t * ray;
    return (1);
};

template<typename scalar_type>
inline int ray_sphere_intersection(Vector3<scalar_type> const& p, Vector3<scalar_type> const& ray,
                                   Vector3<scalar_type> const& center, float radius, Vector3<scalar_type>& a, Vector3<scalar_type>& b)
{
    Vector3<scalar_type> neworig = p - center;
    scalar_type t1 = ray.x() * ray.x();
    scalar_type t2 = ray.y() * ray.y();
    scalar_type t3 = ray.z() * ray.z();
    scalar_type t6 = neworig.y() * ray.y();
    scalar_type t7 = neworig.x() * ray.x();
    scalar_type t8 = neworig.z() * ray.z();
    scalar_type t15 = radius * radius;
    scalar_type t17 = neworig.z() * neworig.z();
    scalar_type t19 = neworig.y() * neworig.y();
    scalar_type t21 = neworig.x() * neworig.x();
    scalar_type t28 = double(2.0 * t7 * t6 + 2.0 * t6 * t8 + 2.0 * t7 * t8 + t1 * t15 - t1 * t17 - t1 * t19 - t2 * t21 +
                             t2 * t15 - t2 * t17 - t3 * t21 + t3 * t15 - t3 * t19);
    if (t28 < 0) return (0);
    scalar_type t29  = sqrt(t28);
    scalar_type val0 = 1.0f / (t1 + t2 + t3) * (-t6 - t7 - t8 + t29);
    scalar_type val1 = 1.0f / (t1 + t2 + t3) * (-t6 - t7 - t8 - t29);
    a = p + ray * val0;
    b = p + ray * val1;
    return (1);
};

template<typename scalar_type>
inline scalar_type triangle_quaility(Vector3<scalar_type> const& a, Vector3<scalar_type> const& b,
                                     Vector3<scalar_type> const& c)
{
    /// 2*AreaTri/(MaxEdge^2),
    /// the range is range [0.0, 0.866]
    Vector3<scalar_type> d10 = b - a;
    Vector3<scalar_type> d20 = c - a;
    Vector3<scalar_type> d12 = b - c;
    Vector3<scalar_type> x = cross(d10, d20);
    scalar_type a1 = x.length();//Norm( x );
    if (a1 == 0) return 0; // Area zero triangles have surely quality==0;
    scalar_type b1 = d10.lengthSquared();
    if (b1 == 0) return 0; // Again: area zero triangles have surely quality==0;
    scalar_type t = b1;
    t = d20.lengthSquared();
    if (b1 < t) b1 = t;
    t = d12.lengthSquared();
    if (b1 < t) b1 = t;
    return a1 / b1;
};


template<typename scalar_type>
inline scalar_type pedal(Vector3<scalar_type> const& p, Vector3<scalar_type> const& a, Vector3<scalar_type> const& b,
                         Vector3<scalar_type>& q)
{
    Vector3<scalar_type> e = b - a;
    scalar_type t = dot((p - a), e) / e.lengthSquared();
    q = a + e * t;
    return (p - q).length();
};

template<typename scalar_type>
inline scalar_type pedal(Vector2<scalar_type> const& p, Vector2<scalar_type> const& a, Vector2<scalar_type> const& b,
                         Vector2<scalar_type>& q)
{
    Vector2<scalar_type> e = b - a;
    scalar_type t = dot((p - a), e) / e.lengthSquared();
    q = a + e * t;
    return (p - q).length();
};

template<typename scalar_type>
bool collinear(Vector3<scalar_type> const& a, Vector3<scalar_type> const& b, Vector3<scalar_type> const& c)
{
    return  cross(a - b, a - c).lengthSquared() / (b - c).lengthSquared() <= 1e-6 ? true : false;
}

struct quat2matrix
{

    template<typename scalar_type>
    inline void operator()(Matrix4<scalar_type>& m, Vector4<scalar_type> const& q)
    {
        scalar_type x2 = q.x() + q.x();
        scalar_type y2 = q.y() + q.y();
        scalar_type z2 = q.z() + q.z();
        scalar_type xx2 = q.x() * x2;
        scalar_type yy2 = q.y() * y2;
        scalar_type zz2 = q.z() * z2;
        scalar_type xy2 = q.x() * y2;
        scalar_type xz2 = q.x() * z2;
        scalar_type yz2 = q.y() * z2;
        scalar_type wx2 = q.w() * x2;
        scalar_type wy2 = q.w() * y2;
        scalar_type wz2 = q.w() * z2;
        m.e(0, 0) = 1.0f - yy2 - zz2;
        m.e(0, 1) = xy2 + wz2;
        m.e(0, 2) = xz2 - wy2;
        m.e(0, 3) = 0.0f;
        m.e(1, 0) = xy2 - wz2;
        m.e(1, 1) = 1.0f - xx2 - zz2;
        m.e(1, 2) = yz2 + wx2;
        m.e(1, 3) = 0.0f;
        m.e(2, 0) = xz2 + wy2;
        m.e(2, 1) = yz2 - wx2;
        m.e(2, 2) = 1.0f - xx2 - yy2;
        m.e(2, 3) = 0.0f;
        m.e(3, 0) = 0.0f;
        m.e(3, 1) = 0.0f;
        m.e(3, 2) = 0.0f;
        m.e(3, 3) = 1.0f;
    }
};

struct mat42quat
{
    template<typename scalar_type>
    inline void operator()(Matrix4<scalar_type> const& m, Vector4<scalar_type>& q)
    {
        scalar_type s;
        scalar_type tq[4];
        int  i, j;
        // Use tq to store the largest trace
        tq[0] = 1.0f + m[0][0] + m[1][1] + m[2][2];
        tq[1] = 1.0f + m[0][0] - m[1][1] - m[2][2];
        tq[2] = 1.0f - m[0][0] + m[1][1] - m[2][2];
        tq[3] = 1.0f - m[0][0] - m[1][1] + m[2][2];
        // Find the maximum (could also use stacked if's later)
        j = 0;
        for (i = 1; i < 4; i++) j = (tq[i] > tq[j]) ? i : j;
        // check the diagonal
        if (j == 0)
        {
            /* perform instant calculation */
            q.w() = tq[0];
            q.x() = m[1][2] - m[2][1];
            q.y() = m[2][0] - m[0][2];
            q.z() = m[0][1] - m[1][0];
        }
        else if (j == 1)
        {
            q.w() = m[1][2] - m[2][1];
            q.x() = tq[1];
            q.y() = m[0][1] + m[1][0];
            q.z() = m[2][0] + m[0][2];
        }
        else if (j == 2)
        {
            q.w() = m[2][0] - m[0][2];
            q.x() = m[0][1] + m[1][0];
            q.y() = tq[2];
            q.z() = m[1][2] + m[2][1];
        }
        else     /* if (j==3) */
        {
            q.w() = m[0][1] - m[1][0];
            q.x() = m[2][0] + m[0][2];
            q.y() = m[1][2] + m[2][1];
            q.z() = tq[3];
        }
        s = sqrt(0.25f / tq[j]);
        q.w() *= s;
        q.x() *= s;
        q.y() *= s;
        q.z() *= s;
    }
};

struct mat4decomposition   // t * s * r
{

    template<typename scalar_type>
    inline void operator()(Matrix4<scalar_type>  const& m, Vector3<scalar_type>& t, Vector3<scalar_type>& s,
                           Vector4<scalar_type>& r)
    {
        //translation.
        t = m.getT();
        //scale
        s.x() = m.getX().length();
        s.y() = m.getY().length();
        s.z() = m.getZ().length();
        scalar_type det = m.determinant();
        if (det < 0)
            s.z() = -s.z();
        //rotation
        mat42quat()(m, r);
    }
};

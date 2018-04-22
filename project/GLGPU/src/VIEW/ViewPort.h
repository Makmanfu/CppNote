
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "VBOX.h"

//平面(使用法线和原点定义平面)
class Plane
{
protected:
    vec3 mNormal;                   //原点坐标(x,y,z)
    Real mOrigin;                   //法线长度
public:
    //根据原点和法线长度构造确定平面
    Plane(Real o = 0.0f, vec3 n = vec3(0, 0, 0));
    //根据两个点的向量直接构造确定平面
    Plane(const vec3& o, const vec3& n);
public:
    //计算v点到原点的距离(也是到垂直平面的距离)
    Real distance(const vec3& v) const;
    //法线分类判定[0:原点在VBOX内部,1左正VBOX在原点上方,-1右负原点下方]
    int classify(const VBOX&) const;
    //判断原点点是否在VBOX上
    bool isOutside(const VBOX&) const;
public:
    //一些属性处理
    const vec3& normal() const;
    Real origin() const;
    void setNormal(const vec3& normal);
    void setOrigin(Real origin);
};

//球体(使用中心和半径定义球体)
class Sphere
{
protected:
    vec3 mCenter;               //球心
    Real mRadius;               //半径
public:
    Sphere();
    Sphere(const VBOX& vbox);
    Sphere& operator=(const VBOX& vbox);
    Sphere operator+(const Sphere& other);
    const Sphere& operator+=(const Sphere& other);
public:
    //清空
    void setNull();
    //将球变成一个点
    void setPoint();
    //半径小于0你懂的
    bool isNull() const;
    //确定了是个点
    bool isPoint() const;
    //判断是否包含 球other
    bool includes(const Sphere& other) const;
    //转换
    void transformed(Sphere& sphere, const mat4& mat) const;
    Sphere transformed(const mat4& mat) const;
public:
    void setCenter(const vec3& center);
    const vec3& center() const;
    void setRadius(Real radius);
    Real radius() const;
};

//平截头体(定义用于剔除目的的截头锥体的平面集合)
class Frustum
{
protected:
    std::vector<Plane> mPlanes;         //平面集
public:
    //添加平面
    void setPlane(unsigned i, const Plane& plane);
    //索引平面
    const Plane& plane(unsigned i) const;
    //判断与平面相交
    bool cull(const Sphere& sphere) const;
    bool cull(const VBOX& vbox) const;
    bool cull(const std::vector<fvec3>& points) const;
public:
    std::vector<Plane>& planes();
    const std::vector<Plane>& planes() const;
};

//GL视口(就是屏幕显示的区域)
class Viewport
{
protected:
    fvec4 mClearColor;
    int mX, mY, mWidth, mHeight;
public:
    Viewport();
    Viewport(int x, int y, int w, int h);
    ~Viewport();
public:
    //启用GL视口设置
    void activate(void) const;
    //检查宽或高为0
    bool null(void);
    //取得中心点
    fvec2 center(void) const;
public:
    //一些属性处理
    void setX(int x);
    int x(void) const;
    void setY(int y);
    int y(void) const;
    void setWidth(int width);
    int width() const;
    void setHeight(int height);
    int height() const;
    void setClearColor(float r, float g, float b, float a);
    void setClearColor(const fvec4& color);
    void set(int x, int y, int w, int h);
    const fvec4& clearColor() const;
};



#endif

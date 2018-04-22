
#ifndef VBOX_H
#define VBOX_H

#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"

//实现轴对齐边界框。
class VBOX
{
protected:
    vec3 mMin;                      //最小点
    vec3 mMax;                      //最大点
public:
    VBOX();
    //用球心和半径构造 立方体盒子
    VBOX(const vec3& center, Real radius);
    //两个点来确定[pt1固定起始点,膨胀点]
    VBOX(const vec3& pt1, const vec3& pt2, Real displace = 0);
    VBOX operator+(const VBOX& vbox) const;
    const VBOX& operator+=(const VBOX& other);
    VBOX operator+(const vec3& p);
    const VBOX& operator+=(const vec3& p);
public:
    //设置区域无效
    void setNull();
    //判断区域是否无效
    bool isNull()  const;
    //区域盒是个点
    bool isPoint() const;
    //设置位移值
    void enlarge(Real displace);
    //添加点膨胀VBOX立方体盒子区域范围
    void addPoint(const vec3& v, Real radius);
    void addPoint(const vec3& v);
    //检查bb是VBOX中
    bool intersects(const VBOX& bb) const;
    //根据V修剪新的VBOX空间
    vec3 clip(const vec3& v, bool clipx = true, bool clipy = true, bool clipz = true) const;
    //判定v在VBOX间内
    bool isInside(const vec3& v, bool clipx, bool clipy, bool clipz) const;
    bool isInside(const vec3& v) const;
    //立方体宽,高,深度
    Real height() const;
    Real width() const;
    Real depth() const;
    //mMin和mMax来计算中心点
    vec3 center() const;
    //面积范围
    Real area() const;
    //体积
    Real volume() const;
    //最长侧边长度
    Real longestSideLength() const;
    //转换
    void transformed(VBOX& vbox, const mat4& mat) const;
    VBOX transformed(const mat4& mat) const;
    //一些属性处理
    const vec3& minCorner() const;
    const vec3& maxCorner() const;
    void setMinCorner(Real x, Real y, Real z);
    void setMinCorner(const vec3& v);
    void setMaxCorner(Real x, Real y, Real z);
    void setMaxCorner(const vec3& v);
};


#endif


#ifndef VBOX_H
#define VBOX_H

#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Math.hpp"

//ʵ�������߽��
class VBOX
{
protected:
    vec3 mMin;                      //��С��
    vec3 mMax;                      //����
public:
    VBOX();
    //�����ĺͰ뾶���� ���������
    VBOX(const vec3& center, Real radius);
    //��������ȷ��[pt1�̶���ʼ��,���͵�]
    VBOX(const vec3& pt1, const vec3& pt2, Real displace = 0);
    VBOX operator+(const VBOX& vbox) const;
    const VBOX& operator+=(const VBOX& other);
    VBOX operator+(const vec3& p);
    const VBOX& operator+=(const vec3& p);
public:
    //����������Ч
    void setNull();
    //�ж������Ƿ���Ч
    bool isNull()  const;
    //������Ǹ���
    bool isPoint() const;
    //����λ��ֵ
    void enlarge(Real displace);
    //��ӵ�����VBOX�������������Χ
    void addPoint(const vec3& v, Real radius);
    void addPoint(const vec3& v);
    //���bb��VBOX��
    bool intersects(const VBOX& bb) const;
    //����V�޼��µ�VBOX�ռ�
    vec3 clip(const vec3& v, bool clipx = true, bool clipy = true, bool clipz = true) const;
    //�ж�v��VBOX����
    bool isInside(const vec3& v, bool clipx, bool clipy, bool clipz) const;
    bool isInside(const vec3& v) const;
    //�������,��,���
    Real height() const;
    Real width() const;
    Real depth() const;
    //mMin��mMax���������ĵ�
    vec3 center() const;
    //�����Χ
    Real area() const;
    //���
    Real volume() const;
    //���߳���
    Real longestSideLength() const;
    //ת��
    void transformed(VBOX& vbox, const mat4& mat) const;
    VBOX transformed(const mat4& mat) const;
    //һЩ���Դ���
    const vec3& minCorner() const;
    const vec3& maxCorner() const;
    void setMinCorner(Real x, Real y, Real z);
    void setMinCorner(const vec3& v);
    void setMaxCorner(Real x, Real y, Real z);
    void setMaxCorner(const vec3& v);
};


#endif

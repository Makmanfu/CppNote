
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "VBOX.h"

//ƽ��(ʹ�÷��ߺ�ԭ�㶨��ƽ��)
class Plane
{
protected:
    vec3 mNormal;                   //ԭ������(x,y,z)
    Real mOrigin;                   //���߳���
public:
    //����ԭ��ͷ��߳��ȹ���ȷ��ƽ��
    Plane(Real o = 0.0f, vec3 n = vec3(0, 0, 0));
    //���������������ֱ�ӹ���ȷ��ƽ��
    Plane(const vec3& o, const vec3& n);
public:
    //����v�㵽ԭ��ľ���(Ҳ�ǵ���ֱƽ��ľ���)
    Real distance(const vec3& v) const;
    //���߷����ж�[0:ԭ����VBOX�ڲ�,1����VBOX��ԭ���Ϸ�,-1�Ҹ�ԭ���·�]
    int classify(const VBOX&) const;
    //�ж�ԭ����Ƿ���VBOX��
    bool isOutside(const VBOX&) const;
public:
    //һЩ���Դ���
    const vec3& normal() const;
    Real origin() const;
    void setNormal(const vec3& normal);
    void setOrigin(Real origin);
};

//����(ʹ�����ĺͰ뾶��������)
class Sphere
{
protected:
    vec3 mCenter;               //����
    Real mRadius;               //�뾶
public:
    Sphere();
    Sphere(const VBOX& vbox);
    Sphere& operator=(const VBOX& vbox);
    Sphere operator+(const Sphere& other);
    const Sphere& operator+=(const Sphere& other);
public:
    //���
    void setNull();
    //������һ����
    void setPoint();
    //�뾶С��0�㶮��
    bool isNull() const;
    //ȷ�����Ǹ���
    bool isPoint() const;
    //�ж��Ƿ���� ��other
    bool includes(const Sphere& other) const;
    //ת��
    void transformed(Sphere& sphere, const mat4& mat) const;
    Sphere transformed(const mat4& mat) const;
public:
    void setCenter(const vec3& center);
    const vec3& center() const;
    void setRadius(Real radius);
    Real radius() const;
};

//ƽ��ͷ��(���������޳�Ŀ�ĵĽ�ͷ׶���ƽ�漯��)
class Frustum
{
protected:
    std::vector<Plane> mPlanes;         //ƽ�漯
public:
    //���ƽ��
    void setPlane(unsigned i, const Plane& plane);
    //����ƽ��
    const Plane& plane(unsigned i) const;
    //�ж���ƽ���ཻ
    bool cull(const Sphere& sphere) const;
    bool cull(const VBOX& vbox) const;
    bool cull(const std::vector<fvec3>& points) const;
public:
    std::vector<Plane>& planes();
    const std::vector<Plane>& planes() const;
};

//GL�ӿ�(������Ļ��ʾ������)
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
    //����GL�ӿ�����
    void activate(void) const;
    //������Ϊ0
    bool null(void);
    //ȡ�����ĵ�
    fvec2 center(void) const;
public:
    //һЩ���Դ���
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

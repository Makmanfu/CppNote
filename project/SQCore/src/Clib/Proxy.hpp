//-----------------------------------------------------------------------------
//
//  Proxy���ģʽ����
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

class Subject
{
public:
    virtual ~Subject() {};
    virtual void Request() = 0;
protected:
    Subject() {};
};

class ConcreteSubject: public Subject
{
public:
    ConcreteSubject() {};
    ~ConcreteSubject() {};
    void Request()
    {
        printf("ConcreteSubject......request....");
    };
};

class Proxy
{
private:
    Subject* _sub;
public:
    Proxy() {};
    Proxy(Subject* sub)
    {
        _sub = sub;
    };
    ~Proxy()
    {
        delete _sub;
    };
    void Request()
    {
        printf("Proxy request....");
        _sub->Request();
    };
};


int Mode_Proxy(int argc, char** argv)
{
    Subject* sub = new ConcreteSubject();
    Proxy* p = new Proxy(sub);
    p->Request();
    //p �� Request ����ʵ�����ǽ����� sub ��ʵ��ִ��
    //ʵ�����߼���ʵ�ֵĳ��׽���.
    return 1;
}



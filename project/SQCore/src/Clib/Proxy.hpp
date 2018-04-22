//-----------------------------------------------------------------------------
//
//  Proxy设计模式例子
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
    //p 的 Request 请求实际上是交给了 sub 来实际执行
    //实现了逻辑和实现的彻底解耦.
    return 1;
}



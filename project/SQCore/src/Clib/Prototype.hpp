//-----------------------------------------------------------------------------
//
//                  Prototype设计模式例子
//                                          @2017-02-14阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;

//虚拟基类,所有原型的基类,提供Clone接口函数
class Prototype
{
protected:
    Prototype() {};
public:
    virtual ~Prototype() {};
    virtual Prototype* Clone() const = 0;
};

struct ConcretePrototype : public Prototype
{
    ConcretePrototype() {};
    ~ConcretePrototype() {};
    ConcretePrototype(const ConcretePrototype& cp)
    {
        printf("ConcretePrototype1 copy \n");
    };
    Prototype* Clone() const
    {
        return new ConcretePrototype(*this);
    };
};

int Mode_Prototype(int argc, char** argv)
{
    Prototype* p = new ConcretePrototype();
    Prototype* p1 = p->Clone();
    delete p;
    delete p1;
    return 1;
}





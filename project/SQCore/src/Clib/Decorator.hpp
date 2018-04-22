//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"


class Component
{
public:
    virtual ~Component() {};
    virtual void Operation() {};
protected:
    Component() {};
};

class ConcreteComponent : public Component
{
public:
    ConcreteComponent() {};
    ~ConcreteComponent() {};
    void Operation()
    {
        std::cout << "ConcreteComponent operation..." << std::endl;
    };
};

class Decorator : public Component
{
public:
    Decorator(Component* com)
    {
        this->_com = com;
    };
    virtual ~Decorator()
    {
        delete _com;
    };
    void Operation() {};
protected:
    Component* _com;
};

class ConcreteDecorator : public Decorator
{
public:
    ConcreteDecorator(Component* com) : Decorator(com) {};
    ~ConcreteDecorator() {};
    void Operation()
    {
        _com->Operation();
        this->AddedBehavior();
    };
    void AddedBehavior()
    {
        std::cout << "ConcreteDecorator::AddedBehacior...." << std::endl;
    };
};


int Mode_Decorator(int argc, char** argv)
{
    Component*  com = new ConcreteComponent();
    Decorator*  dec = new ConcreteDecorator(com);
    dec->Operation();
    delete dec;
    return 1;
}


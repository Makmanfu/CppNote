//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

class Subsystem1
{
public:
    Subsystem1() {};
    ~Subsystem1() {};
    void Operation(void)
    {
        cout << "Subsystem1 operation.." << endl;
    };
};

class Subsystem2
{
public:
    Subsystem2() {};
    ~Subsystem2() {};
    void Operation(void)
    {
        cout << "Subsystem2 operation.." << endl;
    };
};

class Facade
{
public:
    Facade()
    {
        this->_subs1 = new Subsystem1();
        this->_subs2 = new Subsystem2();
    };
    ~Facade()
    {
        delete _subs1;
        delete _subs2;
    };
    void OperationWrapper(void)
    {
        this->_subs1->Operation();
        this->_subs2->Operation();
    };
private:
    Subsystem1* _subs1;
    Subsystem2* _subs2;
};


int Mode_Facade(int argc, char** argv)
{
    Facade* f = new Facade();
    f->OperationWrapper();
    return 1;
}






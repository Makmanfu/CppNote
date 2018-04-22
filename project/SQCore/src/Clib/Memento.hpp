//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <string>
using namespace std;

class Memento;

class Originator
{
    typedef string State;
public:
    Originator()
    {
        _sdt = "";
        _mt = 0;
    };
    Originator(const State& sdt)
    {
        _sdt = sdt;
        _mt = 0;
    };
    ~Originator() {};
    Memento* CreateMemento(void)
    {
        return new Memento(_sdt);
    };
    void SetMemento(Memento* men) {};
    void RestoreToMemento(Memento* mt)
    {
        this->_sdt = mt->GetState();
    };
    State GetState()
    {
        return _sdt;
    };
    void SetState(const State& sdt)
    {
        _sdt = sdt;
    };
    void PrintState()
    {
        cout << this->_sdt << "....." << endl;
    };
private:
    State _sdt;
    Memento* _mt;
};

class Memento
{
private:
    Memento() {};
    Memento(const State& sdt)
    {
        _sdt = sdt;
    };
    ~Memento() {};
    void SetState(const State& sdt)
    {
        _sdt = sdt;
    };
    State GetState()
    {
        return _sdt;
    };
private:
    //这是最关键的地方，将 Originator 为friend 类，可以访问内部信息，但是其他类不能访问
    friend class Originator;
    typedef string State;
    State _sdt;
};



int MementoShow(int argc, char** argv)
{
    Originator* o = new Originator();
    o->SetState("old");                 //备忘前状态
    o->PrintState();
    Memento* m = o->CreateMemento();    //将状态备忘
    o->SetState("new");                 //修改状态
    o->PrintState();
    o->RestoreToMemento(m);             //恢复修改前状态
    o->PrintState();
    return 0;
}





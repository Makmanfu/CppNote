//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
using namespace std;

class Mediator;

class Colleage
{
public:
    virtual ~Colleage() {};
    virtual void Aciton() = 0;
    virtual void SetState(const string& sdt) = 0;
    virtual string GetState() = 0;
protected:
    Colleage() {};
    Colleage(Mediator* mdt)
    {
        this->_mdt = mdt;
    };
    Mediator* _mdt;
};

class ConcreteColleageA : public Colleage
{
public:
    ConcreteColleageA() {};
    ConcreteColleageA(Mediator* mdt) : Colleage(mdt) {};
    ~ConcreteColleageA() {};
    void Aciton()
    {
        _mdt->DoActionFromAtoB();
        cout << "State of ConcreteColleageB:"
             << " " << this->GetState() << endl;
    };
    void SetState(const string& sdt)
    {
        _sdt = sdt;
    };
    string GetState()
    {
        return _sdt;
    };
private:
    string _sdt;
};

class ConcreteColleageB : public Colleage
{
public:
    ConcreteColleageB() {};
    ConcreteColleageB(Mediator* mdt) : Colleage(mdt) {};
    ~ConcreteColleageB() {};
    void Aciton()
    {
        _mdt->DoActionFromBtoA();
        cout << "State  of  ConcreteColleageB:"
             << " " << this->GetState() << endl;
    };
    void SetState(const string& sdt)
    {
        _sdt = sdt;
    };
    string GetState()
    {
        return _sdt;
    };
private:
    string _sdt;
};

class Mediator
{
public:
    virtual ~Mediator() {};
    virtual void DoActionFromAtoB() = 0;
    virtual void DoActionFromBtoA() = 0;
protected:
    Mediator() {};
};

class ConcreteMediator : public Mediator
{
public:
    ConcreteMediator() {};
    ConcreteMediator(Colleage* clgA, Colleage* clgB)
    {
        this->_clgA = clgA;
        this->_clgB = clgB;
    };
    ~ConcreteMediator() {};
    void  SetConcreteColleageA(Colleage* clgA)
    {
        this->_clgA = clgA;
    };
    void  SetConcreteColleageB(Colleage* clgB)
    {
        this->_clgB = clgB;
    };
    Colleage* GetConcreteColleageA()
    {
        return _clgA;
    };
    Colleage* GetConcreteColleageB()
    {
        return _clgB;
    };
    void  IntroColleage(Colleage* clgA, Colleage* clgB)
    {
        this->_clgA = clgA;
        this->_clgB = clgB;
    };
    void DoActionFromAtoB()
    {
        _clgB->SetState(_clgA->GetState());
    };
    void DoActionFromBtoA()
    {
        _clgA->SetState(_clgB->GetState());
    };
private:
    Colleage* _clgA;
    Colleage* _clgB;
};




int MediatorShow(int argc, char** argv)
{
    ConcreteMediator* m = new ConcreteMediator();
    ConcreteColleageA* c1 = new ConcreteColleageA(m);
    ConcreteColleageB* c2 = new ConcreteColleageB(m);
    m->IntroColleage(c1, c2);
    c1->SetState("old");
    c2->SetState("old");
    c1->Aciton();
    c2->Aciton();
    cout << endl;
    c1->SetState("new");
    c1->Aciton();
    c2->Aciton();
    cout << endl;
    c2->SetState("old");
    c2->Aciton();
    c1->Aciton();
    return 0;
}





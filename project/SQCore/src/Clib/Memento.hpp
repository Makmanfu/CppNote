//-----------------------------------------------------------------------------
//
//   ���ģʽ����
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
    //������ؼ��ĵط����� Originator Ϊfriend �࣬���Է����ڲ���Ϣ�����������಻�ܷ���
    friend class Originator;
    typedef string State;
    State _sdt;
};



int MementoShow(int argc, char** argv)
{
    Originator* o = new Originator();
    o->SetState("old");                 //����ǰ״̬
    o->PrintState();
    Memento* m = o->CreateMemento();    //��״̬����
    o->SetState("new");                 //�޸�״̬
    o->PrintState();
    o->RestoreToMemento(m);             //�ָ��޸�ǰ״̬
    o->PrintState();
    return 0;
}





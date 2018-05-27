//-----------------------------------------------------------------------------
//
//                              ���ģʽ����
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"


class Context; //ǰ������

class State
{
public:
    State() {};
    virtual ~State() {};
    virtual void OperationInterface(Context* con) = 0;
    virtual void OperationChangeState(Context* con) = 0;
protected:
    bool ChangeState(Context* con, State* st)
    {
        con->ChangeState(st);
        return true;
    };
};

class Context
{
public:
    Context() {};
    Context(State* state)
    {
        this->_state = state;
    };
    ~Context()
    {
        delete _state;
    };
    void OprationInterface()
    {
        _state->OperationInterface(this);
    };
    void OperationChangState()
    {
        _state->OperationChangeState(this);
    }
    ;
private:
    bool ChangeState(State* state)
    {
        this->_state = state;
        return true;
    };
private:
    State* _state;
    friend class State; //������ State ���п��Է��� Context ��� private �ֶ�
};

class ConcreteStateA : public State
{
public:
    ConcreteStateA() {};
    virtual ~ConcreteStateA() {};
    virtual void OperationInterface(Context* con)
    {
        cout << "ConcreteStateA::OperationInterface......" << endl;
    };
    virtual void OperationChangeState(Context* con)
    {
        OperationInterface(con);
        this->ChangeState(con, new ConcreteStateB());
    };
};

class ConcreteStateB : public State
{
public:
    ConcreteStateB() {};
    virtual ~ConcreteStateB() {};
    virtual void OperationInterface(Context* con)
    {
        cout << "ConcreteStateB::OperationInterface......" << endl;
    };
    virtual void OperationChangeState(Context* con)
    {
        OperationInterface(con);
        this->ChangeState(con, new ConcreteStateA());
    };
};

int StateShow(int argc, char** argv)
{
    State* st = new ConcreteStateA();
    Context* con = new Context(st);
    con->OprationInterface();
    con->OprationInterface();
    con->OprationInterface();
    if (con != NULL)
        delete con;
    if (st != NULL)
        st = NULL;
    return 0;
}

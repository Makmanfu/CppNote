//-----------------------------------------------------------------------------
//
//   ���ģʽ����
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"


class Strategy
{
    //������� Strategy ģʽ�Ĺؼ���Ҳ�� Strategy
    //ģʽ�� Template ģʽ�ĸ����������ڡ�
    //*Strategy ͨ������ϡ���ί�У���ʽʵ���㷨
    //��ʵ�֣����칹���� Template ģʽ���ȡ��
    //�Ǽ̳еķ�ʽ
    //*������ģʽ������Ҳ�Ǽ̳к��������ʵ
    //�ֽӿ����õķ�ʽ������
public:
    Strategy() {};
    virtual ~Strategy()
    {
        cout << "~Strategy....." << endl;
    };
    virtual void AlgrithmInterface() = 0;
protected:
private:
};

class ConcreteStrategyA : public Strategy
{
public:
    ConcreteStrategyA() {};
    virtual ~ConcreteStrategyA()
    {
        cout << "~ConcreteStrategyA....." << endl;
    };
    void AlgrithmInterface()
    {
        cout << "test ConcreteStrategyA....." << endl;
    };
};

class ConcreteStrategyB : public Strategy
{
public:
    ConcreteStrategyB() {};
    virtual ~ConcreteStrategyB()
    {
        cout << "~ConcreteStrategyB....." << endl;
    };
    void AlgrithmInterface()
    {
        cout << "test ConcreteStrategyB....." << endl;
    };
};

class Context
{
public:
    Context(Strategy* stg)
    {
        _stg = stg;
    };
    ~Context()
    {
        if (!_stg)
            delete _stg;
    };
    void DoAction()
    {
        _stg->AlgrithmInterface();
    };
private:
    Strategy* _stg;
};



int StateShow(int argc, char** argv)
{
    Strategy* ps;
    ps = new ConcreteStrategyA();
    Context* pc = new Context(ps);
    pc->DoAction();
    if (NULL != pc)
        delete pc;
    return 0;
}


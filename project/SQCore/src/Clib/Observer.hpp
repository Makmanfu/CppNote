//-----------------------------------------------------------------------------
//
//   ���ģʽ����
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <list>
#include <string>
using namespace std;
typedef string State;


class Observer;

class Subject
{
public:
    virtual ~Subject() {};
    virtual void Attach(Observer* obv)
    {
        _obvs->push_front(obv);
    };
    virtual void Detach(Observer* obv)
    {
        if (obv != NULL)
            _obvs->remove(obv);
    };
    virtual void Notify(void)
    {
        for (list<Observer*>::iterator it = _obvs->begin(); it != _obvs->end(); it++)
        {
            //����ģ��� iterator ���÷�
            (*it)->Update(this);
        }
    };
    virtual void SetState(const State& st) = 0;
    virtual State GetState(void) = 0;
protected:
    Subject()
    {
        //��ģ���ʹ��֮ǰһ��Ҫ new������
        _obvs = new list<Observer*>;
    };
private:
    list<Observer* >* _obvs;
};

class ConcreteSubject : public Subject
{
public:
    ConcreteSubject()
    {
        _st = '\0';
    };
    ~ConcreteSubject() {};
    State GetState(void)
    {
        return _st;
    };
    void SetState(const State& st)
    {
        _st = st;
    };
private:
    State _st;
};

class Observer
{
public:
    virtual ~Observer() {};
    virtual void Update(Subject* sub) = 0;
    virtual void PrintInfo(void) = 0;
protected:
    Observer()
    {
        _st = '\0';
    };
    State _st;
};

class ConcreteObserverA : public Observer
{
public:
    ConcreteObserverA(Subject* sub)
    {
        _sub = sub;
        _sub->Attach(this);
    };
    virtual ~ConcreteObserverA()
    {
        _sub->Detach(this);
        if (_sub != 0)
            delete _sub;
    };
    virtual Subject* GetSubject()
    {
        return _sub;
    };
    //���� Subject ��Ϊ����������������һ��View ���ڶ���� Subject��
    void Update(Subject* sub)
    {
        _st = sub->GetState();
        PrintInfo();
    };
    void PrintInfo(void)
    {
        cout << "ConcreteObserverA  observer...."
             << _sub->GetState() << endl;
    };
private:
    Subject* _sub;
};

class ConcreteObserverB : public Observer
{
public:
    ConcreteObserverB(Subject* sub)
    {
        _sub = sub;
        _sub->Attach(this);
    }    ;
    virtual ~ConcreteObserverB()
    {
        _sub->Detach(this);
        if (_sub != 0)
            delete _sub;
    };
    virtual Subject* GetSubject()
    {
        return _sub;
    };
    //���� Subject ��Ϊ����������������һ��View ���ڶ���� Subject��
    void Update(Subject* sub)
    {
        _st = sub->GetState();
        PrintInfo();
    };
    void PrintInfo(void)
    {
        cout << "ConcreteObserverB  observer...."
             << _sub->GetState() << endl;
    };
private:
    Subject* _sub;
};


int ObserverShow(int argc, char** argv)
{
    ConcreteSubject* sub = new ConcreteSubject();
    Observer* o1 = new ConcreteObserverA(sub);
    Observer* o2 = new ConcreteObserverB(sub);
    sub->SetState("old");
    sub->Notify();
    sub->SetState("new");  // Ҳ������Observer����
    sub->Notify();
    return 0;
}




//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

class Flyweight
{
public:
    virtual ~Flyweight() {};
    virtual void Operation(const string& extrinsicState) {};
    string GetIntrinsicState()
    {
        return this->_intrinsicState;
    };
protected:
    Flyweight(string intrinsicState)
    {
        this->_intrinsicState = intrinsicState;
    };
private:
    string _intrinsicState;
};

class ConcreteFlyweight : public Flyweight
{
public:
    ConcreteFlyweight(string intrinsicState) : Flyweight(intrinsicState)
    {
        cout << "ConcreteFlyweight Build....." << intrinsicState << endl;
    };
    ~ConcreteFlyweight() {};
    void  Operation(const string& extrinsicState)
    {
        cout << "ConcreteFlyweight: 内 蕴 ["
             << this->GetIntrinsicState()
             << "]  外 蕴  ["
             << extrinsicState << "]" << endl;
    };
};

class FlyweightFactory
{
public:
    FlyweightFactory() {};
    ~FlyweightFactory() {};
    Flyweight* GetFlyweight(const string& key)
    {
        vector<Flyweight*>::iterator  it = _fly.begin();
        for (; it != _fly.end(); it++)
        {
            //找到了，就一起用，^_^
            if ((*it)->GetIntrinsicState() == key)
            {
                cout << "already  created  by users...." << endl;
                return *it;
            }
        }
        Flyweight* fn = new ConcreteFlyweight(key);
        _fly.push_back(fn);
        return fn;
    };
private:
    vector<Flyweight*> _fly;
};


int Mode_Flyweight(int argc, char** argv)
{
    FlyweightFactory* fc = new FlyweightFactory();
    Flyweight*  fw1 = fc->GetFlyweight("hello");
    Flyweight*  fw2 = fc->GetFlyweight("world!");
    Flyweight*  fw3 = fc->GetFlyweight("hello");
    return 1;
}







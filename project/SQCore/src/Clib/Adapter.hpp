//-----------------------------------------------------------------------------
//
//                  Adapter设计模式例子
//                                          @2017-02-14阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;

//需要被Adapt的类
struct Target
{
    Target() {};
    virtual ~Target() {};
    virtual void Request()
    {
        printf("Target::Request\n");
    };
};

//与被Adapt对象提供不兼容接口的类
struct Adaptee
{
    Adaptee() {};
    virtual ~Adaptee() {};
    void SpecificRequest()
    {
        printf("Adaptee::SpecificRequest\n");
    };
};

//多重继承(组合)也能聚合原有接口类的方式
struct Adapter : public Target, private Adaptee
{
    Adapter() {};
    virtual ~Adapter() {};
    virtual void Request()
    {
        this->SpecificRequest();
    };
};

//进行Adapt的类,采用继承原有接口类的方式
class AdapterTE : public Target
{
private:
    Adaptee* m_pAdptee;
public:
    AdapterTE(Adaptee* pAdaptee) : m_pAdptee(pAdaptee) {};
    virtual ~AdapterTE()
    {
        delete m_pAdptee;
        m_pAdptee = NULL;
    };
    virtual void Request()
    {
        m_pAdptee->SpecificRequest();
    };
};

//适配器模式实例========================================
class Player
{
protected:
    string name;
public:
    Player(string strName)
    {
        name = strName;
    };
    virtual void Attack() = 0;
    virtual void Defense() = 0;
};

struct Forwards : public Player
{
    Forwards(string strName) : Player(strName) {};
    virtual void Attack()
    {
        printf("前锋进攻\n");
    };
    virtual void Defense()
    {
        printf("前锋防守\n");
    };
};

struct Center : public Player
{
    Center(string strName) : Player(strName) {};
    virtual void Attack()
    {
        printf("中场进攻\n");
    };
    virtual void Defense()
    {
        printf("中场防守\n");
    };
};

//为中场翻译
class TransLater : public Player
{
private:
    Center* player;
public:
    TransLater(string strName) : Player(strName)
    {
        player = new Center(strName);
    };
    virtual void Attack()
    {
        player->Attack();
    };
    virtual void Defense()
    {
        player->Defense();
    };
};


int Mode_Adapter(int argc, char** argv)
{
    Adaptee* pAdaptee = new Adaptee;
    Target* pTarget = new AdapterTE(pAdaptee);
    pTarget->Request();
    //Adapter* pAdapter = new Adapter();
    //pAdapter->Request();
    printf("实例演示!============\n");
    Player* p = new TransLater("小李");
    p->Attack();
    return 1;
}






//-----------------------------------------------------------------------------
//
//                  Builder设计模式例子
//                                          @2017-02-14阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

//虚拟基类,是所有Builder的基类,提供不同部分的构建接口函数
struct Builder
{
    Builder() {};
    virtual ~Builder() {};
    virtual void BuilderPartA() = 0;
    virtual void BuilderPartB() = 0;
};

//使用Builder构建产品,构建产品的过程都一致,但是不同的builder有不同的实现
//这个不同的实现通过不同的Builder派生类来实现,存有一个Builder的指针,通过这个来实现多态调用
class Director
{
private:
    Builder* m_pBuilder;
public:
    Director(Builder* pBuilder) : m_pBuilder(pBuilder) {};
    ~Director()
    {
        delete m_pBuilder;
        m_pBuilder = NULL;
    };
    //Construct函数表示一个对象的整个构建过程,不同的部分之间的装配方式都是一致的,
    //首先构建PartA其次是PartB,只是根据不同的构建者会有不同的表示
    void Construct()
    {
        m_pBuilder->BuilderPartA();
        m_pBuilder->BuilderPartB();
    };
};

struct ConcreateBuilder1 : public Builder
{
    ConcreateBuilder1() {};
    virtual ~ConcreateBuilder1() {};

    virtual void BuilderPartA()
    {
        printf("BuilderPartA by ConcreateBuilder1\n");
    };
    virtual void BuilderPartB()
    {
        printf("BuilderPartB by ConcreateBuilder1\n");
    };
};

struct ConcreateBuilder2 : public Builder
{
    ConcreateBuilder2() {};
    virtual ~ConcreateBuilder2() {};

    virtual void BuilderPartA()
    {
        printf("BuilderPartA by ConcreateBuilder2\n");
    };
    virtual void BuilderPartB()
    {
        printf("BuilderPartA by ConcreateBuilder2\n");
    };
};


//建造者类
struct Person
{
    virtual void CreateHead() = 0;
    virtual void CreateHand() = 0;
    virtual void CreateBody() = 0;
    virtual void CreateFoot() = 0;
};

struct ThinPerson : public Person
{
    virtual void CreateHead()
    {
        printf("thin head\n");
    };
    virtual void CreateHand()
    {
        printf("thin hand\n");
    }
    virtual void CreateBody()
    {
        printf("thin body\n");
    };
    virtual void CreateFoot()
    {
        printf("thin foot\n");
    };
};

struct ThickPerson : public Person
{
    virtual void CreateHead()
    {
        printf("ThickPerson head\n");
    };
    virtual void CreateHand()
    {
        printf("ThickPerson hand\n");
    };
    virtual void CreateBody()
    {
        printf("ThickPerson body\n");
    };
    virtual void CreateFoot()
    {
        printf("ThickPerson foot\n");
    };
};

//指挥者类
class Direct
{
private:
    Person* p;
public:
    Direct(Person* temp)
    {
        p = temp;
    };
    virtual ~Direct() {};
    void Create()
    {
        p->CreateHead();
        p->CreateBody();
        p->CreateHand();
        p->CreateFoot();
    };
};



int Mode_Builder(int argc, char** argv)
{
    Builder* pBuilder1 = new ConcreateBuilder1;
    Director* pDirector1 = new Director(pBuilder1);
    pDirector1->Construct();
    Builder* pBuilder2 = new ConcreateBuilder2;
    Director* pDirector2 = new Director(pBuilder2);
    pDirector2->Construct();
    delete pDirector1;
    delete pDirector2;
    //注意下面是多余的因为Director浅拷贝已经处理了
    //delete pBuilder1;
    //delete pBuilder2;
    printf("实例演示!============\n");
    Person* p = new ThickPerson();
    Direct* d = new Direct(p);
    d->Create();
    delete d;
    delete p;
    return 1;
}
//-----------------------------------------------------------------------------
//
//                  Builder���ģʽ����
//                                          @2017-02-14��������
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

//�������,������Builder�Ļ���,�ṩ��ͬ���ֵĹ����ӿں���
struct Builder
{
    Builder() {};
    virtual ~Builder() {};
    virtual void BuilderPartA() = 0;
    virtual void BuilderPartB() = 0;
};

//ʹ��Builder������Ʒ,������Ʒ�Ĺ��̶�һ��,���ǲ�ͬ��builder�в�ͬ��ʵ��
//�����ͬ��ʵ��ͨ����ͬ��Builder��������ʵ��,����һ��Builder��ָ��,ͨ�������ʵ�ֶ�̬����
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
    //Construct������ʾһ�������������������,��ͬ�Ĳ���֮���װ�䷽ʽ����һ�µ�,
    //���ȹ���PartA�����PartB,ֻ�Ǹ��ݲ�ͬ�Ĺ����߻��в�ͬ�ı�ʾ
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


//��������
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

//ָ������
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
    //ע�������Ƕ������ΪDirectorǳ�����Ѿ�������
    //delete pBuilder1;
    //delete pBuilder2;
    printf("ʵ����ʾ!============\n");
    Person* p = new ThickPerson();
    Direct* d = new Direct(p);
    d->Create();
    delete d;
    delete p;
    return 1;
}
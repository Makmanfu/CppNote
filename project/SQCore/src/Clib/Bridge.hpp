//-----------------------------------------------------------------------------
//
//                  Bridge设计模式例子
//                                          @2017-02-14阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

class Abstraction
{
protected:
    Abstraction() {};
public:
    virtual ~Abstraction() {};
    virtual void Operation() = 0;
};

//为实现Abstraction定义的抽象基类,定义了实现的接口函数
class Implementor
{
protected:
    Implementor() {};
public:
    virtual ~Implementor() {};
    virtual void Operation() = 0;
};

//维护一个Implementor类的指针
class RefinedAbstraction: public Abstraction
{
private:
    Implementor* m_pImp;
public:
    RefinedAbstraction(Implementor* pImp) : m_pImp(pImp) {};
    ~RefinedAbstraction()
    {
        delete m_pImp;
        m_pImp = NULL;
    };
    void Operation()
    {
        m_pImp->Operation();
    };
};

//继承自Implementor,是Implementor的不同实现之一
struct ConcreateImplementorA : public Implementor
{
    ConcreateImplementorA() {};
    virtual ~ConcreateImplementorA() {};
    virtual void Operation()
    {
        printf("ConcreateImplementorA\n");
    };
};

//继承自Implementor,是Implementor的不同实现之一
struct ConcreateImplementorB : public Implementor
{
    ConcreateImplementorB() {};
    ~ConcreateImplementorB() {};
    virtual void Operation()
    {
        printf("ConcreateImplementorB\n");
    };
};

//桥接模式实例========================================
//画刷接口
class BrushPenAbstraction
{
private:
    string m_color;
protected:
    BrushPenAbstraction() {};
public:
    virtual ~BrushPenAbstraction() {};
    //绘制过程 虚接口
    virtual void operationDraw() = 0;
};

//着色接口
class ImplementorColor
{
protected:
    ImplementorColor() {};
public:
    virtual ~ImplementorColor() {};
    //着色过程 虚接口
    virtual void bepaint() = 0;
};

//大笔
class BigBrushPenRefinedAbstraction : public BrushPenAbstraction
{
private:
    ImplementorColor* _imp;
public:
    BigBrushPenRefinedAbstraction(ImplementorColor* imp)
    {
        _imp = imp;
    };
    ~BigBrushPenRefinedAbstraction() {};
    void operationDraw()
    {
        printf("大笔\n");
        _imp->bepaint();
    };
};

//小笔
class SmallBrushPenRefinedAbstraction : public BrushPenAbstraction
{
private:
    ImplementorColor* _imp;
public:
    SmallBrushPenRefinedAbstraction(ImplementorColor* imp)
    {
        _imp = imp;
    };
    ~SmallBrushPenRefinedAbstraction() {};
    void operationDraw()
    {
        printf("小笔\n");
        _imp->bepaint();
    };
};

//红色
struct oncreteImplementorRed : public ImplementorColor
{
    oncreteImplementorRed() {};
    ~oncreteImplementorRed() {};
    virtual void bepaint()
    {
        printf("红色\n");
    };
};

//绿色
struct oncreteImplementorGreen : public ImplementorColor
{
    oncreteImplementorGreen() {};
    ~oncreteImplementorGreen() {};
    virtual void bepaint()
    {
        printf("绿色\n");
    };
};


//手机软件
struct HandsetSoft
{
    virtual void Run() = 0;
};

//游戏软件
struct HandsetGame : public HandsetSoft
{
    virtual void Run()
    {
        printf("运行手机游戏\n");
    };
};

//通讯录软件
struct HandSetAddressList : public HandsetSoft
{
    virtual void Run()
    {
        printf("手机通讯录\n");
    };
};

//手机品牌
struct HandsetBrand
{
protected:
    HandsetSoft* m_soft;
public:
    void SetHandsetSoft(HandsetSoft* temp)
    {
        m_soft = temp;
    };
    virtual void Run() = 0;
};

//M 品牌
struct HandsetBrandM : public HandsetBrand
{
    virtual void Run()
    {
        m_soft->Run();
    };
};

//N 品牌
struct HandsetBrandN : public HandsetBrand
{
    virtual void Run()
    {
        m_soft->Run();
    };
};



int Mode_Bridge(int argc, char** argv)
{
    Implementor* pImplA = new ConcreateImplementorA();
    Abstraction* pAbs1 = new RefinedAbstraction(pImplA);
    pAbs1->Operation();
    Implementor* pImplB = new ConcreateImplementorB();
    Abstraction* pAbs2 = new RefinedAbstraction(pImplB);
    pAbs2->Operation();
    delete pAbs1;
    delete pAbs2;
    printf("实例演示1!============\n");
    //创建红色
    ImplementorColor* pcolor = new oncreteImplementorRed();
    //创建小笔
    BrushPenAbstraction* pen = new SmallBrushPenRefinedAbstraction(pcolor);
    //调用绘制出小笔红色
    pen->operationDraw();
    //同理绘制出大笔绿色
    BrushPenAbstraction* pen2 = new BigBrushPenRefinedAbstraction(new oncreteImplementorGreen());
    pen2->operationDraw();
    printf("实例演示2!============\n");
    HandsetBrand* brand;
    brand = new HandsetBrandM();
    brand->SetHandsetSoft(new HandsetGame());
    brand->Run();
    brand->SetHandsetSoft(new HandSetAddressList());
    brand->Run();
    return 1;
}



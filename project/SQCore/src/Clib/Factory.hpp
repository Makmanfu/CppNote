//-----------------------------------------------------------------------------
//
//                  Factory设计模式例子
//                                          @2017-02-14阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

//产品基类需要继承才能创建
class Product
{
protected:
    //封装产品类构造函数 可以在子类继承后构造
    Product() {};
public:
    virtual ~Product() {};
};

//继承产品基类
struct ConcreteProduct : public Product
{
    ConcreteProduct()
    {
        printf("ConcreteProduct(创建产品)\n");
    };
    virtual ~ConcreteProduct() {};
};

//工厂基类
class IFactory
{
protected:
    IFactory() {};
public:
    virtual ~IFactory() {};
    virtual Product* CreateProduct() = 0;
};

//工厂管理类
struct ConcreteFactory : public IFactory
{
    ConcreteFactory()
    {
        printf("ConcreteFactory(创建工厂管理)\n");
    };
    ~ConcreteFactory() {};
    Product* CreateProduct()
    {
        return new ConcreteProduct();
    };
};


//案列演示======================================================================

class IOperation
{
protected:
    int m_nFirst;
    int m_nSecond;
protected:
    //封装产品类构造函数 可以在子类继承后构造
    IOperation() : m_nFirst(0), m_nSecond(0) {};
public:
    virtual ~IOperation() {};
    void SetKeyValue(int a, int b)
    {
        m_nFirst = a;
        m_nSecond = b;
    };
    virtual double GetResult() = 0;
};

struct AddOperation : public IOperation
{
    virtual double GetResult()
    {
        return m_nFirst + m_nSecond;
    };
};

struct SubOperation : public IOperation
{
    virtual double GetResult()
    {
        return m_nFirst - m_nSecond;
    };
};

class IFactoryc
{
protected:
    IFactoryc() {};
public:
    virtual ~IFactoryc() {};
    virtual IOperation* CreateProduct(char cOperator) = 0;
};

struct CCalculatorFactory : public IFactoryc
{
    CCalculatorFactory() {};
    ~CCalculatorFactory() {};
    IOperation* CreateProduct(char cOperator)
    {
        IOperation* oper;
        switch (cOperator)
        {
            case '+':
                printf("++++创建加法对象++++++++++++++++++++\n");
                oper = new AddOperation();
                break;
            case '-':
                printf("----创建减法对象--------------------\n");
                oper = new SubOperation();
                break;
            default:
                oper = new AddOperation();
                break;
        }
        return oper;
    };
};


//演示
int Mode_Factory(int argc, char** argv)
{
    //创建工厂类
    IFactory* fac = new ConcreteFactory();
    //创建产品类
    Product* p = fac->CreateProduct();
    delete p;
    delete fac;
    //输出结果 :
    //ConcreteFactory......
    //ConcreteProduct......
    printf("实例演示!============\n");
    //创建工厂
    IFactoryc* pFac = new CCalculatorFactory();
    //产品对象指针
    IOperation* pOP = NULL;
    //加法
    pOP = pFac->CreateProduct('+');
    pOP->SetKeyValue(5, 3);
    std::cout << pOP->GetResult() << std::endl;
    //减法
    pOP = pFac->CreateProduct('-');
    pOP->SetKeyValue(5, 3);
    std::cout << pOP->GetResult() << std::endl;
    delete pOP;
    delete pFac;
    return 1;
}



//-----------------------------------------------------------------------------
//
//                  AbstactFactory设计模式例子
//                                          @2017-02-14阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

//产品A的抽象
class AbstractProductA
{
protected:
    AbstractProductA() {};
public:
    virtual ~AbstractProductA() {};
};

//产品B的抽象
class AbstractProductB
{
protected:
    AbstractProductB() {};
public:
    virtual ~AbstractProductB() {};
};

//产品A1
struct ProductA1 : public AbstractProductA
{
    ProductA1()
    {
        printf("ProductA1(产品A1)\n");
    };
    ~ProductA1() {};
};

//产品A2
struct ProductA2 : public AbstractProductA
{
    ProductA2()
    {
        printf("ProductA2(产品A2)\n");
    };
    ~ProductA2() {};
};

//产品B1
struct ProductB1 : public AbstractProductB
{
    ProductB1()
    {
        printf("ProductB1(产品B1)\n");
    };
    ~ProductB1() {};
};

//产品B2
struct ProductB2 : public AbstractProductB
{
    ProductB2()
    {
        printf("ProductB2(产品B2)\n");
    };
    ~ProductB2() {};
};

//抽象工厂基类,生产产品A和产品B
class AbstractFactory
{
protected:
    AbstractFactory() {};
public:
    virtual ~AbstractFactory() {};
    virtual  AbstractProductA* CreateProductA() = 0;
    virtual  AbstractProductB* CreateProductB() = 0;
};

//生产产品A和产品B的第一种实现
struct ConcreteFactory1 : public AbstractFactory
{
    ConcreteFactory1() {};
    ~ConcreteFactory1() {};
    AbstractProductA* CreateProductA()
    {
        return new ProductA1();
    };
    AbstractProductB* CreateProductB()
    {
        return new ProductB1();
    };
};

//生产产品A和产品B的第二种实现
struct ConcreteFactory2 : public AbstractFactory
{
    ConcreteFactory2() {};
    ~ConcreteFactory2() {};
    AbstractProductA* CreateProductA()
    {
        return new ProductA2();
    };
    AbstractProductB* CreateProductB()
    {
        return new ProductB2();
    };
};


//==============================案列演示========================================
//用户抽象接口
struct IUser
{
    virtual void GetUser() = 0;
    virtual void InsertUser() = 0;
};

//部门抽象接口
struct IDepartment
{
    virtual void GetDepartment() = 0;
    virtual void InsertDepartment() = 0;
};

//ACCESS 用户
struct CAccessUser : public IUser
{
    virtual void GetUser()
    {
        printf("Access GetUser(获取Access用户)\n");
    };
    virtual void InsertUser()
    {
        printf("Access InsertUser(插入Access用户)\n");
    };
};

//ACCESS 部门
struct CAccessDepartment : public IDepartment
{
    virtual void GetDepartment()
    {
        printf("Access GetDepartment(获得Access部门)\n");
    };
    virtual void InsertDepartment()
    {
        printf("Access InsertDepartment(插入Access部门)\n");
    };
};

//SQL 用户
struct CSqlUser : public IUser
{
    virtual void GetUser()
    {
        printf("Sql User(获取Sql用户)\n");
    };
    virtual void InsertUser()
    {
        printf("Sql InsertUser(插入Sql用户)\n");
    };
};

//SQL 部门类
struct CSqlDepartment : public IDepartment
{
    virtual void GetDepartment()
    {
        printf("sql getDepartment(获得sql部门)\n");
    };
    virtual void InsertDepartment()
    {
        printf("sql insertdepartment(插入sql部门)\n");
    };
};

//抽象工厂
struct IFactoryA
{
    virtual IUser* CreateUser() = 0;
    virtual IDepartment* CreateDepartment() = 0;
};

//ACCESS 工厂
struct AccessFactory : public IFactoryA
{
    virtual IUser* CreateUser()
    {
        return new  CAccessUser();
    };
    virtual IDepartment* CreateDepartment()
    {
        return new CAccessDepartment();
    };
};

//SQL 工厂
struct SqlFactory : public IFactoryA
{
    virtual IUser* CreateUser()
    {
        return new  CSqlUser();
    };
    virtual IDepartment* CreateDepartment()
    {
        return new CSqlDepartment();
    };
};


int Mode_AbstactFac(int argc, char** argv)
{
    AbstractFactory* cf1 = new ConcreteFactory1();
    cf1->CreateProductA();  //生产产品A的第一种实现
    cf1->CreateProductB();  //生产产品B的第一种实现
    delete cf1;
    AbstractFactory* cf2 = new ConcreteFactory2();
    AbstractProductA* pProductA = cf2->CreateProductA();    //生产产品A的第二种实现
    AbstractProductB* pProductB = cf2->CreateProductB();    //生产产品B的第二种实现
    delete pProductB;
    delete pProductA;
    delete cf2;
    printf("实例演示!============\n");
    //可以实现换数据库的功能 (为了增强理解加入强制类型转换语意)
    //创建需要的工厂类型
    IFactoryA* Acc = new AccessFactory();
    IFactoryA* Sql = new SqlFactory();
    IUser* user = NULL;
    IDepartment* depart = NULL;
    //操作acc数据库
    user = (CAccessUser*)Acc->CreateUser();
    depart = (CAccessDepartment*)Acc->CreateDepartment();
    user->GetUser();
    depart->GetDepartment();
    //操作sql数据库
    user = (CSqlUser*)Sql->CreateUser();
    depart = (CSqlDepartment*)Sql->CreateDepartment();
    user->GetUser();
    depart->GetDepartment();
    //删除指针
    delete depart;
    delete user;
    delete Acc;
    delete Sql;
    return 1;
}




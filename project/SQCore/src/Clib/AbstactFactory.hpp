//-----------------------------------------------------------------------------
//
//                  AbstactFactory���ģʽ����
//                                          @2017-02-14��������
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

//��ƷA�ĳ���
class AbstractProductA
{
protected:
    AbstractProductA() {};
public:
    virtual ~AbstractProductA() {};
};

//��ƷB�ĳ���
class AbstractProductB
{
protected:
    AbstractProductB() {};
public:
    virtual ~AbstractProductB() {};
};

//��ƷA1
struct ProductA1 : public AbstractProductA
{
    ProductA1()
    {
        printf("ProductA1(��ƷA1)\n");
    };
    ~ProductA1() {};
};

//��ƷA2
struct ProductA2 : public AbstractProductA
{
    ProductA2()
    {
        printf("ProductA2(��ƷA2)\n");
    };
    ~ProductA2() {};
};

//��ƷB1
struct ProductB1 : public AbstractProductB
{
    ProductB1()
    {
        printf("ProductB1(��ƷB1)\n");
    };
    ~ProductB1() {};
};

//��ƷB2
struct ProductB2 : public AbstractProductB
{
    ProductB2()
    {
        printf("ProductB2(��ƷB2)\n");
    };
    ~ProductB2() {};
};

//���󹤳�����,������ƷA�Ͳ�ƷB
class AbstractFactory
{
protected:
    AbstractFactory() {};
public:
    virtual ~AbstractFactory() {};
    virtual  AbstractProductA* CreateProductA() = 0;
    virtual  AbstractProductB* CreateProductB() = 0;
};

//������ƷA�Ͳ�ƷB�ĵ�һ��ʵ��
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

//������ƷA�Ͳ�ƷB�ĵڶ���ʵ��
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


//==============================������ʾ========================================
//�û�����ӿ�
struct IUser
{
    virtual void GetUser() = 0;
    virtual void InsertUser() = 0;
};

//���ų���ӿ�
struct IDepartment
{
    virtual void GetDepartment() = 0;
    virtual void InsertDepartment() = 0;
};

//ACCESS �û�
struct CAccessUser : public IUser
{
    virtual void GetUser()
    {
        printf("Access GetUser(��ȡAccess�û�)\n");
    };
    virtual void InsertUser()
    {
        printf("Access InsertUser(����Access�û�)\n");
    };
};

//ACCESS ����
struct CAccessDepartment : public IDepartment
{
    virtual void GetDepartment()
    {
        printf("Access GetDepartment(���Access����)\n");
    };
    virtual void InsertDepartment()
    {
        printf("Access InsertDepartment(����Access����)\n");
    };
};

//SQL �û�
struct CSqlUser : public IUser
{
    virtual void GetUser()
    {
        printf("Sql User(��ȡSql�û�)\n");
    };
    virtual void InsertUser()
    {
        printf("Sql InsertUser(����Sql�û�)\n");
    };
};

//SQL ������
struct CSqlDepartment : public IDepartment
{
    virtual void GetDepartment()
    {
        printf("sql getDepartment(���sql����)\n");
    };
    virtual void InsertDepartment()
    {
        printf("sql insertdepartment(����sql����)\n");
    };
};

//���󹤳�
struct IFactoryA
{
    virtual IUser* CreateUser() = 0;
    virtual IDepartment* CreateDepartment() = 0;
};

//ACCESS ����
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

//SQL ����
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
    cf1->CreateProductA();  //������ƷA�ĵ�һ��ʵ��
    cf1->CreateProductB();  //������ƷB�ĵ�һ��ʵ��
    delete cf1;
    AbstractFactory* cf2 = new ConcreteFactory2();
    AbstractProductA* pProductA = cf2->CreateProductA();    //������ƷA�ĵڶ���ʵ��
    AbstractProductB* pProductB = cf2->CreateProductB();    //������ƷB�ĵڶ���ʵ��
    delete pProductB;
    delete pProductA;
    delete cf2;
    printf("ʵ����ʾ!============\n");
    //����ʵ�ֻ����ݿ�Ĺ��� (Ϊ����ǿ������ǿ������ת������)
    //������Ҫ�Ĺ�������
    IFactoryA* Acc = new AccessFactory();
    IFactoryA* Sql = new SqlFactory();
    IUser* user = NULL;
    IDepartment* depart = NULL;
    //����acc���ݿ�
    user = (CAccessUser*)Acc->CreateUser();
    depart = (CAccessDepartment*)Acc->CreateDepartment();
    user->GetUser();
    depart->GetDepartment();
    //����sql���ݿ�
    user = (CSqlUser*)Sql->CreateUser();
    depart = (CSqlDepartment*)Sql->CreateDepartment();
    user->GetUser();
    depart->GetDepartment();
    //ɾ��ָ��
    delete depart;
    delete user;
    delete Acc;
    delete Sql;
    return 1;
}




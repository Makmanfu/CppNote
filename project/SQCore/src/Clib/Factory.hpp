//-----------------------------------------------------------------------------
//
//                  Factory���ģʽ����
//                                          @2017-02-14��������
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

//��Ʒ������Ҫ�̳в��ܴ���
class Product
{
protected:
    //��װ��Ʒ�๹�캯�� ����������̳к���
    Product() {};
public:
    virtual ~Product() {};
};

//�̳в�Ʒ����
struct ConcreteProduct : public Product
{
    ConcreteProduct()
    {
        printf("ConcreteProduct(������Ʒ)\n");
    };
    virtual ~ConcreteProduct() {};
};

//��������
class IFactory
{
protected:
    IFactory() {};
public:
    virtual ~IFactory() {};
    virtual Product* CreateProduct() = 0;
};

//����������
struct ConcreteFactory : public IFactory
{
    ConcreteFactory()
    {
        printf("ConcreteFactory(������������)\n");
    };
    ~ConcreteFactory() {};
    Product* CreateProduct()
    {
        return new ConcreteProduct();
    };
};


//������ʾ======================================================================

class IOperation
{
protected:
    int m_nFirst;
    int m_nSecond;
protected:
    //��װ��Ʒ�๹�캯�� ����������̳к���
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
                printf("++++�����ӷ�����++++++++++++++++++++\n");
                oper = new AddOperation();
                break;
            case '-':
                printf("----������������--------------------\n");
                oper = new SubOperation();
                break;
            default:
                oper = new AddOperation();
                break;
        }
        return oper;
    };
};


//��ʾ
int Mode_Factory(int argc, char** argv)
{
    //����������
    IFactory* fac = new ConcreteFactory();
    //������Ʒ��
    Product* p = fac->CreateProduct();
    delete p;
    delete fac;
    //������ :
    //ConcreteFactory......
    //ConcreteProduct......
    printf("ʵ����ʾ!============\n");
    //��������
    IFactoryc* pFac = new CCalculatorFactory();
    //��Ʒ����ָ��
    IOperation* pOP = NULL;
    //�ӷ�
    pOP = pFac->CreateProduct('+');
    pOP->SetKeyValue(5, 3);
    std::cout << pOP->GetResult() << std::endl;
    //����
    pOP = pFac->CreateProduct('-');
    pOP->SetKeyValue(5, 3);
    std::cout << pOP->GetResult() << std::endl;
    delete pOP;
    delete pFac;
    return 1;
}



//-----------------------------------------------------------------------------
//
//                  Bridge���ģʽ����
//                                          @2017-02-14��������
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

//Ϊʵ��Abstraction����ĳ������,������ʵ�ֵĽӿں���
class Implementor
{
protected:
    Implementor() {};
public:
    virtual ~Implementor() {};
    virtual void Operation() = 0;
};

//ά��һ��Implementor���ָ��
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

//�̳���Implementor,��Implementor�Ĳ�ͬʵ��֮һ
struct ConcreateImplementorA : public Implementor
{
    ConcreateImplementorA() {};
    virtual ~ConcreateImplementorA() {};
    virtual void Operation()
    {
        printf("ConcreateImplementorA\n");
    };
};

//�̳���Implementor,��Implementor�Ĳ�ͬʵ��֮һ
struct ConcreateImplementorB : public Implementor
{
    ConcreateImplementorB() {};
    ~ConcreateImplementorB() {};
    virtual void Operation()
    {
        printf("ConcreateImplementorB\n");
    };
};

//�Ž�ģʽʵ��========================================
//��ˢ�ӿ�
class BrushPenAbstraction
{
private:
    string m_color;
protected:
    BrushPenAbstraction() {};
public:
    virtual ~BrushPenAbstraction() {};
    //���ƹ��� ��ӿ�
    virtual void operationDraw() = 0;
};

//��ɫ�ӿ�
class ImplementorColor
{
protected:
    ImplementorColor() {};
public:
    virtual ~ImplementorColor() {};
    //��ɫ���� ��ӿ�
    virtual void bepaint() = 0;
};

//���
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
        printf("���\n");
        _imp->bepaint();
    };
};

//С��
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
        printf("С��\n");
        _imp->bepaint();
    };
};

//��ɫ
struct oncreteImplementorRed : public ImplementorColor
{
    oncreteImplementorRed() {};
    ~oncreteImplementorRed() {};
    virtual void bepaint()
    {
        printf("��ɫ\n");
    };
};

//��ɫ
struct oncreteImplementorGreen : public ImplementorColor
{
    oncreteImplementorGreen() {};
    ~oncreteImplementorGreen() {};
    virtual void bepaint()
    {
        printf("��ɫ\n");
    };
};


//�ֻ����
struct HandsetSoft
{
    virtual void Run() = 0;
};

//��Ϸ���
struct HandsetGame : public HandsetSoft
{
    virtual void Run()
    {
        printf("�����ֻ���Ϸ\n");
    };
};

//ͨѶ¼���
struct HandSetAddressList : public HandsetSoft
{
    virtual void Run()
    {
        printf("�ֻ�ͨѶ¼\n");
    };
};

//�ֻ�Ʒ��
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

//M Ʒ��
struct HandsetBrandM : public HandsetBrand
{
    virtual void Run()
    {
        m_soft->Run();
    };
};

//N Ʒ��
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
    printf("ʵ����ʾ1!============\n");
    //������ɫ
    ImplementorColor* pcolor = new oncreteImplementorRed();
    //����С��
    BrushPenAbstraction* pen = new SmallBrushPenRefinedAbstraction(pcolor);
    //���û��Ƴ�С�ʺ�ɫ
    pen->operationDraw();
    //ͬ����Ƴ������ɫ
    BrushPenAbstraction* pen2 = new BigBrushPenRefinedAbstraction(new oncreteImplementorGreen());
    pen2->operationDraw();
    printf("ʵ����ʾ2!============\n");
    HandsetBrand* brand;
    brand = new HandsetBrandM();
    brand->SetHandsetSoft(new HandsetGame());
    brand->Run();
    brand->SetHandsetSoft(new HandSetAddressList());
    brand->Run();
    return 1;
}



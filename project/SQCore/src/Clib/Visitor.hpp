//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"


class Visitor;

class Element
{
public:
    virtual ~Element() {}
    virtual void Accept(Visitor& rVisitor) = 0;
protected:
    Element() {}
};

class ConcreateElementA: public Element
{
public:
    virtual ~ConcreateElementA() {};
    virtual void Accept(Visitor& rVisitor)
    {
        rVisitor.VisitConcreateElementA(this);
    };
};

class ConcreateElementB : public Element
{
public:
    virtual ~ConcreateElementB() {};
    virtual void Accept(Visitor& rVisitor)
    {
        rVisitor.VisitConcreateElementB(this);
    };
};

class Visitor
{
public:
    virtual ~Visitor() {};
    virtual void VisitConcreateElementA(ConcreateElementA* pConcreateElementA) = 0;
    virtual void VisitConcreateElementB(ConcreateElementB* pConcreateElementB) = 0;
protected:
    Visitor() {};
};

class ConcreateVisitorA: public Visitor
{
public:
    virtual ~ConcreateVisitorA() {};
    virtual void VisitConcreateElementA(ConcreateElementA* pConcreateElementA)
    {
        std::cout << "VisitConcreateElementA By ConcreateVisitorA\n";
    };
    virtual void VisitConcreateElementB(ConcreateElementB* pConcreateElementB)
    {
        std::cout << "VisitConcreateElementB By ConcreateVisitorA\n";
    };
};

class ConcreateVisitorB: public Visitor
{
public:
    virtual ~ConcreateVisitorB() {};
    virtual void VisitConcreateElementA(ConcreateElementA* pConcreateElementA)
    {
        std::cout << "VisitConcreateElementA By ConcreateVisitorB\n";
    };
    virtual void VisitConcreateElementB(ConcreateElementB* pConcreateElementB)
    {
        std::cout << "VisitConcreateElementB By ConcreateVisitorB\n";
    };
};



int VisitorShow(int argc, char** argv)
{
    Visitor* pVisitorA = new ConcreateVisitorA();
    Element* pElement = new ConcreateElementA();
    pElement->Accept(*pVisitorA);
    delete pElement;
    delete pVisitorA;
    return 0;
}








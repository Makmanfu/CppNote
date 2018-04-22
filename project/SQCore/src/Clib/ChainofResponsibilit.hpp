//-----------------------------------------------------------------------------
//
//   ���ģʽ����
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

// �������,����һ����������Ľӿ�
class Handler
{
public:
    Handler(Handler* pSuccessor = NULL) : m_pSuccessor(pSuccessor) {};
    virtual ~Handler()
    {
        delete m_pSuccessor;
        m_pSuccessor = NULL;
    };
    // ���麯��,��������ʵ��
    virtual void HandleRequset() = 0;
protected:
    Handler* m_pSuccessor;
};

class ConcreateHandler1: public Handler
{
public:
    ConcreateHandler1(Handler* pSuccessor = NULL) : Handler(pSuccessor) {}
    virtual ~ConcreateHandler1() {}
    virtual void HandleRequset()
    {
        if (NULL != m_pSuccessor)
            m_pSuccessor->HandleRequset();
        else
            std::cout << "HandleRequset by ConcreateHandler1\n";
    };
};

class ConcreateHandler2: public Handler
{
public:
    ConcreateHandler2(Handler* pSuccessor = NULL) : Handler(pSuccessor) {}
    virtual ~ConcreateHandler2() {}
    virtual void HandleRequset()
    {
        if (NULL != m_pSuccessor)
            m_pSuccessor->HandleRequset();
        else
            std::cout << "HandleRequset by ConcreateHandler2\n";
    };
};




int ChainOfResponsibilityShow(int argc, char** argv)
{
    Handler* p1 = new ConcreateHandler1();
    Handler* p2 = new ConcreateHandler2(p1);
    p2->HandleRequset();
    delete p2;
    return 0;
}





//-----------------------------------------------------------------------------
//
//   ���ģʽ����
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
typedef int DATA;

class Iterater;

//�����ĳ������
class Aggregate
{
public:
    virtual ~Aggregate() {};
    virtual Iterater* CreateIterater(Aggregate* pAggregate) = 0;
    virtual int GetSize() = 0;
    virtual DATA GetItem(int nIndex) = 0;
};

//�������ĳ������
class Iterater
{
public:
    virtual ~Iterater() {};
    virtual void First() = 0;
    virtual void Next() = 0;
    virtual bool IsDone() = 0;
    virtual DATA CurrentItem() = 0;
};

// һ�������������,�������������ʾ
class ConcreateAggregate : public Aggregate
{
public:
    ConcreateAggregate(int nSize) : m_nSize(nSize), m_pData(NULL)
    {
        m_pData = new DATA[m_nSize];
        for (int i = 0; i < nSize; ++i)
            m_pData[i] = i;
    };
    virtual ~ConcreateAggregate()
    {
        delete[] m_pData;
        m_pData = NULL;
    };
    virtual Iterater* CreateIterater(Aggregate* pAggregate)
    {
        return new ConcreateIterater(this);
    };
    virtual int GetSize()
    {
        return m_nSize;
    };
    virtual DATA GetItem(int nIndex)
    {
        if (nIndex < m_nSize)
            return m_pData[nIndex];
        else
            return -1;
    };
private:
    int m_nSize;
    DATA* m_pData;
};

// ����ConcreateAggregate������ĵ�������
class ConcreateIterater: public Iterater
{
public:
    ConcreateIterater(Aggregate* pAggregate) : m_pConcreateAggregate(pAggregate), m_nIndex(0) {};
    virtual ~ConcreateIterater() {};
    virtual void First(void)
    {
        m_nIndex = 0;
    };
    virtual void Next(void)
    {
        if (m_nIndex < m_pConcreateAggregate->GetSize())
            ++m_nIndex;
    };
    virtual bool IsDone(void)
    {
        return m_nIndex == m_pConcreateAggregate->GetSize();
    };
    virtual DATA CurrentItem(void)
    {
        return m_pConcreateAggregate->GetItem(m_nIndex);
    };
private:
    Aggregate* m_pConcreateAggregate;
    int m_nIndex;
};




int IteratorShow(int argc, char** argv)
{
    Aggregate* pAggregate = new ConcreateAggregate(4);
    Iterater*  pIterater = new ConcreateIterater(pAggregate);
    for (; false == pIterater->IsDone(); pIterater->Next())
        std::cout << pIterater->CurrentItem() << std::endl;
    return 0;
}






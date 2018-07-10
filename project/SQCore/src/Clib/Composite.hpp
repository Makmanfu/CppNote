//-----------------------------------------------------------------------------
//
//                  Composite���ģʽ����
//                                          @2017-02-14��������
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <vector>
using namespace std;

struct ComPart
{
    ComPart() {};
    virtual ~ComPart() {};
    virtual void Operation() = 0;
    virtual void Add(const ComPart&) {};
    virtual void Remove(const ComPart&) {};
    virtual ComPart* GetChild(int)
    {
        return NULL;
    };
};

struct Leaf : public ComPart
{
    Leaf() {};
    ~Leaf() {};
    void Operation()
    {
        printf("Leaf operation\n");
    };
};

class Composite : public ComPart
{
private:
    vector<ComPart*> comVec;
public:
    Composite() {};
    ~Composite() {};
    void Operation()
    {
        for (auto It = comVec.begin(); It != comVec.end(); It++)
            (*It)->Operation();
    };
    void Add(ComPart* com)
    {
        comVec.push_back(com);
    };
    void Remove(ComPart* com)
    {
        for (auto It = comVec.begin(); It != comVec.end();)
        {
            if (*It == com)
                comVec.erase(It++);
            else
                It++;
        }
    };
    ComPart* GetChild(int index)
    {
        return comVec[index];
    };
};


//��˾
class Company
{
protected:
    string m_strName;
public:
    Company(string strName)
    {
        m_strName = strName;
    };
    virtual void Add(Company* c) = 0;
    virtual void Display(int nDepth) = 0;
    virtual void LineOfDuty() = 0;
};

//����˾
class ConcreteCompany : public Company
{
private:
    vector<Company*> m_company;
public:
    ConcreteCompany(string strName) : Company(strName) {};
    virtual void Add(Company* c)
    {
        m_company.push_back(c);
    };
    virtual void Display(int nDepth)
    {
        string strtemp;
        for (int i = 0; i < nDepth; i++)
            strtemp += "-";
        strtemp += m_strName;
        cout << strtemp << endl;
        vector<Company*>::iterator p = m_company.begin();
        while (p != m_company.end())
        {
            (*p)->Display(nDepth + 2);
            p++;
        }
    };
    virtual void LineOfDuty()
    {
        vector<Company*>::iterator p = m_company.begin();
        while (p != m_company.end())
        {
            (*p)->LineOfDuty();
            p++;
        }
    };
};

//������Դ��
class HrDepartment : public Company
{
public:
    HrDepartment(string strname) : Company(strname) {};
    virtual void Display(int nDepth)
    {
        string strtemp;
        for (int i = 0; i < nDepth; i++)
            strtemp += "-";
        strtemp += m_strName;
        cout << strtemp << endl;
    };
    virtual void Add(Company* c)
    {
        cout << "error" << endl;
    };
    virtual void LineOfDuty()
    {
        cout << m_strName << ":��Ƹ�˲�" << endl;
    };
};


int Mode_Composite(int argc, char** argv)
{
    Leaf* pLeaf1 = new Leaf();
    pLeaf1->Operation();
    Composite* pCom = new Composite();
    pCom->Add(pLeaf1);
    pCom->Operation();
    ComPart* ll = pCom->GetChild(0);
    ll->Operation();
    delete pCom;
    //Leaf* pLeaf1 = new Leaf();
    //Leaf* pLeaf2 = new Leaf();
    //Composite* pCom = new Composite;
    //pCom->Add(pLeaf1);
    //pCom->Add(pLeaf2);
    //pCom->Operation();
    //pCom->GetChild(2)->Operation();


    printf("ʵ����ʾ!============\n");
    ConcreteCompany* p = new ConcreteCompany("�廪��ѧ");
    p->Add(new HrDepartment("�廪��ѧ�˲Ų�"));
    ConcreteCompany* p1 = new ConcreteCompany("��ѧϵ");
    p1->Add(new HrDepartment("��ѧϵ�˲Ų�"));
    ConcreteCompany* p2 = new ConcreteCompany("����ϵ");
    p2->Add(new HrDepartment("����ϵ�˲Ų�"));
    p->Add(p1);
    p->Add(p2);
    p->Display(1);
    p->LineOfDuty();
    return 1;
}



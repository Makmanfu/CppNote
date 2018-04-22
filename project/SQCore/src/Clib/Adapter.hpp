//-----------------------------------------------------------------------------
//
//                  Adapter���ģʽ����
//                                          @2017-02-14��������
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
using namespace std;

//��Ҫ��Adapt����
struct Target
{
    Target() {};
    virtual ~Target() {};
    virtual void Request()
    {
        printf("Target::Request\n");
    };
};

//�뱻Adapt�����ṩ�����ݽӿڵ���
struct Adaptee
{
    Adaptee() {};
    virtual ~Adaptee() {};
    void SpecificRequest()
    {
        printf("Adaptee::SpecificRequest\n");
    };
};

//���ؼ̳�(���)Ҳ�ܾۺ�ԭ�нӿ���ķ�ʽ
struct Adapter : public Target, private Adaptee
{
    Adapter() {};
    virtual ~Adapter() {};
    virtual void Request()
    {
        this->SpecificRequest();
    };
};

//����Adapt����,���ü̳�ԭ�нӿ���ķ�ʽ
class AdapterTE : public Target
{
private:
    Adaptee* m_pAdptee;
public:
    AdapterTE(Adaptee* pAdaptee) : m_pAdptee(pAdaptee) {};
    virtual ~AdapterTE()
    {
        delete m_pAdptee;
        m_pAdptee = NULL;
    };
    virtual void Request()
    {
        m_pAdptee->SpecificRequest();
    };
};

//������ģʽʵ��========================================
class Player
{
protected:
    string name;
public:
    Player(string strName)
    {
        name = strName;
    };
    virtual void Attack() = 0;
    virtual void Defense() = 0;
};

struct Forwards : public Player
{
    Forwards(string strName) : Player(strName) {};
    virtual void Attack()
    {
        printf("ǰ�����\n");
    };
    virtual void Defense()
    {
        printf("ǰ�����\n");
    };
};

struct Center : public Player
{
    Center(string strName) : Player(strName) {};
    virtual void Attack()
    {
        printf("�г�����\n");
    };
    virtual void Defense()
    {
        printf("�г�����\n");
    };
};

//Ϊ�г�����
class TransLater : public Player
{
private:
    Center* player;
public:
    TransLater(string strName) : Player(strName)
    {
        player = new Center(strName);
    };
    virtual void Attack()
    {
        player->Attack();
    };
    virtual void Defense()
    {
        player->Defense();
    };
};


int Mode_Adapter(int argc, char** argv)
{
    Adaptee* pAdaptee = new Adaptee;
    Target* pTarget = new AdapterTE(pAdaptee);
    pTarget->Request();
    //Adapter* pAdapter = new Adapter();
    //pAdapter->Request();
    printf("ʵ����ʾ!============\n");
    Player* p = new TransLater("С��");
    p->Attack();
    return 1;
}






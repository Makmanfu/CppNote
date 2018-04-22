//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"


class Command
{
public:
    virtual ~Command() {}
    virtual void Execute() = 0;
};

class Receiver
{
public:
    Reciever() {};
    ~Reciever() {};
    void Action()
    {
        std::cout << "Receiver Action\n";
    };
};

class Invoker
{
public:
    Invoker(Command* pCommand) : m_pCommand(pCommand) {};
    ~Invoker()
    {
        delete m_pCommand;
        m_pCommand = NULL;
    };
    void Invoke()
    {
        if (NULL != m_pCommand)
            m_pCommand->Execute();
    };
private:
    Command* m_pCommand;
};

class ConcreateComand: public Command
{
public:
    ConcreateComand(Receiver* pReceiver) : m_pReceiver(pReceiver) {};
    virtual ~ConcreateComand()
    {
        delete m_pReceiver;
        m_pReceiver = NULL;
    };
    virtual void Execute()
    {
        if (NULL != m_pReceiver)
            m_pReceiver->Action();
        std::cout << "Execute by ConcreateComand\n";
    };
private:
    Receiver* m_pReceiver;
};


int CommandShow(int argc, char** argv)
{
    Receiver* pReceiver = new Receiver();
    Command*  pCommand = new ConcreateComand(pReceiver);
    Invoker*  pInvoker = new Invoker(pCommand);
    pInvoker->Invoke();
    delete pInvoker;
    return 0;
}



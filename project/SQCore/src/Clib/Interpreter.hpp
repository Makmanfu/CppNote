//-----------------------------------------------------------------------------
//
//   设计模式例子
//
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"
#include <string>
using namespace std;

class Context
{
public:
    Context() {};
    ~Context() {};
};

class AbstractExpression
{
public:
    virtual ~AbstractExpression() {};
    virtual void Interpret(const Context& c) {};
protected:
    AbstractExpression() {};
};

class TerminalExpression : public AbstractExpression
{
public:
    TerminalExpression(const string& statement)
    {
        this->_statement = statement;
    };
    ~TerminalExpression() {};
    void Interpret(const Context& c)
    {
        cout << this->_statement << "TerminalExpression" << endl;
    };
private:
    string _statement;
};

class NonterminalExpression : public AbstractExpression
{
public:
    NonterminalExpression(AbstractExpression* expression, int times)
    {
        this->_expression = expression;
        this->_times = times;
    };
    ~NonterminalExpression() {};
    void Interpret(const Context& c)
    {
        for (int i = 0; i < _times; i++)
            this->_expression->Interpret(c);
    };
private:
    AbstractExpression* _expression;
    int _times;
};


int InterpreterShow(int argc, char** argv)
{
    Context* c = new Context();
    AbstractExpression* te = new TerminalExpression("hello");
    AbstractExpression* nte = new NonterminalExpression(te, 2);
    nte->Interpret(*c);
    return 0;
}





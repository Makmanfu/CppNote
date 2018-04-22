
#include "stdafx.h"
#include "QtState.h"

StateSwitchTransition::StateSwitchTransition(int rand)
    : QAbstractTransition(), m_rand(rand)
{
}

bool StateSwitchTransition::eventTest(QEvent* event)
{
    return (event->type() == QEvent::Type(StateSwitchEvent::StateSwitchType))
           && (static_cast<StateSwitchEvent*>(event)->rand() == m_rand);
}

void StateSwitchTransition::onTransition(QEvent*)
{
}

StateSwitcher::StateSwitcher(QStateMachine* machine)
    : QState(machine), m_stateCount(0), m_lastIndex(0)
{
}

void StateSwitcher::onEntry(QEvent*)
{
    //::MessageBoxA(NULL, "进入状态", "title", MB_OK);
    int n;
    //判断切换
    while ((n = (qrand() % m_stateCount + 1)) == m_lastIndex)
    {
        ::MessageBoxA(NULL, "进入状态", "title", MB_OK);
    }
    m_lastIndex = n;
    machine()->postEvent(new StateSwitchEvent(n));
}

void StateSwitcher::onExit(QEvent*)
{
    //::MessageBoxA(NULL, "退出状态", "title", MB_OK);
}

void StateSwitcher::addState(QAbstractState* state)
{
    StateSwitchTransition* trans = new StateSwitchTransition(++m_stateCount);
    trans->setTargetState(state);
    addTransition(trans);
}

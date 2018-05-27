
#include "stdafx.h"
#include "tabtd.h"
#include "WThread.h"
#include "WThreadGrid.h"

TabTD::TabTD(QWidget* parent) : TabGUIBase(parent)
{
    this->setWindowTitle(TCSR("TabTD"));
    this->addcomboxItem(TCSR("WThread"));
    this->addcomboxItem(TCSR("WThreadGrid"));
}

TabTD::~TabTD()
{
}

void TabTD::OnComBoxPageIndexChanged(int nId)
{
    m_pPageUI->Free();
    switch (nId)
    {
    case 0:
    default:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    case 1:
        m_pCurrUI = new WThread(m_pWgtShow);
        break;
    case 2:
        m_pCurrUI = new WThreadGrid(m_pWgtShow);
        break;
    }
    m_pPageUI->SetQWidget(m_pCurrUI);
    m_pPageUI->Create(m_pGridW);
}







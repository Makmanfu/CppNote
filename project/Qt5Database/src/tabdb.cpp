
#include "stdafx.h"
#include "tabdb.h"
#include "wdb.h"
#include "crmsystem.h"


TabDB::TabDB(QWidget* parent) : TabGUIBase(parent)
{
    this->setWindowTitle(QString::fromLocal8Bit("TabDB"));
    addcomboxItem(QString::fromLocal8Bit("WndDBPage"));
    addcomboxItem(QString::fromLocal8Bit("CrmSystem"));
}

TabDB::~TabDB()
{
}

void TabDB::OnComBoxPageIndexChanged(int nId)
{
    m_pPageUI->Free();
    switch (nId)
    {
    case 0:
    default:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    case 1:
        m_pCurrUI = new WndDBPage(m_pWgtShow);
        break;
    case 2:
        m_pCurrUI = new CrmSystem(m_pWgtShow);
        break;
    }
    m_pCurrUI->setParent(m_pWgtShow);
    m_pPageUI->SetQWidget(m_pCurrUI);
    m_pPageUI->Create(m_pGridW);
}













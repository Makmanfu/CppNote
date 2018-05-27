
#include "stdafx.h"
#include "tabgis.h"

TabGIS::TabGIS(QWidget* parent) : TabGUIBase(parent)
{
    this->setWindowTitle(TCSR("GIS"));
    addcomboxItem(TCSR("MarbleGIS"));
    addcomboxItem(TCSR("SeaGIS"));
    addcomboxItem(TCSR("QMapGis"));
}

TabGIS::~TabGIS()
{
}

void TabGIS::OnComBoxPageIndexChanged(int nId)
{
    m_pPageUI->Free();
    switch (nId)
    {
    case 0:
    default:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    case 1:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    case 2:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    case 3:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    }
    m_pCurrUI->setParent(m_pWgtShow);
    m_pPageUI->SetQWidget(m_pCurrUI);
    m_pPageUI->Create(m_pGridW);
}






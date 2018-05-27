
#include "stdafx.h"
#include "tabnet.h"
#include "frmnettool.h"


TabNet::TabNet(QWidget* parent) : TabGUIBase(parent)
{
    this->setWindowTitle(TCSR("TabNet"));
    this->addcomboxItem(TCSR("QNet"));
}

TabNet::~TabNet()
{
}

void TabNet::OnComBoxPageIndexChanged(int nId)
{
    m_pPageUI->Free();
    switch (nId)
    {
    case 0:
    default:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    case 1:
        m_pCurrUI = new frmNetTool(m_pWgtShow);
        break;
    }
    m_pCurrUI->setParent(m_pWgtShow);
    m_pPageUI->SetQWidget(m_pCurrUI);
    m_pPageUI->Create(m_pGridW);
}







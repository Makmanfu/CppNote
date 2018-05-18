
#include "stdafx.h"
#include "tabgui.h"
#include "wdraw.h"
#include "wplay.h"
#include "wbase.h"
#include "waxis.h"
#include "wtimenumbar.h"
#include "wprogressbar.h"

TabGUI::TabGUI(QWidget* parent) : TabGUIBase(parent)
{
    this->setWindowTitle(QString::fromLocal8Bit("界面演示"));
    addcomboxItem(QString::fromLocal8Bit("WndAxis"));
    addcomboxItem(QString::fromLocal8Bit("WScanRotate"));
    addcomboxItem(QString::fromLocal8Bit("WRotate"));
    addcomboxItem(QString::fromLocal8Bit("WndPlayer"));
    addcomboxItem(QString::fromLocal8Bit("TimeNumBar"));
    addcomboxItem(QString::fromLocal8Bit("TimeBarPanel"));
    addcomboxItem(QString::fromLocal8Bit("WProgressbar"));
    addcomboxItem(QString::fromLocal8Bit("QWidget"));
}

TabGUI::~TabGUI()
{
}

void TabGUI::OnComBoxPageIndexChanged(int nId)
{
    m_pPageUI->Free();
    switch (nId)
    {
        case 0:
        default:            //空面板
            m_pCurrUI = new QWidgetEmp(m_pWgtShow);
            break;
        case 1:             //坐标轴控件
            m_pCurrUI = new WAxis(m_pWgtShow);
            break;
        case 2:             //雷达稍描控件
            m_pCurrUI = new WScanRotate(m_pWgtShow);
            break;
        case 3:             //雷达控件
            m_pCurrUI = new WRotate(m_pWgtShow);
            break;
        case 4:             //播放器
            m_pCurrUI = new WPlayer(m_pWgtShow);
            break;
        case 5:             //
            m_pCurrUI = new TimeNumBar(m_pWgtShow);//DialPlot  NProgreCirWater
            break;
        case 6:             //
            m_pCurrUI = new TimeBarPanel(m_pWgtShow);
            break;
        case 7:             //频谱绘制
            m_pCurrUI = new WProgressbar(m_pWgtShow);
            break;
        case 8:
            m_pCurrUI = new QWidget(m_pWgtShow);
            break;
    }
    m_pPageUI->SetQWidget(m_pCurrUI);
    m_pPageUI->Create(m_pGridW);
}





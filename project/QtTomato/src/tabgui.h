//*************************************************************************
//
//                       TabGUI控件
//
//
//
//*************************************************************************

#ifndef TABGUI_H
#define TABGUI_H

#include "QPublic.h"

class UIDirect;

//界面演示框架
class TabGUI : public TabGUIBase
{
    Q_OBJECT
public:
    explicit TabGUI(QWidget* parent = 0);
    ~TabGUI();
private slots:
    void OnComBoxPageIndexChanged(int nId);
};







#endif

//*************************************************************************
//
//                      线程面板
//
//
//
//*************************************************************************

#ifndef TABTD_H
#define TABTD_H

#include "QPublic.h"

class UIDirect;

class TabTD : public TabGUIBase
{
    Q_OBJECT
public:
    explicit TabTD(QWidget* parent = 0);
    ~TabTD();
private slots:
    void OnComBoxPageIndexChanged(int nId);
};





#endif

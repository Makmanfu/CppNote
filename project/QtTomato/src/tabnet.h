//*************************************************************************
//
//                      网络通信
//
//
//
//*************************************************************************

#ifndef TABNET_H
#define TABNET_H

#include "QPublic.h"

class UIDirect;

class TabNet : public TabGUIBase
{
    Q_OBJECT
public:
    explicit TabNet(QWidget* parent = 0);
    ~TabNet();
private slots:
    void OnComBoxPageIndexChanged(int nId);
};





#endif

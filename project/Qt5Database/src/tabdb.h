//*************************************************************************
//
//                       TabDB控件
//
//
//
//*************************************************************************

#ifndef TABDB_H
#define TABDB_H

#include "QPublic.h"

class UIDirect;

class TabDB : public TabGUIBase
{
    Q_OBJECT
public:
    explicit TabDB(QWidget* parent = 0);
    ~TabDB();
private slots:
    void OnComBoxPageIndexChanged(int nId);
};





#endif

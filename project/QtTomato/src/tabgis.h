//*************************************************************************
//
//                       tabGIS控件
//
//
//
//*************************************************************************

#ifndef WGIS_H
#define WGIS_H

#include "QPublic.h"

class UIDirect;

class TabGIS : public TabGUIBase
{
    Q_OBJECT
public:
    explicit TabGIS(QWidget* parent = 0);
    ~TabGIS();
private slots:
    void OnComBoxPageIndexChanged(int nId);
};





#endif

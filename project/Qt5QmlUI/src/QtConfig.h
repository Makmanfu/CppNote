﻿//********************************************************************
//
//                  QT 全局宏定义和开关定义 依赖关系
//                      除结构体 不要写函数和类
//
//********************************************************************/

#ifndef QTCONFIG_H
#define QTCONFIG_H

//QT HEAD FILE
#include <QtCore/QtCore>
#if (QT_VERSION < QT_VERSION_CHECK(4, 8, 7))
#include <QtGui/QtGui>
#include <QtSql/QtSql>
#else
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>
#include <QtQml/QtQml>
#endif

//QT MODEL NAME
#ifdef _DEBUG
#define DLIB(name) name "d"
#else
#define DLIB(name) name
#endif

//QT MODEL LIB
#if (QT_VERSION < QT_VERSION_CHECK(4, 8, 7))
#define QLIB(name) name "4"
#pragma comment( lib, DLIB("qtmain") )
#pragma comment( lib, QLIB(DLIB("QtCore")) )
#pragma comment( lib, QLIB(DLIB("QtGui")) )
#pragma comment( lib, QLIB(DLIB("QtSql")) )
#else
#pragma comment( lib, DLIB("qtmain") )
#pragma comment( lib, QLIB(DLIB("Qt5Core")) )
#pragma comment( lib, QLIB(DLIB("Qt5Gui")) )
#pragma comment( lib, QLIB(DLIB("Qt5Widgets")) )
#pragma comment( lib, QLIB(DLIB("QtSql")) )
#pragma comment( lib, QLIB(DLIB("Qt5Qml")) )              //qml
#endif







#endif

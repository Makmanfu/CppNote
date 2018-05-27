//********************************************************************
//
//          QT 全局宏定义和开关定义 依赖关系
//          除结构体 不要写函数和类
//
//********************************************************************/

#ifndef QTCONFIG_H
#define QTCONFIG_H

//QT HEAD FILE
#include <QtCore/QtCore>
#if (QT_VERSION < QT_VERSION_CHECK(4, 8, 7))
#include <QtGui/QtGui>
#include <QtNetwork/QtNetwork>
#else
#include <QtGui/QtGui>
#include <QtWidgets/QtWidgets>
#endif

//输出宏定义
#ifndef C_API
#define C_API extern "C" __declspec(dllexport)
#endif
#ifndef CPP_API
#define CPP_API __declspec(dllexport)
#endif
#ifndef DLL_API
#define DLL_API CPP_API
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
#pragma comment( lib, QLIB(DLIB("QtNetwork")) )
#else
#pragma comment( lib, DLIB("qtmain") )
#pragma comment( lib, QLIB(DLIB("Qt5Core")) )
#pragma comment( lib, QLIB(DLIB("Qt5Gui")) )
#pragma comment( lib, QLIB(DLIB("Qt5Widgets")) )
#pragma comment( lib, QLIB(DLIB("Qt5Network")) )
#endif









#endif

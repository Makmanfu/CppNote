//*************************************************************************
//
//
//                          线程列表的演示例子
//
//
//*************************************************************************

#ifndef WTHREADGRID_H
#define WTHREADGRID_H

#include "QPublic.h"

//线程操作
class CPP_API MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject* parent = 0);
    void stop();
protected:
    //运行函数
    void run();
private:
    volatile bool stopped;
};

namespace Ui
{
    class WThreadGrid;
}

//线程演示界面
class CPP_API WThreadGrid : public QWidget
{
    Q_OBJECT
public:
    explicit WThreadGrid(QWidget* parent = 0);
    ~WThreadGrid();
private:
    Ui::WThreadGrid* ui;
};








#endif


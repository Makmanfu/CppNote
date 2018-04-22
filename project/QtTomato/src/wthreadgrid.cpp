
#include "stdafx.h"
#include "wthreadgrid.h"
#include "ui_wthreadgrid.h"



MyThread::MyThread(QObject* parent) : QThread(parent)
{
    stopped = false;
}

void MyThread::stop()
{
    stopped = true;
}

void MyThread::run()
{
    qreal i = 0;
    while (!stopped)
    {
        qDebug() << QString("in MyThread: %1").arg(i);
        msleep(1000);
        i++;
    }
    stopped = false;
}


WThreadGrid::WThreadGrid(QWidget* parent) : QWidget(parent)
, ui(new Ui::WThreadGrid)
{
    ui->setupUi(this);
}

WThreadGrid::~WThreadGrid()
{
    delete ui;
}








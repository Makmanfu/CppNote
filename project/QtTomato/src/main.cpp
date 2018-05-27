

#include "stdafx.h"
#include "WndMain.h"
#include <iostream>
using namespace std;

int main(int argc, char** argv)
{
    //程序唯一性运行
    if (!CommonHelper::Instance()->RunCheckOnly("QtGuiSystemObject"))
        return 0;
    QApplication app(argc, argv);

    //设置图标
    app.setWindowIcon(QIcon(":/Tomato.gif"));
    //设置风格
#if (QT_VERSION <= QT_VERSION_CHECK(4, 8, 7))
    app.setStyle(new QCleanlooksStyle);
#endif
    Appcfg::Instance()->CreateIni("Tomocfg.ini");
    //番茄悬浮窗(主窗体再里面)
    WndSuspenUsr m_tomato(Appcfg::Instance()->Sdat.SuspenPaint, NULL);
    m_tomato.supenShow(Appcfg::Instance()->Sdat.SupenShow);
    m_tomato.setTrayShow(Appcfg::Instance()->Sdat.SuspenExit);
    //番茄动画
    QPropertyAnimation* animation1 = new QPropertyAnimation(&m_tomato, "geometry");
    animation1->setDuration(2000);
    animation1->setStartValue(QRect(30, 900, 50, 50));
    animation1->setEndValue(QRect(30, 40, 50, 50));
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    QPropertyAnimation *animation2 = new QPropertyAnimation(&m_tomato, "geometry");
    animation2->setDuration(100);
    animation2->setStartValue(QRect(30, 40, 50, 50));
    animation2->setEndValue(QRect(70, 70, m_tomato.geometry().width(), m_tomato.geometry().height()));
    //串行动画组
    QSequentialAnimationGroup group;
    group.addAnimation(animation1);
    group.addAnimation(animation2);
    group.start();
    //消息循环
    return app.exec();
}





//+++++++++++++++++++++++++++++++++++++++++++++++++++
//显示logo的方法
//QSharedPointer<QSplashScreen> splash(new QSplashScreen);
////QSplashScreen *splash = new QSplashScreen;
//splash->setPixmap(QPixmap(":/Tomato.png"));
//splash->show();
//Qt::Alignment topLeft = Qt::AlignLeft | Qt::AlignTop;
//splash->showMessage(QObject::tr("start..."),topLeft,Qt::white);
//Sleep(1000);
////启动悬浮窗
//WndSuspenUsr m_tomato;
//m_tomato.show();
//splash->finish(&m_tomato);
//+++++++++++++++++++++++++++++++++++++++++++++++++++

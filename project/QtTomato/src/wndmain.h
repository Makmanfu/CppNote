//*************************************************************************
//
//                        tomato浏览器框架页
//
//
//
//*************************************************************************

#ifndef WIDGET_H
#define WIDGET_H


#include "QPublic.h"
#include "wdraw.h"

class TabGUI;
class TabDB;
class TabTD;
class TabGIS;
class TabNet;

//主窗体框架
class WndMain : public QWidget
{
    Q_OBJECT
public:
    explicit WndMain(QWidget* parent = 0);
    ~WndMain();
private:
    QPushButton* m_lblconfg;      //配置按钮
    QGridLayout* m_gridLayout;    //管理布局Tab页面
    QTabWidget* m_tabWidget;      //Tab页面
    //控件布局
    QGridLayout* m_GUIGrid, *m_ThreadGrid, *m_WebGrid, *m_StructGrid;
    QWidget* m_tabGUI, *m_tabThread, *m_tabWeb, *m_tabGIS;
    TabGUI* m_myGUI;              //界面演示
    TabTD* m_myWndTD;             //线程演示
    TabNet* m_myWndWeb;           //网络通信
    //TabGIS* m_myGIS;              //GIS
private:
    QPoint m_PosDisp;             //鼠标点
    bool m_bPressed;              //记录状态
private:
    void createTab(void);
    void guiInit(void);
protected:
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
    void paintEvent(QPaintEvent* ev);
    void closeEvent(QCloseEvent* event);
private slots:
    void on_lblconfg_click();
};

//悬浮番茄BOSS
class WndSuspenUsr : public WSuspension
{
    Q_OBJECT
public:
    explicit WndSuspenUsr(int _PngType = 1, QWidget* parent = 0);
    virtual ~WndSuspenUsr();
private:
    WndMain* m_wndFrm;                    //窗体
private:
    virtual void showMain(void);
    virtual void exitSuspen(void);
};


#endif



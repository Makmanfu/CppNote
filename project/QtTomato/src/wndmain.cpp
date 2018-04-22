
#include "stdafx.h"
#include "wndmain.h"
#include "dlgfrm.h"
#include "tabgui.h"
#include "tabgis.h"
#include "tabtd.h"
#include "tabnet.h"


WndMain::WndMain(QWidget* parent) : QWidget(parent)
    , m_gridLayout(NULL)
    , m_tabWidget(NULL)
    , m_lblconfg(NULL)
    , m_GUIGrid(NULL)
    , m_tabGUI(NULL)
    , m_myGUI(NULL)
    , m_ThreadGrid(NULL)
    , m_tabThread(NULL)
    , m_myWndTD(NULL)
{
    QFont ft = QFont(QString::fromLocal8Bit("微软雅黑"));
    ft.setPointSize(11);
    this->setFont(ft);
    this->resize(600, 640);
    //this->setStyleSheet("background-color: rgb(35, 104, 188);");
    //锁定控件最小值
    //this->setMinimumSize(600, 640);
    setWindowOpacity(0.95);
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("WndMain"));
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Res/tray.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setWindowTitle(QString::fromLocal8Bit("番茄BOSS浏览器"));
    //测试窗口嵌套
    //HWND calc = FindWindowA(NULL, "计算器");
    //SetParent(calc, this->winId());
    //HWND VMUI = FindWindowA("VMUIFrame", NULL);
    //SetParent(VMUI, this->winId());
    //SetParent(VMUI, findDesktopIconWnd());
    createTab();
    guiInit();
}

WndMain::~WndMain()
{
    if (m_lblconfg != NULL)
    {
        delete m_lblconfg;
        m_lblconfg = NULL;
    }
}

void WndMain::createTab(void)
{
    //tab创建
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
    m_tabWidget->setMovable(false);
    //创建tab页中的对象
    m_tabGUI = new QWidget();
    m_tabGUI->setObjectName(QString::fromUtf8("tabGUI"));
    m_tabWidget->addTab(m_tabGUI, QString());
    m_tabThread = new QWidget();
    m_tabThread->setObjectName(QString::fromUtf8("tabThread"));
    m_tabWidget->addTab(m_tabThread, QString());
    m_tabWeb = new QWidget();
    m_tabWeb->setObjectName(QString::fromUtf8("tabWeb"));
    m_tabWidget->addTab(m_tabWeb, QString());
    m_tabGIS = new QWidget();
    m_tabGIS->setObjectName(QString::fromUtf8("tabStruct"));
    m_tabWidget->addTab(m_tabGIS, QString());
    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setSpacing(6);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    //gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    m_gridLayout->addWidget(m_tabWidget, 0, 0, 1, 1);
    m_tabWidget->setTabText(m_tabWidget->indexOf(m_tabGUI), QString::fromLocal8Bit("图形界面"));
    m_tabWidget->setTabText(m_tabWidget->indexOf(m_tabThread), QString::fromLocal8Bit("线程控制"));
    m_tabWidget->setTabText(m_tabWidget->indexOf(m_tabWeb), QString::fromLocal8Bit("网络通信"));
    m_tabWidget->setTabText(m_tabWidget->indexOf(m_tabGIS), QString::fromLocal8Bit("GIS"));
    m_tabWidget->setCurrentIndex(0);
    QMetaObject::connectSlotsByName(this);
}

void WndMain::guiInit(void)
{
    //配置按钮
    m_lblconfg = new QPushButton(this);
    m_lblconfg->setFlat(true);
    m_lblconfg->setText(QString::fromLocal8Bit("AppCfg"));
    m_lblconfg->setFixedSize(70, 25);
    connect(m_lblconfg, SIGNAL(clicked()), this, SLOT(on_lblconfg_click()));
    //创建界面演示
    m_myGUI = new TabGUI(m_tabGUI);
    m_myGUI->move(0, 0);
    m_GUIGrid = new QGridLayout;     //设置布局哦
    m_GUIGrid->addWidget(m_myGUI, 0, 0);
    m_GUIGrid->setMargin(0);
    m_tabGUI->setLayout(m_GUIGrid);
    //创建线程池演示
    m_myWndTD = new TabTD(m_tabThread);
    m_myWndTD->move(0, 0);
    m_ThreadGrid = new QGridLayout;
    m_ThreadGrid->addWidget(m_myWndTD, 0, 0);
    m_ThreadGrid->setMargin(0);
    m_tabThread->setLayout(m_ThreadGrid);
    //创建分布通信演示
    m_myWndWeb = new TabNet(m_tabWeb);
    m_myWndWeb->move(0, 0);
    m_WebGrid = new QGridLayout;
    m_WebGrid->addWidget(m_myWndWeb, 0, 0);
    m_WebGrid->setMargin(0);
    m_tabWeb->setLayout(m_WebGrid);
    //创建数据结构演示
    m_myGIS = new TabGIS(m_tabGIS);
    m_myGIS->move(0, 0);
    m_StructGrid = new QGridLayout;
    m_StructGrid->addWidget(m_myGIS, 0, 0);
    m_StructGrid->setMargin(0);
    m_tabGIS->setLayout(m_StructGrid);
    //时钟控件
    static WClock* m_clock = new WClock(this);
    m_clock->setParent(this, 0);
    m_clock->move(20, this->size().height() - 150);
    m_clock->supenShow(Appcfg::Instance()->Sdat.Clock);
    //static WndSuspension* pad = new WndClock(this);
    //pad->setParent(this, 0);
    //pad->setUIImage(QString(":/PAD.png"));
    //pad->move(20, this->size().height() - 150);
    //pad->SupenShow();
}

void WndMain::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::MiddleButton)
    {
        m_PosDisp = ev->globalPos() - frameGeometry().topLeft();;
        ev->accept();
    }
}

void WndMain::mouseReleaseEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::MiddleButton)
    {
        m_PosDisp = QPoint(-1, -1);
        ev->accept();
    }
}

void WndMain::mouseMoveEvent(QMouseEvent* ev)
{
    if (ev->buttons() & Qt::MiddleButton)
    {
        if (m_PosDisp != QPoint(-1, -1))
        {
            QPoint point(ev->globalPos() - m_PosDisp);
            //获得桌面
            QRect windowRect(QApplication::desktop()->availableGeometry());
            QRect widgetRect(this->frameGeometry());
            int y = windowRect.bottomRight().y() - widgetRect.height();//this->size().height();
            int x = windowRect.bottomRight().x() - widgetRect.width();//this->size().width();
            if (point.x() <= 0) point = QPoint(0, point.y());
            if (point.y() >= y && widgetRect.topLeft().y() >= y) point = QPoint(point.x(), y);
            if (point.y() <= 0) point = QPoint(point.x(), 0);
            if (point.x() >= x && widgetRect.topLeft().x() >= x) point = QPoint(x, point.y());
            move(point);
        }
        ev->accept();
    }
}

void WndMain::paintEvent(QPaintEvent* ev)
{
    m_lblconfg->move(this->width() - 72, 1);
    ev->accept();
}

void WndMain::closeEvent(QCloseEvent* event)
{
    if (Appcfg::Instance()->Sdat.SuspenExit)
    {
        //界面只是隐藏
        hide();
        event->ignore();
    }
    else
        exit(0);
}

void WndMain::on_lblconfg_click()
{
    DlgFrm dlg(this);
    dlg.exec();
}


WndSuspenUsr::WndSuspenUsr(int _PngType, QWidget* parent) : WSuspension(_PngType, parent)
{
    //默认是居中显示，修改为30,40
    this->move(30, 40);
    this->setObjectName("WndSuspenUsr");
    if (_PngType < 2)
        WSuspension::setUIImage(QPixmap(":/Tomato.png"));
    else
        WSuspension::setUIImage(QString(":/Tomato.gif"), QSize(200, 200));
    //主窗体创建
    m_wndFrm = new WndMain(NULL);
    m_wndFrm->show();
}

WndSuspenUsr::~WndSuspenUsr()
{
    if (m_wndFrm != NULL)
    {
        delete m_wndFrm;
        m_wndFrm = NULL;
    }
}

void WndSuspenUsr::showMain()
{
    if (!m_wndFrm->isVisible())
        m_wndFrm->show();
    else
    {
        QMessageBox msgBox(QMessageBox::Warning, QString::fromLocal8Bit("番茄BOSS提示！"),
                           QString::fromLocal8Bit("不要狂点行不，看清楚任务拦!"));
        QString SySheet = QString(" QLabel {font-size:20px; font-weight:normal; color:rgb(255, 0, 0);}") +
                          QString(" QPushButton {font-size:20px; font-weight:normal; color:rgb(0, 205, 0);} ");
        msgBox.setStyleSheet(SySheet);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok, QString::fromLocal8Bit("确定"));
        msgBox.exec();
    }
}

void WndSuspenUsr::exitSuspen()
{
    WSuspension::exitSuspen();
    exit(0);
}






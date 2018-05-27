
#include "stdafx.h"
#include "wdraw.h"

#define PI 3.1415926535

WSuspension::WSuspension(int _PngType, QWidget* parent) : QMoveDlg(parent)
{
    this->setMinimumSize(50, 50);
    this->setFixedSize(50, 50);
    this->setWindowOpacity(0.9);
    //设置无边框置顶
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    m_iPngType = _PngType;
    //创建菜单
    Act_MainShow = new QAction(TCSR("主界面"), this);
    connect(Act_MainShow, SIGNAL(triggered()), this, SLOT(iconActivated()));
    Act_Suspen = new QAction(TCSR("悬浮窗"), this);
    Act_Suspen->setCheckable(true);
    Act_Suspen->setChecked(true);
    connect(Act_Suspen, SIGNAL(triggered()), this, SLOT(Act_SuspenClick()));
    Act_Tray = new QAction(TCSR("托盘"), this);
    Act_Tray->setCheckable(true);
    connect(Act_Tray, SIGNAL(triggered()), this, SLOT(Act_TrayClick()));
    Act_Exit = new QAction(TCSR("退出"), this);
    connect(Act_Exit, SIGNAL(triggered()), this, SLOT(exitSuspen()));
    m_MenuTray = new QMenu(this);
    //界面菜单设计
    m_TbnWidget = new QWidget();
    m_labGIF = new QLabel(m_TbnWidget);
    m_labGIF->setScaledContents(m_TbnWidget);
    m_vGIF = new QMovie(m_TbnWidget);
    m_labGIF->setMovie(m_vGIF);
    m_vGIF->stop();
    m_vGIF->setFileName(":/mogu.gif");
    m_vGIF->start();
    m_TbnWidgetAction = new QWidgetAction(m_MenuTray);
    QVBoxLayout* mainLout = new QVBoxLayout();
    mainLout->setContentsMargins(0, 0, 0, 0);
    mainLout->addWidget(m_labGIF);
    m_TbnWidget->setLayout(mainLout);
    m_TbnWidgetAction->setDefaultWidget(m_TbnWidget);
    m_MenuTray->addAction(m_TbnWidgetAction);
    m_MenuTray->addSeparator();
    //添加普通菜单
    m_MenuTray->addAction(Act_MainShow);
    m_MenuTray->addAction(Act_Suspen);
    m_MenuTray->addAction(Act_Tray);
    m_MenuTray->addSeparator();
    m_MenuTray->addAction(Act_Exit);
    //创建托盘
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_MenuTray);
    m_trayIcon->setIcon(QIcon(":/Tomato.gif"));
    //SupenShow(true);
    //SetTrayShow(true);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    if (m_iPngType < 2)
        setUIImage(QPixmap(":/Tomato.png"));
    else
    {
        this->setAttribute(Qt::WA_TranslucentBackground, true);
        m_lablogo = new QLabel(this);
        m_lablogo->setScaledContents(true);
        m_vlogo = new QMovie(this);
        m_lablogo->setMovie(m_vlogo);
        this->setUIImage(QString(":/Tomato.gif"));
    }
}

WSuspension::~WSuspension()
{
    m_trayIcon->hide();
    if (m_trayIcon != NULL)
    {
        delete m_trayIcon;
        m_trayIcon = NULL;
    }
}

void WSuspension::supenShow(bool _state)
{
    if (_state)
    {
        Act_Suspen->setChecked(true);
        this->show();
    }
    else
    {
        Act_Suspen->setChecked(false);
        this->hide();
    }
}

void WSuspension::setTrayShow(bool _state)
{
    if (_state)
    {
        Act_Tray->setChecked(true);
        m_trayIcon->show();
    }
    else
    {
        Act_Tray->setChecked(false);
        m_trayIcon->hide();
    }
}

void WSuspension::showMain()
{
    QMessageBox msgBox(QMessageBox::Warning, TCSR("精灵提示！"),
                       TCSR("不要乱点哦，记得加蓝!"));
    QString SySheet = QString(" QLabel {font-size:20px; font-weight:normal; color:rgb(255, 0, 0);} ") +
                      QString(" QPushButton {font-size:20px; font-weight:normal; color:rgb(0, 255, 0);} ");
    msgBox.setStyleSheet(SySheet);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setButtonText(QMessageBox::Ok, TCSR("确定"));
    msgBox.exec();
}

void WSuspension::menuAddAction(QAction* action)
{
    m_MenuTray->insertAction(Act_Suspen/*Act_MainShow*/, action);
}

void WSuspension::setUIImage(QPixmap pix)
{
    m_LogoPixmap = pix;
    if (m_iPngType == 0)
    {
        //设置背景 有锯齿
        //LogoPixmap = LogoPixmap.scaled(QSize(200,200), Qt::IgnoreAspectRatio);
        //设置固定大小
        this->setFixedSize(m_LogoPixmap.size());
        m_trayIcon->setIcon(QIcon(m_LogoPixmap));
        QPalette pal = palette();
        pal.setBrush(this->backgroundRole(), QBrush(m_LogoPixmap));
        this->setPalette(pal);
        this->setMask(m_LogoPixmap.mask());
        this->setAutoFillBackground(true);
        this->setWindowOpacity(0);
    }
    else if (m_iPngType == 1)
    {
        //设置固定大小
        this->setFixedSize(m_LogoPixmap.size());
        //设置背景透明 point重绘无锯齿
        this->setAttribute(Qt::WA_TranslucentBackground);
    }
    m_trayIcon->setIcon(QIcon(m_LogoPixmap));
}

void WSuspension::setUIImage(QString GIF, QSize _size)
{
    if (2 == m_iPngType)
    {
        this->setFixedSize(QPixmap(GIF).size());
        m_vlogo->stop();
        m_vlogo->setFileName(GIF);
        if (_size != QSize(0, 0))
            m_vlogo->setScaledSize(_size);
        m_vlogo->start();
        m_trayIcon->setIcon(QIcon(QString(GIF)));
    }
}

void WSuspension::paintEvent(QPaintEvent* ev)
{
    ev->accept();
    if (m_iPngType == 1)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        m_LogoPixmap.scaled(size());
        painter.drawPixmap(0, 0, m_LogoPixmap);
    }
}

void WSuspension::mouseDoubleClickEvent(QMouseEvent* ev)
{
    //显示主窗体
    iconActivated(QSystemTrayIcon::DoubleClick);
    ev->accept();
}

void WSuspension::closeEvent(QCloseEvent* event)
{
    if (m_trayIcon->isVisible())
    {
        hide();
        event->ignore();
    }
}

void WSuspension::contextMenuEvent(QContextMenuEvent* ev)
{
    ev->accept();
    QCursor cur = this->cursor();
    m_MenuTray->setFont(QFont("Modern", 13));
    m_MenuTray->exec(cur.pos());    //关联到光标
}

void WSuspension::Act_SuspenClick()
{
    if (!Act_Suspen->isChecked())
        this->hide();
    else
        this->show();
}

void WSuspension::Act_TrayClick()
{
    if (!Act_Tray->isChecked())
        m_trayIcon->hide();
    else
        m_trayIcon->show();
}

void WSuspension::exitSuspen()
{
    m_trayIcon->setVisible(false);
    this->close();
    exit(0);
}

void WSuspension::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
            //点击
            break;
        case QSystemTrayIcon::DoubleClick:
            //双击显示主窗口
            showMain();
            break;
        case QSystemTrayIcon::MiddleClick:
            //滚轮点击
            break;
        default:
            break;
    }
}



WClock::WClock(QWidget* parent) : WSuspension(1, parent)
{
    this->setTrayShow(false);
    this->setUIImage(QPixmap(":/modern.png"));
    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
}

WClock::~WClock()
{
}

void WClock::paintEvent(QPaintEvent* event)
{
    WSuspension::paintEvent(event);
    static const QPoint hourHand[3] =
    {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -40)
    };
    static const QPoint minuteHand[3] =
    {
        QPoint(7, 8),
        QPoint(-7, 8),
        QPoint(0, -70)
    };
    QColor hourColor(255, 255, 255, 205);
    QColor minuteColor(245, 127, 127, 255);
    int side = qMin(width(), height());
    QTime time = QTime::currentTime();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    painter.scale(side / 200.0, side / 200.0);
    painter.setPen(Qt::NoPen);
    painter.setBrush(hourColor);
    painter.save();
    painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    painter.drawConvexPolygon(hourHand, 3);
    painter.restore();
    painter.setPen(hourColor);
    painter.setPen(Qt::NoPen);
    painter.setBrush(minuteColor);
    painter.save();
    painter.rotate(6.0 * (time.minute() + time.second() / 60.0));
    painter.drawConvexPolygon(minuteHand, 3);
    painter.restore();
    painter.setPen(minuteColor);
}





WScanRotate::WScanRotate(QWidget* parent) : QWidget(parent)
    , m_pTimer(NULL)
    , m_pCboxSelect(NULL)
    , m_radarWidth(600)
    , m_radarHeight(600)
    , m_xlen(0), m_ylen(0)
    , m_iSelectType(0)
    , m_blinestate(false)
{
    this->setMinimumSize(300, 300);
    //设置背景
    QPalette pal = palette();
    pal.setBrush(QPalette::All, QPalette::Window, QBrush(Qt::black));
    //用图片填背景
    //    QPixmap pixmap = QPixmap(":/Res/bg_scan_window.png");
    //    pixmap = pixmap.scaled(size()); //用scaled()重新进行自适应窗口绘图
    //    pal.setBrush(QPalette::All, QPalette::Window, QBrush(pixmap));
    setPalette(pal);
    setWindowOpacity(1);
    setAutoFillBackground(true);
    setMouseTracking(true);
    //this->setAttribute(Qt::WA_TranslucentBackground);
    m_pCboxSelect = new QComboBox(this);
    m_pCboxlines = new QComboBox(this);
    m_pCboxSelect->setFixedSize(100, 25);
    m_pCboxlines->setFixedSize(100, 25);
    m_pCboxSelect->move(120, 10);
    m_pCboxSelect->setMaxVisibleItems(6);
    m_pCboxSelect->setEditable(false);
    m_pCboxSelect->insertItem(0, TCSR("显示模式"));
    m_pCboxSelect->insertItem(1, TCSR("轨迹模式"));
    m_pCboxSelect->insertItem(2, TCSR("目标模式"));
    connect(m_pCboxSelect, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboSelectIndexChanged(int)));
    m_pCboxlines->setVisible(false);
    //下面是轨迹编辑控件
    m_pCboxlines->move(120, 40);
    m_pCboxlines->setMaxVisibleItems(99);
    m_pCboxlines->setEditable(false);
    //m_pCombolines->insertItem(0, TCSR("0"));
    //m_pCombolines->insertItem(1, TCSR("1"));
    //m_pCombolines->insertItem(2, TCSR("2"));
    //connect(m_pCombolines, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboSelectIndexChanged(int)));
    m_pBtnadd = new QPushButton(this);
    m_pBtnadd->setFixedSize(80, 25);
    m_pBtnadd->move(230, 40);
    m_pBtnadd->setText(TCSR("添加"));
    connect(m_pBtnadd, SIGNAL(clicked()), this, SLOT(OnBtnaddClicked()));
    m_pBtnadd->setVisible(false);
    //初始化计时器。计时器每秒触发12.5次，每次都触发界面重绘，重绘时将扫描扇形区域的位置移动一点点，形成动画效果
    m_pTimer = new QTimer(this);
    m_pTimer->start(100);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(update()));
    m_currentAngle = 360;
}

WScanRotate::~WScanRotate()
{
    m_pTimer->stop();
    if (m_pTimer != NULL)
    {
        delete m_pTimer;
        m_pTimer = NULL;
    }
    if (m_pCboxSelect != NULL)
    {
        delete m_pCboxSelect;
        m_pCboxSelect = NULL;
    }
}

double EuclideanMetric(double x1, double y1, double x2, double y2)
{
    //2维
    return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}

void WScanRotate::paintObj(QPainter& painter)
{
    switch (m_iSelectType)
    {
    case 0:
    default:
        //绘制选择框
        if ((m_PosDisp.x() != 0) && (m_PosDisp.y() != 0))
        {
            //计算圆心点
            if (EuclideanMetric(m_lfCenterX, m_lfCenterY, m_PosDisp.x(), m_PosDisp.y()) < (m_radarWidth / 2))
            {
                painter.setPen(QPen(QBrush(Qt::white), 2));
                painter.drawRect(m_PosDisp.x() - 20, m_PosDisp.y() - 20, 40, 40);
            }
        }
        break;
    case 1:
    {
        static QPointF ptstart, tmppt;
        if (!m_mapPT.isEmpty())
        {
            for (QMap<int, QVector<QPointF> >::const_iterator mapit = m_mapPT.begin();
                mapit != m_mapPT.end(); ++mapit)
            {
                if (!mapit->isEmpty())
                {
                    QVector<QPointF>::const_iterator it = mapit->begin();
                    painter.setPen(QPen(QBrush(QColor(255, 0, 0, 255)), 14.0, Qt::SolidLine, Qt::RoundCap));
                    ptstart = QPointF(m_lfCenterX - it->x() * m_radarWidth, m_lfCenterY - it->y() * m_radarWidth);
                    painter.drawPoint(ptstart);
                    for (it = mapit->begin() + 1; it != mapit->end(); ++it)
                    {
                        painter.setPen(QPen(QBrush(QColor(255, 0, 0, 255)), 14.0, Qt::SolidLine, Qt::RoundCap));
                        tmppt = QPointF(m_lfCenterX - it->x() * m_radarWidth, m_lfCenterY - it->y() * m_radarWidth);
                        painter.drawPoint(tmppt);
                        painter.setPen(QPen(QBrush(QColor(255, 125, 0, 255)), 2.0, Qt::SolidLine, Qt::RoundCap));
                        painter.drawLine(ptstart, tmppt);
                        ptstart = tmppt;
                    }
                }
            }
        }
        //=====================================================
        //绘制轨迹点并联线
        //static QPointF ptstart, tmppt;
        if (!m_vecPT.empty())
        {
            QVector<QPointF>::iterator it = m_vecPT.begin();
            painter.setPen(QPen(QBrush(QColor(255, 0, 0, 255)), 14.0, Qt::SolidLine, Qt::RoundCap));
            ptstart = QPointF(m_lfCenterX - it->x() * m_radarWidth, m_lfCenterY - it->y() * m_radarWidth);
            painter.drawPoint(ptstart);
            for (it = m_vecPT.begin() + 1; it != m_vecPT.end(); ++it)
            {
                painter.setPen(QPen(QBrush(QColor(255, 0, 0, 255)), 14.0, Qt::SolidLine, Qt::RoundCap));
                tmppt = QPointF(m_lfCenterX - it->x() * m_radarWidth, m_lfCenterY - it->y() * m_radarWidth);
                painter.drawPoint(tmppt);
                painter.setPen(QPen(QBrush(QColor(255, 125, 0, 255)), 2.0, Qt::SolidLine, Qt::RoundCap));
                painter.drawLine(ptstart, tmppt);
                ptstart = tmppt;
            }
        }
        //=====================================================
    }
    break;
    case 2:
    {
        if (!m_vecflyPT.empty())
        {
            qreal _px, _py;
            for (QVector<QPointF>::iterator it = m_vecflyPT.begin(); it != m_vecflyPT.end(); ++it)
            {
                _px = m_lfCenterX - it->x() * m_radarWidth;
                _py = m_lfCenterY - it->y() * m_radarWidth;
                radialGradient(painter, _px, _py - 2, 255);
                radialGradient(painter, _px - 2, _py + 2, 255);
                radialGradient(painter, _px + 2, _py + 2, 255);
            }
        }
        /*
        RadialGradient(painter,myPosDisp.x(),myPosDisp.y()-2,255);
        RadialGradient(painter,myPosDisp.x()-2,myPosDisp.y()+2,255);
        RadialGradient(painter,myPosDisp.x()+2,myPosDisp.y()+2,255);

        //计算这个点（确切地说是点的中心像素）的亮度。由这个点的方位角和当前扫描方位角来决定
        int diff = floor(tmpAngle + 0.5) - m_currentAngle;
        if (diff < 0)
        diff += 360;
        int centerAlpha;
        if (diff > 0 && diff <= 25)         //刚进入扫描区，快速亮起
        centerAlpha = diff * 10;
        else if (diff > 25 && diff <= 70)   //扫描区内，最亮
        centerAlpha = 255;
        else if (diff > 70 && diff < 351)   //刚出扫描区，余晖，渐渐暗下去
        centerAlpha = 255 - (diff - 70)*51/56;
        //            else //远离扫描区，不显示
        //                return;

        RadialGradient(painter,myPosDisp.x(),myPosDisp.y(),centerAlpha);
        RadialGradient(painter,myPosDisp.x()+2,myPosDisp.y()+2,centerAlpha);
        RadialGradient(painter,myPosDisp.x()+3,myPosDisp.y()+3,centerAlpha);
        */
    }
    break;
    }
    //移动一点点角度，准备下次绘制。扫描区域顺时针转动，所以数值应递减。保持数值在0~360范围内。
    m_currentAngle -= 3 * 2;
    if (m_currentAngle <= 0)
        m_currentAngle += 360;
}

void WScanRotate::paintRadarline(QPainter& painter)
{
    //绘制圆心
    QPointF point(m_lfCenterX, m_lfCenterY);
    QPalette palette;
    painter.setBrush(QBrush(Qt::green, Qt::RadialGradientPattern));
    //绘制内线
    painter.setPen(QPen(QBrush(QColor(255, 255, 0, 60)), 1.0, Qt::DashLine, Qt::RoundCap));
    for (int i = 1; i < 17; ++i)
        painter.drawEllipse(point, m_radarWidth * i / 32, m_radarWidth * i / 32);
    painter.setPen(QPen(QBrush(QColor(255, 255, 0, 80)), 1.0, Qt::DashLine, Qt::RoundCap));
    for (int i = 1; i <= 60; ++i)
        painter.drawLine(point, QPointF(point.x() + m_radarWidth / 2 * cos(PI * i / 30),
                                        point.y() + m_radarHeight / 2 * sin(PI * i / 30)));
    //X轴水平线和Y轴竖直线
    painter.setPen(QPen(QBrush(QColor(0, 255, 0, 255)), 2.0, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(QPoint(0, m_lfCenterY), QPoint(m_lfCenterX * 2, m_lfCenterY));
    painter.drawLine(QPoint(m_lfCenterX, 0), QPoint(m_lfCenterX, m_lfCenterY * 2));
    painter.setPen(QPen(QBrush(QColor(0, 255, 0, 130)), 1.0, Qt::SolidLine, Qt::RoundCap));
    for (int i = 1; i < 5; ++i)
        painter.drawEllipse(point, m_radarWidth * i / 8, m_radarWidth * i / 8);
    for (int i = 1; i <= 12; ++i)
        painter.drawLine(point, QPointF(point.x() + m_radarWidth / 2 * cos(PI * i / 6),
                                        point.y() + m_radarHeight / 2 * sin(PI * i / 6)));
}

void WScanRotate::OnBtnaddClicked()
{
    if (m_blinestate)
    {
        m_mapPT.insert(m_mapPT.count(), m_vecPT);
        m_vecPT.clear();
        m_blinestate = false;
        m_pBtnadd->setText(TCSR("添加"));
        QString tmpstr = TCSR("轨迹") + QString::number(m_mapPT.size(), 10);
        m_pCboxlines->insertItem(m_pCboxlines->currentIndex(), tmpstr);
    }
    else
    {
        m_blinestate = true;
        m_pBtnadd->setText(TCSR("完成添加"));
    }
}

void WScanRotate::OnComboSelectIndexChanged(int nId)
{
    //选择模式
    m_iSelectType = nId;
    m_pCboxlines->setVisible(m_iSelectType == 1);
    m_pBtnadd->setVisible(m_iSelectType == 1);
    if (m_iSelectType != 1)
    {
        m_vecPT.clear();
        m_mapPT.clear();
        m_pCboxlines->clear();
    }
    if (m_iSelectType != 2)
        m_vecflyPT.clear();
    update();
}

void WScanRotate::paintEvent(QPaintEvent* ev)
{
    //设置背景
    QPalette pal = palette();
    pal.setBrush(QPalette::All, QPalette::Window, QBrush(Qt::black));
    setPalette(pal);
    setWindowOpacity(1);
    setAutoFillBackground(true);
    //设置中心点 以高为边计算
    if (size().height() < 300)
        m_radarWidth = m_radarHeight = 300;
    else if (size().height() < size().width())
        m_radarWidth = m_radarHeight = size().height();
    else
        m_radarWidth = m_radarHeight = size().width();
    m_lfCenterX = size().width() / 2;
    m_xlen = m_lfCenterX - m_radarWidth / 2;    //得到x坐标的偏移
    m_lfCenterY = size().height() / 2;
    m_ylen = m_lfCenterY - m_radarHeight / 2;   //得到y坐标的偏移
    QPainter painter(this);
    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //绘制对象
    paintObj(painter);
    //绘制雷达线
    paintRadarline(painter);
    //画扫描区域
    paintScanArea(painter);
    ev->accept();
}

void WScanRotate::mousePressEvent(QMouseEvent* msv)
{
    switch (m_iSelectType)
    {
        case 0:
        default:
            if (msv->button() == Qt::LeftButton)
                m_PosDisp = msv->pos();
            break;
        case 1:
            if (msv->button() == Qt::RightButton)
            {
                m_vecPT.clear();
                update();
            }
            else if (msv->button() == Qt::LeftButton && m_blinestate)
            {
#if (QT_VERSION <= QT_VERSION_CHECK(4, 8, 7))
                m_vecPT.insert(0, QPointF((m_lfCenterX - msv->posF().x()) / m_radarWidth, (m_lfCenterY - msv->posF().y()) / m_radarWidth));
#else
                m_vecPT.insert(0, QPointF((m_lfCenterX - msv->localPos().x()) / m_radarWidth, (m_lfCenterY - msv->localPos().y()) / m_radarWidth));
#endif
            }
            break;
        case 2:
            if (msv->button() == Qt::RightButton)
            {
                m_vecflyPT.clear();
                update();
            }
            else
            {
#if (QT_VERSION <= QT_VERSION_CHECK(4, 8, 7))
                if (EuclideanMetric(m_lfCenterX, m_lfCenterY, msv->posF().x(), msv->posF().y()) < (m_radarWidth / 2))
                    m_vecflyPT.insert(0, QPointF((m_lfCenterX - msv->posF().x()) / m_radarWidth, (m_lfCenterY - msv->posF().y()) / m_radarWidth));
#else
                if (EuclideanMetric(m_lfCenterX, m_lfCenterY, msv->localPos().x(), msv->localPos().y()) < (m_radarWidth / 2))
                    m_vecflyPT.insert(0, QPointF((m_lfCenterX - msv->localPos().x()) / m_radarWidth, (m_lfCenterY - msv->localPos().y()) / m_radarWidth));
#endif
            }
            break;
    }
}

void WScanRotate::mouseReleaseEvent(QMouseEvent* msv)
{
    update();
    msv->accept();
}

void WScanRotate::mouseMoveEvent(QMouseEvent* msv)
{
    msv->accept();
}

void WScanRotate::paintScanArea(QPainter& painter)
{
    QPointF center(m_lfCenterX, m_lfCenterY);
    QRectF rect(m_lfCenterX - m_radarWidth / 2, m_lfCenterY - m_radarHeight / 2, m_radarWidth, m_radarHeight);    //QRectF rect(0, 0, m_radarWidth, m_radarHeight);
    painter.setPen(QPen(Qt::NoPen));       //无边缘线
    //设置一个锥形渐变，以雷达图圆心为中心，沿着逆时针方向逐渐减少alpha值
    QConicalGradient gradient(center, m_currentAngle);
    gradient.setColorAt(0,    QColor(0, 255, 0, 96/*96*/));
    gradient.setColorAt(0.25, QColor(0, 255, 0, 0));
    painter.setBrush(QBrush(gradient));
    //用指定的颜色渐变绘制扇形区域。渐变和扇形区域的起始角度由m_currentAngle成员控制
    painter.drawPie(rect, m_currentAngle * 16 , 90 * 16);
}

void WScanRotate::radialGradient(QPainter& paint, QPointF& flypt, int centerAlpha)
{
    QPointF pointCenter2(flypt.x() - 1, flypt.y() - 5);
    paint.setPen(QPen(Qt::NoPen));       //无边缘线
    //设置一个辐射渐变，向外逐渐减少alpha值。整体上由centerAlpha来控制
    QRadialGradient gradient(flypt, 20);
    gradient.setColorAt(0, QColor(255, 255, 255, centerAlpha));
    gradient.setColorAt(0.1, QColor(255, 255, 255, centerAlpha));
    gradient.setColorAt(0.2, QColor(128, 255, 128, 0.3 * centerAlpha));
    gradient.setColorAt(1, QColor(128, 255, 128, 0));
    paint.setBrush(QBrush(gradient));
    paint.drawEllipse(flypt, 20, 20);
}

void WScanRotate::radialGradient(QPainter& paint, float pointX, float pointY, int centerAlpha)
{
    QPointF pointCenter(pointX, pointY);
    paint.setPen(QPen(Qt::NoPen));       //无边缘线
    //设置一个辐射渐变，向外逐渐减少alpha值。整体上由centerAlpha来控制
    QRadialGradient gradient(pointCenter, 20);
    gradient.setColorAt(0, QColor(255, 255, 255, centerAlpha));
    gradient.setColorAt(0.1, QColor(255, 255, 255, centerAlpha));
    gradient.setColorAt(0.2, QColor(128, 255, 128, 0.3 * centerAlpha));
    gradient.setColorAt(1, QColor(128, 255, 128, 0));
    paint.setBrush(QBrush(gradient));
    paint.drawEllipse(pointCenter, 20, 20);
}



WRotate::WRotate(QWidget* parent /*= 0*/)
{
    bj.load(":radarbj.png");
}

WRotate::~WRotate()
{
}

void WRotate::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width() / 2, height() / 2);               //移动原点

    int Side = (width() < height()) ? width() : height();       //取边界最小值
    painter.scale(Side / 300.0, Side / 300.0);                  //设置画布边缘
    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::black);
    painter.drawEllipse(QPoint(0, 0), 150, 150);                //绘制黑色背景区
    painter.drawPixmap(0 - bj.width()/2, 0-bj.height()/2, bj);  //背景图

    //绘制标线
    drawDistanceMarking(&painter);
}

void WRotate::drawDistanceMarking(QPainter* painter)
{
    //设置白色画笔.
    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::white);  //white  green
    //painter->save();
    QFont font;
    font.setPixelSize(5);
    painter->setFont(font);
    //绘制线圈数
    int Count = 6;
    int PixelPerMark = 150 / Count;
    for (int i = 1; i < Count; ++i)
        painter->drawEllipse(QPoint(0, 0), PixelPerMark * i, PixelPerMark * i);
    painter->setPen(Qt::white);
    painter->drawEllipse(QPoint(0, 0), 150, 150);    
    painter->restore();
    //绘制角度标线.
    for (int i = 0; i < 12; ++i)
    {
        painter->drawLine(150, 0, -150, 0);
        painter->rotate(30.0);
    }
}



QWidgetEmp::QWidgetEmp(QWidget* parent) :QWidget(parent)
{
    //创建彩蛋
    lvshuiling = new WSuspension(2, this);
    lvshuiling->setParent(this, 0);
    lvshuiling->move(10, 180);
    lvshuiling->setUIImage(QString(":/lvshuiling.gif"));
    lvshuiling->supenShow(Appcfg::Instance()->Sdat.GoodGif);
    mogu = new WSuspension(2, this);
    mogu->setParent(this, 0);
    mogu->move(20, 30);
    mogu->setUIImage(QString(":/mogu.gif"));
    mogu->supenShow(Appcfg::Instance()->Sdat.GoodGif);
}

QWidgetEmp::~QWidgetEmp()
{
}




#define M_PI 3.14159265358979323846

NProgreCirWater::NProgreCirWater(QWidget *parent)
    : QWidget(parent)
    , m_iNowProgre(50)
    , m_fwaterHeight(0.0f)
    , m_fwaterDensity(30)
    , m_fwaterRatio(0.5f)
{
    // A振幅  默认的振幅是高度的0.05f
    m_fwaterHeight = this->height() * 0.15f;
    connect(&m_TUpGuiimer, SIGNAL(timeout()), this, SLOT(upAutoTime()));
    m_TUpGuiimer.start(10);

}

NProgreCirWater::~NProgreCirWater()
{
}

void NProgreCirWater::setWaterProgre(int progre)
{
    this->m_iNowProgre = progre;
}

void NProgreCirWater::setWaterHeight(float progre)
{
    this->m_fwaterHeight = progre;
}

void NProgreCirWater::setWaterDensity(float progre)
{
    this->m_fwaterDensity = progre;
}

void NProgreCirWater::drawWord(QPainter& drawWaterPainter)
{
    //绘制进度显示
    int nFontSize = this->width() / 6;
    QFont font;
    font.setPixelSize(nFontSize);
    drawWaterPainter.setFont(font);
    drawWaterPainter.setPen(QPen(Qt::white));
        int nNowPro = 100 - (int)(m_fwaterRatio * 100);
    QString strProgre = QString::number(nNowPro);
    strProgre.append("%");
    drawWaterPainter.drawText(this->width() / 2 - (nFontSize / 2 - 2), this->height() / 2 + nFontSize / 2, strProgre);
    drawWaterPainter.end();
}

void NProgreCirWater::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::HighQualityAntialiasing);//设置反锯齿
    int height = this->height();
    int width = this->width();

    // ω周期  让一个周期的宽度正好是width
    double frequency = 3 * M_PI / width;
    
    // k（y轴偏移量，进度） 默认的进度是50%
    float level = height * m_fwaterRatio;

    QPixmap waveBitmap(width, height);
    QPainter drawWaterPainter(&waveBitmap);
    drawWaterPainter.setPen(Qt::NoPen);
    drawWaterPainter.setBrush(Qt::red);

    QPainterPath abovePath;
    QPainterPath behindPath;
    abovePath.moveTo(0, height);
    behindPath.moveTo(0, height);
    //m_fwaterHeight++;
    m_fwaterDensity++;
    for (int x = 0; x <= width; x++) 
    {
        // y=Asin(ωx+φ)+k
        float aboveY = (float)(m_fwaterHeight * sin(frequency * x + m_fwaterDensity)) + level;
        // 背面的水波偏移一些，和前面的错开。
        //float behindY = (float)(m_fwaterHeight * sin(frequency * x + width / 4 * frequency + m_fwaterDensity)) + level;
        abovePath.lineTo(x, aboveY);
        //behindPath.lineTo(x, behindY);
    }

    //m_fwaterDensity += 0.05;
    //if (m_fwaterDensity > (width / 2))
    //{
    //    m_fwaterDensity = 0;
    //}
    //for (int x = 0; x <= width; x++) 
    //{
    //    // y=Asin(ωx+φ)+k
    //    float aboveY = (float)(amplitude * sin(frequency * x + m_fwaterDensity)) + level;
    //    // 背面的水波偏移一些，和前面的错开。
    //    float behindY = (float)(amplitude * sin(frequency * x + width / 4 * frequency + m_fwaterDensity)) + level;
    //    abovePath.lineTo(x, aboveY);
    //    behindPath.lineTo(x, behindY);
    //}
    abovePath.lineTo(width + 1, height);
    behindPath.lineTo(width + 1, height);
    drawWaterPainter.setBrush(QColor(169, 245, 233, 255));
    drawWaterPainter.drawPath(behindPath);
    drawWaterPainter.setBrush(QColor(40, 230, 200, 255));
    drawWaterPainter.drawPath(abovePath);
    
    //绘制进度字
    drawWord(drawWaterPainter);
    //旋转
    //waveBitmap=QTool::QPixmapToRound(waveBitmap,(float)100);
    painter.drawPixmap(0, 0, waveBitmap);

    //createShader(painter);
    QWidget::paintEvent(event);
}

void NProgreCirWater::upAutoTime()
{
    update();
}






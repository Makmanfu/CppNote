
#include "stdafx.h"
#include "waxis.h"


PadDraw::PadDraw(QWidget* parent) : QWidget(parent)
    , currentCategory(Shape::dRect)
    , shape(NULL)
    , isLBtnPressed(false)
{
    shapes.clear();
    //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

PadDraw::~PadDraw()
{
}

void PadDraw::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    //painter.setBrush(Qt::white);
    //painter.drawRect(0, 0, size().width(), size().height());
    foreach (Shape* shape, shapes)
        shape->paint(painter);
}

void PadDraw::mousePressEvent(QMouseEvent* event)
{
    switch (currentCategory)
    {
        case Shape::dLine:
        {
            shape = new Line;
            break;
        }
        case Shape::dRect:
        {
            shape = new Rect;
            break;
        }
        case Shape::dCurve:
        {
            shape = new Curve;
            break;
        }
    }
    if (shape != NULL)
    {
        isLBtnPressed = true;
        shapes << shape; //shapes.append(shape);
        shape->setStart(getPos(event));
        shape->setEnd(getPos(event));
    }
}

void PadDraw::mouseMoveEvent(QMouseEvent* event)
{
    if (shape && isLBtnPressed)
    {
        if (currentCategory == Shape::dCurve)
            shape->addPoint(getPos(event));
        else
            shape->setEnd(getPos(event));
        update();
    }
}

void PadDraw::mouseReleaseEvent(QMouseEvent* event)
{
    shape->setEnd(getPos(event));
    isLBtnPressed = false;
}


WAxis::WAxis(QWidget* parent) : QWidget(parent)
    , Btn_Pt0(NULL)
    , Btn_ZoomIn(NULL)
    , Btn_ZoomOut(NULL)
    , menu(NULL)
    , Act_Pt0(NULL)
    , Btn_PtS(NULL)
    , Act_Normal(NULL)
    , Act_curve(NULL)
    , Act_line(NULL)
    , Act_circle(NULL)
    , Act_Data(NULL)

    , m_Width(600)
    , m_Height(600)
    , drawState(0)
    , m_BorderLen(0)    //40
    , Gstep(30)      //默认基线格
{
    this->setMinimumSize(500, 500);
    //设置背景颜色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(255, 255, 255));
    setPalette(pal);
    //计算中心坐标
    m_CenterX = m_Width / 2;
    m_CenterY = m_Height / 2;
    //控件设计
    GuiInit();
    //默认设置白色风格
    //SetStyColor(QColor(0,0,0),QColor(0, 255, 0, 255),QColor(255, 255, 0, 60));
    SetStyColor(QColor(255, 255, 255), QColor(0, 0, 0, 255), QColor(100, 10, 0, 180));
}

WAxis::~WAxis()
{
}

void WAxis::GuiInit(void)
{
    //创建菜单
    menucreate();

    int k = 130;
    //创建放大缩小原点按钮
    Btn_ZoomIn = new QPushButton(this);
    Btn_ZoomIn->setFixedSize(25, 25);
    Btn_ZoomIn->setText("-");
    Btn_ZoomIn->move(k, 10);
    Btn_ZoomIn->setStyleSheet("QPushButton {color: rgb(0,88,152) background-color: rgba(97%,80%,9%,10%)}");
    connect(Btn_ZoomIn, SIGNAL(clicked()), SLOT(OnBtnZoomInClicked()));
    Btn_ZoomOut = new QPushButton(this);
    Btn_ZoomOut->setFixedSize(25, 25);
    Btn_ZoomOut->setText("+");
    Btn_ZoomOut->move(k+30, 10);
    Btn_ZoomOut->setStyleSheet("QPushButton {color: rgb(0,88,152) background-color: rgba(97,80,9,50)}");
    connect(Btn_ZoomOut, SIGNAL(clicked()), SLOT(OnBtnZoomOutClicked()));
    Btn_Pt0 = new QPushButton(this);
    Btn_Pt0->setFixedSize(25, 25);
    Btn_Pt0->setText(QString::fromLocal8Bit("0"));
    Btn_Pt0->move(k+60, 10);
    connect(Btn_Pt0, SIGNAL(clicked()), SLOT(OnBtnPt0Clicked()));
    Btn_PtS = new QPushButton(this);
    Btn_PtS->setFixedSize(25, 25);
    Btn_PtS->setText(QString::fromLocal8Bit("S"));
    Btn_PtS->move(k+90, 10);
    connect(Btn_PtS, SIGNAL(clicked()), SLOT(OnBtnPtSClicked()));
    Btn_PtD = new QPushButton(this);
    Btn_PtD->setFixedSize(25, 25);
    Btn_PtD->setText(QString::fromLocal8Bit("D"));
    Btn_PtD->move(k+120, 10);
    connect(Btn_PtD, SIGNAL(clicked()), SLOT(OnBtnPtDClicked()));
}

void WAxis::menucreate(void)
{
    //定义程序菜单
    Act_Pt0 = new QAction(QString::fromLocal8Bit("回到原点"), this);
    connect(Act_Pt0, SIGNAL(triggered()), this, SLOT(OnBtnPt0Clicked()));
    Act_Normal = new QAction(QString::fromLocal8Bit("移动"), this);
    connect(Act_Normal, SIGNAL(triggered()), this, SLOT(OnAct_NorClicked()));
    Act_Sty = new QAction(QString::fromLocal8Bit("白色主题"), this);
    Act_Sty->setCheckable(true);
    Act_Sty->setChecked(true);
    connect(Act_Sty, SIGNAL(triggered()), this, SLOT(OnAct_StyClicked()));
    Act_line = new QAction(QString::fromLocal8Bit("画直线"), this);
    connect(Act_line, SIGNAL(triggered()), this, SLOT(OnAct_lineClicked()));
    Act_curve = new QAction(QString::fromLocal8Bit("画曲线"), this);
    connect(Act_curve, SIGNAL(triggered()), this, SLOT(OnAct_curveClicked()));
    Act_circle = new QAction(QString::fromLocal8Bit("画圆"), this);
    connect(Act_circle, SIGNAL(triggered()), this, SLOT(OnAct_circleClicked()));
    Act_Data = new QAction(QString::fromLocal8Bit("绘制数据"), this);
    connect(Act_Data, SIGNAL(triggered()), this, SLOT(OnAct_DataClicked()));
    menu = new QMenu(this);
    menu->setFont(QFont("Modern", 13));
    menu->addAction(Act_Pt0);       //添加菜单项
    menu->addAction(Act_Normal);
    menu->addAction(Act_Sty);
    menu->addSeparator();
    menu->addAction(Act_line);
    menu->addAction(Act_curve);
    menu->addAction(Act_circle);
    menu->addSeparator();
    menu->addAction(Act_Data);
}

void WAxis::paintDrawXY()
{
    QPainter painter(this);
    //绘制色标系
    QLinearGradient linearGradient(QPointF(40, 60), QPointF(40, 190));
    linearGradient.setColorAt(0.0, Qt::green);
    linearGradient.setColorAt(1.0, Qt::white);
    painter.setBrush(QBrush(linearGradient));
    painter.drawRect(30, 60, 20, 130);
    //原点
    QPointF point(m_CenterX, m_CenterY);
    painter.setBrush(QBrush(Qt::green, Qt::RadialGradientPattern));
    painter.setPen(QPen(QBrush(lineColor), 2.0, Qt::SolidLine, Qt::RoundCap));
    //绘制Y轴线
    if (m_CenterX < m_Width - m_BorderLen && m_CenterX > m_BorderLen)
        painter.drawLine(QPointF(m_CenterX, m_BorderLen), QPointF(m_CenterX, m_Height - m_BorderLen));
    //绘制X轴线
    if (m_CenterY < m_Height - m_BorderLen && m_CenterY > m_BorderLen)
        painter.drawLine(QPointF(m_BorderLen, m_CenterY), QPointF(m_Width - m_BorderLen, m_CenterY));
    painter.drawRect(m_BorderLen, m_BorderLen, m_Width - 2 * m_BorderLen, m_Height - 2 * m_BorderLen);
    //绘制无穷坐标系
    painter.setPen(QPen(QBrush(smallColor), 1.0, Qt::DashLine, Qt::RoundCap));
    int i = 0;
    while (m_CenterX - Gstep*++i > m_BorderLen ? i = i : i = 0)
        if (m_CenterX - Gstep * i < m_Width - m_BorderLen)
            painter.drawLine(QPointF(m_CenterX - Gstep * i, m_BorderLen), QPointF(m_CenterX - Gstep * i, m_Height - m_BorderLen));
    while (m_CenterX + Gstep*++i < m_Width - m_BorderLen ? i = i : i = 0)
        if (m_CenterX + Gstep * i > m_BorderLen)
            painter.drawLine(QPointF(m_CenterX + Gstep * i, m_BorderLen), QPointF(m_CenterX + Gstep * i, m_Height - m_BorderLen));
    while (m_CenterY - Gstep*++i > m_BorderLen ? i = i : i = 0)
        if (m_CenterY - Gstep * i < m_Height - m_BorderLen)
            painter.drawLine(QPointF(m_BorderLen, m_CenterY - Gstep * i), QPointF(m_Width - m_BorderLen, m_CenterY - Gstep * i));
    while (m_CenterY + Gstep*++i < m_Height - m_BorderLen ? i = i : i = 0)
        if (m_CenterY + Gstep * i > m_BorderLen)
            painter.drawLine(QPointF(m_BorderLen, m_CenterY + Gstep * i), QPointF(m_Width - m_BorderLen, m_CenterY + Gstep * i));
    //绘制无穷标尺
    painter.setFont(QFont("Courier", 8));
    painter.setPen(QPen(QBrush(lineColor), 1.0, Qt::SolidLine, Qt::RoundCap));
    m_ScaleY = m_CenterY < m_BorderLen ? 15 : m_CenterY + 10;
    //标尺刻度计算
    if (m_CenterY > m_Height - m_BorderLen - 10) m_ScaleY = m_Height - m_BorderLen - 10;
    else if (m_CenterY < m_BorderLen) m_ScaleY = m_BorderLen + 10;
    else  m_ScaleY = m_CenterY + 10;
    if (m_CenterX > m_Width - m_BorderLen - 13) m_ScaleX = m_Width - m_BorderLen - 22;
    else if (m_CenterX < m_BorderLen) m_ScaleX = m_BorderLen + 3;
    else  m_ScaleX = m_CenterX + 3;
    //绘制无穷刻度
    while (m_CenterX - Gstep*++i > m_BorderLen ? i = i : i = 0)
        if (m_CenterX - Gstep * i < m_Width - m_BorderLen)
            painter.drawText(QPointF(m_CenterX - Gstep * i, m_ScaleY), QString("-%1").arg(i * 10));
    while (m_CenterX + Gstep*++i < m_Width - m_BorderLen ? i = i : i = 0)
        if (m_CenterX + Gstep * i > m_BorderLen)
            painter.drawText(QPointF(m_CenterX + Gstep * i, m_ScaleY), QString("%1").arg(i * 10));
    while (m_CenterY - Gstep*++i > m_BorderLen ? i = i : i = 0)
        if (m_CenterY - Gstep * i < m_Height - m_BorderLen)
            painter.drawText(QPointF(m_ScaleX, m_CenterY - Gstep * i), QString("%1").arg(i * 10));
    while (m_CenterY + Gstep*++i < m_Height - m_BorderLen ? i = i : i = 0)
        if (m_CenterY + Gstep * i > m_BorderLen)
            painter.drawText(QPointF(m_ScaleX, m_CenterY + Gstep * i), QString("-%1").arg(i * 10));
}

void WAxis::paintDrawData(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);
    //相对于原点绘制数据
    if (m_listData.isEmpty())
        return;
    for (QList<POSTION>::iterator iterList = m_listData.begin(); iterList != m_listData.end(); iterList++)
    {
        painter.setPen(QPen(QBrush(QColor(0, iterList->Z, 0, 255)), 8.0, Qt::SolidLine, Qt::RoundCap));
        painter.drawPoint(m_CenterX + (iterList->X * Gstep / 10), m_CenterY - (iterList->Y * Gstep / 10));
    }
}

void WAxis::SetStyColor(QColor bj, QColor linec, QColor linesmall)
{
    lineColor = linec;
    smallColor = linesmall;
    //设置背景颜色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, bj);
    setPalette(pal);
    //设置为自动填充背景
    setAutoFillBackground(true);
}

void WAxis::OnBtnPt0Clicked()
{
    m_CenterX = m_Width / 2;
    m_CenterY = m_Height / 2;
    update();
}

void WAxis::OnBtnPtSClicked()
{
    m_CenterX = 20;
    m_CenterY = m_Height - 20;
    update();
    //SetMoveCenterPT(POSTION(10-m_Width / 2, 10-m_Height / 2));
}

void WAxis::OnBtnPtDClicked()
{
    if (m_listData.empty())
        OnBtnPtSClicked();
    else
    {
        double xmin(1000), xmax(0), ymin(1000), ymax(0), X(0), Y(0);
        for (QList<POSTION>::const_iterator it = m_listData.begin();
             it != m_listData.end(); ++it)
        {
            if (it->X > xmax) xmax = it->X;
            if (it->X < xmin) xmin = it->X;
            if (it->Y > ymax) ymax = it->Y;
            if (it->Y < ymin) ymin = it->Y;
        }
        X = (xmax + xmin) / 2;
        Y = (ymax + ymin) / 2;
        SetMoveCenterPT(POSTION(-X, -Y));
    }
}

void WAxis::OnAct_StyClicked()
{
    if (!Act_Sty->isChecked())
        SetStyColor(QColor(0, 0, 0), QColor(0, 255, 0, 255), QColor(255, 255, 0, 60));
    else
        SetStyColor(QColor(255, 255, 255), QColor(0, 0, 0, 255), QColor(100, 10, 0, 180));
}

void WAxis::OnAct_NorClicked()
{
    drawState = 0;
    currentCategory = Shape::dMove;
    shapes.clear();
}

void WAxis::OnAct_lineClicked()
{
    drawState = 1;
    currentCategory = Shape::dLine;
}

void WAxis::OnAct_curveClicked()
{
    drawState = 2;
    currentCategory = Shape::dCurve;
}

void WAxis::OnAct_circleClicked()
{
    drawState = 3;
    currentCategory = Shape::dRect;
}

void WAxis::OnAct_DataClicked()
{
    drawState = 4;
    m_listData.clear();
    m_listData.insert(0, POSTION(10, 60, 255));
    m_listData.insert(0, POSTION(20, 70, 215));
    m_listData.insert(0, POSTION(30, 80, 235));
    m_listData.insert(0, POSTION(40, 90, 225));
    m_listData.insert(0, POSTION(50, 100, 145));
    m_listData.insert(0, POSTION(60, 100, 205));
    m_listData.insert(0, POSTION(70, 90, 195));
    m_listData.insert(0, POSTION(80, 80, 185));
    m_listData.insert(0, POSTION(90, 70, 175));
    m_listData.insert(0, POSTION(100, 60, 165));
    update();
}

void WAxis::OnBtnZoomInClicked()
{
    if (Gstep >= 15)
        Gstep -= 5;
    update();
}

void WAxis::OnBtnZoomOutClicked()
{
    if (Gstep <= 100)
        Gstep += 5;
    update();
}

void WAxis::contextMenuEvent(QContextMenuEvent* event)
{
    MousePt = MouseEd = QPointF(0, 0);
    QCursor cur = this->cursor();
    menu->exec(cur.pos());          //关联到光标
    event->accept();
}

void WAxis::paintEvent(QPaintEvent* event)
{
    //初始值
    m_Width = size().width();
    m_Height = size().height();
    //绘制坐标轴
    paintDrawXY();
    QPainter painter(this);
    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);
    foreach (Shape* shape, shapes)
    {
        //shape->setCenterPtstep(QPointF(m_CenterX,m_CenterY), Gstep);
        shape->paint(painter);
    }
#ifdef oldcode 原来代码
    //switch (drawState)
    //{
    //case 0: default:
    //    break;
    //case 1:
    //    {   //绘制直线
    //        painter.setPen(QPen(QBrush(lineColor), 2.0, Qt::SolidLine, Qt::RoundCap ));
    //        painter.drawLine(MousePt,MouseEd);
    //    }
    //    break;
    //case 2:
    //    //曲线
    //    break;
    //case 3:
    //    {
    //        painter.setPen(QPen(QBrush(QColor(250,0,0)), 2.0, Qt::SolidLine, Qt::RoundCap));
    //        painter.drawRect(MousePt.x(), MousePt.y(), MouseEd.x() - MousePt.x(), MouseEd.y() - MousePt.y());
    //    }
    //    break;
    //case 4:
    //    {   //绘制数据
    //        painter.setPen(QPen(QBrush(QColor(0, 0, 250, 255)), 2.0, Qt::SolidLine, Qt::RoundCap ));
    //        painter.drawEllipse(QPoint(m_CenterX+60* Gstep / 10,m_CenterY-50* Gstep / 10),40* Gstep / 10,40* Gstep / 10);
    //        paintDrawData(painter);
    //    }
    //    break;
    //}
#endif
    event->accept();
}

void WAxis::mousePressEvent(QMouseEvent* event)
{
    isPressed = true;
    switch (currentCategory)
    {
        case Shape::dLine:
        {
            shape = new Line;
            break;
        }
        case Shape::dRect:
        {
            shape = new Rect;
            break;
        }
        case Shape::dCurve:
        {
            shape = new Curve;
            break;
        }
    }
    if (shape != NULL)
    {
        shapes << shape;  //shapes.append(shape);
        shape->setStart(getPos(event));
        shape->setEnd(getPos(event));
    }
    MousePt = MouseEd = getPos(event);
    update();
}

void WAxis::mouseReleaseEvent(QMouseEvent* event)
{
    isPressed = false;
    shape->setEnd(getPos(event));
    MousePt = getPos(event);
}

void WAxis::mouseMoveEvent(QMouseEvent* event)
{
    if (shape && isPressed)
    {
        if (currentCategory == Shape::dCurve)
            shape->addPoint(getPos(event));
        else if (currentCategory == Shape::dMove)
        {
            //移动坐标系
            m_CenterX += getPos(event).rx() - MousePt.rx();
            m_CenterY += getPos(event).ry() - MousePt.ry();
            MousePt = getPos(event);
        }
        else
            shape->setEnd(getPos(event));
        update();
    }
    //    if(isPressed)
    //    {
    //        switch (drawState)
    //        {
    //        case 0:default:
    //            //移动坐标系
    //            m_CenterX += getPos(event).rx() - MousePt.rx();
    //            m_CenterY += getPos(event).ry() - MousePt.ry();
    //            MousePt = getPos(event);
    //            break;
    //        case 1:
    //            //绘制直线
    //            MouseEd = getPos(event);
    //            break;
    //        case 2:
    //            //绘制曲线
    //            break;
    //        case  3:
    //            MouseEd = getPos(event);
    //            break;
    //        }
    //        update();
    //    }
}

void WAxis::wheelEvent(QWheelEvent* event)
{
    //滚动的角度，*8就是鼠标滚动的距离
    int numDegrees = event->delta() / 8;
    //滚动的步数，*15就是鼠标滚动的角度
    int numSteps = numDegrees / 15;
    if (Gstep <= 15) Gstep = 15;
    if (Gstep >= 100) Gstep = 100;
    Gstep += numSteps;
    update();
    event->accept();      //接收该事件
}

void WAxis::SetMoveCenterPT(POSTION pt)
{
    m_CenterX = m_Width / 2 + (pt.X * Gstep / 10);
    m_CenterY = m_Height / 2 - (pt.Y * Gstep / 10);
    update();
}

void WAxis::setCurrentShape(Shape::Category s)
{
    if (s != currentCategory)
        currentCategory = s;
}










#include "stdafx.h"
#include "wtimenumbar.h"

TimeNumBar::TimeNumBar(QWidget* _parent) :QWidget(_parent)
, m_minvalue(0)
, m_maxvalue(1000)
, m_value(0)
, m_minWvalue(200)
, m_maxWvalue(800)
, m_bStart(false)
{
    this->setMinimumSize(QSize(100, 180));
    //this->setMaximumWidth(100);
    //qApp->setStyle(new QCleanlooksStyle);
    m_vSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    //创建time
    m_timeRecord = new QTime(0, 0, 0); //初始化时间
    m_labName = new QLabel(this);
    m_labName->setFrameShape(QFrame::Panel);
    m_labName->setFrameShadow(QFrame::Plain);
    m_labName->setAlignment(Qt::AlignCenter);
    m_labName->setText(TCSR("1分钟后爆炸"));
    m_timelcd = new QLCDNumber(this);
    m_timelcd->setDigitCount(8);
    m_timelcd->setSegmentStyle(QLCDNumber::Flat);
    m_timelcd->display(m_timeRecord->toString("hh:mm:ss"));
    m_alltimelcd = new QLCDNumber(this);
    m_alltimelcd->setDigitCount(8);
    m_alltimelcd->setSegmentStyle(QLCDNumber::Flat);
    m_alltimelcd->display(m_timeRecord->toString("hh:mm:ss"));
    m_grid = new QGridLayout(this);
    m_grid->setSpacing(5);
    m_grid->setContentsMargins(5, 5, 5, 5);
    m_grid->addWidget(m_labName, 0, 0, 1, 1);
    m_grid->addWidget(m_alltimelcd, 1, 0, 1, 1);
    m_grid->addItem(m_vSpacer, 2, 0, 1, 1);
    m_grid->addWidget(m_timelcd, 3, 0, 1, 1);
    m_grid->setRowStretch(0, 1);
    m_grid->setRowStretch(1, 1);
    m_grid->setRowStretch(2, 7);
    m_grid->setRowStretch(3, 1);

    //计时器创建
    m_Timer= new QTimer(this);
    m_Timer->setInterval(1000);
    //把信号与槽进行连接
    connect(m_Timer,SIGNAL(timeout()),this,SLOT(TimeSlot()));
    //下面是测试----------------------------------------------------------
    m_TestTmr= new QTimer(this);
    m_TestTmr->setInterval(1000);
    connect(m_TestTmr, SIGNAL(timeout()), this, SLOT(TestTimeSlot()));
    kk = 60;
    setShowName(TCSR("一分钟炸死"));  //显示标题
    setAllTimeShow("00:01:00");                         //总时间
    setTimeHMS(0,1,0);                                  //倒计时
    setBarMaxMin(60, 0);
    setBarWarllMaxMin(45, 15);
    m_TestTmr->start();
    timerPlayPause();                                   //开始
}

TimeNumBar::~TimeNumBar()
{
}

void TimeNumBar::setBarMaxMin(double _maxval, double _minval)
{
    this->m_minvalue = _minval;
    this->m_maxvalue = _maxval;
}

void TimeNumBar::setBarWarllMaxMin(double _maxval, double _minval)
{
    this->m_minWvalue = _minval;
    this->m_maxWvalue = _maxval;
}

void TimeNumBar::setBarValue(double val)
{
    if (val < this->m_minvalue)
        this->m_value = this->m_minvalue;
    else if (val > this->m_maxvalue)
        this->m_value = this->m_maxvalue;
    else 
        this->m_value = val;
}

void TimeNumBar::timerPlayPause(void)
{
    if(!m_bStart) //尚未开始 开始计时
    {
        m_Timer->start();
    }
    else //已经开始，暂停
    {
        m_Timer->stop();
    }
    m_bStart = !m_bStart;
}

void TimeNumBar::playTimer(bool bl)
{
    if (bl)
    {
        m_Timer->start();
        m_bStart = true;
    }
    else {
        m_Timer->stop();
        m_bStart = false;
    }
}

void TimeNumBar::setTimeHMS(int s)
{
    if (s >= 24 * 3600 || s <= 0)
        return;
    int hour(s / 3600), min(s % 3600 / 60), sec(s % 3600 % 60);
    m_timeRecord->setHMS(hour,min,sec);
}

void TimeNumBar::setTimeHMS(int h, int m, int s)
{
    m_timeRecord->setHMS(h, m, s);
}

void TimeNumBar::setAllTimeShow(int s)
{
    if (s >= 24 * 3600 || s <= 0)
        return;
    int hour(s / 3600), min(s % 3600 / 60), sec(s % 3600 % 60);
    QTime* tmp = new QTime(hour, min, sec);
    m_alltimelcd->display(tmp->toString("hh:mm:ss"));
}

void TimeNumBar::setAllTimeShow(const QString& str)
{
    m_alltimelcd->display(str);
}

void TimeNumBar::setShowName(const QString& str)
{
    this->m_labName->setText(str);
}

void TimeNumBar::TimeSlot()
{
    if (m_timeRecord->toString("hh:mm:ss").compare("00:00:01")==0)
    {
        m_Timer->stop();
        m_bStart = true;
    }
    *m_timeRecord = m_timeRecord->addSecs(-1);
    m_timelcd->display(m_timeRecord->toString("hh:mm:ss"));
}

void TimeNumBar::TestTimeSlot(void)
{
    kk = kk - 1;
    setBarValue(kk);

    //setBarValue(kk);
    //kk = kk - 100;
    //if (kk < 1)
    //    kk = 1000;
    if (kk < 0)
        m_TestTmr->stop();
    update();
}

void TimeNumBar::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (m_value <= m_minWvalue)
        if (m_value <= (m_minWvalue / 2))
            painter.setBrush(QBrush(QColor(113, 113, 113)));
        else
            painter.setBrush(QBrush(QColor(255, 0, 0)));
    else if (m_value >= m_maxWvalue)
        painter.setBrush(QBrush(QColor(0, 255, 0)));
    else
        painter.setBrush(QBrush(QColor(255, 255, 0)));
    
    float dw(this->width() / 3), dh(this->height() - m_alltimelcd->height() - m_timelcd->height() - m_labName->height() - 12 * 2);
    float dx(this->width() / 3), dy(m_timelcd->height() + m_labName->height() + 12);
    //固定宽度
    //dw = 30;
    //dx = this->width() / 2 - 15; 
    //间距计算公式
    float val = (m_value - m_minvalue) * dh / (m_maxvalue - m_minvalue);
    painter.drawRect(dx, dy + dh - val, dw, val);
    //绘制刻度
    QPainter paintNum(this);
    drawRule(paintNum);
}

void TimeNumBar::drawRule(QPainter& painter)
{
    painter.setPen(QPen(QBrush(QColor(0, 0, 0)), 2.0, Qt::SolidLine, Qt::RoundCap));
    int num(10);            //间距数
    float dw(this->width() / 3), dh(this->height() - m_alltimelcd->height() - m_timelcd->height() -m_labName->height() - 12 * 2);
    float dx(this->width() / 3), dy(m_timelcd->height() + m_labName->height() + 12), dwit(dh / num);
/*
    //固定宽度
    dw = 30;
    dx = this->width() / 2 - 15;
    //----------------------------------------------------------------------
    //QFont ft = QFont(TCSR("宋体"));
    //ft.setPointSize(8);
    //painter.setFont(ft);
    painter.drawText(QPoint(dx - 40, dy + 20), QString::number(this->m_maxvalue));
    painter.drawText(QPoint(dx - 40, dy + dwit*num - 20), QString::number(this->m_minvalue));
    //----------------------------------------------------------------------
*/
    painter.drawRect(dx, dy, dw, dh);
    for (int i = 0; i < num; i++)
        painter.drawLine(dx, dy + dwit*i, (i % 5 ? 0 : 5) + dx + dw / 3, dy + dwit*i);
}

TimeBarPanel::TimeBarPanel(QWidget *_parent):QWidget(_parent)
{
    m_fLayout = new FlowLayout(this);
    this->setLayout(m_fLayout);

    //测试添加控件
    for (int i = 0; i < 10; i++)
        this->addTmrBar(i);
}

TimeBarPanel::~TimeBarPanel()
{
}

void TimeBarPanel::addTmrBar(int key)
{
    TimeNumBar* tmpbar = new TimeNumBar(this);
    m_mapbar.insert(key, tmpbar);
    m_fLayout->addWidget(tmpbar);
}

TimeNumBar *TimeBarPanel::getTmrBar(int key)
{
    QMap<int,TimeNumBar*>::const_iterator findr = m_mapbar.find(key);
    if (findr != m_mapbar.end())
        return findr.value();
    else
        return NULL;
}

void TimeBarPanel::delTmrBar(int key)
{
    QMap<int,TimeNumBar*>::iterator findr = m_mapbar.find(key);
    if (findr != m_mapbar.end())
        findr = m_mapbar.erase(findr);
    else
        return ;
}

void TimeBarPanel::cleanTmrBar()
{
    m_mapbar.clear();
}

void TimeBarPanel::allBarPlayState(bool bl)
{
    for (QMap<int, TimeNumBar*>::const_iterator it = m_mapbar.begin(); it != m_mapbar.end();++it)
        it.value()->playTimer(bl);
}


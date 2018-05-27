
#include "stdafx.h"
#include "wplay.h"
#include "ui_wplay.h"

WLrc::WLrc(QWidget* parent) : QLabel(parent)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    //设置背景颜色
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(178, 255, 180, 255));       //QColor(242,79,101,255)
    setPalette(pal);

    // 设置背景透明
    //setAttribute(Qt::WA_TranslucentBackground);
    this->setWindowOpacity(0.8);
    setText(TCSR("QtPlayer音乐播放器"));
    // 固定部件大小
    setMaximumSize(900, 80);
    setMinimumSize(600, 80);
    // 歌词的线性渐变填充
    linearGradient.setStart(0, 10);
    linearGradient.setFinalStop(0, 40);
    linearGradient.setColorAt(0.1, QColor(14, 179, 255));
    linearGradient.setColorAt(0.5, QColor(114, 232, 255));
    linearGradient.setColorAt(0.9, QColor(14, 179, 255));
    // 遮罩的线性渐变填充
    maskLinearGradient.setStart(0, 10);
    maskLinearGradient.setFinalStop(0, 40);
    maskLinearGradient.setColorAt(0.1, QColor(222, 54, 4));
    maskLinearGradient.setColorAt(0.5, QColor(255, 72, 16));
    maskLinearGradient.setColorAt(0.9, QColor(222, 54, 4));
    // 设置字体
    font.setFamily("Times New Roman");
    font.setBold(true);
    font.setPointSize(33);
    // 设置定时器
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    lrcMaskWidth = 0;
    lrcMaskWidthInterval = 0;
}

void WLrc::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setFont(font);
    // 先绘制底层文字，作为阴影，这样会使显示效果更加清晰，且更有质感
    painter.setPen(QColor(0, 0, 0, 200));
    painter.drawText(1, 1, 800, 60, Qt::AlignLeft, text());
    // 再在上面绘制渐变文字
    painter.setPen(QPen(linearGradient, 0));
    painter.drawText(0, 0, 800, 60, Qt::AlignLeft, text());
    // 设置歌词遮罩
    painter.setPen(QPen(maskLinearGradient, 0));
    painter.drawText(0, 0, lrcMaskWidth, 60, Qt::AlignLeft, text());
}

void WLrc::startLrcMask(qint64 intervalTime)
{
    // 这里设置每隔30毫秒更新一次遮罩的宽度，因为如果更新太频繁
    // 会增加CPU占用率，而如果时间间隔太大，则动画效果就不流畅了
    qreal count = intervalTime / 30;
    // 获取遮罩每次需要增加的宽度，这里的800是部件的固定宽度
    lrcMaskWidthInterval = 800 / count;
    lrcMaskWidth = 0;
    timer->start(30);
}

void WLrc::stopLrcMask()
{
    timer->stop();
    lrcMaskWidth = 0;
    update();
}

void WLrc::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
        offset = event->globalPos() - frameGeometry().topLeft();
}

void WLrc::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        setCursor(Qt::PointingHandCursor);
        QPoint point(event->globalPos() - offset);
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
}

void WLrc::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu;
    menu.addAction(TCSR("隐藏"), this, SLOT(hide()));
    menu.exec(event->globalPos());
}

void WLrc::timeout()
{
    lrcMaskWidth += lrcMaskWidthInterval;
    update();
}




WPlayer::WPlayer(QWidget* parent) :
    QWidget(parent)
    , ui(new Ui::WPlayer)
    , m_bPressed(false)
{
    ui->setupUi(this);
    //设置无边框置顶
    //this->setWindowFlags ( Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint */ | Qt::Tool );
    //设置背景颜色
    //QPalette pal = palette();
    //pal.setColor ( QPalette::Background, QColor ( 33, 33, 40 ) );
    //setPalette ( pal );
    //设置为自动填充背景
    //setAutoFillBackground ( true );
    Lrc.show();
}

WPlayer::~WPlayer()
{
    delete ui;
}






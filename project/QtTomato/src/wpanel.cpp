
#include "stdafx.h"
#include "wpanel.h"


WaveFrom::WaveFrom(QWidget* parent) : QWidget(parent)
, base(0)       // 边界基数
, columnInc(0)    // 动态列偏移值
, pts(NULL)     // 随机数据数组
, mLen(0)       // 记录数组长度
, ptNum(0)      // 点数目
{
    //this->setWindowTitle(tr("内存监控"));
    // 黑色背景图
    QPalette pal = palette();
    pal.setBrush(QPalette::All, QPalette::Window, QBrush(Qt::black));
    setPalette(pal);
    setAutoFillBackground(true);
    // 定时器刷新线条
    startTimer(100);
}

WaveFrom::~WaveFrom()
{
    if (pts)
        delete pts;
}

void WaveFrom::paintEvent(QPaintEvent* e)
{
    QPainter painter(this);
    // 线条色
    QPen pen(QColor(46, 139, 87), 1.0);
    painter.setPen(pen);
    int graphW = size().width();//pixmap.width();
    int graphH = size().height();//pixmap.height();
    int graphRow(15), graphColumn(15);
    // 动态列指示参数
    if (columnInc == 0)
        columnInc = graphColumn;
    for (int i = base; i <= graphH + base; i += graphRow)
        painter.drawLine(base, i, base + graphW, i);
    for (int j = base + columnInc; j < graphW + base; j += graphColumn)
        painter.drawLine(j, base, j, base + graphH);
    --columnInc; // 改变x坐标值,使曲线产生运动效果
    if (pts == NULL)
    {
        pts = new int[graphW];
        mLen = graphW;
        ptNum = 0;
    }
    else
    {
        painter.setPen(QPen(Qt::yellow, 1.0));
        // 产生随机数模拟CPU使用情况
        pts[ptNum] = base + graphH * usage();
        // 遍历数组,从后面开始画曲线
        for (int j = base + graphW - ptNum, k = 0; k < ptNum; k++, j++)
        {
            if (k != 0)
            {
                if (pts[k] != pts[k - 1])
                    painter.drawLine(j - 1, pts[k - 1], j, pts[k]);         // 画直线
                else
                    painter.fillRect(j, pts[k], 1, 1, Qt::SolidPattern);    // 相等则画矩形
            }
        }
        if (ptNum + 2 == mLen)   // 曲线到达最左端
        {
            for (int i = 1; i < ptNum; i++)
                pts[i - 1] = pts[i];  // 舍弃最前面2点,从后向前赋值
            --ptNum;
        }
        else     // 曲线在中间
            ptNum++;
    }
}

void WaveFrom::timerEvent(QTimerEvent* e)
{
    // 定时刷新界面
    update();
}

float WaveFrom::usage() const
{
    // 用当前时间设置种子
    QTime time = QTime::currentTime();
    qint64 msecs = time.second() * 1000 + time.msec();
    qsrand(msecs);
    // 取(0-1)间小数值
    float usage = 0.0;
    while (true)
    {
        usage = (qrand() % 100) / 100.0;
        if (usage > 0.1 && usage < 0.9)
            break;
    }
    return usage;
}



DialPlot::DialPlot(QWidget* parent) : QWidget(parent)
{
    //this->setFixedSize(200, 200);       //设置尺寸
    radius = 0;
    minValue = 0;
    maxValue = 100;
    value = 40;
    unit = "";
    centerPoint = QPointF(0, 0);
    textFont = QFont("Arial", 8, QFont::Normal);
    textColor = QColor(255, 255, 255);
    bgColor = QColor(173, 173, 173);
    firstColor = QColor(34, 139, 34);
    secondColor = QColor(238, 238, 0);
    thirdColor = QColor(205, 51, 51);
    centerColor = QColor(94, 94, 94);
}

DialPlot::~DialPlot()
{
}

void DialPlot::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,旋转坐标轴中心
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(width / 2, height / 2);
    //更新仪表盘最外圈半径大小
    radius = width > height ? height : width;
    radius = radius / 2;
    //绘制表盘背景
    drawBg(&painter);
    //绘制表盘
    drawDial(&painter);
    //绘制表盘刻度
    drawTicks(&painter);
    //绘制指针
    drawPointer(&painter);
    //绘制当前值
    drawValue(&painter);
}

void DialPlot::drawBg(QPainter* painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QRadialGradient bgGradient(centerPoint, radius, centerPoint);
    bgGradient.setColorAt(0, bgColor.lighter(100));
    bgGradient.setColorAt(0.6, bgColor.darker(150));
    bgGradient.setColorAt(0.61, bgColor);
    bgGradient.setColorAt(0.8, bgColor.darker(150));
    bgGradient.setColorAt(1, bgColor);
    painter->setBrush(bgGradient);
    painter->drawEllipse(centerPoint, radius, radius);
    painter->restore();
}

void DialPlot::drawDial(QPainter* painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QPointF pieRectTopLeftPot(-radius, -radius);
    QPointF pieRectBottomRightPot(radius, radius);
    QRectF pieRect = QRectF(pieRectTopLeftPot, pieRectBottomRightPot);
    //将表盘分区间三种不同颜色显示
    QRadialGradient firstGradient(centerPoint, radius, centerPoint);
    firstGradient.setColorAt(0, Qt::transparent);
    firstGradient.setColorAt(0.6, Qt::transparent);
    firstGradient.setColorAt(0.61, firstColor.darker(150));
    firstGradient.setColorAt(0.8, firstColor.lighter(100));
    firstGradient.setColorAt(1.0, firstColor.darker(150));
    painter->setBrush(firstGradient);
    painter->drawPie(pieRect, 130 * 16, 80 * 16);
    QRadialGradient secondGradient(centerPoint, radius, centerPoint);
    secondGradient.setColorAt(0, Qt::transparent);
    secondGradient.setColorAt(0.6, Qt::transparent);
    secondGradient.setColorAt(0.61, secondColor.darker(150));
    secondGradient.setColorAt(0.8, secondColor.lighter(100));
    secondGradient.setColorAt(1.0, secondColor.darker(150));
    painter->setBrush(secondGradient);
    painter->drawPie(pieRect, 50 * 16, 80 * 16);
    QRadialGradient thirdGradient(centerPoint, radius, centerPoint);
    thirdGradient.setColorAt(0, Qt::transparent);
    thirdGradient.setColorAt(0.6, Qt::transparent);
    thirdGradient.setColorAt(0.61, thirdColor.darker(150));
    thirdGradient.setColorAt(0.8, thirdColor.lighter(100));
    thirdGradient.setColorAt(1.0, thirdColor.darker(150));
    painter->setBrush(thirdGradient);
    painter->drawPie(pieRect, 330 * 16, 80 * 16);
    painter->restore();
}

void DialPlot::drawTicks(QPainter* painter)
{
    painter->save();
    painter->setPen(textColor);
    //刻度长度占圆盘半径比例
    double tickRatio = 0.05;
    //旋转坐标轴到刻度起始位置
    painter->rotate(60);
    for (int scale = 0; scale <= 100; ++scale)
    {
        //逢十的倍数,刻度变长
        tickRatio = scale % 10 ? 0.05 : 0.1;
        //绘制刻度线
        painter->drawLine(QPointF(0, radius * (0.6 - tickRatio)), QPointF(0, radius * 0.6));
        if (scale == 0)
        {
            //绘制表盘上最小值
            painter->drawText(-10, radius * (0.38), 20, 10, Qt::AlignCenter, QString::number(minValue));
        }
        else if (scale == 100)
        {
            //绘制表盘上最大值
            painter->drawText(-10, radius * (0.38), 20, 10, Qt::AlignCenter, QString::number(maxValue));
        }
        painter->rotate(2.4);
    }
    painter->restore();
}

void DialPlot::drawPointer(QPainter* painter)
{
    //指针宽度
    int pointerWidth = 6;
    //指针路径点
    const QPoint pointers[3] =
    {
        QPoint(centerPoint.x() - pointerWidth / 3, centerPoint.y()),
        QPoint(centerPoint.x() + pointerWidth / 3, centerPoint.y()),
        QPoint(centerPoint.x(), centerPoint.y() - radius)
    };
    painter->save();
    painter->rotate(240);
    painter->setPen(Qt::NoPen);
    //将当前实际值转换为刻度值
    double first = (double)100 / 3;
    double second = first * 2;
    double currentValue = 100 * (this->value - this->minValue) / (this->maxValue - this->minValue);
    QColor pointerColor = firstColor;
    if (currentValue < first)
        pointerColor = firstColor;
    else if (currentValue >= first && currentValue <= second)
        pointerColor = secondColor;
    else
        pointerColor = thirdColor;
    //画刷颜色设定
    painter->setBrush(pointerColor);
    if (currentValue <= 100)
        painter->rotate(2.4 * currentValue);
    else
        painter->rotate(360);
    painter->drawConvexPolygon(pointers, 3);
    painter->restore();
    //绘制指针中心圆
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(centerColor);
    painter->drawEllipse(QPoint(0, 0), pointerWidth, pointerWidth);
    painter->restore();
}

void DialPlot::drawValue(QPainter* painter)
{
    painter->save();
    painter->setPen(textColor);
    painter->setFont(textFont);
    painter->drawText(QRectF(-30, radius * 0.6 * 0.5, 60, 30), Qt::AlignCenter, QString::number(value) + unit);
    painter->restore();
}

void DialPlot::setRange(double minValue, double maxValue)
{
    //如果最小值大于或者等于最大值则不设置
    if (minValue >= maxValue)
        return;
    this->minValue = minValue;
    this->maxValue = maxValue;
    //如果目标值不在范围值内,则重新设置目标值
    if (value < minValue || value > maxValue)
        setValue(value);
    update();
}

void DialPlot::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void DialPlot::setValue(double value)
{
    //如果目标值比最小值小则取最小值,如果目标值比最大值大则取最大值
    if (value < minValue)
        this->value = minValue;
    else if (value > maxValue)
        this->value = maxValue;
    else
        this->value = value;
    update();
    emit valueChanged(value);
}

void DialPlot::setValue(int value)
{
    setValue((double)value);
}

void DialPlot::setUnit(QString unit)
{
    this->unit = unit;
    update();
}

void DialPlot::setTextFont(QFont textFont)
{
    this->textFont = textFont;
    update();
}

void DialPlot::setTextColor(QColor textColor)
{
    this->textColor = textColor;
    update();
}

void DialPlot::setBgColor(QColor bgColor)
{
    this->bgColor = bgColor;
    update();
}

void DialPlot::setFirstColor(QColor firstColor)
{
    this->firstColor = firstColor;
    update();
}

void DialPlot::setSecondColor(QColor secondColor)
{
    this->secondColor = secondColor;
    update();
}

void DialPlot::setThirdColor(QColor thirdColor)
{
    this->thirdColor = thirdColor;
    update();
}

void DialPlot::setCenterColor(QColor centerColor)
{
    this->centerColor = centerColor;
    update();
}



RoundPlot::RoundPlot(QWidget* parent) : QWidget(parent)
{
    setMinimumSize(167, 167);
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor(255, 255, 255));
    setPalette(palette);
    minValue = 0;
    maxValue = 100;
    value = 0;
    unit = "";
    currentValue = 0;
    step = maxValue / 100;
    usedColor = QColor(165, 220, 62);
    freeColor = QColor(215, 215, 215);
    rangeTextColor = QColor(137, 137, 137);
    valueTextColor = QColor(52, 155, 218);
    valueBgColor = QColor(239, 239, 239);
    outBgColor = QColor(233, 233, 233);
    centerBgColorStart = QColor(45, 204, 112);
    centerBgColorEnd = QColor(51, 152, 219);
    rangeTextFont = QFont("Arial", 10, QFont::Normal);
    valueTextFont = QFont("Arial", 15, QFont::Bold);
    updateTimer = new QTimer(this);
    updateTimer->setInterval(10);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateValue()));
}

RoundPlot::~RoundPlot()
{
    if (updateTimer->isActive())
        updateTimer->stop();
}

void RoundPlot::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
        int width = this->width();
    int height = this->height();
    //直径取宽高中最短的一个
    int diameter = width > height ? height : width;
    //绘制准备工作,旋转坐标轴中心
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(width / 2, height / 2);
    painter.save();
    painter.rotate(40);
    //自动计算比例关系
    double percent = (double)100 / (maxValue - minValue);
    int currentPercent = currentValue * percent;
    int maxPercent = maxValue * percent;
    //自动计算位置比例
    double diameterLine = ((double)3 / 167) * diameter;
    double diameterWidth = ((double)70 / 167) * diameter;
    double diameterHeight = ((double)80 / 167) * diameter;
    double diameter1 = ((double)55 / 167) * diameter;
    double diameter2 = ((double)35 / 167) * diameter;
    double diameter3 = ((double)30 / 167) * diameter;
    double diameter4 = ((double)80 / 167) * diameter;
    double diameter5 = ((double)135 / 167) * diameter;
    //绘制当前百分比
    painter.setPen(QPen(usedColor, diameterLine));
    for (int i = minValue; i < currentPercent; i++)
    {
        painter.drawLine(0, diameterWidth, 0, diameterHeight);
        painter.rotate(2.8);
    }
    //绘制空余百分比
    painter.setPen(QPen(freeColor, diameterLine));
    for (int i = currentPercent; i < maxPercent; i++)
    {
        painter.drawLine(0, diameterWidth, 0, diameterHeight);
        painter.rotate(2.8);
    }
    painter.restore();
    //绘制最外框圆形背景
    painter.translate(-width / 2, -height / 2);
    painter.setBrush(outBgColor);
    painter.setPen(QPen(outBgColor, 15));
    painter.drawEllipse(QRectF((width / 2 - diameter1), (height / 2 - diameter1), diameter1 * 2, diameter1 * 2));
    //绘制中间框圆形渐变背景
    QConicalGradient conicalGradient(width / 2, height / 2, 90);
    conicalGradient.setColorAt(0, centerBgColorStart);
    conicalGradient.setColorAt(1.0, centerBgColorEnd);
    painter.setPen(QPen(QBrush(conicalGradient), 30));
    painter.drawEllipse(QRectF((width / 2 - diameter2), (height / 2 - diameter2), diameter2 * 2, diameter2 * 2));
    //绘制最里框百分比文字圆形背景
    painter.setPen(Qt::NoPen);
    painter.setBrush(valueBgColor);
    painter.drawEllipse(QRectF((width / 2 - diameter3), (height / 2 - diameter3), diameter3 * 2, diameter3 * 2));
    //绘制百分比文字及范围值文字
    painter.setPen(QPen(valueTextColor));
    painter.setFont(valueTextFont);
    painter.drawText(0, 0, width, height, Qt::AlignCenter, tr("%1%2").arg(currentValue).arg(unit));
    painter.setPen(QPen(rangeTextColor));
    painter.setFont(rangeTextFont);
    painter.drawText(0, 0, width - diameter4, height + diameter5, Qt::AlignCenter, tr("%1%2").arg(minValue).arg(unit));
    painter.drawText(0, 0, width + diameter4, height + diameter5, Qt::AlignCenter, tr("%1%2").arg(maxValue).arg(unit));
}

void RoundPlot::updateValue()
{
    if (currentValue >= value)
    {
        updateTimer->stop();
        return;
    }
    currentValue = currentValue + step;
    update();
}

void RoundPlot::setRange(int minValue, int maxValue)
{
    if (updateTimer->isActive())
        updateTimer->stop();
    currentValue = minValue;
    this->minValue = minValue;
    this->maxValue = maxValue;
    //如果目标值不在范围值内,则重新设置目标值
    if (value < minValue || value > maxValue)
        setValue(value);
    //自动计算步长,步长=最大值/100
    step = maxValue / 100;
}

void RoundPlot::setValue(int value)
{
    //如果目标值比最小值小则取最小值,如果目标值比最大值大则取最大值
    if (value < minValue)
        this->value = minValue;
    else if (value > maxValue)
        this->value = maxValue;
    else
        this->value = value;
    updateTimer->start();
}

void RoundPlot::setUnit(QString unit)
{
    this->unit = unit;
    update();
}

void RoundPlot::setUsedColor(QColor usedColor)
{
    this->usedColor = usedColor;
    update();
}

void RoundPlot::setFreeColor(QColor freeColor)
{
    this->freeColor = freeColor;
    update();
}

void RoundPlot::setRangeTextColor(QColor rangeTextColor)
{
    this->rangeTextColor = rangeTextColor;
    update();
}

void RoundPlot::setValueTextColor(QColor valueTextColor)
{
    this->valueTextColor = valueTextColor;
    update();
}

void RoundPlot::setValueBgColor(QColor valueBgColor)
{
    this->valueBgColor = valueBgColor;
    update();
}

void RoundPlot::setOutBgColor(QColor outBgColor)
{
    this->outBgColor = outBgColor;
    update();
}

void RoundPlot::setCenterBgColor(QColor centerBgColorStart, QColor centerBgColorEnd)
{
    this->centerBgColorStart = centerBgColorStart;
    this->centerBgColorEnd = centerBgColorEnd;
    update();
}

void RoundPlot::setRangeTextFont(QFont rangeTextFont)
{
    this->rangeTextFont = rangeTextFont;
    update();
}

void RoundPlot::setValueTextFont(QFont valueTextFont)
{
    this->valueTextFont = valueTextFont;
    update();
}
















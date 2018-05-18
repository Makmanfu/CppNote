
#include "stdafx.h"
#include "wprogressbar.h"
#include "ui_wprogressbar.h"

/**
    1:支持三种风格样式 圆环风格/饼状风格/线条风格
    2:自动居中绘制显示
    3:可设置顺时针逆时针走向
    4:可设置起始进度位置
    5:可设置范围值
    6:可设置各种颜色
    7:可设置当前值
    QPalette::Window         设置整个部件的背景
    QPalette::Base           未填充进度区域的背景
    QPalette::AlternateBase   文本显示所在的中间圆环的背景
    QPalette::Shadow         未填充区域的前景色-边框色
    QPalette::Highlight      填充进度条区域的背景色
    QPalette::Text           在中心显示的文本色
*/

ProgressBarRound::ProgressBarRound(QWidget* parent) : QWidget(parent)
{
    value = 0;
    minValue = 0;
    maxValue = 100;
    decimals = 0;
    nullPosition = 90;
    barStyle = BarStyle_Donut;
    format = "%p%";
    outlinePenWidth = 1;
    dataPenWidth = 1;
    updateFlags = 2;
    clockwise = true;
    setFont(QFont("Arial", 8));
}

ProgressBarRound::~ProgressBarRound()
{
}

void ProgressBarRound::paintEvent(QPaintEvent*)
{
    //半径取宽高的最小值
    double outerRadius = qMin(width(), height());
    //绘制区域在画面区域的基础上上下左右各减少一个像素,使得看起来完整
    QRectF baseRect(1, 1, outerRadius - 2, outerRadius - 2);
    //先将需要绘制的图形绘制到一张图片上,最后将图片绘制到屏幕
    QImage buffer(outerRadius, outerRadius, QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&buffer);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    //绘制数据颜色集合
    rebuildDataBrushIfNeeded();
    //绘制背景
    drawBg(&painter, buffer.rect());
    //绘制外框
    drawBase(&painter, baseRect);
    //绘制数据
    drawValue(&painter, baseRect);
    //绘制中心
    double innerRadius(0);
    QRectF innerRect;
    calculateInnerRect(baseRect, outerRadius, innerRect, innerRadius);
    drawInnerBackground(&painter, innerRect);
    //绘制当前值
    drawText(&painter, innerRect, innerRadius);
    painter.end();
    QPainter p(this);
    p.fillRect(baseRect, palette().background());
    //改成居中绘制图片
    int pixX = rect().center().x() - buffer.width() / 2;
    int pixY = rect().center().y() - buffer.height() / 2;
    QPoint point(pixX, pixY);
    p.drawImage(point, buffer);
}

void ProgressBarRound::drawBg(QPainter* painter, const QRectF& baseRect)
{
    painter->fillRect(baseRect, palette().background());
}

void ProgressBarRound::drawBase(QPainter* painter, const QRectF& baseRect)
{
    switch (barStyle)
    {
        case BarStyle_Donut:
            painter->setPen(QPen(palette().shadow().color(), outlinePenWidth));
            painter->setBrush(palette().base());
            painter->drawEllipse(baseRect);
            break;
        case BarStyle_Pie:
            painter->setPen(QPen(palette().base().color(), outlinePenWidth));
            painter->setBrush(palette().base());
            painter->drawEllipse(baseRect);
            break;
        case BarStyle_Line:
            painter->setPen(QPen(palette().base().color(), outlinePenWidth));
            painter->setBrush(Qt::NoBrush);
            painter->drawEllipse(baseRect.adjusted(outlinePenWidth / 2, outlinePenWidth / 2, -outlinePenWidth / 2, -outlinePenWidth / 2));
            break;
    }
}

void ProgressBarRound::drawValue(QPainter* painter, const QRectF& baseRect)
{
    if (value == minValue)
        return;
    double arcLength = 360.0 / (maxValue - minValue) * value;
    //逆时针为顺时针分负数
    if (!clockwise)
        arcLength = -arcLength;
    if (barStyle == BarStyle_Line)
    {
        painter->setPen(QPen(palette().highlight().color(), dataPenWidth));
        painter->setBrush(Qt::NoBrush);
        painter->drawArc(baseRect.adjusted(outlinePenWidth / 2, outlinePenWidth / 2, -outlinePenWidth / 2, -outlinePenWidth / 2),
                         nullPosition * 16, -arcLength * 16);
        return;
    }
    QPainterPath dataPath;
    dataPath.setFillRule(Qt::WindingFill);
    dataPath.moveTo(baseRect.center());
    dataPath.arcTo(baseRect, nullPosition, -arcLength);
    dataPath.lineTo(baseRect.center());
    painter->setBrush(palette().highlight());
    painter->setPen(QPen(palette().shadow().color(), dataPenWidth));
    painter->drawPath(dataPath);
}

void ProgressBarRound::calculateInnerRect(const QRectF& /*baseRect*/, double outerRadius, QRectF& innerRect, double& innerRadius)
{
    if (barStyle == BarStyle_Line)
        innerRadius = outerRadius - outlinePenWidth;
    else
        innerRadius = outerRadius * 0.75;
    double delta = (outerRadius - innerRadius) / 2;
    innerRect = QRectF(delta, delta, innerRadius, innerRadius);
}

void ProgressBarRound::drawInnerBackground(QPainter* painter, const QRectF& innerRect)
{
    if (barStyle == BarStyle_Donut)
    {
        painter->setBrush(palette().alternateBase());
        painter->drawEllipse(innerRect);
    }
}

void ProgressBarRound::drawText(QPainter* painter, const QRectF& innerRect, double innerRadius)
{
    if (format.isEmpty())
        return;
    QFont f(font());
    f.setPixelSize(innerRadius * qMax(0.05, (0.35 - (double)decimals * 0.08)));
    painter->setFont(f);
    QRectF textRect(innerRect);
    painter->setPen(palette().text().color());
    painter->drawText(textRect, Qt::AlignCenter, valueToText(value));
}

QString ProgressBarRound::valueToText(double value) const
{
    QString textToDraw(format);
    if (updateFlags & UF_VALUE)
        textToDraw.replace("%v", QString::number(value, 'f', decimals));
    if (updateFlags & UF_PERCENT)
    {
        double procent = (value - minValue) / (maxValue - minValue) * 100.0;
        textToDraw.replace("%p", QString::number(procent, 'f', decimals));
    }
    if (updateFlags & UF_MAX)
        textToDraw.replace("%m", QString::number(maxValue - minValue + 1, 'f', decimals));
    return textToDraw;
}

void ProgressBarRound::valueFormatChanged()
{
    updateFlags = 0;
    if (format.contains("%v"))
        updateFlags |= UF_VALUE;
    if (format.contains("%p"))
        updateFlags |= UF_PERCENT;
    if (format.contains("%m"))
        updateFlags |= UF_MAX;
    update();
}

void ProgressBarRound::rebuildDataBrushIfNeeded()
{
    if (gradientData.count() == 0)
        return;
    QConicalGradient dataBrush;
    dataBrush.setCenter(0.5, 0.5);
    dataBrush.setCoordinateMode(QGradient::StretchToDeviceMode);
    for (int i = 0; i < gradientData.count(); i++)
        dataBrush.setColorAt(1.0 - gradientData.at(i).first, gradientData.at(i).second);
    dataBrush.setAngle(nullPosition);
    QPalette p(palette());
    p.setBrush(QPalette::Highlight, dataBrush);
    setPalette(p);
}

void ProgressBarRound::test01(void)
{
    //默认样式+值格式+圆环颜色
    QPalette p1;
    p1.setColor(QPalette::Highlight, QColor(24, 189, 155));
    p1.setColor(QPalette::Text, QColor(24, 189, 155));
    p1.setColor(QPalette::Shadow, QColor(24, 189, 155));
    this->setPalette(p1);
    this->setFormat("%v");
    this->setDecimals(0);
    this->setNullPosition(0);
}

void ProgressBarRound::test02(void)
{
    //饼状图+背景色+起始位置
    QPalette p2;
    p2.setColor(QPalette::Base, QColor(97, 117, 118));
    p2.setColor(QPalette::Text, QColor(250, 250, 250));
    p2.setColor(QPalette::Highlight, QColor(255, 107, 107));
    p2.setColor(QPalette::Shadow, QColor(255, 107, 107));
    this->setPalette(p2);
    this->setNullPosition(ProgressBarRound::PositionRight);
    this->setBarStyle(ProgressBarRound::BarStyle_Pie);
}

void ProgressBarRound::test03(void)
{
    //线状图+线颜色
    QPalette p3;
    p3.setColor(QPalette::Highlight, QColor(100, 184, 255));
    this->setPalette(p3);
    this->setBarStyle(ProgressBarRound::BarStyle_Line);
    this->setOutlinePenWidth(5);
    this->setDataPenWidth(5);
}

void ProgressBarRound::test04(void)
{
    //默认样式+自定义颜色组合
    QPalette p4;
    p4.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
    p4.setColor(QPalette::Text, QColor(255, 107, 107));
    p4.setColor(QPalette::Highlight, QColor(255, 107, 107));
    p4.setColor(QPalette::Shadow, QColor(255, 107, 107));
    this->setPalette(p4);
    this->setNullPosition(ProgressBarRound::PositionLeft);
    this->setDecimals(0);
    QGradientStops gradientPoints4;
    gradientPoints4 << QGradientStop(0, QColor(254, 64, 64)) << QGradientStop(0.5, QColor(255, 127, 36)) << QGradientStop(1, QColor(238, 238, 0));
    this->setDataColors(gradientPoints4);
}

void ProgressBarRound::test05(void)
{
    //饼图样式+自定义颜色组合
    QPalette p5;
    p5.setColor(QPalette::Highlight, QColor(24, 189, 155));
    p5.setBrush(QPalette::Base, QColor(255, 107, 107));
    p5.setColor(QPalette::Text, QColor(250, 250, 250));
    p5.setColor(QPalette::Shadow, QColor(24, 189, 155));
    this->setPalette(p5);
    this->setNullPosition(ProgressBarRound::PositionLeft);
    this->setBarStyle(ProgressBarRound::BarStyle_Pie);
}

void ProgressBarRound::test06(void)
{
    //线状样式+自定义边框宽度
    QPalette p3;
    p3.setColor(QPalette::Highlight, QColor(100, 184, 255));
    \
    this->setPalette(p3);
    this->setDecimals(2);
    this->setBarStyle(ProgressBarRound::BarStyle_Line);
    this->setOutlinePenWidth(18);
    this->setDataPenWidth(10);
    this->setClockWise(false);
}

void ProgressBarRound::setRange(double minValue, double maxValue)
{
    this->minValue = minValue;
    this->maxValue = maxValue;
    if (this->maxValue < this->minValue)
        qSwap(this->maxValue, this->minValue);
    if (value < this->minValue)
        value = this->minValue;
    else if (value > this->maxValue)
        value = this->maxValue;
    update();
}

void ProgressBarRound::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void ProgressBarRound::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value)
        return;
    this->value = value;
    update();
}

void ProgressBarRound::setValue(int value)
{
    setValue(double(value));
}

void ProgressBarRound::setDecimals(int decimals)
{
    if (decimals >= 0 && this->decimals != decimals)
    {
        this->decimals = decimals;
        valueFormatChanged();
    }
}

void ProgressBarRound::setNullPosition(double nullPosition)
{
    if (this->nullPosition != nullPosition)
    {
        this->nullPosition = nullPosition;
        update();
    }
}

void ProgressBarRound::setBarStyle(ProgressBarRound::BarStyle barStyle)
{
    if (this->barStyle != barStyle)
    {
        this->barStyle = barStyle;
        update();
    }
}

void ProgressBarRound::setFormat(QString format)
{
    if (this->format != format)
    {
        this->format = format;
        valueFormatChanged();
    }
}

void ProgressBarRound::setOutlinePenWidth(double penWidth)
{
    if (penWidth != outlinePenWidth)
    {
        outlinePenWidth = penWidth;
        update();
    }
}

void ProgressBarRound::setDataPenWidth(double penWidth)
{
    if (penWidth != dataPenWidth)
    {
        dataPenWidth = penWidth;
        update();
    }
}

void ProgressBarRound::setDataColors(const QGradientStops& stopPoints)
{
    if (stopPoints != gradientData)
    {
        gradientData = stopPoints;
        update();
    }
}

void ProgressBarRound::setClockWise(bool clockwise)
{
    if (this->clockwise != clockwise)
    {
        this->clockwise = clockwise;
        update();
    }
}



/**
    1:支持指示器样式选择 圆形指示器/指针指示器/三角形指示器
    2:支持鼠标按下旋转改变值
    3:支持负数范围值
    4:支持设置当前值及范围值
    5:支持设置起始旋转角度和结束旋转角度
    6:支持设置背景色/进度颜色/中间圆渐变颜色
    7:随窗体拉伸自动变化
    8:支持鼠标进入和离开动画效果
    9:可设置是否显示当前值
    10:可设置是否显示指示器
*/

GaugeProgressBar::GaugeProgressBar(QWidget* parent) : QWidget(parent)
{
    value = 0;
    minValue = 0;
    maxValue = 100;
    precision = 0;
    startAngle = 0;
    endAngle = 0;
    bgColor = QColor(30, 30, 30);
    progressColor = QColor(100, 184, 255);
    progressBgColor = QColor(50, 50, 50);
    circleColorStart = QColor(80, 80, 80);
    circleColorEnd = QColor(50, 50, 50);
    textColor = QColor(200, 200, 200);
    showPointer = true;
    showValue = false;
    pointerStyle = PointerStyle_Circle;
    hover = false;
    radiusCoverCircle = 85;
    radiusCircle = 80;
    //初始化随机数种子
    QTime t = QTime::currentTime();
    qsrand(t.msec() + t.second() * 1000);
    animation = new QPropertyAnimation(this, "");
    animation->setStartValue(0);
    animation->setEndValue(10);
    animation->setDuration(300);
    connect(animation, SIGNAL(valueChanged(QVariant)), this, SLOT(updateRadius(QVariant)));
    setFont(QFont("Arial", 9));
}

GaugeProgressBar::~GaugeProgressBar()
{
}

void GaugeProgressBar::enterEvent(QEvent*)
{
    hover = true;
    animation->stop();
    animation->start();
}

void GaugeProgressBar::leaveEvent(QEvent*)
{
    hover = false;
    animation->stop();
    animation->start();
}

void GaugeProgressBar::mousePressEvent(QMouseEvent* e)
{
    pressed = true;
    setPressedValue(e->pos());
}

void GaugeProgressBar::mouseReleaseEvent(QMouseEvent*)
{
    pressed = false;
}

void GaugeProgressBar::mouseMoveEvent(QMouseEvent* e)
{
    if (pressed)
        setPressedValue(e->pos());
}

void GaugeProgressBar::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    int side = qMin(width, height);
    painter.scale(side / 200.0, side / 200.0);
    //绘制背景
    drawBg(&painter);
    //绘制饼圆
    drawColorPie(&painter);
    //绘制覆盖圆 用以遮住饼圆多余部分
    drawCoverCircle(&painter);
    //绘制中心圆
    drawCircle(&painter);
    //根据指示器形状绘制指示器
    if (pointerStyle == PointerStyle_Circle)
        drawPointerCircle(&painter);
    else if (pointerStyle == PointerStyle_Indicator)
        drawPointerIndicator(&painter);
    else if (pointerStyle == PointerStyle_Triangle)
        drawPointerTriangle(&painter);
    //绘制当前值
    drawText(&painter);
}

void GaugeProgressBar::drawBg(QPainter* painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeProgressBar::drawColorPie(QPainter* painter)
{
    int radius = 95;
    painter->save();
    painter->setPen(Qt::NoPen);
    QRectF rect(-radius, -radius, radius * 2, radius * 2);
    //计算总范围角度,当前值范围角度,剩余值范围角度
    double angleAll = 360.0 - startAngle - endAngle;
    double angleCurrent = angleAll * ((value - minValue) / (maxValue - minValue));
    double angleOther = angleAll - angleCurrent;
    //绘制当前值饼圆
    painter->setBrush(progressColor);
    painter->drawPie(rect, (270 - startAngle - angleCurrent) * 16, angleCurrent * 16);
    //绘制剩余值饼圆
    painter->setBrush(progressBgColor);
    painter->drawPie(rect, (270 - startAngle - angleCurrent - angleOther) * 16, angleOther * 16);
    painter->restore();
}

void GaugeProgressBar::drawCoverCircle(QPainter* painter)
{
    int radius = radiusCoverCircle;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(bgColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeProgressBar::drawCircle(QPainter* painter)
{
    int radius = radiusCircle;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(0, -radius, 0, radius);
    bgGradient.setColorAt(0.0, circleColorStart);
    bgGradient.setColorAt(1.0, circleColorEnd);
    painter->setBrush(bgGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeProgressBar::drawPointerCircle(QPainter* painter)
{
    if (!showPointer)
        return;
    int radius = 15;
    int offset = radiusCircle - 60;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (value - minValue);
    painter->rotate(degRotate);
    painter->setBrush(progressColor);
    painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);
    painter->restore();
}

void GaugeProgressBar::drawPointerIndicator(QPainter* painter)
{
    if (!showPointer)
        return;
    int radius = radiusCircle - 15;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(progressColor);
    QPolygon pts;
    pts.setPoints(3, -8, 0, 8, 0, 0, radius);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (value - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);
    //绘制中心圆点
    radius = radius / 4;
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeProgressBar::drawPointerTriangle(QPainter* painter)
{
    if (!showPointer)
        return;
    int radius = 20;
    int offset = radiusCircle - 25;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(progressColor);
    QPolygon pts;
    pts.setPoints(3, -radius / 2, offset, radius / 2, offset, 0, radius + offset);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (value - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeProgressBar::drawText(QPainter* painter)
{
    if (!showValue)
        return;
    int radius = 100;
    painter->save();
    painter->setPen(textColor);
    QFont font = painter->font();
    if (showPointer)
        font.setPixelSize(radius - 50);
    else
        font.setPixelSize(radius - 15);
    font.setBold(true);
    painter->setFont(font);
    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
    painter->drawText(textRect, Qt::AlignCenter, strValue);
    painter->restore();
}

void GaugeProgressBar::setEasingCurve()
{
    //随机选择一种动画效果
    int index = qrand() % 40;
    animation->setEasingCurve((QEasingCurve::Type)index);
}

void GaugeProgressBar::updateRadius(QVariant radius)
{
    //如果鼠标悬停则逐渐变小,鼠标移开则逐渐变大直到恢复
    int step = radius.toInt();
    if (hover)
    {
        radiusCoverCircle = 85 - step;
        radiusCircle = 80 - step;
    }
    else
    {
        radiusCoverCircle = 75 + step;
        radiusCircle = 70 + step;
    }
    update();
}

void GaugeProgressBar::setPressedValue(QPointF pressedPoint)
{
    //计算总角度
    double length = 360 - startAngle - endAngle;
    //计算最近的刻度
    QPointF point = pressedPoint - rect().center();
    double theta = -atan2(-point.x(), -point.y()) * 180 / M_PI;
    theta = theta + length / 2;
    //计算每一个角度对应值移动多少
    double increment = (maxValue - minValue) / length;
    double currentValue = (theta * increment) + minValue;
    //过滤圆弧外的值
    if (currentValue <= minValue)
        currentValue = minValue;
    else if (currentValue >= maxValue)
        currentValue = maxValue;
    setValue(currentValue);
}

void GaugeProgressBar::setRange(double minValue, double maxValue)
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

void GaugeProgressBar::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void GaugeProgressBar::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value)
        return;
    this->value = value;
    emit valueChanged(value);
    update();
}

void GaugeProgressBar::setValue(int value)
{
    setValue((double)value);
}

void GaugeProgressBar::setPrecision(int precision)
{
    //最大精确度为 3
    if (precision <= 3)
    {
        this->precision = precision;
        update();
    }
}

void GaugeProgressBar::setStartAngle(int startAngle)
{
    this->startAngle = startAngle;
    update();
}

void GaugeProgressBar::setEndAngle(int endAngle)
{
    this->endAngle = endAngle;
    update();
}

void GaugeProgressBar::setBgColor(QColor bgColor)
{
    this->bgColor = bgColor;
    update();
}

void GaugeProgressBar::setProgressColor(QColor progressColor, QColor progressBgColor)
{
    this->progressColor = progressColor;
    this->progressBgColor = progressBgColor;
    update();
}

void GaugeProgressBar::setCircleColor(QColor circleColorStart, QColor circleColorEnd)
{
    this->circleColorStart = circleColorStart;
    this->circleColorEnd = circleColorEnd;
    update();
}

void GaugeProgressBar::setTextColor(QColor textColor)
{
    this->textColor = textColor;
    update();
}

void GaugeProgressBar::setShowPointer(bool showPointer)
{
    this->showPointer = showPointer;
    update();
}

void GaugeProgressBar::setShowValue(bool showValue)
{
    this->showValue = showValue;
    update();
}

void GaugeProgressBar::setPointerStyle(GaugeProgressBar::PointerStyle pointerStyle)
{
    this->pointerStyle = pointerStyle;
    update();
}

void GaugeProgressBar::test01(void)
{
    this->setRange(0, 100);
    this->setValue(30);
}

void GaugeProgressBar::test02(void)
{
    this->setProgressColor(QColor(255, 107, 107), QColor(50, 50, 50));
    this->setPointerStyle(GaugeProgressBar::PointerStyle_Indicator);
    this->setRange(-50, 100);
    this->setValue(30);
}

void GaugeProgressBar::test03(void)
{
    this->setProgressColor(QColor(24, 189, 155), QColor(50, 50, 50));
    this->setPointerStyle(GaugeProgressBar::PointerStyle_Triangle);
    this->setRange(0, 200);
    this->setValue(30);
}

void GaugeProgressBar::test04(void)
{
    this->setRange(0, 1000);
    this->setStartAngle(30);
    this->setEndAngle(30);
    this->setShowValue(true);
    this->setShowPointer(false);
    this->setPointerStyle(GaugeProgressBar::PointerStyle_Triangle);
    this->setValue(300);
}

void GaugeProgressBar::test05(void)
{
    this->setRange(0, 1000);
    this->setStartAngle(30);
    this->setEndAngle(30);
    this->setShowValue(true);
    this->setShowPointer(false);
    this->setPointerStyle(GaugeProgressBar::PointerStyle_Triangle);
    this->setProgressColor(QColor(255, 107, 107), QColor(50, 50, 50));
    this->setValue(300);
}

void GaugeProgressBar::test06(void)
{
    this->setRange(0, 1000);
    this->setStartAngle(30);
    this->setEndAngle(30);
    this->setShowValue(true);
    this->setShowPointer(false);
    this->setPointerStyle(GaugeProgressBar::PointerStyle_Triangle);
    this->setProgressColor(QColor(24, 189, 155), QColor(50, 50, 50));
    this->setValue(300);
}



/**
    1:可设置精确度/步长/圆角角度
    2:可设置范围值和当前值
    3:可设置前景色背景色等各种颜色
*/

ProgressBarColor::ProgressBarColor(QWidget* parent) : QWidget(parent)
{
    value = 0;
    minValue = 0;
    maxValue = 100;
    precision = 0;
    step = 0;
    space = 5;
    radius = 5;
    bgColorStart = QColor(100, 100, 100);
    bgColorEnd = QColor(60, 60, 60);
    lineColor = QColor(230, 230, 230);
    textColor = QColor(0, 0, 0);
    barBgColor = QColor(250, 250, 250);
    barColor = QColor(100, 184, 255);
    setFont(QFont("Arial", 8));
}

void ProgressBarColor::paintEvent(QPaintEvent*)
{
    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    //绘制背景
    //drawBg(&painter);
    //绘制进度
    drawBar(&painter);
}

void ProgressBarColor::drawBg(QPainter* painter)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(QPointF(0, 0), QPointF(0, height()));
    bgGradient.setColorAt(0.0, bgColorStart);
    bgGradient.setColorAt(1.0, bgColorEnd);
    painter->setBrush(bgGradient);
    painter->drawRect(rect());
    painter->restore();
}

void ProgressBarColor::drawBar(QPainter* painter)
{
    painter->save();
    //自动计算文字字体大小
    QFont f(font());
    f.setPixelSize((width() / 10) * 0.35);
    painter->setFont(f);
    //计算进度值字符的宽度
    double currentValue = (double)(value - minValue) * 100 / (maxValue - minValue);
    QString strValue = QString("%1%").arg(currentValue, 0, 'f', precision);
    QString strMaxValue = QString("%1%").arg(maxValue, 0, 'f', precision);
    //字符的宽度取最大值字符的宽度 + 10
    int textWidth = painter->fontMetrics().width(strMaxValue) + 10;
    //绘制进度值背景
    QPointF textTopLeft(width() - space - textWidth, space);
    QPointF textBottomRight(width() - space, height() - space);
    QRectF textRect(textTopLeft, textBottomRight);
    painter->setPen(barBgColor);
    painter->setBrush(barBgColor);
    painter->drawRoundedRect(textRect, radius, radius);
    //绘制进度值
    painter->setPen(textColor);
    painter->drawText(textRect, Qt::AlignCenter, strValue);
    //绘制进度条背景
    QRectF barBgRect(QPointF(space, space), QPointF(width() - space * 2 - textWidth, height() - space));
    painter->setPen(Qt::NoPen);
    painter->setBrush(barBgColor);
    painter->drawRoundedRect(barBgRect, radius, radius);
    //绘制进度条
    double length = width() - space - space * 2 - textWidth;
    //计算每一格移动多少
    double increment = length / (maxValue - minValue);
    QRectF barRect(QPointF(space, space), QPointF(space + increment * (value - minValue), height() - space));
    painter->setBrush(barColor);
    painter->drawRoundedRect(barRect, radius, radius);
    //绘制背景分割线条 每一格长度7
    painter->setPen(lineColor);
    int initX = 5;
    int lineCount = barBgRect.width() / step;
    double lineX = (double)barBgRect.width() / lineCount;
    //线条高度在进度条高度上 - 1
    while (lineCount > 0)
    {
        QPointF topPot(initX + lineX, space + 1);
        QPointF bottomPot(initX + lineX, height() - space - 1);
        painter->drawLine(topPot, bottomPot);
        initX += lineX;
        lineCount--;
    }
    painter->restore();
}

void ProgressBarColor::setRange(double minValue, double maxValue)
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

void ProgressBarColor::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void ProgressBarColor::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value)
        return;
    this->value = value;
    emit valueChanged(value);
    update();
}

void ProgressBarColor::setValue(int value)
{
    setValue((double)value);
}

void ProgressBarColor::setPrecision(int precision)
{
    this->precision = precision;
    update();
}

void ProgressBarColor::setStep(int step)
{
    this->step = step;
    update();
}

void ProgressBarColor::setSpace(int space)
{
    this->space = space;
    update();
}

void ProgressBarColor::setRadius(int radius)
{
    this->radius = radius;
    update();
}

void ProgressBarColor::setBgColor(QColor bgColorStart, QColor bgColorEnd)
{
    this->bgColorStart = bgColorStart;
    this->bgColorEnd = bgColorEnd;
    update();
}

void ProgressBarColor::setLineColor(QColor lineColor)
{
    this->lineColor = lineColor;
    update();
}

void ProgressBarColor::setTextColor(QColor textColor)
{
    this->textColor = textColor;
    update();
}

void ProgressBarColor::setBarColor(QColor barBgColor, QColor barColor)
{
    this->barBgColor = barBgColor;
    this->barColor = barColor;
    update();
}

void ProgressBarColor::test01(void)
{
    this->setRange(0, 1000);
}

void ProgressBarColor::test02(void)
{
    this->setBarColor(QColor(255, 255, 255), QColor(255, 107, 107));
    this->setPrecision(1);
    this->setStep(5);
    this->setRange(0, 1000);
}

void ProgressBarColor::test03(void)
{
    this->setBarColor(QColor(250, 250, 250), QColor(24, 189, 155));
    this->setPrecision(0);
    this->setStep(7);
    this->setRange(0, 1000);
}



/**
    1:可设置各种颜色
    2:可启用动画效果显示
    3:可设置起始旋转角度及结束旋转角度
    4:可设置单位和标题
    5:可设置大刻度数量和小刻度数量
    6:增加一些完整性校验及改进部分算法
*/

GaugeSpeed::GaugeSpeed(QWidget* parent) : QWidget(parent)
{
    value = 0;
    minValue = 0;
    maxValue = 100;
    precision = 0;
    unit = "km/h";
    title = "时速表";
    scaleMajor = 10;
    scaleMinor = 10;
    startAngle = 60;
    endAngle = 60;
    animation = false;
    animationStep = 0.5;
    crownColorStart = QColor(255, 255, 255);
    crownColorEnd = QColor(166, 166, 166);
    foreground = QColor(255, 255, 255);
    background = QColor(65, 65, 65);
    pointerColor = QColor(160, 160, 160);
    reverse = false;
    currentValue = 0;
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));
    setFont(QFont("Arial", 7));
}

GaugeSpeed::~GaugeSpeed()
{
    if (timer->isActive())
        timer->stop();
}

void GaugeSpeed::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    int side = qMin(width, height);
    painter.scale(side / 200.0, side / 200.0);
    //绘制外边框
    drawCrown(&painter);
    //绘制背景
    drawBg(&painter);
    //绘制刻度线
    drawScale(&painter);
    //绘制刻度值
    drawScaleNum(&painter);
    //绘制标题
    drawTitle(&painter);
    //绘制当前值
    drawValue(&painter);
    //绘制指针
    drawIndicator(&painter);
}

void GaugeSpeed::drawCrown(QPainter* painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient crownGradient(0, -radius, 0, radius);
    crownGradient.setColorAt(0, crownColorStart);
    crownGradient.setColorAt(1, crownColorEnd);
    painter->setBrush(crownGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeSpeed::drawBg(QPainter* painter)
{
    int radius = 92;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(background);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeSpeed::drawScale(QPainter* painter)
{
    int radius = 72;
    painter->save();
    painter->setPen(foreground);
    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    for (int i = 0; i <= steps; i++)
    {
        if (i % scaleMinor == 0)
        {
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 10, 0, radius);
        }
        else
        {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 5, 0, radius);
        }
        painter->rotate(angleStep);
    }
    painter->restore();
}

void GaugeSpeed::drawScaleNum(QPainter* painter)
{
    int radius = 82;
    painter->save();
    painter->setPen(foreground);
    double startRad = (360 - startAngle - 90) * (M_PI / 180);
    double deltaRad = (360 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;
    for (int i = 0; i <= scaleMajor; i++)
    {
        double sina = sin(startRad - i * deltaRad);
        double cosa = cos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;
        QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
        double textWidth = fontMetrics().width(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight / 4;
        painter->drawText(x, y, strValue);
    }
    painter->restore();
}

void GaugeSpeed::drawTitle(QPainter* painter)
{
    painter->save();
    painter->setPen(foreground);
    painter->setFont(QFont("Microsoft Yahei", 10));
    double textWidth = painter->fontMetrics().width(title);
    painter->drawText(-textWidth / 2, -30, title);
    painter->restore();
}

void GaugeSpeed::drawValue(QPainter* painter)
{
    painter->save();
    painter->setPen(foreground);
    painter->setFont(QFont("Arial", 9));
    QString strValue = QString("%1 %2").arg(currentValue, 0, 'f', precision).arg(unit);
    double textWidth = painter->fontMetrics().width(strValue);
    painter->drawText(-textWidth / 2, 42, strValue);
    painter->restore();
}

void GaugeSpeed::drawIndicator(QPainter* painter)
{
    painter->save();
    QPolygon pts;
    pts.setPoints(3, -2, 0, 2, 0, 0, 60);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    //画指针
    painter->rotate(degRotate);
    QRadialGradient haloGradient(0, 0, 60, 0, 0);
    haloGradient.setColorAt(0, pointerColor.lighter(100));
    haloGradient.setColorAt(1, pointerColor);
    painter->setPen(foreground);
    painter->setBrush(haloGradient);
    painter->drawConvexPolygon(pts);
    painter->restore();
    //画中心点
    painter->save();
    QConicalGradient coneGradient(0, 0, -90.0);
    coneGradient.setColorAt(0.0, background);
    coneGradient.setColorAt(0.5, foreground);
    coneGradient.setColorAt(1.0, background);
    painter->setPen(Qt::NoPen);
    painter->setBrush(coneGradient);
    painter->drawEllipse(-5, -5, 10, 10);
    painter->restore();
}

void GaugeSpeed::updateValue()
{
    if (!reverse)
    {
        if (currentValue >= value)
            timer->stop();
        else
            currentValue += animationStep;
    }
    else
    {
        if (currentValue <= value)
            timer->stop();
        else
            currentValue -= animationStep;
    }
    update();
}

void GaugeSpeed::setRange(double minValue, double maxValue)
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

void GaugeSpeed::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void GaugeSpeed::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value)
        return;
    if (value > this->value)
        reverse = false;
    else if (value < this->value)
        reverse = true;
    this->value = value;
    emit valueChanged(value);
    if (!animation)
    {
        currentValue = this->value;
        update();
    }
    else
        timer->start();
}

void GaugeSpeed::setValue(int value)
{
    setValue((double)value);
}

void GaugeSpeed::setPrecision(int precision)
{
    //最大精确度为 3
    if (precision <= 3)
    {
        this->precision = precision;
        update();
    }
}

void GaugeSpeed::setUnit(QString unit)
{
    this->unit = unit;
    update();
}

void GaugeSpeed::setTitle(QString title)
{
    this->title = title;
    update();
}

void GaugeSpeed::setScaleMajor(int scaleMajor)
{
    this->scaleMajor = scaleMajor;
    update();
}

void GaugeSpeed::setScaleMinor(int scaleMinor)
{
    this->scaleMinor = scaleMinor;
    update();
}

void GaugeSpeed::setStartAngle(int startAngle)
{
    this->startAngle = startAngle;
    update();
}

void GaugeSpeed::setEndAngle(int endAngle)
{
    this->endAngle = endAngle;
    update();
}

void GaugeSpeed::setAnimation(bool animation)
{
    this->animation = animation;
    update();
}

void GaugeSpeed::setAnimationStep(double animationStep)
{
    this->animationStep = animationStep;
    update();
}

void GaugeSpeed::setCrownColor(QColor crownColorStart, QColor crownColorEnd)
{
    this->crownColorStart = crownColorStart;
    this->crownColorEnd = crownColorEnd;
    update();
}

void GaugeSpeed::setForeground(QColor foreground)
{
    this->foreground = foreground;
    update();
}

void GaugeSpeed::setBackground(QColor background)
{
    this->background = background;
    update();
}

void GaugeSpeed::setPointerColor(QColor pointerColor)
{
    this->pointerColor = pointerColor;
    update();
}

void GaugeSpeed::test01(void)
{
    this->setRange(0, 1000);
    this->setValue(100);
}

void GaugeSpeed::test02(void)
{
    this->setTitle("电压值");
    this->setUnit("Kv");
    this->setRange(0, 1000);
    this->setValue(100);
    this->setScaleMajor(10);
    this->setScaleMinor(5);
    this->setStartAngle(40);
    this->setEndAngle(40);
    this->setAnimation(true);
    this->setAnimationStep(1);
    this->setCrownColor(QColor(218, 218, 218), QColor(160, 160, 160));
    this->setBackground(QColor(207, 31, 41));
    this->setForeground(QColor(255, 255, 255));
    this->setPointerColor(QColor(213, 71, 68));
}



/**
    1:可设置背景色/第一第二第三颜色
    2:可设置文字和刻度颜色
    3:可设置中间圆点颜色
    4:修正当范围值中最小值为负数时的bug
    5:修正刻度值在三分之一阶段时切换不准确的bug
    6:增加一些完整性校验
*/

GaugeColor::GaugeColor(QWidget* parent) : QWidget(parent)
{
    value = 0;
    minValue = 0;
    maxValue = 100;
    unit = "";
    radius = 0;
    centerPoint = QPointF(0, 0);
    textColor = QColor(255, 255, 255);
    bgColor = QColor(173, 173, 173);
    firstColor = QColor(34, 139, 34);
    secondColor = QColor(238, 238, 0);
    thirdColor = QColor(205, 51, 51);
    centerColor = QColor(94, 94, 94);
    setFont(QFont("Arial", 8));
}

GaugeColor::~GaugeColor()
{
}

void GaugeColor::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,启用反锯齿,平移坐标轴中心
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    //更新仪表盘最外圈半径大小
    radius = qMin(width, height) / 2;
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

void GaugeColor::drawBg(QPainter* painter)
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

void GaugeColor::drawDial(QPainter* painter)
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

void GaugeColor::drawTicks(QPainter* painter)
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

void GaugeColor::drawPointer(QPainter* painter)
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

void GaugeColor::drawValue(QPainter* painter)
{
    QString strValue = QString("%1%2").arg(value).arg(unit);
    painter->save();
    painter->setPen(textColor);
    painter->drawText(QRectF(-30, radius * 0.6 * 0.5, 60, 30), Qt::AlignCenter, strValue);
    painter->restore();
}

void GaugeColor::setRange(double minValue, double maxValue)
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

void GaugeColor::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void GaugeColor::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value)
        return;
    this->value = value;
    emit valueChanged(value);
    update();
}

void GaugeColor::setValue(int value)
{
    setValue((double)value);
}

void GaugeColor::setUnit(QString unit)
{
    this->unit = unit;
    update();
}

void GaugeColor::setTextColor(QColor textColor)
{
    this->textColor = textColor;
    update();
}

void GaugeColor::setBgColor(QColor bgColor)
{
    this->bgColor = bgColor;
    update();
}

void GaugeColor::setFirstColor(QColor firstColor)
{
    this->firstColor = firstColor;
    update();
}

void GaugeColor::setSecondColor(QColor secondColor)
{
    this->secondColor = secondColor;
    update();
}

void GaugeColor::setThirdColor(QColor thirdColor)
{
    this->thirdColor = thirdColor;
    update();
}

void GaugeColor::setCenterColor(QColor centerColor)
{
    this->centerColor = centerColor;
    update();
}

void GaugeColor::test01(void)
{
    this->setUnit("km/h");
    this->setRange(0, 1000);
    this->setValue(100);
}

void GaugeColor::test02(void)
{
    this->setTextColor(QColor(0, 0, 0));
    this->setBgColor(QColor(255, 255, 255));
    this->setFirstColor(QColor(112, 112, 112));
    this->setSecondColor(QColor(102, 205, 0));
    this->setThirdColor(QColor(135, 205, 254));
    this->setCenterColor(QColor(51, 51, 51));
    this->setUnit("℃");
    this->setRange(0, 1000);
    this->setValue(100);
}



/**
    1:可设置范围值,支持负数值
    2:可设置精确度,最大支持小数点后3位
    3:可设置大刻度数量/小刻度数量
    4:可设置开始旋转角度/结束旋转角度
    5:可设置是否启用动画效果以及动画效果每次移动的步长
    6:可设置外圆背景/内圆背景/饼圆三种颜色/刻度尺颜色/文字颜色
    7:自适应窗体拉伸,刻度尺和文字自动缩放
    8:可自由拓展各种渐变色,各圆的半径
    9:三色圆环按照比例设置范围角度 用户可以自由设置三色占用比例
    10:圆环样式可选择 三色圆环 当前圆环
    11:指示器样式可选择 圆形指示器 指针指示器 三角形指示器
*/
GaugeCar::GaugeCar(QWidget* parent) : QWidget(parent)
{
    value = 0;
    minValue = 0;
    maxValue = 100;
    precision = 0;
    scaleMajor = 10;
    scaleMinor = 10;
    startAngle = 45;
    endAngle = 45;
    animation = false;
    animationStep = 0.5;
    outerCircleColor = QColor(80, 80, 80);
    innerCircleColor = QColor(60, 60, 60);
    pieColorStart = QColor(24, 189, 155);
    pieColorMid = QColor(218, 218, 0);
    pieColorEnd = QColor(255, 107, 107);
    coverCircleColor = QColor(100, 100, 100);
    scaleColor = QColor(255, 255, 255);
    pointerColor = QColor(255, 107, 107);
    centerCircleColor = QColor(200, 200, 200);
    textColor = QColor(0, 0, 0);
    showOverlay = false;
    overlayColor = QColor(255, 255, 255, 60);
    pieStyle = PieStyle_Three;
    pointerStyle = PointerStyle_Indicator;
    reverse = false;
    currentValue = 0;
    timer = new QTimer(this);
    timer->setInterval(10);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateValue()));
    setFont(QFont("Arial", 7));
}

GaugeCar::~GaugeCar()
{
    if (timer->isActive())
        timer->stop();
}

void GaugeCar::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    int side = qMin(width, height);
    painter.scale(side / 200.0, side / 200.0);
    //绘制外圆
    drawOuterCircle(&painter);
    //绘制内圆
    drawInnerCircle(&painter);
    //绘制饼圆
    drawColorPie(&painter);
    //绘制覆盖圆 用以遮住饼圆多余部分
    drawCoverCircle(&painter);
    //绘制刻度线
    drawScale(&painter);
    //绘制刻度值
    drawScaleNum(&painter);
    //根据指示器形状绘制指示器
    if (pointerStyle == PointerStyle_Circle)
        drawPointerCircle(&painter);
    else if (pointerStyle == PointerStyle_Indicator)
        drawPointerIndicator(&painter);
    else if (pointerStyle == PointerStyle_Triangle)
        drawPointerTriangle(&painter);
    //绘制指针中心圆外边框
    drawRoundCircle(&painter);
    //绘制指针中心圆
    drawCenterCircle(&painter);
    //绘制当前值
    drawText(&painter);
    //绘制遮罩层
    drawOverlay(&painter);
}

void GaugeCar::drawOuterCircle(QPainter* painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(outerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawInnerCircle(QPainter* painter)
{
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(innerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawColorPie(QPainter* painter)
{
    int radius = 60;
    painter->save();
    painter->setPen(Qt::NoPen);
    QRectF rect(-radius, -radius, radius * 2, radius * 2);
    if (pieStyle == PieStyle_Three)
    {
        //计算总范围角度,根据占比例自动计算三色圆环范围角度
        double angleAll = 360.0 - startAngle - endAngle;
        double angleStart = angleAll * 0.7;
        double angleMid = angleAll * 0.15;
        double angleEnd = angleAll * 0.15;
        //绘制开始饼圆
        painter->setBrush(pieColorStart);
        painter->drawPie(rect, (270 - startAngle - angleStart) * 16, angleStart * 16);
        //绘制中间饼圆
        painter->setBrush(pieColorMid);
        painter->drawPie(rect, (270 - startAngle - angleStart - angleMid) * 16, angleMid * 16);
        //绘制结束饼圆
        painter->setBrush(pieColorEnd);
        painter->drawPie(rect, (270 - startAngle - angleStart - angleMid - angleEnd) * 16, angleEnd * 16);
    }
    else if (pieStyle == PieStyle_Current)
    {
        //计算总范围角度,当前值范围角度,剩余值范围角度
        double angleAll = 360.0 - startAngle - endAngle;
        double angleCurrent = angleAll * ((currentValue - minValue) / (maxValue - minValue));
        double angleOther = angleAll - angleCurrent;
        //绘制当前值饼圆
        painter->setBrush(pieColorStart);
        painter->drawPie(rect, (270 - startAngle - angleCurrent) * 16, angleCurrent * 16);
        //绘制剩余值饼圆
        painter->setBrush(pieColorEnd);
        painter->drawPie(rect, (270 - startAngle - angleCurrent - angleOther) * 16, angleOther * 16);
    }
    painter->restore();
}

void GaugeCar::drawCoverCircle(QPainter* painter)
{
    int radius = 50;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(coverCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawScale(QPainter* painter)
{
    int radius = 72;
    painter->save();
    painter->setPen(scaleColor);
    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    for (int i = 0; i <= steps; i++)
    {
        if (i % scaleMinor == 0)
        {
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 10, 0, radius);
        }
        else
        {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 5, 0, radius);
        }
        painter->rotate(angleStep);
    }
    painter->restore();
}

void GaugeCar::drawScaleNum(QPainter* painter)
{
    int radius = 82;
    painter->save();
    painter->setPen(scaleColor);
    double startRad = (360 - startAngle - 90) * (M_PI / 180);
    double deltaRad = (360 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;
    for (int i = 0; i <= scaleMajor; i++)
    {
        double sina = sin(startRad - i * deltaRad);
        double cosa = cos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;
        QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
        double textWidth = fontMetrics().width(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight / 4;
        painter->drawText(x, y, strValue);
    }
    painter->restore();
}

void GaugeCar::drawPointerCircle(QPainter* painter)
{
    int radius = 12;
    int offset = 10;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);
    painter->setPen(QPen(scaleColor, 2, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(0, radius * 2 + offset, 0, radius * 2 + 10 + offset);
    painter->restore();
}

void GaugeCar::drawPointerIndicator(QPainter* painter)
{
    int radius = 75;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);
    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeCar::drawPointerTriangle(QPainter* painter)
{
    int radius = 10;
    int offset = 38;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);
    QPolygon pts;
    pts.setPoints(3, -5, 0 + offset, 5, 0 + offset, 0, radius + offset);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (currentValue - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeCar::drawRoundCircle(QPainter* painter)
{
    int radius = 18;
    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(pointerColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawCenterCircle(QPainter* painter)
{
    int radius = 15;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(centerCircleColor);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeCar::drawText(QPainter* painter)
{
    int radius = 100;
    painter->save();
    painter->setPen(textColor);
    painter->setFont(QFont("Arial", 9));
    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue = QString("%1").arg((double)currentValue, 0, 'f', precision);
    painter->drawText(textRect, Qt::AlignCenter, strValue);
    painter->restore();
}

void GaugeCar::drawOverlay(QPainter* painter)
{
    if (!showOverlay)
        return;
    int radius = 90;
    painter->save();
    painter->setPen(Qt::NoPen);
    QPainterPath smallCircle;
    QPainterPath bigCircle;
    radius -= 1;
    smallCircle.addEllipse(-radius, -radius, radius * 2, radius * 2);
    radius *= 2;
    bigCircle.addEllipse(-radius, -radius + 140, radius * 2, radius * 2);
    //高光的形状为小圆扣掉大圆的部分
    QPainterPath highlight = smallCircle - bigCircle;
    QLinearGradient linearGradient(0, -radius / 2, 0, 0);
    overlayColor.setAlpha(100);
    linearGradient.setColorAt(0.0, overlayColor);
    overlayColor.setAlpha(30);
    linearGradient.setColorAt(1.0, overlayColor);
    painter->setBrush(linearGradient);
    painter->rotate(-20);
    painter->drawPath(highlight);
    painter->restore();
}

void GaugeCar::updateValue()
{
    if (!reverse)
    {
        if (currentValue >= value)
            timer->stop();
        else
            currentValue += animationStep;
    }
    else
    {
        if (currentValue <= value)
            timer->stop();
        else
            currentValue -= animationStep;
    }
    update();
}

void GaugeCar::setRange(double minValue, double maxValue)
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

void GaugeCar::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void GaugeCar::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value)
        return;
    if (value > this->value)
        reverse = false;
    else if (value < this->value)
        reverse = true;
    this->value = value;
    emit valueChanged(value);
    if (!animation)
    {
        currentValue = this->value;
        update();
    }
    else
        timer->start();
}

void GaugeCar::setValue(int value)
{
    setValue((double)value);
}

void GaugeCar::setPrecision(int precision)
{
    //最大精确度为 3
    if (precision <= 3)
    {
        this->precision = precision;
        update();
    }
}

void GaugeCar::setScaleMajor(int scaleMajor)
{
    this->scaleMajor = scaleMajor;
    update();
}

void GaugeCar::setScaleMinor(int scaleMinor)
{
    this->scaleMinor = scaleMinor;
    update();
}

void GaugeCar::setStartAngle(int startAngle)
{
    this->startAngle = startAngle;
    update();
}

void GaugeCar::setEndAngle(int endAngle)
{
    this->endAngle = endAngle;
    update();
}

void GaugeCar::setAnimation(bool animation)
{
    this->animation = animation;
    update();
}

void GaugeCar::setAnimationStep(double animationStep)
{
    this->animationStep = animationStep;
    update();
}

void GaugeCar::setOuterCircleColor(QColor outerCircleColor)
{
    this->outerCircleColor = outerCircleColor;
    update();
}

void GaugeCar::setInnerCircleColor(QColor innerCircleColor)
{
    this->innerCircleColor = innerCircleColor;
    update();
}

void GaugeCar::setPieColor(QColor pieColorStart, QColor pieColorMid, QColor pieColorEnd)
{
    this->pieColorStart = pieColorStart;
    this->pieColorMid = pieColorMid;
    this->pieColorEnd = pieColorEnd;
    update();
}

void GaugeCar::setCoverCircleColor(QColor coverCircleColor)
{
    this->coverCircleColor = coverCircleColor;
    update();
}

void GaugeCar::setScaleColor(QColor scaleColor)
{
    this->scaleColor = scaleColor;
    update();
}

void GaugeCar::setPointerColor(QColor pointerColor)
{
    this->pointerColor = pointerColor;
    update();
}

void GaugeCar::setCenterCircleColor(QColor centerCircleColor)
{
    this->centerCircleColor = centerCircleColor;
    update();
}

void GaugeCar::setTextColor(QColor textColor)
{
    this->textColor = textColor;
    update();
}

void GaugeCar::setShowOverlay(bool showOverlay)
{
    this->showOverlay = showOverlay;
    update();
}

void GaugeCar::setOverlayColor(QColor overlayColor)
{
    this->overlayColor = overlayColor;
    update();
}

void GaugeCar::setPieStyle(GaugeCar::PieStyle pieStyle)
{
    this->pieStyle = pieStyle;
    update();
}

void GaugeCar::setPointerStyle(GaugeCar::PointerStyle pointerStyle)
{
    this->pointerStyle = pointerStyle;
    update();
}

void GaugeCar::test01(void)
{
    this->setRange(0, 1000);
}

void GaugeCar::test02(void)
{
    this->setScaleMajor(15);
    this->setScaleMinor(5);
    this->setStartAngle(30);
    this->setEndAngle(30);
    this->setAnimation(true);
    this->setAnimationStep(0.5);
    this->setOuterCircleColor(QColor(150, 150, 150));
    this->setInnerCircleColor(QColor(221, 221, 221));
    this->setPieColor(QColor(97, 117, 118), QColor(238, 238, 0), QColor(255, 255, 255));
    this->setCoverCircleColor(QColor(192, 192, 192));
    this->setScaleColor(QColor(13, 13, 13));
    this->setPointerColor(QColor(24, 189, 155));
    this->setCenterCircleColor(QColor(210, 210, 210));
    this->setTextColor(QColor(5, 5, 5));
    this->setPieStyle(GaugeCar::PieStyle_Current);
    this->setPointerStyle(GaugeCar::PointerStyle_Triangle);
    this->setRange(0, 1000);
}



/**
    1:支持指示器样式选择 圆形指示器/指针指示器/三角形指示器
    2:支持鼠标按下旋转改变值
    3:支持负数刻度值
    4:支持设置当前值及范围值
    5:支持左右旋转角度设置
    6:支持设置大刻度数量和小刻度数量
    7:支持设置各种颜色
    8:字号随窗体拉伸自动变化
    9:可设置是否显示当前值
*/
GaugeDial::GaugeDial(QWidget* parent) : QWidget(parent)
{
    value = 0;
    minValue = 0;
    maxValue = 100;
    precision = 0;
    scaleMajor = 5;
    scaleMinor = 5;
    startAngle = 45;
    endAngle = 45;
    darkColor = QColor(100, 100, 100);
    lightColor = QColor(250, 250, 250);
    textColor = QColor(0, 0, 0);
    textFontName = "Arial";
    textFontSize = 8;
    showValue = true;
    pointerStyle = PointerStyle_Circle;
    setFont(QFont(textFontName, textFontSize));
}

GaugeDial::~GaugeDial()
{
}

void GaugeDial::mousePressEvent(QMouseEvent* e)
{
    pressed = true;
    setPressedValue(e->pos());
}

void GaugeDial::mouseReleaseEvent(QMouseEvent*)
{
    pressed = false;
}

void GaugeDial::mouseMoveEvent(QMouseEvent* e)
{
    if (pressed)
        setPressedValue(e->pos());
}

void GaugeDial::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    int side = qMin(width, height);
    painter.scale(side / 200.0, side / 200.0);
    //绘制刻度线
    drawScale(&painter);
    //绘制刻度值
    drawScaleNum(&painter);
    //绘制边界凹凸圆
    drawBorderCircle(&painter);
    //绘制中心圆
    drawCenterCircle(&painter);
    //根据指示器形状绘制指示器
    if (pointerStyle == PointerStyle_Circle)
        drawPointerCircle(&painter);
    else if (pointerStyle == PointerStyle_Indicator)
        drawPointerIndicator(&painter);
    else if (pointerStyle == PointerStyle_Triangle)
        drawPointerTriangle(&painter);
    //绘制当前值中心背景圆
    drawTextCircle(&painter);
    //绘制当前值
    drawText(&painter);
}

void GaugeDial::drawScale(QPainter* painter)
{
    int radius = 80;
    painter->save();
    painter->setPen(textColor);
    painter->rotate(startAngle);
    int steps = (scaleMajor * scaleMinor);
    double angleStep = (360.0 - startAngle - endAngle) / steps;
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    for (int i = 0; i <= steps; i++)
    {
        if (i % scaleMinor == 0)
        {
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 8, 0, radius);
        }
        else
        {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 8, 0, radius - 3);
        }
        painter->rotate(angleStep);
    }
    painter->restore();
}

void GaugeDial::drawScaleNum(QPainter* painter)
{
    int radius = 90;
    painter->save();
    painter->setPen(textColor);
    double startRad = (360 - startAngle - 90) * (M_PI / 180);
    double deltaRad = (360 - startAngle - endAngle) * (M_PI / 180) / scaleMajor;
    for (int i = 0; i <= scaleMajor; i++)
    {
        double sina = sin(startRad - i * deltaRad);
        double cosa = cos(startRad - i * deltaRad);
        double value = 1.0 * i * ((maxValue - minValue) / scaleMajor) + minValue;
        QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
        double textWidth = fontMetrics().width(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight / 4;
        painter->drawText(x, y, strValue);
    }
    painter->restore();
}

void GaugeDial::drawBorderCircle(QPainter* painter)
{
    int radius = 70;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(0, -radius, 0, radius);
    bgGradient.setColorAt(0.0, darkColor);
    bgGradient.setColorAt(1.0, lightColor);
    painter->setBrush(bgGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeDial::drawCenterCircle(QPainter* painter)
{
    int radius = 60;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(0, -radius, 0, radius);
    bgGradient.setColorAt(0.0, lightColor);
    bgGradient.setColorAt(1.0, darkColor);
    painter->setBrush(bgGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeDial::drawPointerCircle(QPainter* painter)
{
    int radius = 15;
    int offset = 10;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (value - minValue);
    painter->rotate(degRotate);
    QLinearGradient bgGradient(0, 0, radius * 2, radius * 2);
    bgGradient.setColorAt(0.0, darkColor);
    bgGradient.setColorAt(1.0, lightColor);
    painter->setBrush(bgGradient);
    painter->drawEllipse(-radius, radius + offset, radius * 2, radius * 2);
    painter->setPen(QPen(darkColor, 2, Qt::SolidLine, Qt::RoundCap));
    painter->drawLine(0, radius * 2 + offset, 0, radius * 2 + 10 + offset);
    painter->restore();
}

void GaugeDial::drawPointerIndicator(QPainter* painter)
{
    int radius = 55;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(darkColor);
    QPolygon pts;
    pts.setPoints(3, -5, 0, 5, 0, 0, radius);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (value - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeDial::drawPointerTriangle(QPainter* painter)
{
    int radius = 10;
    int offset = 45;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(darkColor);
    QPolygon pts;
    pts.setPoints(3, -radius / 2, offset, radius / 2, offset, 0, radius + offset);
    painter->rotate(startAngle);
    double degRotate = (360.0 - startAngle - endAngle) / (maxValue - minValue) * (value - minValue);
    painter->rotate(degRotate);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeDial::drawTextCircle(QPainter* painter)
{
    if (!showValue)
        return;
    int radius = 15;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient bgGradient(0, -radius, 0, radius);
    bgGradient.setColorAt(0.0, darkColor);
    bgGradient.setColorAt(1.0, lightColor);
    painter->setBrush(bgGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeDial::drawText(QPainter* painter)
{
    if (!showValue)
        return;
    int radius = 100;
    painter->save();
    painter->setPen(textColor);
    painter->setFont(QFont(textFontName, textFontSize, QFont::Bold));
    QRectF textRect(-radius, -radius, radius * 2, radius * 2);
    QString strValue = QString("%1").arg((double)value, 0, 'f', precision);
    painter->drawText(textRect, Qt::AlignCenter, strValue);
    painter->restore();
}

void GaugeDial::setPressedValue(QPointF pressedPoint)
{
    //计算总角度
    double length = 360 - startAngle - endAngle;
    //计算最近的刻度
    QPointF point = pressedPoint - rect().center();
    double theta = -atan2(-point.x(), -point.y()) * 180 / M_PI;
    theta = theta + length / 2;
    //计算每一个角度对应值移动多少
    double increment = (maxValue - minValue) / length;
    double currentValue = (theta * increment) + minValue;
    //过滤圆弧外的值
    if (currentValue <= minValue)
        currentValue = minValue;
    else if (currentValue >= maxValue)
        currentValue = maxValue;
    setValue(currentValue);
}

void GaugeDial::setRange(double minValue, double maxValue)
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

void GaugeDial::setRange(int minValue, int maxValue)
{
    setRange((double)minValue, (double)maxValue);
}

void GaugeDial::setValue(double value)
{
    //值小于最小值或者值大于最大值或者值和当前值一致则无需处理
    if (value < minValue || value > maxValue || value == this->value)
        return;
    this->value = value;
    emit valueChanged(value);
    update();
}

void GaugeDial::setValue(int value)
{
    setValue((double)value);
}

void GaugeDial::setPrecision(int precision)
{
    //最大精确度为 3
    if (precision <= 3)
    {
        this->precision = precision;
        update();
    }
}

void GaugeDial::setScaleMajor(int scaleMajor)
{
    this->scaleMajor = scaleMajor;
    update();
}

void GaugeDial::setScaleMinor(int scaleMinor)
{
    this->scaleMinor = scaleMinor;
    update();
}

void GaugeDial::setStartAngle(int startAngle)
{
    this->startAngle = startAngle;
    update();
}

void GaugeDial::setEndAngle(int endAngle)
{
    this->endAngle = endAngle;
    update();
}

void GaugeDial::setColor(QColor darkColor, QColor lightColor)
{
    this->darkColor = darkColor;
    this->lightColor = lightColor;
    update();
}

void GaugeDial::setText(QColor textColor, QString textFontName, int textFontSize)
{
    this->textColor = textColor;
    this->textFontName = textFontName;
    this->textFontSize = textFontSize;
    setFont(QFont(textFontName, textFontSize));
    update();
}

void GaugeDial::setShowValue(bool showValue)
{
    this->showValue = showValue;
    update();
}

void GaugeDial::setPointerStyle(GaugeDial::PointerStyle pointerStyle)
{
    this->pointerStyle = pointerStyle;
    update();
}



/**
    1:可设置边框颜色
    2:可设置前景色背景色
    3:可设置时钟分钟秒钟指针颜色
    4:可设置刷新间隔
    5:鼠标右键可设置四种效果 普通效果/弹簧效果/连续效果/隐藏效果
    6:增加设置系统时间公共槽函数,支持任意操作系统
*/
GaugeClock::GaugeClock(QWidget* parent) : QWidget(parent)
{
    action_secondstyle = new QAction(QString::fromLocal8Bit("弹簧效果"), this);
    connect(action_secondstyle, SIGNAL(triggered(bool)), this, SLOT(doAction()));
    this->addAction(action_secondstyle);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    crownColorStart = QColor(255, 255, 255);
    crownColorEnd = QColor(166, 166, 166);
    foreground = QColor(255, 255, 255);
    background = QColor(65, 65, 65);
    pointerHourColor = QColor(160, 160, 160);
    pointerMinColor = QColor(160, 160, 160);
    pointerSecColor = QColor(160, 160, 160);
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start();
    timerSpring = new QTimer(this);
    timerSpring->setInterval(30);
    connect(timerSpring, SIGNAL(timeout()), this, SLOT(updateSpring()));
    angleSpring = 6.0 * (sec + (double)msec / 1000);
    setSecondStyle(SecondStyle_Normal);
    updateTime();
    setFont(QFont("Arial", 9));
}

GaugeClock::~GaugeClock()
{
    if (timer->isActive())
        timer->stop();
    if (timerSpring->isActive())
        timerSpring->stop();
}

void GaugeClock::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.translate(width / 2, height / 2);
    int side = qMin(width, height);
    painter.scale(side / 200.0, side / 200.0);
    //绘制外边框
    drawCrown(&painter);
    //绘制背景
    drawBg(&painter);
    //绘制刻度线
    drawScale(&painter);
    //绘制刻度值
    drawScaleNum(&painter);
    //绘制时钟指针
    drawHour(&painter);
    //绘制分钟指针
    drawMin(&painter);
    //绘制秒钟指针
    drawSec(&painter);
    //绘制中心盖板
    drawDot(&painter);
}

void GaugeClock::drawCrown(QPainter* painter)
{
    int radius = 99;
    painter->save();
    painter->setPen(Qt::NoPen);
    QLinearGradient crownGradient(0, -radius, 0, radius);
    crownGradient.setColorAt(0, crownColorStart);
    crownGradient.setColorAt(1, crownColorEnd);
    painter->setBrush(crownGradient);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeClock::drawBg(QPainter* painter)
{
    int radius = 92;
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(background);
    painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    painter->restore();
}

void GaugeClock::drawScale(QPainter* painter)
{
    int radius = 90;
    painter->save();
    painter->setPen(foreground);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    for (int i = 0; i <= 60; i++)
    {
        if (i % 5 == 0)
        {
            pen.setWidthF(1.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 10, 0, radius);
        }
        else
        {
            pen.setWidthF(0.5);
            painter->setPen(pen);
            painter->drawLine(0, radius - 5, 0, radius);
        }
        painter->rotate(6);
    }
    painter->restore();
}

void GaugeClock::drawScaleNum(QPainter* painter)
{
    int radius = 70;
    painter->save();
    painter->setPen(foreground);
    double startRad = 60 * (M_PI / 180);
    double deltaRad = 30 * (M_PI / 180);
    for (int i = 0; i < 12; i++)
    {
        double sina = sin(startRad - i * deltaRad);
        double cosa = cos(startRad - i * deltaRad);
        QString strValue = QString("%1").arg(i + 1);
        double textWidth = fontMetrics().width(strValue);
        double textHeight = fontMetrics().height();
        int x = radius * cosa - textWidth / 2;
        int y = -radius * sina + textHeight / 4;
        painter->drawText(x, y, strValue);
    }
    painter->restore();
}

void GaugeClock::drawHour(QPainter* painter)
{
    painter->save();
    QPolygon pts;
    pts.setPoints(4, -3, 8, 3, 8, 2, -40, -2, -40);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    painter->rotate(30.0 * ((hour + min / 60.0)));
    painter->setPen(pointerHourColor);
    painter->setBrush(pointerHourColor);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeClock::drawMin(QPainter* painter)
{
    painter->save();
    QPolygon pts;
    pts.setPoints(4, -2, 8, 2, 8, 1, -60, -1, -60);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    painter->rotate(6.0 * (min + sec / 60.0));
    painter->setPen(pointerMinColor);
    painter->setBrush(pointerMinColor);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeClock::drawSec(QPainter* painter)
{
    if (secondStyle == SecondStyle_Hide)
        return;
    painter->save();
    QPolygon pts;
    pts.setPoints(3, -1, 10, 1, 10, 0, -70);
    QPen pen = painter->pen();
    pen.setCapStyle(Qt::RoundCap);
    painter->rotate(angleSpring);
    painter->setPen(pointerSecColor);
    painter->setBrush(pointerSecColor);
    painter->drawConvexPolygon(pts);
    painter->restore();
}

void GaugeClock::drawDot(QPainter* painter)
{
    painter->save();
    QConicalGradient coneGradient(0, 0, -90.0);
    coneGradient.setColorAt(0.0, background);
    coneGradient.setColorAt(0.5, foreground);
    coneGradient.setColorAt(1.0, background);
    painter->setOpacity(0.9);
    painter->setPen(Qt::NoPen);
    painter->setBrush(coneGradient);
    painter->drawEllipse(-5, -5, 10, 10);
    painter->restore();
}

void GaugeClock::doAction()
{
    QAction* action = (QAction*)sender();
    QString str = action->text();
    if (str == QString::fromLocal8Bit("弹簧效果"))
    {
        action->setText(QString::fromLocal8Bit("连续效果"));
        setSecondStyle(SecondStyle_Spring);
    }
    else if (str == QString::fromLocal8Bit("连续效果"))
    {
        action->setText(QString::fromLocal8Bit("隐藏效果"));
        setSecondStyle(SecondStyle_Continue);
    }
    else if (str == QString::fromLocal8Bit("隐藏效果"))
    {
        action->setText(QString::fromLocal8Bit("普通效果"));
        setSecondStyle(SecondStyle_Hide);
    }
    else if (str == QString::fromLocal8Bit("普通效果"))
    {
        action->setText(QString::fromLocal8Bit("弹簧效果"));
        setSecondStyle(SecondStyle_Normal);
    }
}

void GaugeClock::updateTime()
{
    QTime now = QTime::currentTime();
    hour = now.hour();
    min = now.minute();
    sec = now.second();
    msec = now.msec();
    if (secondStyle != SecondStyle_Hide)
    {
        angleSpring = 6.0 * (sec + (double)msec / 1000);
        if (secondStyle == SecondStyle_Spring)
        {
            angleSpring += 5;
            timerSpring->start();
        }
    }
    update();
}

void GaugeClock::updateSpring()
{
    angleSpring = 6.0 * (sec + (double)msec / 1000);
    update();
    timerSpring->stop();
}

void GaugeClock::setSecondStyle(GaugeClock::SecondStyle secondStyle)
{
    this->secondStyle = secondStyle;
    if (secondStyle == SecondStyle_Continue)
        timer->setInterval(100);
    else
        timer->setInterval(1000);
    if (secondStyle == SecondStyle_Spring)
        action_secondstyle->setText(QString::fromLocal8Bit("连续效果"));
    else if (secondStyle == SecondStyle_Continue)
        action_secondstyle->setText(QString::fromLocal8Bit("隐藏效果"));
    else if (secondStyle == SecondStyle_Hide)
        action_secondstyle->setText(QString::fromLocal8Bit("普通效果"));
    else if (secondStyle == SecondStyle_Normal)
    {
        action_secondstyle->setText(QString::fromLocal8Bit("弹簧效果"));
        updateTime();
        return;
    }
    update();
}

void GaugeClock::setSystemDateTime(QString year, QString month, QString day, QString hour, QString min, QString sec)
{
#ifdef Q_OS_WIN
    QProcess p(0);
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
#else
    QString cmd = QString("date %1%2%3%4%5.%6").arg(month).arg(day).arg(hour).arg(min).arg(year).arg(sec);
    system(cmd.toLatin1());
    system("hwclock -w");
#endif
}

void GaugeClock::setCrownColor(QColor crownColorStart, QColor crownColorEnd)
{
    this->crownColorStart = crownColorStart;
    this->crownColorEnd = crownColorEnd;
    update();
}

void GaugeClock::setForeground(QColor foreground)
{
    this->foreground = foreground;
    update();
}

void GaugeClock::setBackground(QColor background)
{
    this->background = background;
    update();
}

void GaugeClock::setPointerHourColor(QColor pointerHourColor)
{
    this->pointerHourColor = pointerHourColor;
    update();
}

void GaugeClock::setPointerMinColor(QColor pointerMinColor)
{
    this->pointerMinColor = pointerMinColor;
    update();
}

void GaugeClock::setPointerSecColor(QColor pointerSecColor)
{
    this->pointerSecColor = pointerSecColor;
    update();
}

void GaugeClock::testred(void)
{
    this->setCrownColor(QColor(218, 218, 218), QColor(160, 160, 160));
    this->setBackground(QColor(207, 31, 41));
    this->setForeground(QColor(230, 230, 230));
    this->setPointerHourColor(QColor(200, 200, 200));
    this->setPointerMinColor(QColor(200, 200, 200));
    this->setPointerSecColor(QColor(200, 200, 200));
    this->setSecondStyle(GaugeClock::SecondStyle_Spring);
}



/**
    1:支持鼠标右键切换风格
    2:支持设置四种背景风格样式
    3:支持四种秒针走动风格样式
    4:增加设置时间接口
*/
ImageClock::ImageClock(QWidget* parent) : QWidget(parent)
{
    setFont(QFont("Microsoft Yahei", 9));
    QAction* action_trad = new QAction(QString::fromLocal8Bit("黑色风格"), this);
    connect(action_trad, SIGNAL(triggered(bool)), this, SLOT(doAction()));
    this->addAction(action_trad);
    QAction* action_system = new QAction(QString::fromLocal8Bit("银色风格"), this);
    connect(action_system, SIGNAL(triggered(bool)), this, SLOT(doAction()));
    this->addAction(action_system);
    QAction* action_modern = new QAction(QString::fromLocal8Bit("红色风格"), this);
    connect(action_modern, SIGNAL(triggered(bool)), this, SLOT(doAction()));
    this->addAction(action_modern);
    QAction* action_flower = new QAction(QString::fromLocal8Bit("花瓣风格"), this);
    connect(action_flower, SIGNAL(triggered(bool)), this, SLOT(doAction()));
    this->addAction(action_flower);
    action_secondstyle = new QAction(QString::fromLocal8Bit("弹簧效果"), this);
    connect(action_secondstyle, SIGNAL(triggered(bool)), this, SLOT(doAction()));
    this->addAction(action_secondstyle);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    imageNames << "trad" << "system" << "modern" << "flower";
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start();
    timerSpring = new QTimer(this);
    timerSpring->setInterval(30);
    connect(timerSpring, SIGNAL(timeout()), this, SLOT(updateSpring()));
    angleSpring = 6.0 * (sec + (double)msec / 1000);
    setClockStyle(ClockStyle_Trad);
    setSecondStyle(SecondStyle_Normal);
    updateTime();
}

ImageClock::~ImageClock()
{
    if (timer->isActive())
        timer->stop();
    if (timerSpring->isActive())
        timerSpring->stop();
}

void ImageClock::paintEvent(QPaintEvent*)
{
    int width = this->width();
    int height = this->height();
    //绘制准备工作,启用反锯齿,启用图片平滑缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //绘制背景
    drawBg(&painter);
    painter.translate(width / 2, height / 2);
    //绘制时钟指针 尺寸：13×129
    drawHour(&painter);
    //绘制分钟指针 尺寸：13×129
    drawMin(&painter);
    //绘制秒钟指针 尺寸：13×129
    drawSec(&painter);
    //绘制中心盖板
    drawDot(&painter);
}

void ImageClock::drawBg(QPainter* painter)
{
    painter->save();
    int pixX = rect().center().x() - clockBg.width() / 2;
    int pixY = rect().center().y() - clockBg.height() / 2;
    QPoint point(pixX, pixY);
    painter->drawImage(point, clockBg);
    painter->drawImage(point, clockHighlights);
    painter->restore();
}

void ImageClock::drawHour(QPainter* painter)
{
    painter->save();
    painter->rotate(30.0 * ((hour + min / 60.0)));
    painter->drawImage(QRect(-6.5, -64.5, 13, 129), clockHour);
    painter->restore();
}

void ImageClock::drawMin(QPainter* painter)
{
    painter->save();
    painter->rotate(6.0 * (min + sec / 60.0));
    painter->drawImage(QRect(-6.5, -64.5, 13, 129), clockMin);
    painter->restore();
}

void ImageClock::drawSec(QPainter* painter)
{
    if (secondStyle == SecondStyle_Hide)
        return;
    painter->save();
    painter->rotate(angleSpring);
    painter->drawImage(QRect(-6.5, -64.5, 13, 129), clockSec);
    painter->restore();
}

void ImageClock::drawDot(QPainter* painter)
{
    painter->save();
    painter->drawImage(QRect(-6.5, -64.5, 13, 129), clockDot);
    painter->restore();
}

void ImageClock::doAction()
{
    QAction* action = (QAction*)sender();
    QString str = action->text();
    if (str == QString::fromLocal8Bit("黑色风格"))
        setClockStyle(ClockStyle_Trad);
    else if (str == QString::fromLocal8Bit("银色风格"))
        setClockStyle(ClockStyle_System);
    else if (str == QString::fromLocal8Bit("红色风格"))
        setClockStyle(ClockStyle_Modern);
    else if (str == QString::fromLocal8Bit("花瓣风格"))
        setClockStyle(ClockStyle_Flower);
    else if (str == QString::fromLocal8Bit("弹簧效果"))
    {
        action->setText(QString::fromLocal8Bit("连续效果"));
        setSecondStyle(SecondStyle_Spring);
    }
    else if (str == QString::fromLocal8Bit("连续效果"))
    {
        action->setText(QString::fromLocal8Bit("隐藏效果"));
        setSecondStyle(SecondStyle_Continue);
    }
    else if (str == QString::fromLocal8Bit("隐藏效果"))
    {
        action->setText(QString::fromLocal8Bit("普通效果"));
        setSecondStyle(SecondStyle_Hide);
    }
    else if (str == QString::fromLocal8Bit("普通效果"))
    {
        action->setText(QString::fromLocal8Bit("弹簧效果"));
        setSecondStyle(SecondStyle_Normal);
    }
}

void ImageClock::updateTime()
{
    QTime now = QTime::currentTime();
    hour = now.hour();
    min = now.minute();
    sec = now.second();
    msec = now.msec();
    if (secondStyle != SecondStyle_Hide)
    {
        angleSpring = 6.0 * (sec + (double)msec / 1000);
        if (secondStyle == SecondStyle_Spring)
        {
            angleSpring += 5;
            timerSpring->start();
        }
    }
    update();
}

void ImageClock::updateSpring()
{
    angleSpring = 6.0 * (sec + (double)msec / 1000);
    update();
    timerSpring->stop();
}

void ImageClock::setClockStyle(ImageClock::ClockStyle clockStyle)
{
    QString imageName = imageNames.at(clockStyle);
    this->clockStyle = clockStyle;
    clockBg = QImage(QString(":/image/%1.png").arg(imageName));
    clockHour = QImage(QString(":/image/%1_h.png").arg(imageName));
    clockMin = QImage(QString(":/image/%1_m.png").arg(imageName));
    clockSec = QImage(QString(":/image/%1_s.png").arg(imageName));
    clockDot = QImage(QString(":/image/%1_dot.png").arg(imageName));
    clockHighlights = QImage(QString(":/image/%1_highlights.png").arg(imageName));
    update();
}

void ImageClock::setSecondStyle(ImageClock::SecondStyle secondStyle)
{
    this->secondStyle = secondStyle;
    if (secondStyle == SecondStyle_Continue)
        timer->setInterval(100);
    else
        timer->setInterval(1000);
    if (secondStyle == SecondStyle_Spring)
        action_secondstyle->setText(QString::fromLocal8Bit("连续效果"));
    else if (secondStyle == SecondStyle_Continue)
        action_secondstyle->setText(QString::fromLocal8Bit("隐藏效果"));
    else if (secondStyle == SecondStyle_Hide)
        action_secondstyle->setText(QString::fromLocal8Bit("普通效果"));
    else if (secondStyle == SecondStyle_Normal)
    {
        action_secondstyle->setText(QString::fromLocal8Bit("弹簧效果"));
        updateTime();
        return;
    }
    update();
}

void ImageClock::setSystemDateTime(QString year, QString month, QString day, QString hour, QString min, QString sec)
{
#ifdef Q_OS_WIN
    QProcess p(0);
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("date %1-%2-%3\n").arg(year).arg(month).arg(day).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
    p.start("cmd");
    p.waitForStarted();
    p.write(QString("time %1:%2:%3.00\n").arg(hour).arg(min).arg(sec).toLatin1());
    p.closeWriteChannel();
    p.waitForFinished(1000);
    p.close();
#else
    QString cmd = QString("date %1%2%3%4%5.%6").arg(month).arg(day).arg(hour).arg(min).arg(year).arg(sec);
    system(cmd.toLatin1());
    system("hwclock -w");
#endif
}


WProgressbar::WProgressbar(QWidget* parent) : QWidget(parent)
    , ui(new Ui::WProgressbar)
{
    ui->setupUi(this);
    InitGui();
}

WProgressbar::~WProgressbar()
{
}

void WProgressbar::InitGui(void)
{
    initGuiRound();
    initGuiGauge();
    initGuiPro();
    initGuiGS();
    initGuiGC();
    initGuiCar();
    initGuiGD();
    initGuiClock();
}

void WProgressbar::initGuiRound(void)
{
    roundBar1 = new ProgressBarRound(ui->wdg01);
    roundBar1->test01();
    m_lay1 = new CMylay(ui->wdg01);
    m_lay1->setWgt(roundBar1);
    connectToSlider(roundBar1);
    roundBar2 = new ProgressBarRound(ui->wdg02);
    roundBar2->test02();
    m_lay2 = new CMylay(ui->wdg02);
    m_lay2->setWgt(roundBar2);
    connectToSlider(roundBar2);
    roundBar3 = new ProgressBarRound(ui->wdg03);
    roundBar3->test03();
    m_lay3 = new CMylay(ui->wdg03);
    m_lay3->setWgt(roundBar3);
    connectToSlider(roundBar3);
    roundBar4 = new ProgressBarRound(ui->wdg04);
    roundBar4->test04();
    m_lay4 = new CMylay(ui->wdg04);
    m_lay4->setWgt(roundBar4);
    connectToSlider(roundBar4);
    roundBar5 = new ProgressBarRound(ui->wdg05);
    roundBar5->test05();
    m_lay5 = new CMylay(ui->wdg05);
    m_lay5->setWgt(roundBar5);
    connectToSlider(roundBar5);
    roundBar6 = new ProgressBarRound(ui->wdg06);
    roundBar6->test06();
    m_lay6 = new CMylay(ui->wdg06);
    m_lay6->setWgt(roundBar6);
    connectToSlider(roundBar6);
}

void WProgressbar::initGuiGauge(void)
{
    GaugeBar1 = new GaugeProgressBar(ui->wdgg01);
    GaugeBar1->test01();
    m_glay1 = new CMylay(ui->wdgg01);
    m_glay1->setWgt(GaugeBar1);
    GaugeBar2 = new GaugeProgressBar(ui->wdgg02);
    GaugeBar2->test02();
    m_glay2 = new CMylay(ui->wdgg02);
    m_glay2->setWgt(GaugeBar2);
    GaugeBar3 = new GaugeProgressBar(ui->wdgg03);
    GaugeBar3->test03();
    m_glay3 = new CMylay(ui->wdgg03);
    m_glay3->setWgt(GaugeBar3);
    GaugeBar4 = new GaugeProgressBar(ui->wdgg04);
    GaugeBar4->test04();
    m_glay4 = new CMylay(ui->wdgg04);
    m_glay4->setWgt(GaugeBar4);
    GaugeBar5 = new GaugeProgressBar(ui->wdgg05);
    GaugeBar5->test05();
    m_glay5 = new CMylay(ui->wdgg05);
    m_glay5->setWgt(GaugeBar5);
    GaugeBar6 = new GaugeProgressBar(ui->wdgg06);
    GaugeBar6->test06();
    m_glay6 = new CMylay(ui->wdgg06);
    m_glay6->setWgt(GaugeBar6);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GaugeBar4, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GaugeBar5, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GaugeBar6, SLOT(setValue(int)));
    //connect(GaugeBar4, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
    //connect(GaugeBar5, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
    //connect(GaugeBar6, SIGNAL(valueChanged(double)), this, SLOT(setValue(double)));
    //void setValue(double value)
    //{
    //    GaugeProgressBar *dial = (GaugeProgressBar *)sender();
    //    if (dial == ui->widget1) {
    //        ui->horizontalSlider1->setValue(value);
    //    }
    //    else if (dial == ui->widget2) {
    //        ui->horizontalSlider2->setValue(value);
    //    }
    //    else if (dial == ui->widget3) {
    //        ui->horizontalSlider3->setValue(value);
    //    }
    //    else {
    //        ui->verticalSlider->setValue(value);
    //    }
    //}
}

void WProgressbar::initGuiPro(void)
{
    proBar1 = new ProgressBarColor(ui->wdgpro_1);
    proBar1->test01();
    m_prolay1 = new CMylay(ui->wdgpro_1);
    m_prolay1->setWgt(proBar1);
    proBar2 = new ProgressBarColor(ui->wdgpro_2);
    proBar2->test02();
    m_prolay2 = new CMylay(ui->wdgpro_2);
    m_prolay2->setWgt(proBar2);
    proBar3 = new ProgressBarColor(ui->wdgpro_3);
    proBar3->test03();
    m_prolay3 = new CMylay(ui->wdgpro_3);
    m_prolay3->setWgt(proBar3);
    ui->horizontalSlider->setRange(0, 1000);
    ui->horizontalSlider->setValue(80);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), proBar1, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), proBar2, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), proBar3, SLOT(setValue(int)));
}

void WProgressbar::initGuiGS(void)
{
    GSBar1 = new GaugeSpeed(ui->wdgspd1);
    GSBar1->test01();
    m_GSlay1 = new CMylay(ui->wdgspd1);
    m_GSlay1->setWgt(GSBar1);
    GSBar2 = new GaugeSpeed(ui->wdgspd2);
    GSBar2->test02();
    m_GSlay2 = new CMylay(ui->wdgspd2);
    m_GSlay2->setWgt(GSBar2);
    ui->horizontalSlider->setRange(0, 1000);
    ui->horizontalSlider->setValue(100);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GSBar1, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GSBar2, SLOT(setValue(int)));
}

void WProgressbar::initGuiGC(void)
{
    GCBar1 = new GaugeColor(ui->wdgcpd1);
    GCBar1->test01();
    m_GClay1 = new CMylay(ui->wdgcpd1);
    m_GClay1->setWgt(GCBar1);
    GCBar2 = new GaugeColor(ui->wdgcpd2);
    GCBar2->test02();
    m_GClay2 = new CMylay(ui->wdgcpd2);
    m_GClay2->setWgt(GCBar2);
    ui->horizontalSlider->setRange(0, 1000);
    ui->horizontalSlider->setValue(100);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GCBar1, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GCBar2, SLOT(setValue(int)));
}

void WProgressbar::initGuiCar(void)
{
    CarBar1 = new GaugeCar(ui->wdgcar1);
    CarBar1->test01();
    m_Carlay1 = new CMylay(ui->wdgcar1);
    m_Carlay1->setWgt(CarBar1);
    CarBar2 = new GaugeCar(ui->wdgcar2);
    CarBar2->test02();
    m_Carlay2 = new CMylay(ui->wdgcar2);
    m_Carlay2->setWgt(CarBar2);
    ui->horizontalSlider->setRange(0, 1000);
    ui->horizontalSlider->setValue(100);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), CarBar1, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), CarBar2, SLOT(setValue(int)));
}

void WProgressbar::initGuiGD(void)
{
    GDBar1 = new GaugeDial(ui->wdgGD1);
    GDBar1->setRange(0, 1000);
    GDBar1->setValue(1);
    m_GDlay1 = new CMylay(ui->wdgGD1);
    m_GDlay1->setWgt(GDBar1);
    GDBar2 = new GaugeDial(ui->wdgGD2);
    GDBar2->setRange(0, 1000);
    GDBar2->setValue(1);
    GDBar2->setScaleMajor(10);
    GDBar2->setScaleMinor(10);
    GDBar2->setPointerStyle(GaugeDial::PointerStyle_Triangle);
    m_GDlay2 = new CMylay(ui->wdgGD2);
    m_GDlay2->setWgt(GDBar2);
    ui->horizontalSlider->setRange(0, 1000);
    ui->horizontalSlider->setValue(100);
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GDBar1, SLOT(setValue(int)));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), GDBar2, SLOT(setValue(int)));
    //下面不对 不会触发
    //connect(GDBar1, SIGNAL(valueChanged(double)), ui->horizontalSlider, SLOT(setValue(double)));
    //connect(GDBar2, SIGNAL(valueChanged(double)), ui->horizontalSlider, SLOT(setValue(double)));
}

void WProgressbar::initGuiClock(void)
{
    GClock1 = new GaugeClock(ui->wdgClock1);
    GClock1->testred();
    m_GClocklay1 = new CMylay(ui->wdgClock1);
    m_GClocklay1->setWgt(GClock1);

    GClock2 = new ImageClock(ui->wdgClock2);
    //GClock2->testred();
    m_GClocklay2 = new CMylay(ui->wdgClock2);
    m_GClocklay2->setWgt(GClock2);
}

void WProgressbar::connectToSlider(ProgressBarRound* bar)
{
    bar->setRange(ui->horizontalSlider->minimum(), ui->horizontalSlider->maximum());
    bar->setValue(ui->horizontalSlider->value());
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int)));
}




//*************************************************************************
//
//                         进度条大全
//
//                                               阿甘收集 20180516
//
//*************************************************************************
#ifndef WPROGRESSBAR_H
#define WPROGRESSBAR_H

#include "QPublic.h"

//圆形进度条控件
class CPP_API ProgressBarRound : public QWidget
{
    Q_OBJECT
public:
    enum BarStyle
    {
        BarStyle_Donut = 0,                 //圆环风格
        BarStyle_Pie = 1,                   //饼状风格
        BarStyle_Line = 2                   //线条风格
    };

    explicit ProgressBarRound(QWidget* parent = 0);
    ~ProgressBarRound();
public:
    static const int PositionLeft = 180;
    static const int PositionTop = 90;
    static const int PositionRight = 0;
    static const int PositionBottom = -90;

    double getValue() const
    {
        return value;
    }
    double getMinValue() const
    {
        return minValue;
    }
    double getMaxValue() const
    {
        return maxValue;
    }
    int getDecimals() const
    {
        return decimals;
    }
    double getNullPosition() const
    {
        return nullPosition;
    }
    BarStyle getBarStyle() const
    {
        return barStyle;
    }
    QString getFormat() const
    {
        return format;
    }
    double getOutlinePenWidth() const
    {
        return outlinePenWidth;
    }
    double getDataPenWidth() const
    {
        return dataPenWidth;
    }
    bool getClockwise() const
    {
        return clockwise;
    }
public slots:
    //设置范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);
    //设置目标值
    void setValue(double value);
    void setValue(int value);
    //设置精确度,小数点后几位
    void setDecimals(int decimals);
    //设置最小值位置
    void setNullPosition(double nullPosition);
    //设置样式
    void setBarStyle(BarStyle barStyle);
    //设置格式
    void setFormat(QString format);
    //设置外边框宽度
    void setOutlinePenWidth(double penWidth);
    //设置数据文本宽度
    void setDataPenWidth(double penWidth);
    //设置颜色集合
    void setDataColors(const QGradientStops& stopPoints);
    //设置进度旋转方向
    void setClockWise(bool clockwise);
protected:
    void paintEvent(QPaintEvent*);
    void drawBg(QPainter* painter, const QRectF& baseRect);
    void drawBase(QPainter* painter, const QRectF& baseRect);
    void drawValue(QPainter* painter, const QRectF& baseRect);
    void calculateInnerRect(const QRectF& baseRect, double outerRadius, QRectF& innerRect, double& innerRadius);
    void drawInnerBackground(QPainter* painter, const QRectF& innerRect);
    void drawText(QPainter* painter, const QRectF& innerRect, double innerRadius);
private:
    QString valueToText(double value) const;
    void valueFormatChanged();
    void rebuildDataBrushIfNeeded();
private:
    double maxValue, minValue;  //最大/小值
    double value;               //目标值
    int decimals;               //精确度,小数点后几位

    double nullPosition;        //最小值位置
    BarStyle barStyle;          //样式
    QString format;             //文本格式

    double outlinePenWidth;     //外边框宽度
    double dataPenWidth;        //数据文本宽度

    QGradientStops gradientData;//颜色集合
    bool clockwise;             //顺时针

    static const int UF_VALUE = 1;
    static const int UF_PERCENT = 2;
    static const int UF_MAX = 4;
    int updateFlags;
public:
    //下面是测试用例
    void test01(void);
    void test02(void);
    void test03(void);
    void test04(void);
    void test05(void);
    void test06(void);

};

//进度条仪表盘控件
class CPP_API GaugeProgressBar : public QWidget
{
    Q_OBJECT
public:
    enum PointerStyle
    {
        PointerStyle_Circle = 0,                 //圆形指示器
        PointerStyle_Indicator = 1,              //指针指示器
        PointerStyle_Triangle = 2                //三角形指示器
    };
    explicit GaugeProgressBar(QWidget* parent = 0);
    ~GaugeProgressBar();
protected:
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    void drawBg(QPainter* painter);
    void drawColorPie(QPainter* painter);
    void drawCoverCircle(QPainter* painter);
    void drawCircle(QPainter* painter);
    void drawPointerCircle(QPainter* painter);
    void drawPointerIndicator(QPainter* painter);
    void drawPointerTriangle(QPainter* painter);
    void drawText(QPainter* painter);
private:
    double value;                                //目标值
    double maxValue;                             //最小值
    double minValue;                             //最大值
    int precision;                               //精确度,小数点后几位

    int startAngle;                              //开始旋转角度
    int endAngle;                                //结束旋转角度

    QColor bgColor;                              //背景色
    QColor progressColor;                        //当前进度颜色
    QColor progressBgColor;                      //进度背景颜色
    QColor circleColorStart;                     //中间圆渐变开始颜色
    QColor circleColorEnd;                       //中间圆渐变结束颜色
    QColor textColor;                            //文字颜色

    bool showPointer;                            //是否显示指示器
    bool showValue;                              //是否显示当前值
    PointerStyle pointerStyle;                   //指针样式

    bool hover;                                  //是否鼠标悬停
    int radiusCoverCircle;                       //覆盖圆半径
    int radiusCircle;                            //中间圆半径
    QPropertyAnimation* animation;               //动画对象
private slots:
    void setEasingCurve();
    void updateRadius(QVariant radius);
private:
    //鼠标是否按下
    bool pressed;
    //根据鼠标按下的坐标设置当前按下坐标处的值
    void setPressedValue(QPointF pressedPoint);
public:
    double getValue() const
    {
        return value;
    }
    double getMinValue() const
    {
        return minValue;
    }
    double getMaxValue() const
    {
        return maxValue;
    }
    int getPrecision() const
    {
        return precision;
    }
    int getStartAngle() const
    {
        return startAngle;
    }
    int getEndAngle() const
    {
        return endAngle;
    }
    QColor getBgColor()const
    {
        return bgColor;
    }
    QColor getProgressColor()const
    {
        return progressColor;
    }
    QColor getProgressBgColor()const
    {
        return progressBgColor;
    }
    QColor getCircleColorStart()const
    {
        return circleColorStart;
    }
    QColor getCircleColorEnd()const
    {
        return circleColorEnd;
    }
    QColor getTextColor()const
    {
        return textColor;
    }
    bool getShowPointer()const
    {
        return showPointer;
    }
    bool getShowValue()const
    {
        return showValue;
    }
    PointerStyle getPointerStyle()const
    {
        return pointerStyle;
    }
public slots:
    //设置最大最小值-范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);
    //设置目标值
    void setValue(double value);
    void setValue(int value);
    //设置精确度
    void setPrecision(int precision);
    //设置开始旋转角度
    void setStartAngle(int startAngle);
    //设置结束旋转角度
    void setEndAngle(int endAngle);
    //设置背景色
    void setBgColor(QColor bgColor);
    //设置进度颜色
    void setProgressColor(QColor progressColor, QColor progressBgColor);
    //设置中间圆颜色
    void setCircleColor(QColor circleColorStart, QColor circleColorEnd);
    //设置文本颜色
    void setTextColor(QColor textColor);
    //设置是否显示指示器
    void setShowPointer(bool showPointer);
    //设置是否显示当前值
    void setShowValue(bool showValue);
    //设置指针样式
    void setPointerStyle(PointerStyle pointerStyle);
signals:
    void valueChanged(double value);
public:
    //下面是测试用例
    void test01(void);
    void test02(void);
    void test03(void);
    void test04(void);
    void test05(void);
    void test06(void);
};

//多彩进度条控件
class CPP_API ProgressBarColor : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressBarColor(QWidget* parent = 0);
protected:
    void paintEvent(QPaintEvent*);
    void drawBg(QPainter* painter);
    void drawBar(QPainter* painter);
private:
    //目标值,最小值,最大值
    double value, maxValue, minValue;
    //步长,间距,圆角角度,精确度(小数点后几位)
    int step, space, radius, precision;
    //背景渐变开始颜色/背景渐变结束颜色
    QColor bgColorStart, bgColorEnd;
    //线条颜色/文本颜色
    QColor lineColor, textColor;
    //进度背景颜色/进度颜色
    QColor barBgColor, barColor;
public:
    double getValue() const
    {
        return value;
    }
    double getMinValue() const
    {
        return minValue;
    }
    double getMaxValue() const
    {
        return maxValue;
    }
    int getPrecision() const
    {
        return precision;
    }
    int getStep() const
    {
        return step;
    }
    int getSpace() const
    {
        return space;
    }
    int getRadius() const
    {
        return radius;
    }
    QColor getBgColorStart()const
    {
        return bgColorStart;
    }
    QColor getBgColorEnd()const
    {
        return bgColorEnd;
    }
    QColor getLineColor()const
    {
        return lineColor;
    }
    QColor getTextColor()const
    {
        return textColor;
    }
    QColor getBarBgColor()const
    {
        return barBgColor;
    }
    QColor getBarColor()const
    {
        return barColor;
    }
public slots:
    //设置最大最小值-范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);

    //设置目标值
    void setValue(double value);
    void setValue(int value);

    //设置精确度
    void setPrecision(int precision);
    //设置步长
    void setStep(int step);
    //设置间距
    void setSpace(int space);
    //设置圆角角度
    void setRadius(int radius);

    //设置背景颜色
    void setBgColor(QColor bgColorStart, QColor bgColorEnd);
    //设置线条颜色
    void setLineColor(QColor lineColor);
    //设置文本颜色
    void setTextColor(QColor textColor);
    //设置进度颜色
    void setBarColor(QColor barBgColor, QColor barColor);

signals:
    void valueChanged(double value);
public:
    //下面是测试用例
    void test01(void);
    void test02(void);
    void test03(void);
};

//速度仪表盘控件
class CPP_API GaugeSpeed : public QWidget
{
    Q_OBJECT
public:
    explicit GaugeSpeed(QWidget* parent = 0);
    ~GaugeSpeed();
protected:
    void paintEvent(QPaintEvent*);
    void drawCrown(QPainter* painter);
    void drawBg(QPainter* painter);
    void drawScale(QPainter* painter);
    void drawScaleNum(QPainter* painter);
    void drawTitle(QPainter* painter);
    void drawValue(QPainter* painter);
    void drawIndicator(QPainter* painter);
private slots:
    void updateValue();
private:
    //目标值,最小值,最大值,精确度(小数点后几位)
    double value, maxValue, minValue, precision;
    //单位,标题
    QString unit, title;
    //大刻度数量,小刻度数量,开始旋转角度,结束旋转角度
    int scaleMajor, scaleMinor, startAngle, endAngle;
    //是否启用动画显示,是否倒退
    bool animation, reverse;
    //动画显示时步长,当前值
    double animationStep, currentValue;
    //外边框渐变开始颜色,外边框渐变结束颜色,前景色,背景色,指针颜色
    QColor crownColorStart, crownColorEnd, foreground, background, pointerColor;

    QTimer* timer;          //定时器绘制动画
public:
    double getValue() const
    {
        return value;
    }
    double getMinValue() const
    {
        return minValue;
    }
    double getMaxValue() const
    {
        return maxValue;
    }
    int getPrecision() const
    {
        return precision;
    }
    QString getUnit()const
    {
        return unit;
    }
    QString getTitle() const
    {
        return title;
    }
    int getScaleMajor() const
    {
        return scaleMajor;
    }
    int getScaleMinor() const
    {
        return scaleMinor;
    }
    int getStartAngle() const
    {
        return startAngle;
    }
    int getEndAngle() const
    {
        return endAngle;
    }
    bool getAnimation()const
    {
        return animation;
    }
    double getAnimationStep()const
    {
        return animationStep;
    }
    QColor getCrownColorStart()const
    {
        return crownColorStart;
    }
    QColor getCrownColorEnd()const
    {
        return crownColorEnd;
    }
    QColor getForeground()const
    {
        return foreground;
    }
    QColor getBackground()const
    {
        return background;
    }
    QColor getPointerColor()const
    {
        return pointerColor;
    }
public slots:
    //设置最大最小值-范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);
    //设置目标值
    void setValue(double value);
    void setValue(int value);
    //设置精确度
    void setPrecision(int precision);
    //设置单位
    void setUnit(QString unit);
    //设置标题
    void setTitle(QString title);
    //设置主刻度数量
    void setScaleMajor(int scaleMajor);
    //设置小刻度数量
    void setScaleMinor(int scaleMinor);
    //设置开始旋转角度
    void setStartAngle(int startAngle);
    //设置结束旋转角度
    void setEndAngle(int endAngle);
    //设置是否启用动画显示
    void setAnimation(bool animation);
    //设置动画显示的步长
    void setAnimationStep(double animationStep);
    //设置外边框渐变颜色
    void setCrownColor(QColor crownColorStart, QColor crownColorEnd);
    //设置前景色
    void setForeground(QColor foreground);
    //设备背景色
    void setBackground(QColor background);
    //设置指针颜色
    void setPointerColor(QColor pointerColor);

signals:
    void valueChanged(double value);
public:
    //下面是测试用例
    void test01(void);
    void test02(void);
};

//三色仪表盘控件
class CPP_API GaugeColor : public QWidget
{
    Q_OBJECT
public:
    explicit GaugeColor(QWidget* parent = 0);
    ~GaugeColor();
protected:
    void paintEvent(QPaintEvent*);
    void drawBg(QPainter* painter);         //绘制表盘背景
    void drawDial(QPainter* painter);       //绘制表盘
    void drawTicks(QPainter* painter);      //绘制表盘刻度
    void drawPointer(QPainter* painter);    //绘制指针
    void drawValue(QPainter* painter);      //绘制显示值和单位
private:
    double value;           //当前值
    double minValue;        //表盘刻度最小值
    double maxValue;        //表盘刻度最大值

    QString unit;           //单位
    int radius;             //仪表盘最外圈半径
    QPointF centerPoint;    //中心坐标

    QColor textColor;       //刻度及文字颜色
    QColor bgColor;         //背景色
    QColor firstColor;      //第一颜色
    QColor secondColor;     //第二颜色
    QColor thirdColor;      //第三颜色
    QColor centerColor;     //中间圆点颜色
public:
    //获取目标值
    double getValue()const
    {
        return value;
    }
    //获取最小值
    double getMinValue()const
    {
        return minValue;
    }
    //获取最大值
    double getMaxValue()const
    {
        return maxValue;
    }
    //获取单位
    QString getUnit()const
    {
        return unit;
    }
public slots:
    //设置最大最小值-范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);
    //设置目标值
    void setValue(double value);
    void setValue(int value);
    //设置文字后面的单位
    void setUnit(QString unit);
    //设置文字及刻度的颜色
    void setTextColor(QColor textColor);
    //设置背景色
    void setBgColor(QColor bgColor);
    //设置第一/第二/第三颜色
    void setFirstColor(QColor firstColor);
    void setSecondColor(QColor secondColor);
    void setThirdColor(QColor thirdColor);
    //设置中间圆点颜色
    void setCenterColor(QColor centerColor);

signals:
    void valueChanged(double value);
public:
    //下面是测试用例
    void test01(void);
    void test02(void);
};

//汽车仪表盘控件
class GaugeCar : public QWidget
{
    Q_OBJECT
public:
    enum PieStyle
    {
        PieStyle_Three = 0,                 //三色圆环
        PieStyle_Current = 1                //当前圆环
    };
    enum PointerStyle
    {
        PointerStyle_Circle = 0,            //圆形指示器
        PointerStyle_Indicator = 1,         //指针指示器
        PointerStyle_Triangle = 2           //三角形指示器
    };
    explicit GaugeCar(QWidget* parent = 0);
    ~GaugeCar();

protected:
    void paintEvent(QPaintEvent*);
    void drawOuterCircle(QPainter* painter);
    void drawInnerCircle(QPainter* painter);
    void drawColorPie(QPainter* painter);
    void drawCoverCircle(QPainter* painter);
    void drawScale(QPainter* painter);
    void drawScaleNum(QPainter* painter);
    void drawPointerCircle(QPainter* painter);
    void drawPointerIndicator(QPainter* painter);
    void drawPointerTriangle(QPainter* painter);
    void drawRoundCircle(QPainter* painter);
    void drawCenterCircle(QPainter* painter);
    void drawText(QPainter* painter);
    void drawOverlay(QPainter* painter);
private slots:
    void updateValue();
private:
    double value;                            //目标值
    double maxValue;                         //最小值
    double minValue;                         //最大值
    int precision;                           //精确度,小数点后几位

    int scaleMajor;                          //大刻度数量
    int scaleMinor;                          //小刻度数量
    int startAngle;                          //开始旋转角度
    int endAngle;                            //结束旋转角度

    bool animation;                          //是否启用动画显示
    double animationStep;                    //动画显示时步长

    QColor outerCircleColor;                 //外圆背景颜色
    QColor innerCircleColor;                 //内圆背景颜色

    QColor pieColorStart;                    //饼圆开始颜色
    QColor pieColorMid;                      //饼圆中间颜色
    QColor pieColorEnd;                      //饼圆结束颜色

    QColor coverCircleColor;                 //覆盖圆背景颜色
    QColor scaleColor;                       //刻度尺颜色
    QColor pointerColor;                     //指针颜色
    QColor centerCircleColor;                //中心圆颜色
    QColor textColor;                        //文字颜色

    bool showOverlay;                        //显示遮罩层
    QColor overlayColor;                     //遮罩层颜色

    PieStyle pieStyle;                       //饼图样式
    PointerStyle pointerStyle;               //指针样式

    bool reverse;                            //是否往回走
    double currentValue;                     //当前值
    QTimer* timer;                           //定时器绘制动画

public:
    double getValue() const
    {
        return value;
    }
    double getMinValue() const
    {
        return minValue;
    }
    double getMaxValue() const
    {
        return maxValue;
    }
    int getPrecision() const
    {
        return precision;
    }
    int getScaleMajor() const
    {
        return scaleMajor;
    }
    int getScaleMinor() const
    {
        return scaleMinor;
    }
    int getStartAngle() const
    {
        return startAngle;
    }
    int getEndAngle() const
    {
        return endAngle;
    }
    bool getAnimation()const
    {
        return animation;
    }
    double getAnimationStep()const
    {
        return animationStep;
    }
    QColor getOuterCircleColor()const
    {
        return outerCircleColor;
    }
    QColor getInnerCircleColor()const
    {
        return innerCircleColor;
    }
    QColor getPieColorStart()const
    {
        return pieColorStart;
    }
    QColor getPieColorMid()const
    {
        return pieColorMid;
    }
    QColor getPieColorEnd()const
    {
        return pieColorEnd;
    }
    QColor getCoverCircleColor()const
    {
        return coverCircleColor;
    }
    QColor getScaleColor()const
    {
        return scaleColor;
    }
    QColor getPointerColor()const
    {
        return pointerColor;
    }
    QColor getCenterCircleColor()const
    {
        return centerCircleColor;
    }
    QColor getTextColor()const
    {
        return textColor;
    }
    bool getShowOverlay()const
    {
        return showOverlay;
    }
    QColor getOverlayColor()const
    {
        return overlayColor;
    }
    PieStyle getPieStyle()const
    {
        return pieStyle;
    }
    PointerStyle getPointerStyle()const
    {
        return pointerStyle;
    }
public slots:
    //设置最大最小值-范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);
    //设置目标值
    void setValue(double value);
    void setValue(int value);
    //设置精确度
    void setPrecision(int precision);
    //设置主刻度数量
    void setScaleMajor(int scaleMajor);
    //设置小刻度数量
    void setScaleMinor(int scaleMinor);
    //设置开始旋转角度
    void setStartAngle(int startAngle);
    //设置结束旋转角度
    void setEndAngle(int endAngle);
    //设置是否启用动画显示
    void setAnimation(bool animation);
    //设置动画显示的步长
    void setAnimationStep(double animationStep);
    //设置外圆背景颜色
    void setOuterCircleColor(QColor outerCircleColor);
    //设置内圆背景颜色
    void setInnerCircleColor(QColor innerCircleColor);
    //设置饼圆三种颜色
    void setPieColor(QColor pieColorStart, QColor pieColorMid, QColor pieColorEnd);
    //设置覆盖圆背景颜色
    void setCoverCircleColor(QColor coverCircleColor);
    //设置刻度尺颜色
    void setScaleColor(QColor scaleColor);
    //设置指针颜色
    void setPointerColor(QColor pointerColor);
    //设置中心圆颜色
    void setCenterCircleColor(QColor centerCircleColor);
    //设置文本颜色
    void setTextColor(QColor textColor);
    //设置是否显示遮罩层
    void setShowOverlay(bool showOverlay);
    //设置遮罩层颜色
    void setOverlayColor(QColor overlayColor);
    //设置饼图样式
    void setPieStyle(PieStyle pieStyle);
    //设置指针样式
    void setPointerStyle(PointerStyle pointerStyle);
signals:
    void valueChanged(double value);
public:
    //下面是测试用例
    void test01(void);
    void test02(void);
};

//旋转仪表盘控件
class GaugeDial : public QWidget
{
    Q_OBJECT
public:
    enum PointerStyle
    {
        PointerStyle_Circle = 0,    //圆形指示器
        PointerStyle_Indicator = 1, //指针指示器
        PointerStyle_Triangle = 2   //三角形指示器
    };
    explicit GaugeDial(QWidget* parent = 0);
    ~GaugeDial();
protected:
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void paintEvent(QPaintEvent*);
    void drawScale(QPainter* painter);
    void drawScaleNum(QPainter* painter);
    void drawBorderCircle(QPainter* painter);
    void drawCenterCircle(QPainter* painter);
    void drawPointerCircle(QPainter* painter);
    void drawPointerIndicator(QPainter* painter);
    void drawPointerTriangle(QPainter* painter);
    void drawTextCircle(QPainter* painter);
    void drawText(QPainter* painter);
private:
    double value;               //目标值
    double maxValue;            //最小值
    double minValue;            //最大值
    int precision;              //精确度,小数点后几位

    int scaleMajor;             //大刻度数量
    int scaleMinor;             //小刻度数量
    int startAngle;             //开始旋转角度
    int endAngle;               //结束旋转角度

    QColor darkColor;           //暗黑颜色
    QColor lightColor;          //明亮颜色

    QColor textColor;           //文字颜色
    QString textFontName;       //文字字体
    int textFontSize;           //文字字号

    bool showValue;             //是否显示当前值
    PointerStyle pointerStyle;  //指针样式

private:
    //鼠标是否按下
    bool pressed;
    //根据鼠标按下的坐标设置当前按下坐标处的值
    void setPressedValue(QPointF pressedPoint);
public:
    double getValue() const
    {
        return value;
    }
    double getMinValue() const
    {
        return minValue;
    }
    double getMaxValue() const
    {
        return maxValue;
    }
    int getPrecision() const
    {
        return precision;
    }
    int getScaleMajor() const
    {
        return scaleMajor;
    }
    int getScaleMinor() const
    {
        return scaleMinor;
    }
    int getStartAngle() const
    {
        return startAngle;
    }
    int getEndAngle() const
    {
        return endAngle;
    }
    QColor getDarkColor()const
    {
        return darkColor;
    }
    QColor getLightColor()const
    {
        return lightColor;
    }
    QColor getTextColor()const
    {
        return textColor;
    }
    QString getTextFontName()const
    {
        return textFontName;
    }
    int getTextFontSize()const
    {
        return textFontSize;
    }
    bool getShowValue()const
    {
        return showValue;
    }
    PointerStyle getPointerStyle()const
    {
        return pointerStyle;
    }
public slots:
    //设置最大最小值-范围值
    void setRange(double minValue, double maxValue);
    void setRange(int minValue, int maxValue);
    //设置目标值
    void setValue(double value);
    void setValue(int value);
    //设置精确度
    void setPrecision(int precision);
    //设置主刻度数量
    void setScaleMajor(int scaleMajor);
    //设置小刻度数量
    void setScaleMinor(int scaleMinor);
    //设置开始旋转角度
    void setStartAngle(int startAngle);
    //设置结束旋转角度
    void setEndAngle(int endAngle);
    //设置颜色
    void setColor(QColor darkColor, QColor lightColor);
    //设置文本颜色
    void setText(QColor textColor, QString textFontName, int textFontSize);
    //设置是否显示当前值
    void setShowValue(bool showValue);
    //设置指针样式
    void setPointerStyle(PointerStyle pointerStyle);
signals:
    void valueChanged(double value);
};

//时钟仪表盘控件
class GaugeClock : public QWidget
{
    Q_OBJECT
public:
    enum SecondStyle
    {
        SecondStyle_Normal = 0,     //普通效果
        SecondStyle_Spring = 1,     //弹簧效果
        SecondStyle_Continue = 2,   //连续效果
        SecondStyle_Hide = 3        //隐藏效果
    };
    explicit GaugeClock(QWidget* parent = 0);
    ~GaugeClock();
protected:
    void paintEvent(QPaintEvent*);
    void drawCrown(QPainter* painter);
    void drawBg(QPainter* painter);
    void drawScale(QPainter* painter);
    void drawScaleNum(QPainter* painter);
    void drawHour(QPainter* painter);
    void drawMin(QPainter* painter);
    void drawSec(QPainter* painter);
    void drawDot(QPainter* painter);
private:
    SecondStyle secondStyle;    //秒针走动样式

    QColor crownColorStart;     //外边框渐变开始颜色
    QColor crownColorEnd;       //外边框渐变结束颜色

    QColor foreground;          //前景色
    QColor background;          //背景色

    QColor pointerHourColor;    //时钟指针颜色
    QColor pointerMinColor;     //分钟指针颜色
    QColor pointerSecColor;     //秒钟指针颜色

    QTimer* timer;              //定时器绘制
    int hour, min, sec, msec;   //时分秒毫秒

    QTimer* timerSpring;        //定时器显示弹簧效果
    double angleSpring;         //弹簧角度

    QAction* action_secondstyle;//秒针样式右键菜单

private slots:
    void doAction();
    void updateTime();
    void updateSpring();
public:
    SecondStyle getSecondStyle()const
    {
        return secondStyle;
    }
    QColor getCrownColorStart()const
    {
        return crownColorStart;
    }
    QColor getCrownColorEnd()const
    {
        return crownColorEnd;
    }
    QColor getForeground()const
    {
        return foreground;
    }
    QColor getBackground()const
    {
        return background;
    }
    QColor getPointerHourColor()const
    {
        return pointerHourColor;
    }
    QColor getPointerMinColor()const
    {
        return pointerMinColor;
    }
    QColor getPointerSecColor()const
    {
        return pointerSecColor;
    }
public slots:
    //设置秒针走动样式
    void setSecondStyle(SecondStyle secondStyle);
    //设置系统时间
    void setSystemDateTime(QString year, QString month, QString day, QString hour, QString min, QString sec);
    //设置外边框渐变颜色
    void setCrownColor(QColor crownColorStart, QColor crownColorEnd);
    //设置前景色
    void setForeground(QColor foreground);
    //设备背景色
    void setBackground(QColor background);
    //设置时钟指针颜色
    void setPointerHourColor(QColor pointerHourColor);
    //设置分钟指针颜色
    void setPointerMinColor(QColor pointerMinColor);
    //设置秒钟指针颜色
    void setPointerSecColor(QColor pointerSecColor);
public:
    //红色面板时钟例子
    void testred(void);
};

//图片时钟控件
class ImageClock : public QWidget
{
    Q_OBJECT
public:
    enum ClockStyle
    {
        ClockStyle_Trad = 0,        //黑色风格
        ClockStyle_System = 1,      //银色风格
        ClockStyle_Modern = 2,      //红色风格
        ClockStyle_Flower = 3       //花瓣风格
    };

    enum SecondStyle
    {
        SecondStyle_Normal = 0,     //普通效果
        SecondStyle_Spring = 1,     //弹簧效果
        SecondStyle_Continue = 2,   //连续效果
        SecondStyle_Hide = 3        //隐藏效果
    };

    explicit ImageClock(QWidget* parent = 0);
    ~ImageClock();

protected:
    void paintEvent(QPaintEvent*);
    void drawBg(QPainter* painter);
    void drawHour(QPainter* painter);
    void drawMin(QPainter* painter);
    void drawSec(QPainter* painter);
    void drawDot(QPainter* painter);

private:
    ClockStyle clockStyle;      //背景样式
    SecondStyle secondStyle;    //秒针走动样式

    QImage clockBg;             //主背景
    QImage clockHour;           //时钟背景
    QImage clockMin;            //分钟背景
    QImage clockSec;            //秒钟背景
    QImage clockDot;            //中间点背景
    QImage clockHighlights;     //高亮背景

    QStringList imageNames;     //图片名称集合

    QTimer* timer;              //定时器计算时间
    int hour, min, sec, msec;   //时分秒毫秒

    QTimer* timerSpring;        //定时器显示弹簧效果
    double angleSpring;         //弹簧角度

    QAction* action_secondstyle;//秒针样式右键菜单

private slots:
    void doAction();
    void updateTime();
    void updateSpring();

public:
    ClockStyle getClockStyle()const
    {
        return clockStyle;
    }
    SecondStyle getSecondStyle()const
    {
        return secondStyle;
    }

public slots:
    //设置图片背景时钟样式
    void setClockStyle(ClockStyle clockStyle);
    //设置秒针走动样式
    void setSecondStyle(SecondStyle secondStyle);
    //设置系统时间
    void setSystemDateTime(QString year, QString month, QString day, QString hour, QString min, QString sec);
};



namespace Ui
{
    class WProgressbar;
}

//演示界面
class WProgressbar : public QWidget
{
    Q_OBJECT
public:
    explicit WProgressbar(QWidget* parent = 0);
    ~WProgressbar();
public:
    void InitGui(void);
    void initGuiRound(void);
    void initGuiGauge(void);
    void initGuiPro(void);
    void initGuiGS(void);
    void initGuiGC(void);
    void initGuiCar(void);
    void initGuiGD(void);
    void initGuiClock(void);
private:
    //圆形进度条控件
    CMylay* m_lay1, *m_lay2, *m_lay3, *m_lay4, *m_lay5, *m_lay6;
    ProgressBarRound* roundBar1, *roundBar2, *roundBar3, *roundBar4, *roundBar5, *roundBar6;
    //进度条仪表盘控件
    CMylay* m_glay1, *m_glay2, *m_glay3, *m_glay4, *m_glay5, *m_glay6;
    GaugeProgressBar* GaugeBar1, *GaugeBar2, *GaugeBar3, *GaugeBar4, *GaugeBar5, *GaugeBar6;
    //多彩进度条控件
    CMylay* m_prolay1, *m_prolay2, *m_prolay3;
    ProgressBarColor* proBar1, *proBar2, *proBar3;
    //速度仪表
    CMylay* m_GSlay1, *m_GSlay2;
    GaugeSpeed* GSBar1, *GSBar2;
    //三色仪表盘
    CMylay* m_GClay1, *m_GClay2;
    GaugeColor* GCBar1, *GCBar2;
    //汽车仪表
    CMylay* m_Carlay1, *m_Carlay2;
    GaugeCar* CarBar1, *CarBar2;
    //旋转仪表
    CMylay* m_GDlay1, *m_GDlay2;
    GaugeDial* GDBar1, *GDBar2;
    //Clock
    CMylay* m_GClocklay1, *m_GClocklay2;
    GaugeClock* GClock1;
    ImageClock* GClock2;


    //界面
    Ui::WProgressbar* ui;
private slots:
    void connectToSlider(ProgressBarRound* bar);
};





#endif
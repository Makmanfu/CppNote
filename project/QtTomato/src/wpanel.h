//*************************************************************************
//
//
//                           所有控件记录
//
//
//*************************************************************************

#ifndef WPANEL_H
#define WPANEL_H

#include "QPublic.h"

//闪动波形 
class CPP_API WaveFrom : public QWidget
{
    Q_OBJECT
public:
    WaveFrom(QWidget* parent = 0);
    ~WaveFrom();
protected:
    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
    float usage() const;
private:
    //QPixmap pixmap;
    int base;       // 边界基数
    int columnInc;  // 动态列偏移值
    int* pts;       // 随机数据数组
    int mLen;       // 记录数组长度
    int ptNum;      // 点数目
};

//三色圆环仪表盘
class CPP_API DialPlot : public QWidget
{
    //1:增加设置背景色/第一第二第三颜色
    //2:增加设置文字字体及文字和刻度颜色
    //3:增加设置中间圆点颜色
    //4:修正当范围值中最小值为负数时的bug
    //5:修正刻度值在三分之一阶段时切换不准确的bug
    //6:增加一些完整性校验
    Q_OBJECT
public:
    explicit DialPlot(QWidget* parent = 0);
    ~DialPlot();
protected:
    void paintEvent(QPaintEvent*);
private:
    int radius;             //仪表盘最外圈半径
    QPointF centerPoint;    //中心坐标
    int minValue;           //表盘刻度最小值
    int maxValue;           //表盘刻度最大值
    double value;           //当前值
    QString unit;           //单位

    QFont textFont;         //文字字体
    QColor textColor;       //刻度及文字颜色
    QColor bgColor;         //背景色
    QColor firstColor;      //第一颜色
    QColor secondColor;     //第二颜色
    QColor thirdColor;      //第三颜色
    QColor centerColor;     //中间圆点颜色
    private slots:
    void drawBg(QPainter* painter);         //绘制表盘背景
    void drawDial(QPainter* painter);       //绘制表盘
    void drawTicks(QPainter* painter);      //绘制表盘刻度
    void drawPointer(QPainter* painter);    //绘制指针
    void drawValue(QPainter* painter);      //绘制显示值和单位
public:
    //获取最小值
    double getMinValue() const
    {
        return minValue;
    }
    //获取最大值
    double getMaxValue() const
    {
        return maxValue;
    }
    //获取目标值
    double getValue() const
    {
        return value;
    }
    //获取单位
    QString getUnit() const
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
    //设置文字字体
    void setTextFont(QFont textFont);
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
};

//圆盘进度
class CPP_API RoundPlot : public QWidget
{
    Q_OBJECT
public:
    RoundPlot(QWidget* parent = 0);
    ~RoundPlot();
protected:
    void paintEvent(QPaintEvent* event);
private:
    int minValue;               //最小值
    int maxValue;               //最大值
    int value;                  //目标值
    QString unit;               //单位
    int currentValue;           //当前值
    int step;                   //步长,动态变化时每次增加的步长
    QColor usedColor;           //已使用百分比颜色
    QColor freeColor;           //未使用百分比颜色
    QColor rangeTextColor;      //范围值文字颜色
    QColor valueTextColor;      //目标值文字颜色
    QColor valueBgColor;        //目标值背景色
    QColor outBgColor;          //外边框背景色
    QColor centerBgColorStart;  //中间圆环渐变背景起始颜色
    QColor centerBgColorEnd;    //中间圆环渐变背景结束颜色
    QFont rangeTextFont;        //范围值文字字体
    QFont valueTextFont;        //目标值文字字体
    QTimer* updateTimer;        //绘制定时器
    private slots:
    void updateValue();
public:
    //获取最小值
    int getMinValue() const
    {
        return minValue;
    }
    //获取最大值
    int getMaxValue() const
    {
        return maxValue;
    }
    //获取目标值
    int getValue() const
    {
        return value;
    }
    //获取单位
    QString getUnit() const
    {
        return unit;
    }
    public slots:
    //设置最大最小值-范围值
    void setRange(int minValue, int maxValue);
    //设置目标值
    void setValue(int value);
    //设置文字后面的单位
    void setUnit(QString unit);
    //设置已使用百分比颜色
    void setUsedColor(QColor usedColor);
    //设置未使用百分比颜色
    void setFreeColor(QColor freeColor);
    //设置范围值文字颜色
    void setRangeTextColor(QColor rangeTextColor);
    //设置目标值文字颜色
    void setValueTextColor(QColor valueTextColor);
    //设置目标值背景色
    void setValueBgColor(QColor valueBgColor);
    //设置外边框背景色
    void setOutBgColor(QColor outBgColor);
    //设置中央圆形背景色
    void setCenterBgColor(QColor centerBgColorStart, QColor centerBgColorEnd);
    //设置范围值文字字体
    void setRangeTextFont(QFont rangeTextFont);
    //设置目标值文字字体
    void setValueTextFont(QFont valueTextFont);
};











#endif




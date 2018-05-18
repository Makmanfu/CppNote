//*************************************************************************
//
//                          倒计时控件
//
//
//
//*************************************************************************

#ifndef WTIMENUMBAR_H
#define WTIMENUMBAR_H

#include "QPublic.h"

class FlowLayout;

//倒计时底层控件
class CPP_API TimeNumBar : public QWidget
{
    Q_OBJECT
public:
    TimeNumBar(QWidget* _parent = 0);
    virtual ~TimeNumBar();
public:
    //设置进度最大值最小值
    void setBarMaxMin(double _maxval, double _minval);
    //设置最大最小警告阈值
    void setBarWarllMaxMin(double _maxval, double _minval);
    //设置进度显示值
    void setBarValue(double val);
    //打开/暂停倒计时
    void timerPlayPause(void);
    void playTimer(bool bl);
    //设置倒计时 时分秒
    void setTimeHMS(int s);
    void setTimeHMS(int h, int m, int s);
    //设置显示总时间
    void setAllTimeShow(int s);
    void setAllTimeShow(const QString& str);
    //设置显示标题
    void setShowName(const QString& str);
private slots:
    void TimeSlot(void);
    void TestTimeSlot(void);
protected:
    void paintEvent(QPaintEvent* event);
private:
    int kk;
    double m_minvalue, m_maxvalue, m_value, m_minWvalue, m_maxWvalue;

    bool m_bStart;        //记录是否已经开始计时
    QTimer* m_Timer,*m_TestTmr;
    QTime* m_timeRecord;   //记录时间
    QGridLayout* m_grid;
    QLabel* m_labName;
    QSpacerItem* m_vSpacer;
    QLCDNumber* m_timelcd,*m_alltimelcd;

    void drawRule(QPainter& painter);
};

//多个进度控件
class CPP_API TimeBarPanel : public QWidget
{
    Q_OBJECT
public:
    TimeBarPanel(QWidget* _parent = 0);
    ~TimeBarPanel();
public:
    //添加倒计时控件
    void addTmrBar(int key);
    //获取倒计时控件
    TimeNumBar* getTmrBar(int key);
    //删除指定控件
    void delTmrBar(int key);
    //清空控件
    void cleanTmrBar(void);
    //批量开始暂停
    void allBarPlayState(bool bl);
private:
    FlowLayout* m_fLayout;

    QMap<int, TimeNumBar*> m_mapbar;
};


#endif


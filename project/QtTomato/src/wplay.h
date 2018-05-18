//*************************************************************************
//
//
//                       播放器控件
//
//
//*************************************************************************

#ifndef WPLAY_H
#define WPLAY_H

#include "QPublic.h"

namespace Ui
{
    class WPlayer;
}

class CPP_API WLrc : public QLabel
{
    Q_OBJECT
public:
    explicit WLrc(QWidget* parent = 0);
    //开启遮罩，需要指定当前歌词开始与结束之间的时间间隔
    void startLrcMask(qint64 intervalTime);
    //停止遮罩
    void stopLrcMask();
protected:
    void paintEvent(QPaintEvent*);
    //两个鼠标事件处理函数实现了部件的拖动
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    //实现右键菜单来隐藏部件
    void contextMenuEvent(QContextMenuEvent* event);
    private slots:
    //定时器溢出时增加遮罩的宽度，并更新显示
    void timeout();
private:
    QLinearGradient linearGradient;
    QLinearGradient maskLinearGradient;
    QFont font;
    QTimer* timer;
    qreal lrcMaskWidth;
    // 每次歌词遮罩增加的宽度
    qreal lrcMaskWidthInterval;
    QPoint offset;
};

class CPP_API WPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit WPlayer(QWidget* parent = 0);
    virtual ~WPlayer();
protected:
    //void mousePressEvent(QMouseEvent* ev);
    //void mouseReleaseEvent(QMouseEvent* ev);
    //void mouseMoveEvent(QMouseEvent* ev);
    //void mouseDoubleClickEvent(QMouseEvent* ev);
private:
    //鼠标点
    QPoint myPosDisp;
    //记录状态
    bool m_bPressed;
private:
    //界面
    Ui::WPlayer* ui;
    WLrc Lrc;
};



#endif



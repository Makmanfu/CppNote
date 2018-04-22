//*************************************************************************
//
//                       
//
//
//
//*************************************************************************

#ifndef WDRAW_H
#define WDRAW_H

#include "QPublic.h"

//悬浮窗控件
class WSuspension : public QMoveDlg
{
    Q_OBJECT
public:
    //PngType 1抗锯齿 2动画
    WSuspension(int _PngType = 0, QWidget* parent = 0);
    virtual ~WSuspension();
private:
    //鼠标点
    QPoint m_PosDisp;
    //记录状态
    bool m_bPressed;
    //UI图片
    QPixmap m_LogoPixmap;
    //动画logo
    QLabel* m_lablogo;
    QMovie* m_vlogo;
    //托盘
    QSystemTrayIcon* m_trayIcon;
    QMenu* m_MenuTray;
    QAction* Act_Exit, *Act_MainShow, *Act_Suspen, *Act_Tray;
    //------菜单添加窗体-----------------
    QWidget* m_TbnWidget;
    QWidgetAction* m_TbnWidgetAction;
    QLabel* m_labGIF;
    QMovie* m_vGIF;
    //---------------------------------
    //存放绘制方法
    int m_iPngType;
public:
    void supenShow(bool _state = true);
    void setTrayShow(bool _state = true);
    //添加菜单接口
    void menuAddAction(QAction* action);
    //设置UI图片
    void setUIImage(QPixmap pix);
    void setUIImage(QString GIF, QSize _size = QSize(0, 0));
    //主界面事件
    virtual void showMain(void);
protected:
    void paintEvent(QPaintEvent* ev);
    void mouseDoubleClickEvent(QMouseEvent* ev);
    void closeEvent(QCloseEvent* event);
    void contextMenuEvent(QContextMenuEvent* ev);
private slots:
    void Act_SuspenClick(void);
    void Act_TrayClick(void);
    void iconActivated(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::DoubleClick);
public slots:
    //悬浮窗退出事件
    virtual void exitSuspen(void);
};

//win7时钟
class WClock : public WSuspension
{
    Q_OBJECT
public:
    WClock(QWidget* parent = 0);
    virtual ~WClock();
protected:
    void paintEvent(QPaintEvent* event);
};

//雷达扇形扫描控件
class WScanRotate : public QWidget
{
    Q_OBJECT
public:
    WScanRotate(QWidget* parent = 0);
    virtual ~WScanRotate();
private:
    double m_radarWidth, m_radarHeight;          //控件宽高
    double m_lfCenterX, m_lfCenterY;             //控件中心
    int m_currentAngle, m_xlen, m_ylen;          //扫描角度,x/y轴偏移
    QPointF m_PosDisp;
    int m_iSelectType;                           //操作状态
    bool m_blinestate;                           //是否为绘制轨迹状态
    QVector<QPointF> m_vecPT;                    //记录临时轨迹点集
    QVector<QPointF> m_vecflyPT;                 //记录目标点集
    QMap<int, QVector<QPointF> > m_mapPT;        //航迹集合
private:
    QComboBox* m_pCboxSelect;                     //模式选择框
    QComboBox* m_pCboxlines;                      //航迹选择
    QPushButton* m_pBtnadd;                       //添加按钮
    QTimer* m_pTimer;                             //扫描计时器
private:
    //绘制图层
    void paintObj(QPainter& painter);
    //绘制雷达线
    void paintRadarline(QPainter& painter);
protected:
    virtual void paintEvent(QPaintEvent* ev);
    virtual void mousePressEvent(QMouseEvent* msv);
    virtual void mouseReleaseEvent(QMouseEvent* msv);
    virtual void mouseMoveEvent(QMouseEvent* msv);
    //绘制一个半透明、颜色锥形渐变的扇形区域，表示雷达当前扫描的范围
    virtual void paintScanArea(QPainter& painter);
    virtual void radialGradient(QPainter& paint, QPointF& flypt, int centerAlpha);
    virtual void radialGradient(QPainter& paint, float pointX, float pointY, int centerAlpha);
private slots:
    void OnBtnaddClicked();
    void OnComboSelectIndexChanged(int nId);
};

//雷达控件
class WRotate : public QWidget
{
    Q_OBJECT
public:
    WRotate(QWidget* parent = 0);
    virtual ~WRotate();
protected:
    void paintEvent(QPaintEvent*);
private:
    QPixmap bj;
    void drawDistanceMarking(QPainter* painter);
};

//空面板
class QWidgetEmp :public QWidget
{
    Q_OBJECT
public:
    explicit QWidgetEmp(QWidget* parent = 0);
    ~QWidgetEmp();
private:
    //彩蛋控件
    WSuspension* lvshuiling, *mogu;
};

//加速球控件
class NProgreCirWater : public QWidget
{
    Q_OBJECT
public:
    NProgreCirWater(QWidget *parent = 0);
    ~NProgreCirWater();
public:
    //设置进度0-100
    void setWaterProgre(int progre);
    //设置高度
    void setWaterHeight(float progre);
    //设置密度
    void setWaterDensity(float progre);
private:
    //绘制进度字
    void drawWord(QPainter& drawWaterPainter);
private:
    int m_iNowProgre;                   //进度
    float m_fwaterDensity;              //密度
    float m_fwaterHeight;               //水波高度


    QTimer m_TUpGuiimer;
    float m_fwaterRatio;                //水波进度
protected:
    void paintEvent(QPaintEvent *event);
private slots:
    //自动增加  
    void upAutoTime();

};




#endif



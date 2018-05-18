//*************************************************************************
//
//                       简单绘图控件
//
//
//
//*************************************************************************
#ifndef WAXIS_H
#define WAXIS_H

#include "QPublic.h"
#include "waxis_def.h"

//形状接口
class Shape : public QWidget
{
    Q_OBJECT
public:
    enum Category { dCurve, dLine, dRect , dMove };     //曲线,直线,矩形,移动
    explicit Shape(QWidget* parent = 0) : QWidget(parent)
    {
        pen = QPen(QColor(0, 0, 0), 2.0, Qt::SolidLine, Qt::RoundCap);
        setBackgroundRole(QPalette::Base);
        startPt = endPt = mCenter = QPointF(0, 0);
        mGstep = 10;
    };
    virtual ~Shape()
    {
    };
public:
    void setStart(QPointF s)
    {
        startPt = s;
    };
    void setEnd(QPointF e)
    {
        endPt = e;
    };
    QPointF startPoint()
    {
        return startPt;
    };
    QPointF endPoint()
    {
        return endPt;
    };
    //需要具体图形子类实现
    void virtual paint(QPainter& painter) = 0;
    //为了画任意曲线特意留的一个口子
    void virtual addPoint(QPointF point) {};
    void setCenterPtstep(QPointF& Center, int Gstep)
    {
        mCenter = Center;
        mGstep = Gstep;
    }
    //转换坐标
    QPointF toPts(QPointF& pt, QPointF& Center, int Gstep)
    {
        return QPointF(Center.rx() + (pt.rx() * Gstep / 10), Center.ry() - (pt.ry() * Gstep / 10));
    };
    QPointF toPts(QPointF& pt)
    {
        return QPointF(mCenter.rx() + (pt.rx() * mGstep / 10), mCenter.ry() - (pt.ry() * mGstep / 10));
    };
public slots:
    void setPen(const QPen& pen)
    {
        this->pen = pen;
    };
    void setBrush(const QBrush& brush)
    {
        this->brush = brush;
    };
protected:
    QPointF startPt, endPt, mCenter;
    QBrush brush;
    QPen pen;
    int mGstep;
};

//直线
class Line : public Shape
{
public:
    Line() {};
    void paint(QPainter& painter)
    {
        painter.setPen(pen);
        //painter.drawLine(toPts(startPt), toPts(endPt));
        painter.drawLine(startPt, endPt);
    };
};

//曲线
class Curve : public Shape
{
public:
    Curve() {};
    QPointF startPoint()
    {
        return points.at(0);
    }
    void setStart(QPointF point)
    {
        this->addPoint(point);
    }
    QPointF endPoint()
    {
        return points.at(points.size() - 1);
    }
    void setEnd(QPointF point)
    {
        this->addPoint(point);
    }
    void addPoint(QPointF point)
    {
        this->points.append(point);
    };
    void paint(QPainter& painter)
    {
        painter.setPen(pen);
        painter.setBrush(brush);
        //此处是画任意曲线的关键，如果修改为drawPolygon，可以看到鼠标画图是实现了，但在起始和结束点均有一条   直线相连
        painter.drawPolyline(QPolygonF(points));
    };
private:
    //曲线其实就是一堆QPoint的点集合，此处存放，鼠标移动时候将点存入此处
    QVector<QPointF> points;
};

//矩形
class Rect : public Shape
{
public:
    Rect() {};
    void paint(QPainter& painter)
    {
        painter.setPen(pen);
        painter.drawRect(startPt.rx(), startPt.ry(),
                         endPt.rx() - startPt.rx(), endPt.ry() - startPt.ry());
    };
};

//画板
class PadDraw : public QWidget
{
    Q_OBJECT
public:
    PadDraw(QWidget* parent = 0);
    virtual ~PadDraw();
public slots:
    void setCurrentShape(Shape::Category s)
    {
        if (s != currentCategory)
            currentCategory = s;
    }
protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
private:
    QList<Shape*> shapes;
    Shape* shape;
    Shape::Category currentCategory;
    bool isLBtnPressed;
};

//坐标轴系统控件
class CPP_API WAxis : public QWidget
{
    Q_OBJECT
public:
    WAxis(QWidget* parent = 0);
    virtual ~WAxis();
private:
    QPushButton* Btn_Pt0, *Btn_ZoomIn, *Btn_ZoomOut, *Btn_PtD,
        *Btn_PtS;
    QAction* Act_Pt0, *Act_Normal, *Act_Sty,
        *Act_curve, *Act_line, *Act_circle, *Act_Data;
    QMenu* menu;
    //绘制数据
    QList<POSTION> m_listData;
private:
    int m_Width, m_Height, m_BorderLen;          //控件宽高 外边距
    int Gstep;                                   //基线格大小
    double m_CenterX, m_CenterY;                 //坐标原点
    double m_ScaleX, m_ScaleY;                   //刻度范围
    bool isPressed;                              //鼠标状态
    QPointF MousePt, MouseEd;                    //鼠标记录点
    QColor lineColor, smallColor;                //基线颜色
    int drawState;                               //绘画状态(0移动,1直线,2圆)

    QList<Shape*> shapes;
    Shape* shape;
    Shape::Category currentCategory;
private:
    void GuiInit(void);
    void menucreate(void);
    void paintDrawXY(void);
public:
    void setCurrentShape(Shape::Category s);
    //设置颜色风格
    void SetStyColor(QColor bj, QColor linec, QColor linesmall);
    //移动原点位置(基于屏幕中心点移动 pt可负)
    void SetMoveCenterPT(POSTION pt);
    //绘制数据
    virtual void paintDrawData(QPainter& painter);
private slots:
    void OnBtnPt0Clicked();                     //原点
    void OnBtnPtSClicked();
    void OnBtnPtDClicked();
    void OnAct_StyClicked();
    void OnAct_NorClicked();                    //移动
    void OnAct_lineClicked();                   //直线
    void OnAct_curveClicked();                  //曲线
    void OnAct_circleClicked();                 //圆
    void OnAct_DataClicked();                   //绘制数据
    void OnBtnZoomInClicked();
    void OnBtnZoomOutClicked();
protected:
    void contextMenuEvent(QContextMenuEvent* event);
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
};



#endif





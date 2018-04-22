//********************************************************************
//
//                  QT全局工具方法
//
//
//********************************************************************/

#ifndef QPUBLIC_H
#define QPUBLIC_H

#include "QtConfig.h"

//转换字符
QString TCSR(QString str);

//获得点
QPointF getPos(QMouseEvent* event);

//全局工具类
class CommonHelper
{
private:
    static CommonHelper* _instance;
protected:
    CommonHelper(){};
public:
    ~CommonHelper() {};
    static CommonHelper* Instance();
};

//可移动窗口[锁定移动区在桌面内]
class QMoveDlg : public QWidget
{
    Q_OBJECT
public:
    QMoveDlg(QWidget* parent = 0);
private:
    QPoint m_PosDisp;
    bool m_bPressed;
protected:
    virtual void mousePressEvent(QMouseEvent* ev);
    virtual void mouseReleaseEvent(QMouseEvent* ev);
    virtual void mouseMoveEvent(QMouseEvent* ev);
};

//自动排列布局器
class FlowLayout : public QLayout
{
public:
    explicit FlowLayout(QWidget* parent, int margin = -1, int hSpacing = -1, int vSpacing = -1);
    explicit FlowLayout(int margin = -1, int hSpacing = -1, int vSpacing = -1);
    ~FlowLayout();
public:
    void addItem(QLayoutItem* item);
    int horizontalSpacing() const;
    int verticalSpacing() const;
    Qt::Orientations expandingDirections() const;
    bool hasHeightForWidth() const;
    int heightForWidth(int) const;
    int count() const;
    QLayoutItem* itemAt(int index) const;
    QSize minimumSize() const;
    void setGeometry(const QRect& rect);
    QSize sizeHint() const;
    QLayoutItem* takeAt(int index);

private:
    int doLayout(const QRect& rect, bool testOnly) const;
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem*> itemList;
    int m_hSpace;
    int m_vSpace;
};

//UI切换器指挥者类
class UIDirect
{
private:
    QWidget* p;
public:
    UIDirect() : p(NULL)
    {
    };
    UIDirect(QWidget* temp) : p(NULL)
    {
        SetQWidget(temp);
    };
    virtual ~UIDirect()
    {
        Free();
    };
    void SetQWidget(QWidget* temp)
    {
        p = temp;
    };
    void Create(QGridLayout* WGrid)
    {
        //显示
        p->show();
        //添加到布局器
        WGrid->addWidget(p, 0, 0);
    };
    void Free(void)
    {
        if (p != NULL)
        {
            delete p;
            p = NULL;
        }
    };
};

//界面演示框架
class TabGUIBase : public QWidget
{
    Q_OBJECT
public:
    explicit TabGUIBase(QWidget* parent = 0);
    virtual ~TabGUIBase();
public:
    void addcomboxItem(const QString& str);
    virtual void GuiInit(void);
public:
    QComboBox* m_pComBoxPage;              //切换页
    QWidget* m_pWgtShow;                   //通用控件显示面板框架
    QGridLayout* m_pGrid, *m_pGridW;       //通用控件布局器
    UIDirect* m_pPageUI;                   //ui页管理器
    QWidget* m_pCurrUI;                    //当前UI页
    public slots :
    virtual void OnComBoxPageIndexChanged(int nId);
};

//界面工具类
class CUiTool
{
private:
    static CUiTool* _instance;
protected:
    //注意单键不要能实例化
    CUiTool(){};
public:
    ~CUiTool() {};
    static CUiTool* Instance();
public:
    //界面测试
    void TestBox(const QString& str, QWidget* parent = NULL);
    //刷全局Style
    void setStyle(const QString &style);
    //刷QWidget的Style
    void QWidgetSetStyle(QWidget* QW, const QString &style);
    //动态加载MB
    void loaddll(QString dllname, QWidget* dlg);
    void loaddll(QString dllname, QWidget* dlg, QGridLayout* gl);
    void loaddll(QString dllname, char* funname);
    //程序唯一性运行
    bool RunCheckOnly(QString appname);
    //QT获取目录下扩展名的文件表
    void GetFilelist(QStringList& files, QString path, QString extname);
    //初始化按键
    void InitPushButton(QPushButton* button, QString strPmpNormal, QString strPmpHover, QString strPmpPressed, QString strPmpDisabled);
    //初始化按键
    void InitPushButton(QPushButton* button, QString strPmp, QString strText, int nWidth, int nHeight, int nFontSize, int nColor);
};













#endif

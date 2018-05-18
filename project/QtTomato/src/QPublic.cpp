
#include "stdafx.h"
#include "QPublic.h"


QString TCSR(QString str)
{
#if (QT_VERSION > QT_VERSION_CHECK(4, 8, 7))
    return "";
#else
    return QString::fromLocal8Bit(str.toAscii());
#endif
}

QPointF getPos(QMouseEvent* event)
{
#if (QT_VERSION > QT_VERSION_CHECK(4, 8, 7))
    return event->localPos();
#else
    return event->posF();
#endif
}


CommonHelper* CommonHelper::_instance = 0;

CommonHelper* CommonHelper::Instance()
{
    if (NULL == _instance)
        _instance = new CommonHelper();
    return _instance;
}



QMoveDlg::QMoveDlg(QWidget* parent /*= 0*/) : QWidget(parent)
{
}

void QMoveDlg::mousePressEvent(QMouseEvent* ev)
{
    m_PosDisp = ev->globalPos() - pos();           //记下当前点击的点坐标
    //mPosDisp = ev->pos();
    m_bPressed = true;
}

void QMoveDlg::mouseReleaseEvent(QMouseEvent* ev)
{
    ev->accept();
    m_bPressed = false;
}

void QMoveDlg::mouseMoveEvent(QMouseEvent* ev)
{
    if (m_bPressed)
    {
        //方法1：达到移动窗口作用 同时限制移动区域
        QPoint point(ev->globalPos() - m_PosDisp);
        QRect windowRect;
        //获得父窗口
        if (nullptr != this->parent())
            windowRect = QRect(((QWidget*)(this->parent()))->geometry());
        else
            windowRect = QRect(QApplication::desktop()->availableGeometry());
        QRect widgetRect(this->frameGeometry());
        int y = windowRect.bottomRight().y() - widgetRect.height();
        int x = windowRect.bottomRight().x() - widgetRect.width();
        //以下是防止窗口拖出可见范围外
        //左,右,上,下
        if (point.x() <= 0) point = QPoint(0, point.y());
        if (point.y() >= y && widgetRect.topLeft().y() >= y) point = QPoint(point.x(), y);
        if (point.y() <= 0) point = QPoint(point.x(), 0);
        if (point.x() >= x && widgetRect.topLeft().x() >= x) point = QPoint(x, point.y());
        move(point);
        //方法2：
        //可以通过判断QRect windowRect是否包含（contains） QRect widgetRect 再移动
        //这里没有给出代码
    }
}


FlowLayout::FlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    QLayoutItem* item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem* item)
{
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0)
        return m_hSpace;
    else
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0)
        return m_vSpace;
    else
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem* FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem* FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    else
        return 0;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return 0;
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void FlowLayout::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem* item;
    foreach(item, itemList)
        size = size.expandedTo(item->minimumSize());
    size += QSize(2 * margin(), 2 * margin());
    return size;
}

int FlowLayout::doLayout(const QRect& rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;
    QLayoutItem* item;
    foreach(item, itemList)
    {
        QWidget* wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
            spaceX = wid->style()->layoutSpacing(
            QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        int spaceY = verticalSpacing();
        if (spaceY == -1)
            spaceY = wid->style()->layoutSpacing(
            QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }
        if (!testOnly)
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject* parent = this->parent();
    if (!parent)
        return -1;
    else if (parent->isWidgetType())
    {
        QWidget* pw = static_cast<QWidget*>(parent);
        return pw->style()->pixelMetric(pm, 0, pw);
    }
    else
        return static_cast<QLayout*>(parent)->spacing();
}


TabGUIBase::TabGUIBase(QWidget* parent) : QWidget(parent)
{
    //设置字体
    QFont ft = QFont(QString::fromLocal8Bit("微软雅黑"));
    ft.setPointSize(8);
    this->setFont(ft);
    // 加载QSS样式
    //CUiTool::Instance()->QWidgetSetStyle(this, "style.qss");
    //CommonHelper::Instance()->setStyle("style.qss");
    //锁定控件最小值
    this->setMinimumSize(600, 600);
    //setWindowOpacity(0.95);
    if (this->objectName().isEmpty())
        this->setObjectName(QString::fromUtf8("TabGUIBase"));
    this->resize(373, 146);
    this->setContextMenuPolicy(Qt::DefaultContextMenu);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/Res/tray.ico"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    this->setWindowTitle(QString::fromLocal8Bit("TabGUIBase"));
    GuiInit();
}

TabGUIBase::~TabGUIBase()
{
    //改成未选择就行了
    OnComBoxPageIndexChanged(0);
    if (m_pGrid != NULL)
    {
        delete m_pGrid;
        m_pGrid = NULL;
    }
}

void TabGUIBase::addcomboxItem(const QString& str)
{
    m_pComBoxPage->insertItem(m_pComBoxPage->count(), str);
}

void TabGUIBase::GuiInit(void)
{
    //创建启动器
    m_pPageUI = new UIDirect();
    m_pWgtShow = new QWidget(this);
    m_pWgtShow->move(0, 0);
    m_pGrid = new QGridLayout;     //设置布局哦
    m_pGrid->addWidget(m_pWgtShow, 0, 0);
    m_pGrid->setMargin(0);
    this->setLayout(m_pGrid);
    m_pGridW = new QGridLayout;
    m_pGridW->setMargin(0);
    m_pWgtShow->setLayout(m_pGridW);
    m_pComBoxPage = new QComboBox(this);
    m_pComBoxPage->setFixedSize(100, 25);
    m_pComBoxPage->setWindowFlags(Qt::WindowStaysOnTopHint);
    m_pComBoxPage->move(10, 10);
    m_pComBoxPage->setEditable(false);                  //屏蔽编辑
    m_pComBoxPage->setMaxCount(15);                     //超过15个出现滚动条
    connect(m_pComBoxPage, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComBoxPageIndexChanged(int)));
    m_pComBoxPage->insertItem(0, QString::fromLocal8Bit("NULL"));
    //addcomboxItem(QString::fromLocal8Bit("QWidget"));
}

void TabGUIBase::setHideComBox(bool _bl)
{
    this->m_pComBoxPage->setVisible(_bl);
}

void TabGUIBase::OnComBoxPageIndexChanged(int nId)
{
    m_pPageUI->Free();
    switch (nId)
    {
    case 0:
    default:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    case 1:
        m_pCurrUI = new QWidget(m_pWgtShow);
        break;
    }
    m_pCurrUI->setParent(m_pWgtShow);
    m_pPageUI->SetQWidget(m_pCurrUI);
    m_pPageUI->Create(m_pGridW);
}



CUiTool* CUiTool::_instance = 0;

CUiTool* CUiTool::Instance()
{
    if (NULL == _instance)
        _instance = new CUiTool();
    return _instance;
}

void CUiTool::TestBox(const QString& str, QWidget* parent)
{
    //QMessageBox::about(parent, QString::fromLocal8Bit("TEST"), QString::fromLocal8Bit("here it is!"));
    QMessageBox::about(parent, QString::fromLocal8Bit("TEST"), str);
}

void CUiTool::setStyle(const QString& style)
{
    QFile qss(style);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

void CUiTool::QWidgetSetStyle(QWidget* QW, const QString& style)
{
    // 加载QSS样式
    QFile qss("style.qss");
    qss.open(QFile::ReadOnly);
    QW->setStyleSheet(qss.readAll());
    qss.close();
}

void CUiTool::loaddll(QString dllname, QWidget* dlg)
{
    typedef void(*myfun)(QWidget*);       // 定义导出函数类型
    QLibrary hdll(dllname);
    if (hdll.load())
    {
        //用resolve来解析fun1函数
        myfun fun = (myfun)hdll.resolve("ShowWidget");
        //解析成功则进行运算并提示相关信息
        if (fun) fun(dlg);
    }
}

void CUiTool::loaddll(QString dllname, QWidget* dlg, QGridLayout* gl)
{
    typedef void(*myfun)(QWidget*, QGridLayout*);        // 定义导出函数类型
    QLibrary hdll(dllname);
    if (hdll.load())
    {
        //用resolve来解析fun1函数
        myfun fun = (myfun)hdll.resolve("ShowWidgetGL");
        //解析成功则进行运算并提示相关信息
        if (fun) fun(dlg, gl);
    }
}

void CUiTool::loaddll(QString dllname, char* funname)
{
    typedef void(*myfun)(void);      // 定义导出函数类型
    QLibrary hdll(dllname);
    if (hdll.load())
    {
        //用resolve来解析fun1函数
        myfun fun = (myfun)hdll.resolve(funname);
        //解析成功则进行运算并提示相关信息
        if (fun) fun();
    }
}

bool CUiTool::RunCheckOnly(QString appname)
{
    // 确保只运行一次
    QSystemSemaphore sema("QtJAMKey", 1, QSystemSemaphore::Open);
    // 在临界区操作共享内存   SharedMemory
    sema.acquire();
    // 全局对象名
    static QSharedMemory mem(appname);
    // 如果全局对象以存在则退出
    if (!mem.create(1))
    {
        sema.release();// 如果是 Unix 系统，会自动释放。
        return false;
    }
    sema.release();// 临界区
    return true;
}

void CUiTool::GetFilelist(QStringList& files, QString path, QString extname)
{
    //判断路径是否存在
    QDir dir(path);
    if (!dir.exists())
        return;
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    int file_count = list.count();
    if (file_count <= 0)
        return;
    for (int i = 0; i < file_count; i++)
    {
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();
        if (QString::compare(suffix, extname, Qt::CaseInsensitive) == 0)
        {
            QString absolute_file_path = file_info.absoluteFilePath();
            files.append(absolute_file_path);
            //qDebug() << absolute_file_path;
        }
    }
}

//内部使用
QString GetResPath()
{
#ifdef RES_STATIC
    return ":/images/Resources/";
#else
    return "./images/";
#endif
}

void CUiTool::InitPushButton(QPushButton* button, QString strPmpNormal, QString strPmpHover, QString strPmpPressed, QString strPmpDisabled)
{
    button->setFocusPolicy(Qt::NoFocus);
    QPixmap pmp = QPixmap(GetResPath() + strPmpNormal);
    button->setFixedSize(pmp.size());
    QString strStyleSheet = QString(" QPushButton { border: 0pt solid #abc; border-image: url(" + GetResPath() + "%1);}").arg(strPmpNormal);
    if (strPmpHover.length() > 0)
        strStyleSheet += QString(" QPushButton:hover { border-image: url(" + GetResPath() + "%1);}").arg(strPmpHover);
    if (strPmpPressed.length() > 0)
        strStyleSheet += QString(" QPushButton:pressed { border-image: url(" + GetResPath() + "%1);}").arg(strPmpPressed);
    if (strPmpDisabled.length() > 0)
        strStyleSheet += QString(" QPushButton:disabled { border-image: url(" + GetResPath() + "%1);}").arg(strPmpDisabled);
    button->setStyleSheet(strStyleSheet);
}

void CUiTool::InitPushButton(QPushButton* button, QString strPmp, QString strText, int nWidth, int nHeight, int nFontSize, int nColor)
{
    button->setFocusPolicy(Qt::NoFocus);
    button->setFixedSize(nWidth, nHeight);
    QPixmap pmp = QPixmap(GetResPath() + strPmp);
    QString strStyleSheet = QString(" QPushButton { font-size:%1px; font-weight:normal; color:rgb(%2, %3, %4); }").arg(nFontSize).arg(nColor >> 16).arg((nColor >> 8) & 0xFF).arg(nColor & 0xFF);
    strStyleSheet += " QPushButton { background-repeat: no repeat; background-position: left; border-radius: 5px; border: 0pt solid #abc;}";
    strStyleSheet += " QPushButton:hover { border: 2pt solid #abc; color:rgb(0, 0, 0); }";
    strStyleSheet += " QPushButton:pressed { border: 0pt solid #abc; color:rgb(0, 0, 0); }";
    button->setStyleSheet(strStyleSheet);
    //  QFont Font = button->font();
    //  Font.setPixelSize(nFontSize);
    //  button->setFont(Font);
    button->setIcon(QIcon(pmp));
    button->setText(strText);
    button->setFlat(true);
}


CMylay::CMylay(QWidget *parent) :QGridLayout(parent)
{
    this->setParent(parent);
    this->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);

    //// 下面代码将被优化掉
    //m_mapWidget = new MapWidget(ui->wdgmap);
    //m_maplay = new QGridLayout(ui->wdgmap);
    //m_maplay->setSpacing(0);
    //m_maplay->setContentsMargins(0, 0, 0, 0);
    //m_maplay->addWidget(m_mapWidget);

    //调用如下
    //m_mapWidget = new SeaMapWidget(ui->wdgmap);
    //m_maplay = new CMylay(ui->wdgmap);
    //m_maplay->setWgt(m_mapWidget);
}

CMylay::~CMylay()
{
}

void CMylay::setWgt(QWidget *wgt)
{
    this->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
    this->addWidget(wgt);
}










#include "stdafx.h"
#include "WndDB.h"
#include <queue>



ProgressBarDelegate::ProgressBarDelegate(int cols, QObject* parent) : QItemDelegate(parent), col(cols)
{
}

void ProgressBarDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() == col)
    {
        int value = index.model()->data(index).toInt();
        QStyleOptionProgressBarV2 progressBarOption;
        progressBarOption.rect = option.rect.adjusted(4, 4, -4, -4);
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.textAlignment = Qt::AlignRight;
        progressBarOption.textVisible = true;
        progressBarOption.progress = value;
        progressBarOption.text = tr("%1%").arg(progressBarOption.progress);
        painter->save();
        if (option.state & QStyle::State_Selected)
        {
            painter->fillRect(option.rect, option.palette.highlight());
            painter->setBrush(option.palette.highlightedText());
        }
        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
        painter->restore();
    }
    else
        return QItemDelegate::paint(painter, option, index);
}




TableModel::TableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

TableModel::~TableModel()
{
}

int TableModel::rowCount(const QModelIndex& parent) const
{
    return m_data.size();
}

int TableModel::columnCount(const QModelIndex& parent) const
{
    return m_HorizontalHeader.count();
}

QVariant TableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole)
    {
        int ncol = index.column();
        int nrow = index.row();
        QStringList values = m_data.at(nrow);
        if (values.size() > ncol)
            return values.at(ncol);
        else
            return QVariant();
    }
    return QVariant();
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    Qt::ItemFlags flag = QAbstractItemModel::flags(index);
    // flag|=Qt::ItemIsEditable
    // 设置单元格可编辑,此处注释,单元格无法被编辑
    return flag;
}

void TableModel::setHorizontalHeader(const QStringList& headers)
{
    m_HorizontalHeader = headers;
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return m_HorizontalHeader.at(section);
    return QAbstractTableModel::headerData(section, orientation, role);
}

void TableModel::setData(const QVector<QStringList>& data)
{
    m_data = data;
}




TableView::TableView(QWidget* parent) : QTableView(parent)
{
    m_model = new TableModel();
    Act_add = new QAction(QString::fromLocal8Bit("添加"), this);
    Act_addroot = new QAction(QString::fromLocal8Bit("添加root"), this);
    menutb = new QMenu(this);
    menutb->addAction(Act_add);
    menuroot = new QMenu(this);
    menuroot->addAction(Act_addroot);
    //表的设置
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);       //不能编辑
    this->setSelectionMode(SelectionMode::SingleSelection);         //单选
    this->setSelectionBehavior(SelectionBehavior::SelectRows);      //整行选中的方式
    //this->verticalHeader()->setVisible(false);      //隐藏列表头
    //this->horizontalHeader()->setVisible(false);    //隐藏行表头
    this->setModel(m_model);
    iniData();
}

TableView::~TableView()
{
    delete m_model;
}

void TableView::SetHead(QStringList& headtxt)
{
    m_model->setHorizontalHeader(headtxt);
}

void TableView::SetData(QVector<QStringList>& datas)
{
    m_model->setData(datas);
    //设置第一列宽
    setColumnWidth(0, 60);
    ProgressBarDelegate* m_progressBarDelegate = new ProgressBarDelegate(2, this);
    this->setItemDelegate(m_progressBarDelegate);
}

void TableView::iniData()
{
    //测试数据
    SetHead(QStringList() << "Id" << "Progress");
    QVector<QStringList> data;
    data.append(QStringList() << "1" << "22");
    data.append(QStringList() << "2" << "32");
    data.append(QStringList() << "3" << "2");
    data.append(QStringList() << "4" << "80");
    data.append(QStringList() << "5" << "40");
    m_model->setData(data);
}

void TableView::contextMenuEvent(QContextMenuEvent* ev)
{
    QCursor cur = this->cursor();
    index0 = this->indexAt(ev->pos());    //mztree->currentIndex();
    qDebug() << index0.row();
    if (index0.row() < 0)
    {
        if (menuroot != NULL)
            menuroot->exec(cur.pos());
    }
    else
    {
        if (menutb != NULL)
            menutb->exec(cur.pos());
    }
    ev->accept();
    //qDebug() << index0.row();
    //qDebug() << model->data(index0).toString();
    //qDebug() << index0.data(0).toString();
}

void TableView::mousePressEvent(QMouseEvent* event)
{
    QTableView::mousePressEvent(event);
    index0 = this->indexAt(event->pos());
    //currentItem = m_model->itemFromIndex(index0);
    //qDebug() << m_model->rowCount(index0);
    //qDebug() << index0.data(0).toString();
    event->accept();
}

void TableView::SetMenu(QMenu* menu, int roottype/*=0*/)
{
    if (0 == roottype)
        menutb = menu;
    else
        menuroot = menu;
}




TreeView::TreeView(QWidget* _parent) : QTreeView(_parent)
{
    setMenuInit();
    setTableInit();
    //假数据
    map<QString, QVector<QString> > testdat;
    QVector<QString> objtest;
    for (int i = 0; i < 5; ++i)
        objtest.push_back(QString::fromLocal8Bit("飞机"));
    testdat.insert(map<QString, QVector<QString> >::value_type(QString::fromLocal8Bit("航母系列"), objtest));
    testdat.insert(map<QString, QVector<QString> >::value_type(QString::fromLocal8Bit("雷达系列"), objtest));
    testdat.insert(map<QString, QVector<QString> >::value_type(QString::fromLocal8Bit("飞机系列"), objtest));
    testdat.insert(map<QString, QVector<QString> >::value_type(QString::fromLocal8Bit("其他"), objtest));
    //加载数据
    LoadTableData(testdat);
    //全部展开
    //this->expandAll();
}

TreeView::~TreeView()
{
}

QList<QStandardItem*> TreeView::returnTheItems()
{
    return model->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
}

void TreeView::iterateOverItems()
{
    QList<QStandardItem*> list = returnTheItems();
    foreach (QStandardItem* item, list)
        qDebug() << item->text();
}

void TreeView::mousePressEvent(QMouseEvent* event)
{
    QTreeView::mousePressEvent(event);
    index0 = this->indexAt(event->pos());
    currentItem = model->itemFromIndex(index0);
    qDebug() << model->rowCount(index0);
    qDebug() << model->data(index0, Qt::EditRole).toString();
    //qDebug() << index0.data(0).toString();
    event->accept();
}

void TreeView::LoadTableData(map<QString, QVector<QString> >& datas_)
{
    if (!datas_.empty())
    {
        model->setRowCount(datas_.size());
        //设置一列显示
        model->setColumnCount(1);
        //遍历树
        int treeid(0);
        for (map<QString, QVector<QString> >::const_iterator mapit = datas_.begin();
             mapit != datas_.end(); ++mapit, ++treeid)
        {
            QStandardItem* ItTmp = new QStandardItem(QString(mapit->first));
            model->setItem(treeid, 0, ItTmp);
            for (QVector<QString>::const_iterator vecit = mapit->second.begin();
                 vecit != mapit->second.end(); ++vecit)
            {
                QStandardItem* ItTmp2 = new QStandardItem(QString(*vecit));
                ItTmp->appendRow(ItTmp2);
                QStandardItem* ItTmp3 = new QStandardItem(QString("asd"));
                ItTmp2->appendRow(ItTmp3);
            }
        }
    }
    this->setModel(model);
}

void TreeView::LoadlistData(QVector<QString>& datas_)
{
    if (!datas_.empty())
    {
        model->setRowCount(datas_.size());
        //设置一列显示
        model->setColumnCount(1);
        //遍历树
        int treeid(0);
        for (QVector<QString>::const_iterator it = datas_.begin();
             it != datas_.end(); ++it, ++treeid)
        {
            QStandardItem* ItTmp = new QStandardItem(QString(*it));
            model->setItem(treeid, 0, ItTmp);
        }
    }
    this->setModel(model);
    return;
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    model->setRowCount(4);
    model->setColumnCount(1);
    //model->setHeaderData(1, Qt::Horizontal, tr("Details"));   //设置头显示
    QStandardItem* item1 = new QStandardItem(QString::fromLocal8Bit("航母系列"));
    item1->setIcon(QIcon("gparted.png"));
    QStandardItem* item2 = new QStandardItem(QString::fromLocal8Bit("雷达"));
    item2->setIcon(QIcon("gparted.png"));
    QStandardItem* item3 = new QStandardItem(QString::fromLocal8Bit("飞机"));
    item3->setIcon(QIcon("gparted.png"));
    QStandardItem* item4 = new QStandardItem(QString::fromLocal8Bit("其他"));
    item4->setIcon(QIcon("gparted.png"));
    model->setItem(0, 0, item1);
    model->setItem(1, 0, item2);
    model->setItem(2, 0, item3);
    model->setItem(3, 0, item4);
    QStandardItem* item5 = new QStandardItem(QString::fromLocal8Bit("敌机"));
    item4->appendRow(item5);
    //QModelIndex parent;
    //for (int i = 0; i < 4; ++i)
    //{
    //    parent = model->index(0, 0, parent);
    //    model->insertRows(0, 1, parent);
    //    model->insertColumns(0, 1, parent);
    //    QModelIndex index = model->index(0, 0, parent);
    //    model->setData(index, QString::fromLocal8Bit("航母%i").arg(i));
    //}
    this->setModel(model);
}

void TreeView::setMenuInit(void)
{
    Act_add = new QAction(QString::fromLocal8Bit("添加子节点"), this);
    connect(Act_add, SIGNAL(triggered()), this, SLOT(OnAct_addClicked()));
    Act_edit = new QAction(QString::fromLocal8Bit("编辑节点"), this);
    connect(Act_edit, SIGNAL(triggered()), this, SLOT(OnAct_editClicked()));
    Act_del = new QAction(QString::fromLocal8Bit("删除节点"), this);
    connect(Act_del, SIGNAL(triggered()), this, SLOT(OnAct_delClicked()));
    Act_xml = new QAction(QString::fromLocal8Bit("导入文件"), this);
    connect(Act_xml, SIGNAL(triggered()), this, SLOT(OnAct_xmlClicked()));
    Act_xmlout = new QAction(QString::fromLocal8Bit("导出文件"), this);
    connect(Act_xmlout, SIGNAL(triggered()), this, SLOT(OnAct_xmloutClicked()));
    menutb = new QMenu(this);
    menutb->addAction(Act_add);
    menutb->addAction(Act_edit);
    menutb->addAction(Act_del);
    menutb->addSeparator();
    menutb->addAction(Act_xml);
    menutb->addAction(Act_xmlout);
    menutb->addSeparator();
    Act_addroot = new QAction(QString::fromLocal8Bit("添加跟节点"), this);
    connect(Act_addroot, SIGNAL(triggered()), this, SLOT(OnAct_addrootClicked()));
    menuroot = new QMenu(this);
    menuroot->addAction(Act_addroot);
    menuroot->addSeparator();
    menuroot->addAction(Act_xml);
    menuroot->addAction(Act_xmlout);
}

void TreeView::setTableInit(void)
{
    //treeview标题做些处理
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->header()->setResizeMode(QHeaderView::ResizeToContents);
    this->header()->setHidden(true);
    this->setStyleSheet(QString(" QWidget {font-size:18px; font-weight:normal; color:rgb(0, 100, 0);} "));
    //this->iterateOverItems();
    //默认数据
    model = new QStandardItemModel();
    model->setRowCount(0);
    model->setColumnCount(1);
    model->setHeaderData(0, Qt::Horizontal, QString::fromLocal8Bit(""));       //设置头显示
    //设置默认数据
    this->setModel(model);
}

void TreeView::contextMenuEvent(QContextMenuEvent* ev)
{
    QCursor cur = this->cursor();
    index0 = this->indexAt(ev->pos());    //mztree->currentIndex();
    qDebug() << index0.row();
    if (index0.row() < 0)
        menuroot->exec(cur.pos());
    else
        menutb->exec(cur.pos());
    ev->accept();
    //qDebug() << index0.row();
    //qDebug() << model->data(index0).toString();
    //qDebug() << index0.data(0).toString();
}

void TreeView::OnAct_addClicked()
{
    bool isOK(false);
    QString text = QInputDialog::getText(this, QString::fromLocal8Bit("添加节点"), QString::fromLocal8Bit("请输入节点名:"),
                                         QLineEdit::Normal, currentItem->text(), &isOK);
    if (isOK)
    {
        QStandardItem* item1 = new QStandardItem(text);
        currentItem->insertRow(0, item1);
    }
}

void TreeView::OnAct_editClicked()
{
    bool isOK(false);
    QString text = QInputDialog::getText(this, QString::fromLocal8Bit(" 编辑节点"), QString::fromLocal8Bit("请输入节点名:"),
                                         QLineEdit::Normal, currentItem->text(), &isOK);
    if (isOK)
        currentItem->setText(text);
}

void TreeView::OnAct_delClicked()
{
    if (index0.row() < 0)
        return;
    //移除选中行
    model->removeRow(index0.row(), index0.parent());
}

void TreeView::OnAct_addrootClicked()
{
    bool isOK(false);
    QString text = QInputDialog::getText(this, QString::fromLocal8Bit("添加跟节点"),
                                         QString::fromLocal8Bit("请输入跟节点名:"),
                                         QLineEdit::Normal, "root", &isOK);
    if (isOK)
    {
        QStandardItem* item1 = new QStandardItem(text);
        model->setItem(model->rowCount(), 0, item1);
    }
}

void TreeView::OnAct_xmlClicked()
{
    QString XMLfile = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开对话框"), "C:", tr("*.XML;*.JOSN"));
}

void TreeView::OnAct_xmloutClicked()
{
    QString XMLfile = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("保存对话框"), "C:", tr("*.XML;*.JOSN"));
}

void TreeView::printTree(void)
{
    // 获取模型的根项（Root Item），根项是不可见的
    QStandardItem* parentItem = model->invisibleRootItem();
    QModelIndex indexroot = currentIndex();
    qDebug() << model->rowCount(index0);
    qDebug() << model->data(index0, Qt::EditRole).toString();
    queue<QModelIndex> aIndex;
    aIndex.push(currentIndex());    //设置root
    while (aIndex.size() > 0)
    {
        for (int i = 0; i < model->rowCount(aIndex.front()); ++i)
        {
        }
        aIndex.pop();
    }
}



WndDBPage::WndDBPage(QWidget* _parent) : QWidget(_parent)
    , m_tree(NULL)
    , m_table(NULL)
    , m_Text(NULL)
{
    m_tree = new TreeView(this);
    m_table = new TableView(this);
    m_Text = new QPlainTextEdit(this);
    verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(6);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->addWidget(m_table);
    verticalLayout->addWidget(m_Text);
    verticalLayout->setStretch(0, 2);
    verticalLayout->setStretch(1, 3);
    horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->addWidget(m_tree);
    horizontalLayout->addLayout(verticalLayout);
    horizontalLayout->setStretch(0, 1);
    horizontalLayout->setStretch(1, 3);
    //造标题
    m_table->SetHead(QStringList() << TCSR("姓名") << TCSR("性别") << TCSR("年龄"));
    //造假数据
    TeatDatas.append(QStringList() << TCSR("张三") << TCSR("男") << "20");
    TeatDatas.append(QStringList() << TCSR("李四") << TCSR("女") << "18");
    TeatDatas.append(QStringList() << TCSR("王五") << TCSR("人妖") << "28");
    m_table->SetData(TeatDatas);
    //设置菜单方法
    Act_add = new QAction(QString::fromLocal8Bit("n添加"), this);
    Act_addroot = new QAction(QString::fromLocal8Bit("n添加root"), this);
    menutb = new QMenu(this);
    menutb->addAction(Act_add);
    menuroot = new QMenu(this);
    menuroot->addAction(Act_addroot);
    m_table->SetMenu(menutb);
    m_table->SetMenu(menuroot, 1);
}

WndDBPage::~WndDBPage()
{
}




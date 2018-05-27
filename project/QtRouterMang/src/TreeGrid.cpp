


#include "TreeGrid.h"


GridLay::GridLay(QWidget* parent) : QGridLayout(parent)
{
    this->setSpacing(0);
    this->setContentsMargins(0, 0, 0, 0);
}

GridLay::~GridLay()
{
}




TreeGrid::TreeGrid(QWidget* _parent) : QTreeView(_parent)
    , selectid(0)
{
    //默认数据
    model = new QStandardItemModel();
    model->setRowCount(0);
    model->setColumnCount(1);
    model->setHeaderData(0, Qt::Horizontal, TCSR(""));       //设置头显示
    setTableHead();
    //假数据
    map<QString, QVector<QString> > testdat;
    QVector<QString> objtest;
    for (int i = 0; i < 5; ++i)
        objtest.push_back(TCSR("飞机"));
    testdat.insert(map<QString, QVector<QString> >::value_type(TCSR("航母系列"), objtest));
    testdat.insert(map<QString, QVector<QString> >::value_type(TCSR("雷达系列"), objtest));
    testdat.insert(map<QString, QVector<QString> >::value_type(TCSR("飞机系列"), objtest));
    testdat.insert(map<QString, QVector<QString> >::value_type(TCSR("其他"), objtest));
    //加载数据
    //LoadTableData(testdat);
}

TreeGrid::~TreeGrid()
{
}

void TreeGrid::ItemAdd(int _id, QString _name)
{
    QStandardItem* item1 = new QStandardItem(_name);
    model->setItem(model->rowCount(), 0, item1);
}

void TreeGrid::ItemAdd(QString _name)
{
    QStandardItem* item1 = new QStandardItem(_name);
    model->setItem(model->rowCount(), 0, item1);
}

void TreeGrid::ItemEdit(int _id, QString _name)
{
    QStandardItem* item1 = new QStandardItem(_name);
    //将修改值覆盖到选中ID值
    model->setItem(selectid, 0, item1);
}

void TreeGrid::ItemDel(int _id)
{
    //移除行
    model->removeRow(_id);
}

QList<QStandardItem*> TreeGrid::returnTheItems()
{
    return model->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
}

void TreeGrid::iterateOverItems()
{
    QList<QStandardItem*> list = returnTheItems();
    foreach (QStandardItem* item, list)
        qDebug() << item->text();
}

void TreeGrid::mousePressEvent(QMouseEvent* event)
{
    QTreeView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        index0 = currentIndex();
        selectid = index0.row();
        //model->removeRow(index0.row());
        qDebug() << index0.data().toString();
    }
    //event->accept();
}

void TreeGrid::mouseMoveEvent(QMouseEvent* event)
{
    event->accept();
}

void TreeGrid::LoadTableData(map<QString, QVector<QString> >& datas_)
{
    if (!datas_.empty())
    {
        model->setRowCount(datas_.size());
        //设置一列显示
        model->setColumnCount(1);
        //设置头显示
        //model->setHeaderData(0, Qt::Horizontal, TCSR("兵力布局"));
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
            }
        }
    }
    this->setModel(model);
}

void TreeGrid::LoadlistData(QVector<QString>& datas_)
{
    if (!datas_.empty())
    {
        model->setRowCount(datas_.size());
        //设置一列显示
        model->setColumnCount(1);
        //设置头显示
        //model->setHeaderData(0, Qt::Horizontal, TCSR("兵力布局"));
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
    //model = new QStandardItemModel(4,1);
    model->setRowCount(4);
    model->setColumnCount(1);
    //model->setHeaderData(1, Qt::Horizontal, tr("Details"));
    QStandardItem* item1 = new QStandardItem(TCSR("航母系列"));
    item1->setIcon(QIcon("gparted.png"));
    QStandardItem* item2 = new QStandardItem(TCSR("雷达"));
    item2->setIcon(QIcon("gparted.png"));
    QStandardItem* item3 = new QStandardItem(TCSR("飞机"));
    item3->setIcon(QIcon("gparted.png"));
    QStandardItem* item4 = new QStandardItem(TCSR("其他"));
    item4->setIcon(QIcon("gparted.png"));
    model->setItem(0, 0, item1);
    model->setItem(1, 0, item2);
    model->setItem(2, 0, item3);
    model->setItem(3, 0, item4);
    QStandardItem* item5 = new QStandardItem(TCSR("敌机"));
    item4->appendRow(item5);
    //QModelIndex parent;
    //for (int i = 0; i < 4; ++i)
    //{
    //    parent = model->index(0, 0, parent);
    //    model->insertRows(0, 1, parent);
    //    model->insertColumns(0, 1, parent);
    //    QModelIndex index = model->index(0, 0, parent);
    //    model->setData(index, TCSR("航母%i").arg(i));
    //}
    this->setModel(model);
}

void TreeGrid::setTableHead(void)
{
    //treeview标题做些处理
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->header()->setResizeMode(QHeaderView::ResizeToContents);
    this->header()->setHidden(true);
    this->iterateOverItems();
}





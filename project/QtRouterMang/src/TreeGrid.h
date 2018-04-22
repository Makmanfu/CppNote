
#ifndef TreeGrid_H_
#define TreeGrid_H_

#include "stdafx.h"
#include <iostream>
#include <map>
using namespace std;

//布局器
class GridLay : public QGridLayout
{
public:
    GridLay(QWidget* parent);
    virtual ~GridLay();
};

class TreeGrid : public QTreeView
{
public:
    TreeGrid(QWidget* _parent = 0);
    ~TreeGrid();
public:
    //添加对象
    void ItemAdd(QString _name);
    void ItemAdd(int _id, QString _name);
    //编辑对象
    void ItemEdit(int _id, QString _name);
    //删除对象
    void ItemDel(int _id);
    //树控件加载树结构数据
    void LoadTableData(map<QString, QVector<QString> >& datas_);
    void LoadlistData(QVector<QString>& datas_);
    //设置头显示
    void setTableHead(void);
private:
    void iterateOverItems();
    QList<QStandardItem*> returnTheItems();
public:
    int selectid;
private:
    QStandardItemModel* model;
    QModelIndex index0;
protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
};




#endif

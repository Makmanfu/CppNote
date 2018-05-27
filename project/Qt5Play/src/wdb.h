﻿//*************************************************************************
//
//                       数据显示控件 
//
//
//
//*************************************************************************

#ifndef WDB_H

#define WDB_H

#include "QPublic.h"

//列表中处理进度条
class ProgressBarDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ProgressBarDelegate(int cols, QObject* parent = 0);
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
private:
    int col;
};

//数据模块
class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject* parent = 0);
    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;
    void setHorizontalHeader(const QStringList& headers);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    void setData(const QVector<QStringList>& data);
    QVector<QStringList>& DataVector()
    {
        return m_data;
    }
    ~TableModel(void);
private:
    QStringList m_HorizontalHeader;
    QVector<QStringList> m_data;
};

//列表控件使用
class TableView : public QTableView
{
    Q_OBJECT
public:
    explicit TableView(QWidget* parent = 0);
    ~TableView();
private:
    QModelIndex index0;                 //当前选中id
    QStandardItem* currentItem;         //当前选中项
    TableModel* m_model;                //数据model
    QMenu* menutb, *menuroot;
    QAction* Act_add, *Act_addroot, *Act_edt, *Act_del;
public:
    //显示数据
    void SetData(QVector<QStringList>& datas);
    //设置标题
    void SetHead(QStringList& headtxt);
    //默认测试
    void iniData();
    //设置菜单
    void SetMenu(QMenu* menu, int roottype = 0);
    //添加数据
    void AddData(void);
    //编辑数据
    void EdtData(void);
    //删除数据
    void DelData(void);
public:
    QPlainTextEdit* pTextEdit;
    //测试打印
    void PrintText(const QString& txt);
    void PrintText(QString& txt, QPlainTextEdit* TextEdit);
private:
    TableModel* tableModel()
    {
        return m_model;
    };
protected:
    void contextMenuEvent(QContextMenuEvent* ev);
    void mousePressEvent(QMouseEvent* event);
};

//树控件的使用
class TreeView : public QTreeView //QTreeView
{
    Q_OBJECT
public:
    TreeView(QWidget* _parent = 0);
    ~TreeView();
private:
    QModelIndex index0;                 //当前选中id
    QStandardItem* currentItem;         //当前选中项
    QStandardItemModel* model;          //tree中的数据
    QMenu* menutb, *menuroot;
    QAction* Act_add, *Act_edit, *Act_del, *Act_xml, *Act_xmlout,
             *Act_addroot;
public:
    //树控件加载树结构数据
    void LoadTableData(QMap<QString, QVector<QString> >& datas_);
    void LoadlistData(QVector<QString>& datas_);
    void printTree(void);
private:
    void setTableInit(void);
    void setMenuInit(void);
    void iterateOverItems();
    QList<QStandardItem*> returnTheItems();
public:
    QPlainTextEdit* pTextEdit;
    //测试打印
    void PrintText(const QString& txt);
    void PrintText(QString& txt, QPlainTextEdit* TextEdit);
private slots:
    void OnAct_addrootClicked();
    void OnAct_addClicked();
    void OnAct_editClicked();
    void OnAct_delClicked();            //删除对象
    void OnAct_xmlClicked();
    void OnAct_xmloutClicked();
protected:
    void contextMenuEvent(QContextMenuEvent* ev);
    void mousePressEvent(QMouseEvent* event);
};

//演示数据显示
class WndDBPage : public QWidget
{
    Q_OBJECT
public:
    WndDBPage(QWidget* _parent = 0);
    ~WndDBPage();
public:
    //测试数据
    QVector<QStringList> TeatDatas;
private:
    //管理布局Tab页面
    QHBoxLayout* horizontalLayout;
    QVBoxLayout* verticalLayout;

    TreeView* m_tree;                   //树控件
    TableView* m_table;                 //数据显示
    QPlainTextEdit* m_Text;             //显示文本
    QMenu* menutb, *menuroot;           //菜单
    QAction* Act_add, *Act_addroot, *Act_edt, *Act_del;
};






#endif
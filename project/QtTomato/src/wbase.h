//*************************************************************************
//
//                       基本控件的扩展
//
//
//
//*************************************************************************

#ifndef WBASE_H
#define WBASE_H
#include "QPublic.h"

class CComboBoxCheck : public QComboBox
{
    Q_OBJECT
private:
    bool bSelected;
    QListWidget* pListWidget;
    QLineEdit* pLineEdit;
    QString strSelectedText;    
public:
    CComboBoxCheck(QWidget* parent = 0);
    virtual ~CComboBoxCheck();
protected: 
    void stateChanged(int state);
    void textChanged(const QString &text);
};

class QCompleterDemo:public QWidget
{
    Q_OBJECT
private:
    // DEMO1
    QLineEdit *search_line_edit;
    QCompleter *completer;
    QDirModel *dir_model;
    // DEMO2
    QLineEdit *txtline_edit;
    QCompleter *txtcompleter;
    // DEMO3
    QStringList word_list2;
    QLineEdit *txtline_edit2;
    QCompleter *txtcompleter2;
    QStringListModel* string_list_model;

public:
    QCompleterDemo(QWidget* parent = 0);
    ~QCompleterDemo();
//signals:
    void editComplete();
};

class ListWidget : public QWidget
{
public:
    ListWidget();
private:
    QLabel *label;
    QListWidget *list;
};

class TreeWidget : public QWidget
{
public:
    TreeWidget();
private:
    QTreeWidget *tree;
};



#endif

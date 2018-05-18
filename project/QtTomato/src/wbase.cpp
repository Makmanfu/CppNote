
#include "stdafx.h"
#include "wbase.h"

CComboBoxCheck::CComboBoxCheck(QWidget* parent) //:QWidget(parent)
{
    pListWidget = new QListWidget(this);
    pLineEdit = new QLineEdit(this);
    for (int i = 0; i < 5; ++i)
    {
        QListWidgetItem *pItem = new QListWidgetItem(pListWidget);
        pListWidget->addItem(pItem);
        pItem->setData(Qt::UserRole, i);
        QCheckBox *pCheckBox = new QCheckBox(this);
        pCheckBox->setText("sadsa");//QStringLiteral("Qter%1").arg(i));
        pListWidget->addItem(pItem);
        pListWidget->setItemWidget(pItem, pCheckBox);
        connect(pCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
    }
    this->setModel(pListWidget->model());
    this->setView(pListWidget);
    this->setLineEdit(pLineEdit);
    pLineEdit->setReadOnly(true);
    //ui.comboBox->setEditable(true);
    connect(pLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));
}

CComboBoxCheck::~CComboBoxCheck()
{
}

void CComboBoxCheck::stateChanged(int state)
{
    bSelected = true;
    QString strSelectedData("");
    strSelectedText.clear();
    QObject *object = QObject::sender();
    //QCheckBox *pSenderCheckBox = static_cast(object);
    QCheckBox *pSenderCheckBox = static_cast<QCheckBox*>(object);
    int nCount = pListWidget->count();
    for (int i = 0; i < nCount; ++i)
    {
        QListWidgetItem *pItem = pListWidget->item(i);
        QWidget *pWidget = pListWidget->itemWidget(pItem);
        QCheckBox *pCheckBox = (QCheckBox *)pWidget;
        if (pCheckBox->isChecked())
        {
            QString strText = pCheckBox->text();
            strSelectedData.append(strText).append(";");
        }
        //所点击的复选框
        if (pSenderCheckBox == pCheckBox)
        {
            int nData = pItem->data(Qt::UserRole).toInt();
            qDebug() << QString("I am sender...id : %1").arg(nData);
        }
    }
    if (strSelectedData.endsWith(";"))
        strSelectedData.remove(strSelectedData.count() - 1, 1);
    if (!strSelectedData.isEmpty())
    {
        //ui.comboBox->setEditText(strSelectedData);
        strSelectedText = strSelectedData;
        pLineEdit->setText(strSelectedData);
        pLineEdit->setToolTip(strSelectedData);
    }
    else
    {
        pLineEdit->clear();
        //ui.comboBox->setEditText("");
    }
    bSelected = false;
}

void CComboBoxCheck::textChanged(const QString &text)
{
    if (!bSelected)
        pLineEdit->setText(strSelectedText);
}


QCompleterDemo::QCompleterDemo(QWidget* parent /*= 0*/) :QWidget(parent)
{
    this->setMinimumSize(500, 500);
    //QCompleter补全文件路径
    search_line_edit = new QLineEdit(this);
    completer = new QCompleter(this);
    dir_model = new QDirModel(this);
    completer->setModel(dir_model);
    search_line_edit->setCompleter(completer);
    
    //QCompleter补全单词
    QStringList word_list;
    word_list << "Java" << "C++" << "C#" << "PHP" << "Perl" << "Python" << "Delphi" << "Ruby";
    txtline_edit = new QLineEdit(this);
    txtline_edit->move(0, 28);
    txtcompleter = new QCompleter(word_list, this);
    txtcompleter->setCaseSensitivity(Qt::CaseInsensitive);
    txtline_edit->setCompleter(txtcompleter);



    //QCompleter添加新单词
    txtline_edit2 = new QLineEdit(this);
    txtline_edit2->move(0, 48);
    string_list_model = new QStringListModel();
    txtcompleter2 = new QCompleter();
    txtcompleter2->setModel(string_list_model);
    //connect(txtline_edit2, &editingFinished, this, &editComplete);
    txtcompleter2->setCaseSensitivity(Qt::CaseInsensitive);
    txtline_edit2->setCompleter(txtcompleter2);
}

void QCompleterDemo::editComplete()
{
    QString text = txtline_edit->text();
    if (QString::compare(text, QString("")) != 0)
    {
        bool is_contains = word_list2.contains(text, Qt::CaseInsensitive);
        if (!is_contains)
        {
            word_list2 << text;
            string_list_model->setStringList(word_list2);
        }
    }
}

QCompleterDemo::~QCompleterDemo()
{
}


ListWidget::ListWidget()
{
    label = new QLabel;
    label->setFixedWidth(70);
    list = new QListWidget;
    list->addItem(new QListWidgetItem(QIcon(":/Tomato.png"), tr("Line")));
    list->addItem(new QListWidgetItem(QIcon(":/Tomato.png"), tr("Rectangle")));
    list->addItem(new QListWidgetItem(QIcon(":/Tomato.png"), tr("Oval")));
    list->addItem(new QListWidgetItem(QIcon(":/Tomato.png"), tr("Triangle")));
    //我们还可以设置viewModel这个参数，来确定使用不同的视图进行显示
    list->setViewMode(QListView::IconMode);
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->addWidget(list);
    setLayout(layout);
    connect(list, SIGNAL(currentTextChanged(QString)), label, SLOT(setText(QString)));
}

TreeWidget::TreeWidget()
{
    //tree = new QTreeWidget(this);
    //tree->setColumnCount(1);
    //QTreeWidgetItem *root = new QTreeWidgetItem(tree, QStringList(QString("Root")));
    //QTreeWidgetItem *leaf = new QTreeWidgetItem(root, QStringList(QString("Leaf 1")));
    //root->addChild(leaf);
    //QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root, QStringList(QString("Leaf 1")));
    //leaf2->setCheckState(0, Qt::Checked);
    //root->addChild(leaf2);
    //QList<QTreeWidgetItem *> rootList;
    //rootList << root;
    //tree->insertTopLevelItems(0, rootList);


    tree = new QTreeWidget(this);
    tree->setColumnCount(2);
    QStringList headers;
    headers << "Name" << "Number";
    tree->setHeaderLabels(headers);
    QStringList rootTextList;
    rootTextList << "Root" << "0";
    QTreeWidgetItem *root = new QTreeWidgetItem(tree, rootTextList);
    QStringList leafTextList;
    leafTextList << "Leaf 1" << "1";
    QTreeWidgetItem *leaf = new QTreeWidgetItem(root, leafTextList);
    root->addChild(leaf);
    QStringList leaf2TextList;
    leaf2TextList << "Leaf 2" << "2";
    QTreeWidgetItem *leaf2 = new QTreeWidgetItem(root, leaf2TextList);
    leaf2->setCheckState(0, Qt::Checked);
    root->addChild(leaf2);
    QList<QTreeWidgetItem *> rootList;
    rootList << root;
    tree->insertTopLevelItems(0, rootList);
}





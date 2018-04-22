
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("智能调配管理器"));
    GuiInit();
}

MainWindow::~MainWindow()
{
    trayIcon->hide();
    delete ui;
}

void MainWindow::GuiInit(void)
{
    //创建托盘
    trayIcon = new QSystemTrayIcon(this);
    //trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":/icon.png"));
    trayIcon->show();
    AllGrid = new TableView(ui->groupAll);
    QGridLayout* AllGridLayout = new GridLay(ui->groupAll);     //QGridLayout
    AllGridLayout->addWidget(AllGrid, 0, 0, 1, 1);
    TaskGrid = new TableView(ui->groupTask);
    QGridLayout* TaskGridLayout = new GridLay(ui->groupTask);
    TaskGridLayout->addWidget(TaskGrid, 0, 0, 1, 1);
    ClientGrid = new TableView(ui->groupClient);
    QGridLayout* groupClientLayout = new GridLay(ui->groupClient);
    groupClientLayout->addWidget(ClientGrid, 0, 0, 1, 1);
    //创建日志显示
    plainTextLog = new QPlainTextEdit(ui->groupLog);
    QGridLayout* groupLogLayout = new GridLay(ui->groupLog);
    groupLogLayout->addWidget(plainTextLog, 0, 0, 1, 1);
    plainTextLog->setReadOnly(true);
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
    //AllGrid->LoadTableData(testdat);
}

void MainWindow::on_Act_help_triggered()
{
    plainTextLog->appendPlainText(QString::fromLocal8Bit("打印日志测试!"));
    cfgDLG = new DlgFrm(3, this);
    cfgDLG->setModal(true);
    cfgDLG->show();
}

void MainWindow::on_Act_syscfg_triggered()
{
    cfgDLG = new DlgFrm(2, this);
    cfgDLG->setModal(true);
    cfgDLG->show();
}

void MainWindow::on_Act_addtask_triggered()
{
    cfgDLG = new DlgFrm(0, this);
    cfgDLG->setModal(true);
    cfgDLG->show();
}

void MainWindow::on_Act_model_triggered()
{
    cfgDLG = new DlgFrm(1, this);
    cfgDLG->setModal(true);
    cfgDLG->show();
}



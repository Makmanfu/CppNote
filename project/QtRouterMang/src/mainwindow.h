
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "stdafx.h"
#include "DlgFrm.h"
#include "TreeGrid.h"
#include "WndDB.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
private slots:
    void on_Act_help_triggered();
    void on_Act_syscfg_triggered();
    void on_Act_addtask_triggered();
    void on_Act_model_triggered();
private:
    //托盘
    QSystemTrayIcon* trayIcon;
    //所有的grid
    TableView* AllGrid, *TaskGrid, *ClientGrid;
    //打印日志
    QPlainTextEdit* plainTextLog;
    //配置界面
    DlgFrm* cfgDLG;
    //主UI
    Ui::MainWindow* ui;
private:
    //界面布局
    void GuiInit(void);
};

#endif



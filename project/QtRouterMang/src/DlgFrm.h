#ifndef DLGCFG_H
#define DLGCFG_H

#include "stdafx.h"

namespace Ui
{
    class DlgFrm;
}

class DlgFrm : public QDialog
{
    Q_OBJECT
public:
    explicit DlgFrm(QWidget* parent = 0);
    DlgFrm(int type_, QWidget* parent = 0);
    ~DlgFrm();
private:
    QPoint myPosDisp;                                   //鼠标点
    bool m_bPressed;                                    //记录状态
    int numPage;                                        //当前页面ID
    Ui::DlgFrm* ui;
private:
    //配置页面
    void SyscfgShow(void);
    void SyscfgSave(void);
    //help显示
    void HelpShow(void);
    //显示页面类型
    void showtype(int type_);
    //页面类型(1开始,0结束)
    void PageInitOut(int type_, bool state_ = true);
private slots:
    void on_BtnAdd_clicked();
    void on_BtnExit_clicked();
protected:
    void mousePressEvent(QMouseEvent* ev);
    void mouseReleaseEvent(QMouseEvent* ev);
    void mouseMoveEvent(QMouseEvent* ev);
};

#endif




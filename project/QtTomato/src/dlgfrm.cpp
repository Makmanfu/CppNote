
#include "stdafx.h"
#include "dlgfrm.h"
#include "ui_dlgfrm.h"

DlgFrm::DlgFrm(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::DlgFrm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
    this->setModal(true);
    this->setWindowTitle(QString::fromLocal8Bit("程序设置"));
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(on_OKclicked()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    //加载显示数据
    ui->chkSuspenShow->setChecked(Appcfg::Instance()->Sdat.SupenShow);
    //设置显示托盘退出选项
    ui->chkSuspenExit->setEnabled(ui->chkSuspenShow->checkState());
    ui->chkSuspenExit->setChecked(Appcfg::Instance()->Sdat.SuspenExit);
    ui->cboxSuspenPaint->setCurrentIndex(Appcfg::Instance()->Sdat.SuspenPaint);
    ui->chkGoodGif->setChecked(Appcfg::Instance()->Sdat.GoodGif);
    ui->chkClock->setChecked(Appcfg::Instance()->Sdat.Clock);
}

DlgFrm::~DlgFrm()
{
    delete ui;
}

void DlgFrm::on_OKclicked()
{
    //写入ini数据
    Appcfg::Instance()->Sdat.SupenShow = ui->chkSuspenShow->checkState();
    Appcfg::Instance()->Sdat.SuspenExit = ui->chkSuspenExit->checkState();
    Appcfg::Instance()->Sdat.SuspenPaint = ui->cboxSuspenPaint->currentIndex();
    Appcfg::Instance()->Sdat.GoodGif = ui->chkGoodGif->checkState();
    Appcfg::Instance()->Sdat.Clock = ui->chkClock->checkState();
    Appcfg::Instance()->WriteDataIni();
    accept();
}



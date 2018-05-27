
#include "DlgFrm.h"
#include "ui_DlgFrm.h"

DlgFrm::DlgFrm(QWidget* parent) : QDialog(parent)
    , ui(new Ui::DlgFrm)
    , numPage(1)
{
    ui->setupUi(this);
    this->setWindowTitle(TCSR("通用框架"));
    this->setWindowFlags(Qt::Tool);
    showtype(numPage);
    PageInitOut(numPage);
}

DlgFrm::DlgFrm(int type_, QWidget* parent) : QDialog(parent)
    , ui(new Ui::DlgFrm)
    , numPage(type_)
{
    ui->setupUi(this);
    this->setWindowTitle(TCSR("通用框架"));
    this->setWindowFlags(Qt::Tool);
    //显示对应的页面
    showtype(numPage);
    PageInitOut(numPage);
}

DlgFrm::~DlgFrm()
{
    delete ui;
}

void DlgFrm::mousePressEvent(QMouseEvent* ev)
{
    myPosDisp = ev->globalPos() - pos();
    m_bPressed = true;
}

void DlgFrm::mouseReleaseEvent(QMouseEvent* ev)
{
    ev->accept();
    m_bPressed = false;
}

void DlgFrm::mouseMoveEvent(QMouseEvent* ev)
{
    if (m_bPressed)
        move(ev->globalPos() - myPosDisp);
}

void DlgFrm::showtype(int type_)
{
    if (type_ != 3)    ui->tabWidget->removeTab(3);
    if (type_ != 2)    ui->tabWidget->removeTab(2);
    if (type_ != 1)    ui->tabWidget->removeTab(1);
    if (type_ != 0)    ui->tabWidget->removeTab(0);
}

void DlgFrm::PageInitOut(int type_, bool state_)
{
    switch (type_)
    {
        case 0:
            break;
        case 1:
            break;
        case 2:
            if (state_)
                SyscfgShow();
            else
                SyscfgSave();
            break;
        case 3:
        default:
            HelpShow();
            break;
    }
}

void DlgFrm::HelpShow(void)
{
    ui->BtnAdd->setVisible(false);
    ui->plainTextHelp->appendPlainText(TCSR("1.使用说明"));
}

void DlgFrm::SyscfgShow(void)
{
    ui->BtnAdd->setVisible(false);
    ui->cBoxSvrName->setEditText(QString(CInIcfg::Instance()->SvrName));
    ui->cBoxautorunexe->setCurrentIndex(CInIcfg::Instance()->autorunexe);
    ui->cBoxWebPort->setEditText(QString::number(CInIcfg::Instance()->WebPort, 10));
    ui->cBoxSetthread->setCurrentIndex(CInIcfg::Instance()->Setthread);
    ui->cBoxalgotype->setCurrentIndex(CInIcfg::Instance()->algotype);
    ui->cBoxnettype->setCurrentIndex(CInIcfg::Instance()->nettype);
    ui->cBoxcoreconftype->setCurrentIndex(CInIcfg::Instance()->coreconftype);
    ui->cBoxlooptime->setCurrentIndex(CInIcfg::Instance()->looptime);
}

void DlgFrm::SyscfgSave(void)
{
    CInIcfg::Instance()->SvrName = ui->cBoxSvrName->currentText().trimmed();
    CInIcfg::Instance()->autorunexe = ui->cBoxautorunexe->currentIndex();
    CInIcfg::Instance()->WebPort = atoi(ui->cBoxWebPort->currentText().trimmed().toStdString().c_str());
    CInIcfg::Instance()->Setthread = ui->cBoxSetthread->currentIndex();
    CInIcfg::Instance()->algotype = ui->cBoxalgotype->currentIndex();
    CInIcfg::Instance()->nettype = ui->cBoxnettype->currentIndex();
    CInIcfg::Instance()->coreconftype = ui->cBoxcoreconftype->currentIndex();
    CInIcfg::Instance()->looptime = ui->cBoxlooptime->currentIndex();
    CInIcfg::Instance()->WriteDataIni();
}

void DlgFrm::on_BtnAdd_clicked()
{
}

void DlgFrm::on_BtnExit_clicked()
{
    PageInitOut(numPage, false);
    accept();
}






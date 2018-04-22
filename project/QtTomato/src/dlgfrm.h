#ifndef DLGCFG_H
#define DLGCFG_H

#include "QPublic.h"

namespace Ui
{
    class DlgFrm;
}

class DlgFrm : public QDialog
{
    Q_OBJECT
public:
    explicit DlgFrm(QWidget* parent = 0);
    ~DlgFrm();
private slots:
    void on_OKclicked();

private:
    Ui::DlgFrm* ui;
};

#endif


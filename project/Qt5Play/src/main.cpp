

#include "stdafx.h"
#include "QTPlayer.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QTPlayer WD;    //QWidget WD;  QTPlayer  QMoveDlg
    WD.show();
    return app.exec();
}


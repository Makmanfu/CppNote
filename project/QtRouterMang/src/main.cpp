
#include "stdafx.h"
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyle(new QCleanlooksStyle);
    app.setWindowIcon(QIcon(":/icon.png"));
    MainWindow wnd;
    wnd.show();
    return app.exec();
}

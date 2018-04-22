

#include "stdafx.h"
#include "tabdb.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    //app.setWindowIcon(QIcon(":/player.png"));
    TabDB WD;
    WD.show();
    return app.exec();
}


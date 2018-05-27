

#include "stdafx.h"
#include "WndOpenCV.h"
#include "siftmatch.h"
//#include <QTextCodec>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/gimep.png"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    SiftMatch w;
    w.show();
    //´´½¨OpenCV
    WndOpenCV myOpenCV;
    myOpenCV.show();
    return app.exec();
}


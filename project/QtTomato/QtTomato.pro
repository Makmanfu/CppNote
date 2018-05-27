#-------------------------------------------------
#
# Project created by QtCreator 2016-04-01T11:47:15
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += c++11 static

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtTomato
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS


win32:RC_FILE = res/res.rc

#配置第三方头文件和库路径方法
#INCLUDEPATH += C:\MinGW\opencv3\build\include \
#    ../../SQ_Core/src \
#    ../../SQ_CV/src
#INCLUDEPATH += $$PWD/../../../../Develop/QT/Qt5.8.0/5.8/msvcs12/plugins/imageformats
#DEPENDPATH += $$PWD/../../../../Develop/QT/Qt5.8.0/5.8/msvcs12/plugins/imageformats
#LIBS += -L$$PWD/../../../../Develop/QT/Qt5.8.0/5.8/msvcs12/plugins/imageformats/ -lqjpeg
#LIBS += -L$$PWD/../../../../Develop/QT/Qt5.8.0/5.8/msvcs12/plugins/imageformats/ -lqgif


INCLUDEPATH += ./ \
    ./src \
    ./src/dlg \
    ./moc \
    ./lib

#宏定义方�DEFINES += _QT_CREATE_LIB

#加当前路�#win32: LIBS += -L$$PWD/ -lopencv_world -lopencv_ts
INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

MOC_DIR += ../QtTomato/moc
UI_DIR += ../QtTomato/moc
RCC_DIR += ../QtTomato/moc

FORMS += \
    src/dlgfrm.ui \
    src/frmnettool.ui \
    src/wplay.ui \
    src/wprogressbar.ui \
    src/wthreadgrid.ui

HEADERS += \
    src/appcfg.h \
    src/dlgfrm.h \
    src/frmnettool.h \
    src/QPublic.h \
    src/QtConfig.h \
    src/stdafx.h \
    src/tabgis.h \
    src/tabgui.h \
    src/tabnet.h \
    src/tabtd.h \
    src/tcpserver.h \
    src/waxis.h \
    src/waxis_def.h \
    src/wbase.h \
    src/wdraw.h \
    src/wndmain.h \
    src/wpanel.h \
    src/wplay.h \
    src/wprogressbar.h \
    src/wthread.h \
    src/wthreadgrid.h \
    src/wtimenumbar.h

SOURCES += \
    src/appcfg.cpp \
    src/dlgfrm.cpp \
    src/frmnettool.cpp \
    src/main.cpp \
    src/QPublic.cpp \
    src/QtConfig.cpp \
    src/stdafx.cpp \
    src/tabgis.cpp \
    src/tabgui.cpp \
    src/tabnet.cpp \
    src/tabtd.cpp \
    src/tcpserver.cpp \
    src/waxis.cpp \
    src/wbase.cpp \
    src/wdraw.cpp \
    src/wndmain.cpp \
    src/wpanel.cpp \
    src/wplay.cpp \
    src/wprogressbar.cpp \
    src/wthread.cpp \
    src/wthreadgrid.cpp \
    src/wtimenumbar.cpp

RESOURCES += \
    res/wdgres.qrc











#-------------------------------------------------
#
# Project created by QtCreator 2016-05-07T22:27:12
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtRouterMang
TEMPLATE = app

win32:RC_FILE = Res/res.rc

INCLUDEPATH += ./ \
    ./src \
    ./lib \
    ../SQ_Core/src/SQ/ \
    ../SQ_Clibrary/src/Sqlite/

#DEFINES += _QT_CREATE_LIB

MOC_DIR += ../QtRouterMang/moc
UI_DIR += ../QtRouterMang/moc
RCC_DIR += ../QtRouterMang/moc

SOURCES += \
    src/stdafx.cpp \
    src/QPublic.cpp \
    src/mainwindow.cpp \
    src/main.cpp \
    src/Cinicfg.cpp \
    src/CManOperate.cpp \
    src/CSqlMan.cpp \
    src/DlgFrm.cpp \
    src/Dvr_Socket.cpp \
    src/P_Sql.cpp \
    src/TreeGrid.cpp \
    src/WndDB.cpp \
    ../SQ_Core/src/HP_Socket_Core/HPSocket_Core.cpp \
    ../SQ_Core/src/SQ/SQ_File.cpp \
    ../SQ_Core/src/SQ/SQ_timer.cpp \
    ../SQ_Core/src/SQ/SQ_txtini.cpp \
    ../SQ_Core/src/SQ/SQ_windows.cpp \
    ../SQ_Core/src/tinyxml/tinyxml.cpp \
    ../SQ_Clibrary/src/Sqlite/SQ_Sqlite.cpp \
    ../SQ_Clibrary/src/Sqlite/sqlite3.c

HEADERS  += \
    src/stdafx.h \
    src/QPublic.h \
    src/mainwindow.h \
    src/Cinicfg.h \
    src/CManOperate.h \
    src/CSqlMan.h \
    src/DlgFrm.h \
    src/Dvr_Socket.h \
    src/P_Sql.h \
    src/TreeGrid.h \
    src/WndDB.h \
    ../SQ_Core/src/HP_Socket_Core/HPSocket_Core.h \
    ../SQ_Core/src/SQ/SQ_File.h \
    ../SQ_Core/src/SQ/SQ_timer.h \
    ../SQ_Core/src/SQ/SQ_txtini.h \
    ../SQ_Core/src/SQ/SQ_windows.h \
    ../SQ_Core/src/tinyxml/tinyxml.h \
    ../SQ_Clibrary/src/Sqlite/SQ_Sqlite.h \
    ../SQ_Clibrary/src/Sqlite/sqlite3.h

FORMS    += \
    src/mainwindow.ui \
    src/DlgFrm.ui

RESOURCES += \
    Res/RouterMang.qrc

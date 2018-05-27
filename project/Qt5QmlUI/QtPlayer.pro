#-------------------------------------------------
#
# Project created by QtCreator 2016-04-01T11:47:15
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtPlayer
TEMPLATE = app

win32:RC_FILE = Res/res.rc
LIBS += ws2_32.lib user32.lib ole32.lib

#���õ�����ͷ�ļ��Ϳ�·������
#INCLUDEPATH += C:\MinGW\opencv3\build\include \
#    ../../SQ_Core/src \
#    ../../SQ_CV/src
INCLUDEPATH += ./ \
    ./src \
    ./src/dlg \
    ./lib

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/

#�궨�巽��
#DEFINES += _QT_CREATE_LIB

#�ӵ�ǰ·��
#win32: LIBS += -L$$PWD/ -lopencv_world -lopencv_ts
#INCLUDEPATH += $$PWD/
#DEPENDPATH += $$PWD/

MOC_DIR += ../QtPlayer/moc
UI_DIR += ../QtPlayer/moc
RCC_DIR += ../QtPlayer/moc

SOURCES += \
    src/main.cpp \ 
    src/QPublic.cpp \
    src/stdafx.cpp \
    src/playfrm.cpp \
    src/WndLrc.cpp \
    src/WndSuspension.cpp

HEADERS  += \
    src/QPublic.h \
    src/stdafx.h \
    src/playfrm.h \
    src/WndLrc.h \
    src/WndSuspension.h

RESOURCES += \
    Res/PlayRes.qrc

FORMS += \
    src/playfrm.ui








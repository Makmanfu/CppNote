#-------------------------------------------------
#
# Project created by QtCreator 2016-04-01T11:47:15
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtOpenCV
TEMPLATE = app

win32:RC_FILE = Res/res.rc

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

#��opencv��·��
win32: LIBS += -L$$PWD/../../Note/3rdparty/OpenCV3/build/x86/vc12/lib/ -lopencv_world -lopencv_ts
INCLUDEPATH += $$PWD/../../Note/3rdparty/OpenCV3/build/x86/vc12/lib \
               $$PWD/../../Note/3rdparty/OpenCV3/build/include \
               $$PWD/../../Note/3rdparty/OpenCV3/build/include/opencv \
               $$PWD/../../Note/3rdparty/OpenCV3/build/include/opencv2
DEPENDPATH += $$PWD/../../Note/3rdparty/OpenCV3/build/x86/vc12/lib


MOC_DIR += ../QtOpenCV/moc
UI_DIR += ../QtOpenCV/moc
RCC_DIR += ../QtOpenCV/moc

SOURCES += \
    src/main.cpp \ 
    src/QPublic.cpp \
    src/stdafx.cpp \
    src/Imgcv.cpp \
    src/StuOpDetect.cpp \
    src/WndOpenCV.cpp

HEADERS  += \
    src/QPublic.h \
    src/stdafx.h \
    src/Imgcv.h \
    src/StuOpDetect.h \
    src/StuOpDetect_typedef.h \
    src/WndOpenCV.h

RESOURCES +=










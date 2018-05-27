# �汾�޶�
cmake_minimum_required(VERSION 2.8.7)
# ��Ŀ����(sln)
project(QtTomato)

#-----------------------------------------------------------------------
# �������ɿ�ִ���ļ�������
SET(EXECUTABLE_FILE_NAME QtTomato)
# Ĭ��RELEASE
set(CMAKE_BUILD_TYPE RELEASE)
#set(CMAKE_BUILD_TYPE DEBUG)
# ���ñ�����ѡ�� ����ʾ���о���
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
# ��������·��
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)

#-----------------------------------------------------------------------
# ����ͷ�ļ� ���ļ� Դ�ļ���·��
SET(INCLUDE_PATH ${PROJECT_BINARY_DIR} ${PROJECT_BINARY_DIR}/SRC) 
SET(LIB_PATH ${PROJECT_BINARY_DIR}/SRC)
SET(SOURCE_PATH ${INCLUDE_PATH}) 
# ����ͷ�ļ�����·��
INCLUDE_DIRECTORIES(${INCLUDE_PATH}) 
# ����������·��
LINK_DIRECTORIES(${LIB_PATH})

#-----------------------------------------------------------------------
# QT4��֧��
FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtNetWork)
INCLUDE(${QT_USE_FILE})
SET(QT_USE_QTMAIN TRUE)
SET(QT_USE_QTCORE TRUE)
SET(QT_USE_QTGUI TRUE)
SET(QT_USE_QTNETWORK TRUE)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# MOC
#QT4_WRAP_CPP(QT_MOCS src/widget.h)
# UIC
#QT4_WRAP_UI(QT_UIS src/widget.ui)
# RCC
QT4_ADD_RESOURCES(QT_RCCS res/wdgres.qrc)

#-----------------------------------------------------------------------
# WIN�� SRC����
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/SRC DIR_SRCS)
# WIN�� RC����
SET(DIR_SRCS ${DIR_SRCS} res/res.rc)
# WIN�°���SRC+MOC
SET(ALL_SRCS ${DIR_SRCS} ${QT_MOCS} ${QT_UIS} ${QT_RCCS})
# ���ɿ�ִ��EXE�ļ�(WIN32Ϊ���ڳ���)
ADD_EXECUTABLE(${EXECUTABLE_FILE_NAME} WIN32 ${ALL_SRCS} )
# ���ɿ�ִ��DLL�ļ�(SHAREDΪ��̬��)
#ADD_LIBRARY(${EXECUTABLE_FILE_NAME} SHARED ${ALL_SRCS})

#-----------------------------------------------------------------------
# ����QT��
TARGET_LINK_LIBRARIES(${EXECUTABLE_FILE_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
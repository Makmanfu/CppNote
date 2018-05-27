# 版本限定
cmake_minimum_required(VERSION 2.8.7)
# 项目名称(sln)
project(QtTomato)

#-----------------------------------------------------------------------
# 设置生成可执行文件的名称
SET(EXECUTABLE_FILE_NAME QtTomato)
# 默认RELEASE
set(CMAKE_BUILD_TYPE RELEASE)
#set(CMAKE_BUILD_TYPE DEBUG)
# 设置编译器选项 不显示所有警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
# 设置生成路径
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)

#-----------------------------------------------------------------------
# 定义头文件 库文件 源文件的路径
SET(INCLUDE_PATH ${PROJECT_BINARY_DIR} ${PROJECT_BINARY_DIR}/SRC) 
SET(LIB_PATH ${PROJECT_BINARY_DIR}/SRC)
SET(SOURCE_PATH ${INCLUDE_PATH}) 
# 包含头文件搜索路径
INCLUDE_DIRECTORIES(${INCLUDE_PATH}) 
# 包含库搜索路径
LINK_DIRECTORIES(${LIB_PATH})

#-----------------------------------------------------------------------
# QT4库支持
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
# WIN下 SRC处理
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/SRC DIR_SRCS)
# WIN下 RC处理
SET(DIR_SRCS ${DIR_SRCS} res/res.rc)
# WIN下包含SRC+MOC
SET(ALL_SRCS ${DIR_SRCS} ${QT_MOCS} ${QT_UIS} ${QT_RCCS})
# 生成可执行EXE文件(WIN32为窗口程序)
ADD_EXECUTABLE(${EXECUTABLE_FILE_NAME} WIN32 ${ALL_SRCS} )
# 生成可执行DLL文件(SHARED为动态库)
#ADD_LIBRARY(${EXECUTABLE_FILE_NAME} SHARED ${ALL_SRCS})

#-----------------------------------------------------------------------
# 链接QT库
TARGET_LINK_LIBRARIES(${EXECUTABLE_FILE_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
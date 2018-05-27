# 版本限定
cmake_minimum_required(VERSION 3.4.0)
# 项目名称(sln)
project(Qt5QmlUI)
#-----------------------------------------------------------------------
# 默认RELEASE
set(CMAKE_BUILD_TYPE RELEASE)
#set(CMAKE_BUILD_TYPE DEBUG)
# 设置编译器选项 不显示所有警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
# 设置生成路径
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)
# 设置生成可执行文件的名称
SET(EXECUTABLE_FILE_NAME Qt5QmlUI)
#-----------------------------------------------------------------------
# 定义头文件 库文件 源文件的路径
SET(INCLUDE_PATH ${PROJECT_BINARY_DIR} ${PROJECT_BINARY_DIR}/SRC) 
SET(LIB_PATH ${INCLUDE_PATH})
SET(SOURCE_PATH ${INCLUDE_PATH}) 
# 包含头文件搜索路径
INCLUDE_DIRECTORIES(${INCLUDE_PATH}) 
# 包含库搜索路径
LINK_DIRECTORIES(${LIB_PATH})

#-----------------------------------------------------------------------
# QT5库支持(设置自动生成moc文件，一定要设置)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
#查找需要的Qt库文件，最好每一个库都要写，Qt也会根据依赖关系自动添加
find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Qml)
# MOC
#QT5_WRAP_CPP(QT_MOCS src/widget.h)
# UIC
#QT5_WRAP_UI(QT_UIS src/widget.ui)
# RCC
#QT5_ADD_RESOURCES(QT_RCCS RES/PlayRes.qrc)

#-----------------------------------------------------------------------
# WIN下 SRC处理
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/SRC DIR_SRCS)
# WIN下 RC处理
SET(DIR_SRCS ${DIR_SRCS} RES/RunICO.rc)
# WIN下包含SRC+MOC
SET(ALL_SRCS ${DIR_SRCS} ${QT_MOCS} ${QT_UIS} ${QT_RCCS})
# 生成可执行EXE文件(WIN32为窗口程序)
ADD_EXECUTABLE(${EXECUTABLE_FILE_NAME} WIN32 ${ALL_SRCS} )
# 生成可执行DLL文件(SHARED为动态库)
#ADD_LIBRARY(${EXECUTABLE_FILE_NAME} SHARED ${ALL_SRCS})

#-----------------------------------------------------------------------
# 链接QT库
#TARGET_LINK_LIBRARIES(${EXECUTABLE_FILE_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
TARGET_LINK_LIBRARIES(${EXECUTABLE_FILE_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Qml)

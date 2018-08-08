# 版本限定
cmake_minimum_required(VERSION 2.8.7)
# 项目名称(sln)
project(SQffmpeg)

# 设置编译器选项 不显示所有警告
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")

#设置字符集  
add_definitions(-DUNICODE -D_UNICODE)  

# 定义头文件的路径变量
SET(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/3rdParty/ffmpeg/include
    ${CMAKE_CURRENT_SOURCE_DIR}/RES
    ${CMAKE_CURRENT_SOURCE_DIR}/VC12
) 
MESSAGE(STATUS "Include Path, ${INCLUDE_PATH}")

# 定义库文件路径
SET(LIB_PATH ${INCLUDE_PATH})
MESSAGE(STATUS "Libs Path, ${LIB_PATH}")

# 定义源文件路径变量
SET(SOURCE_PATH ${INCLUDE_PATH}) 
MESSAGE(STATUS "Source Path , ${SOURCE_PATH}") 

# 包含头文件搜索路径
INCLUDE_DIRECTORIES(${INCLUDE_PATH}) 
# 包含库搜索路径
if(CMAKE_CL_64)    
LINK_DIRECTORIES(${LIB_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/3rdParty/ffmpeg/x64/lib)
else()
LINK_DIRECTORIES(${LIB_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/3rdParty/ffmpeg/x86/lib)
endif()
link_libraries(avcodec.lib avformat.lib avfilter.lib avutil.lib swscale.lib avdevice.lib postproc.lib swresample.lib SDL2.lib SDL2main.lib) 
#link_libraries(Ws2_32.lib Secur32.lib Bcrypt.lib Version.lib WINMM.lib IMM32.lib)

# 添加源文件路径下所有源文件存放到变量中(*.c && *.cpp)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/ DIR_SRCS)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/SRC DIR_SRCS)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/vc12 DIR_SRCS)
SET(DIR_SRCS ${DIR_SRCS} res/SQffmpeg.rc)
MESSAGE(STATUS "DIR_SRCS , ${DIR_SRCS}") 

# 设置生成可执行文件的路径
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)
MESSAGE(STATUS "BINARY_DIR , ${CMAKE_SOURCE_DIR}") 
# 设置生成可执行文件的名称
if(CMAKE_CL_64)    
SET(EXECUTABLE_FILE_NAME SQffmpeg64)
else()
SET(EXECUTABLE_FILE_NAME SQffmpeg)
endif()


# 生成可执行文件
ADD_EXECUTABLE(${EXECUTABLE_FILE_NAME} ${DIR_SRCS})








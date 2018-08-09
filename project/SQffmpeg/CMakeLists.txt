# �汾�޶�
cmake_minimum_required(VERSION 2.8.7)
# ��Ŀ����(sln)
project(SQffmpeg)

# ���ñ�����ѡ�� ����ʾ���о���
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")

#�����ַ���  
add_definitions(-DUNICODE -D_UNICODE)  

# ����ͷ�ļ���·������
SET(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/Common
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/3rdParty/ffmpeg/include
    ${CMAKE_CURRENT_SOURCE_DIR}/RES
    ${CMAKE_CURRENT_SOURCE_DIR}/VC12
) 
MESSAGE(STATUS "Include Path, ${INCLUDE_PATH}")

# ������ļ�·��
SET(LIB_PATH ${INCLUDE_PATH})
MESSAGE(STATUS "Libs Path, ${LIB_PATH}")

# ����Դ�ļ�·������
SET(SOURCE_PATH ${INCLUDE_PATH}) 
MESSAGE(STATUS "Source Path , ${SOURCE_PATH}") 

# ����ͷ�ļ�����·��
INCLUDE_DIRECTORIES(${INCLUDE_PATH}) 
# ����������·��
if(CMAKE_CL_64)    
LINK_DIRECTORIES(${LIB_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/3rdParty/ffmpeg/x64/lib)
else()
LINK_DIRECTORIES(${LIB_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../Note/3rdParty/ffmpeg/x86/lib)
endif()
link_libraries(avcodec.lib avformat.lib avfilter.lib avutil.lib swscale.lib avdevice.lib postproc.lib swresample.lib SDL2.lib SDL2main.lib) 
#link_libraries(Ws2_32.lib Secur32.lib Bcrypt.lib Version.lib WINMM.lib IMM32.lib)

# ����Դ�ļ�·��������Դ�ļ���ŵ�������(*.c && *.cpp)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/ DIR_SRCS)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/SRC DIR_SRCS)
AUX_SOURCE_DIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/vc12 DIR_SRCS)
SET(DIR_SRCS ${DIR_SRCS} res/SQffmpeg.rc)
MESSAGE(STATUS "DIR_SRCS , ${DIR_SRCS}") 

# �������ɿ�ִ���ļ���·��
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/CBIN)
MESSAGE(STATUS "BINARY_DIR , ${CMAKE_SOURCE_DIR}") 
# �������ɿ�ִ���ļ�������
if(CMAKE_CL_64)    
SET(EXECUTABLE_FILE_NAME SQffmpeg64)
else()
SET(EXECUTABLE_FILE_NAME SQffmpeg)
endif()


# ���ɿ�ִ���ļ�
ADD_EXECUTABLE(${EXECUTABLE_FILE_NAME} ${DIR_SRCS})







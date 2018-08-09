//***********************************************************************
//
//                  open编译配置 [请勿修改]
//                                          @2016-06-13 阿甘
//
//***********************************************************************


#ifndef OPENCVCONF_H
#define OPENCVCONF_H
//-------------------opencv 三选一必选编译开关-------------------------------
//#define OPENCV_STATIC       //静态链接
//#define OPENCV_DLL          //DLL链接
//-----------------------------------------------------------------------

#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
//----------------------------------------------------------------------

//静态编译链接
#ifdef OPENCV_STATIC
    #pragma comment( lib, "vfw32.lib" )
    #pragma comment( lib, "comctl32.lib" )
    #pragma comment( lib, "opencv_static" )
#endif
//动态编译链接
#ifdef OPENCV_DLL
    #pragma comment( lib, "OpenCV.lib" )
#endif
//全都不定义会报错
#if !defined(OPENCV_STATIC) && !defined(OPENCV_DLL)
    #pragma message("error:没有定义编译选项[OPENCV_STATIC/OPENCV_DLL]搞个飞机啊!")
#endif


#endif
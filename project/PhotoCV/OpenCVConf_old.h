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
//#define OPENCV_INDLL        //内部opencv链接
//-----------------------------------------------------------------------
//#define OPENCV_TS           //打开ts链接

//自动识别opencv头文件
#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
//opencv3以前自动支持
#if CV_MAJOR_VERSION<3
    #include <opencv2/nonfree/nonfree.hpp>
    #include <opencv2/nonfree/features2d.hpp>
    #include <opencv2/legacy/legacy.hpp>
#endif
using namespace cv;
//----------------------------------------------------------------------

//自动识别opencv库版本号处理
#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
    #define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
    #define oLIB(name) name "d"
#else
    #define cvLIB(name) "opencv_" name CV_VERSION_ID
    #define oLIB(name) name
#endif

//静态编译链接
#ifdef OPENCV_STATIC
    #if CV_MAJOR_VERSION>2
        #pragma comment( lib, "ippicvmt.lib" )
        #pragma comment( lib, oLIB("libwebp") )
        #pragma comment( lib, cvLIB("hal") )
        #pragma comment( lib, cvLIB("imgcodecs") )
        #pragma comment( lib, cvLIB("shape") )
        #pragma comment( lib, cvLIB("videoio") )
    #else
        #pragma comment( lib, cvLIB("nonfree") )
        #pragma comment( lib, cvLIB("ocl") )
        #pragma comment( lib, cvLIB("contrib") )
        #pragma comment( lib, cvLIB("gpu") )
        #pragma comment( lib, cvLIB("legacy") )
    #endif
    #pragma comment( lib, "vfw32.lib" )
    #pragma comment( lib, "comctl32.lib" )
    #pragma comment( lib, oLIB("IlmImf") )
    #pragma comment( lib, oLIB("libjasper") )
    #pragma comment( lib, oLIB("libjpeg") )
    #pragma comment( lib, oLIB("libpng") )
    #pragma comment( lib, oLIB("libtiff") )
    #pragma comment( lib, oLIB("zlib") )
    #pragma comment( lib, cvLIB("core") )
    #pragma comment( lib, cvLIB("calib3d") )
    #pragma comment( lib, cvLIB("features2d") )
    #pragma comment( lib, cvLIB("flann") )
    #pragma comment( lib, cvLIB("highgui") )
    #pragma comment( lib, cvLIB("imgproc") )
    #pragma comment( lib, cvLIB("ml") )
    #pragma comment( lib, cvLIB("objdetect") )
    #pragma comment( lib, cvLIB("photo") )
    #pragma comment( lib, cvLIB("stitching") )
    #pragma comment( lib, cvLIB("superres") )
    #pragma comment( lib, cvLIB("video") )
    #pragma comment( lib, cvLIB("videostab") )
    #ifdef OPENCV_TS
        #pragma comment( lib, cvLIB("ts") )
    #endif
#endif
//动态编译链接
#ifdef OPENCV_DLL
    #if CV_MAJOR_VERSION>2
        #pragma comment( lib, cvLIB("world") )
    #else
        #pragma comment( lib, cvLIB("core") )
        #pragma comment( lib, cvLIB("imgproc") )
        #pragma comment( lib, cvLIB("highgui") )
        #pragma comment( lib, cvLIB("flann") )
        #pragma comment( lib, cvLIB("features2d") )
        #pragma comment( lib, cvLIB("calib3d") )
        #pragma comment( lib, cvLIB("ml") )
        #pragma comment( lib, cvLIB("nonfree") )
        #pragma comment( lib, cvLIB("objdetect") )
        #pragma comment( lib, cvLIB("ocl") )
        #pragma comment( lib, cvLIB("contrib") )
        #pragma comment( lib, cvLIB("gpu") )
        #pragma comment( lib, cvLIB("legacy") )
        #pragma comment( lib, cvLIB("photo") )
        #pragma comment( lib, cvLIB("stitching") )
        #pragma comment( lib, cvLIB("superres") )
        #pragma comment( lib, cvLIB("video") )
        #pragma comment( lib, cvLIB("videostab") )
    #endif
    #ifdef OPENCV_TS
        #pragma comment( lib, cvLIB("ts") )
    #endif
#endif
//内部优化发布版编译链接
#ifdef OPENCV_INDLL
    #ifdef _WIN64
        #pragma comment( lib, "OpenCV64" )
    #else
        //#pragma comment( lib, "OpenCV" )
        #pragma comment( lib, "vfw32.lib" )
        #pragma comment( lib, "comctl32.lib" )
        #pragma comment( lib, "opencv3_static" )
    #endif
    #ifdef OPENCV_TS
        #pragma comment( lib, cvInLIB("ts") )
    #endif
#endif
//全都不定义会报错
#if !defined(OPENCV_STATIC) && !defined(OPENCV_DLL) && !defined(OPENCV_INDLL)
    #pragma message("error:没有定义编译选项[OPENCV_STATIC/OPENCV_DLL/OPENCV_INDLL]搞个飞机啊!")
#endif


#endif
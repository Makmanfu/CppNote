//***********************************************************************
//
//                  open�������� [�����޸�]
//                                          @2016-06-13 ����
//
//***********************************************************************


#ifndef OPENCVCONF_H
#define OPENCVCONF_H
//-------------------opencv ��ѡһ��ѡ���뿪��-------------------------------
//#define OPENCV_STATIC       //��̬����
//#define OPENCV_DLL          //DLL����
//#define OPENCV_INDLL        //�ڲ�opencv����
//-----------------------------------------------------------------------
//#define OPENCV_TS           //��ts����

//�Զ�ʶ��opencvͷ�ļ�
#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
//opencv3��ǰ�Զ�֧��
#if CV_MAJOR_VERSION<3
    #include <opencv2/nonfree/nonfree.hpp>
    #include <opencv2/nonfree/features2d.hpp>
    #include <opencv2/legacy/legacy.hpp>
#endif
using namespace cv;
//----------------------------------------------------------------------

//�Զ�ʶ��opencv��汾�Ŵ���
#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
    #define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
    #define oLIB(name) name "d"
#else
    #define cvLIB(name) "opencv_" name CV_VERSION_ID
    #define oLIB(name) name
#endif

//��̬��������
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
//��̬��������
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
//�ڲ��Ż��������������
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
//ȫ��������ᱨ��
#if !defined(OPENCV_STATIC) && !defined(OPENCV_DLL) && !defined(OPENCV_INDLL)
    #pragma message("error:û�ж������ѡ��[OPENCV_STATIC/OPENCV_DLL/OPENCV_INDLL]����ɻ���!")
#endif


#endif
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
//-----------------------------------------------------------------------

#include <opencv2/core/version.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
//----------------------------------------------------------------------

//��̬��������
#ifdef OPENCV_STATIC
    #pragma comment( lib, "vfw32.lib" )
    #pragma comment( lib, "comctl32.lib" )
    #pragma comment( lib, "opencv_static" )
#endif
//��̬��������
#ifdef OPENCV_DLL
    #pragma comment( lib, "OpenCV.lib" )
#endif
//ȫ��������ᱨ��
#if !defined(OPENCV_STATIC) && !defined(OPENCV_DLL)
    #pragma message("error:û�ж������ѡ��[OPENCV_STATIC/OPENCV_DLL]����ɻ���!")
#endif


#endif
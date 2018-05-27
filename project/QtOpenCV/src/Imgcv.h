

#ifndef IMGCV_H__
#define IMGCV_H__

#include <iostream>
using namespace std;
#include "OpenCVConf.h"
using namespace cv;

class cvRecord
{
private:

public:
    //去色
    void GetGrayMat(const Mat& f_img, Mat& f_out);
    //二值化
    void Binarization(const Mat& f_img, Mat& f_out);
    //直方图的均衡化
    void ColorHistEqualize(const Mat& f_img, Mat& f_out);

    //Canny算子
    void CannyMat(const Mat& f_img, Mat& f_out);
    //face test
    void FaceTestMat(const Mat& f_img, Mat& f_out);
    //pep test
    void PepTestMat(Mat& f_out);
    //speed test
    void SpeedTestMat(const Mat& f_img, Mat& f_out);
public:
    cvRecord() {};
    virtual ~cvRecord() {};
};


#endif



#ifdef _CH_
    #pragma package <opencv>
#endif

#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <cv.h>
#include <highgui.h>
#include <ml.h>
#include <stdio.h>
#include <ctype.h>

IplImage preprocessing(IplImage* imgSrc, int new_width, int new_height);

class basicOCR
{
public:
    float classify(IplImage* img, int showResult);
    basicOCR();
    void test();
private:
    char file_path[255];
    int train_samples;
    int classes;
    CvMat* trainData;
    CvMat* trainClasses;
    int size;
    static const int K = 5;//最大邻居个数
#if CV_MAJOR_VERSION<3
    CvKNearest* knn;
#else
    cv::ml::KNearest* knn;
#endif
    void getData();
    void train();
};

int OCRmain(void);

#endif
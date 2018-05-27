
#include "stdafx.h"
#include "Imgcv.h"
#include "StuOpDetect.h"

void cvRecord::GetGrayMat(const Mat& f_img, Mat& f_out)
{
    //图像空间转换
    cvtColor(f_img, f_out, CV_RGB2GRAY);    //灰度模式
}

void cvRecord::Binarization(const Mat& f_img, Mat& f_out)
{
    //二值图
    //cv::threshold(GetGrayMat(f_img), f_out, ff, 255, THRESH_BINARY_INV);
    //二值图并取反
    GetGrayMat(f_img, f_out);
    cv::threshold(f_out, f_out, 60, 255, THRESH_BINARY);    //THRESH_BINARY
}

void cvRecord::ColorHistEqualize(const Mat& f_img, Mat& f_out)
{
    if (f_img.channels() == 1)
    {
        //cvtColor(f_img,f_img,CV_BGR2GRAY);  //转为灰度图
        equalizeHist(f_img, f_out);
    }
    else
    {
        vector<Mat> rgb_planes;                         //分割成3个单通道图像 ( R, G 和 B )
        split(f_img, rgb_planes);                       //分离通道 分别均衡化
        equalizeHist(rgb_planes[0], rgb_planes[0]);
        equalizeHist(rgb_planes[1], rgb_planes[1]);
        equalizeHist(rgb_planes[2], rgb_planes[2]);
        merge(rgb_planes, f_out);                       //合并通道
    }
}

void cvRecord::CannyMat(const Mat& f_img, Mat& f_out)
{
    Mat t_img = f_img.clone(), contours;
    //const double PI = 3.1415926;
    //canny算子
    cv::Canny(f_img, contours, 125, 350);
    cv::threshold(contours, f_out, 128, 255, cv::THRESH_BINARY_INV);
}

void cvRecord::FaceTestMat(const Mat& f_img, Mat& f_out)
{
    f_img.copyTo(f_out);
    double scale = 1;
    bool tryflip = false;
    CascadeClassifier cascade;
    if (!cascade.load("SVM_face.xml")) return;
    StuDetect().FindMatFace(f_out, cascade, scale);
}

void cvRecord::PepTestMat(Mat& f_out)
{
    StuDetect().FindMatHogPeople(f_out);
}

void cvRecord::SpeedTestMat(const Mat& f_img, Mat& f_out)
{
    f_img.copyTo(f_out);
    vector<Rect> Sign_info;
    StuOpDetect().SpeedCheck(f_out, Sign_info);
    if (!Sign_info.empty())
        for (vector<Rect>::const_iterator r = Sign_info.begin(); r != Sign_info.end(); r++)
            cv::rectangle(f_out, *r, Scalar(0, 255, 0), 3);
}





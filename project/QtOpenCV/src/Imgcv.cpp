
#include "stdafx.h"
#include "Imgcv.h"
#include "StuOpDetect.h"

void cvRecord::GetGrayMat(const Mat& f_img, Mat& f_out)
{
    //ͼ��ռ�ת��
    cvtColor(f_img, f_out, CV_RGB2GRAY);    //�Ҷ�ģʽ
}

void cvRecord::Binarization(const Mat& f_img, Mat& f_out)
{
    //��ֵͼ
    //cv::threshold(GetGrayMat(f_img), f_out, ff, 255, THRESH_BINARY_INV);
    //��ֵͼ��ȡ��
    GetGrayMat(f_img, f_out);
    cv::threshold(f_out, f_out, 60, 255, THRESH_BINARY);    //THRESH_BINARY
}

void cvRecord::ColorHistEqualize(const Mat& f_img, Mat& f_out)
{
    if (f_img.channels() == 1)
    {
        //cvtColor(f_img,f_img,CV_BGR2GRAY);  //תΪ�Ҷ�ͼ
        equalizeHist(f_img, f_out);
    }
    else
    {
        vector<Mat> rgb_planes;                         //�ָ��3����ͨ��ͼ�� ( R, G �� B )
        split(f_img, rgb_planes);                       //����ͨ�� �ֱ���⻯
        equalizeHist(rgb_planes[0], rgb_planes[0]);
        equalizeHist(rgb_planes[1], rgb_planes[1]);
        equalizeHist(rgb_planes[2], rgb_planes[2]);
        merge(rgb_planes, f_out);                       //�ϲ�ͨ��
    }
}

void cvRecord::CannyMat(const Mat& f_img, Mat& f_out)
{
    Mat t_img = f_img.clone(), contours;
    //const double PI = 3.1415926;
    //canny����
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





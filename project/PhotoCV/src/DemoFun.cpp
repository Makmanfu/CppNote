
#include "stdafx.h"
#include "DemoFun.h"

#include "ChesFeatures.h"
#include "StuOpDetect.h"
#include "MatDispose.h"
#include "MatMath.h"
#include "MatUseTool.h"

#include "sift/match.hpp"
#include "SURF/surfmain.hpp"

int test_Show(int argc, char** argv)
{
    MatPixelsTool().TestColorMat();
    imshow("����opencv��ʾ", imread("../Resource/lena.jpg"));
    waitKey(0);
    return 1;
}

int test_sift(int argc, char** argv)
{
#if CV_MAJOR_VERSION>2
    matchImg();
    SiftFeatmain();
    dspfeatmain();
    surfmain();
#else
    //sift������ƥ��Ա�
    CharacteristicPt().SiftFeature(imread("../Resource/lena.jpg"), imread("../Resource/lena2.jpg"));
    //����ͼƬ��sift����
    CharacteristicPt().SiftFeature(imread("../Resource/lena.jpg"));
    //�Ա�ͼ��Ĳ���
    //CharacteristicPt().compareMat(imread("../Resource/A.jpg"), imread("../Resource/B.jpg"));
#endif
    return 1;
}

int test_LBP(int argc, char** argv)
{
    Mat face2 = imread("../Resource/lena.JPG", CV_LOAD_IMAGE_GRAYSCALE);
    Mat lbp_face2 = Mat::zeros(face2.size(), face2.type());
    oLBP<uchar> (face2, lbp_face2);
    imshow("LBP Image2", lbp_face2);
    waitKey();
    return 1;
}

int test_face(int argc, char** argv)
{
    CmdDetect(0, 0).FaceCheckShow("../Resource/lena.jpg");
    return 1;
}

int test_Cam(int argc, char** argv)
{
    CmdDetect(0, 0).FaceCheckShow("1.mp4");
    return 1;
}

int test_pep(int argc, char** argv)
{
    //OpPepDetection().PredictTarget(imread("pep.jpg"));
    //system("pause");
    CmdDetect(0, 0).PedestrianCheck("../Resource/pep/1.jpg");
    CmdDetect(0, 0).PedestrianCheck("../Resource/pep/2.jpg");
    CmdDetect(0, 0).PedestrianCheck("../Resource/pep/3.jpg");
    CmdDetect(0, 0).PedestrianCheck("../Resource/pep/4.jpg");
    waitKey(0);
    return 1;
}

int test_Speed(int argc, char** argv)
{
    const char* ymlfile = "../Resource/SVM_SPEED.yml";
    if (!StuDetect::CheckYmlFile(ymlfile))
        if (StuDetect::CheckYmlFile("SVM_SPEED.yml"))
            ymlfile = "SVM_SPEED.yml";
        else
        {
            StuSVM mSvm;
            mSvm.SetHogWH(40, 40);
            //ѵ��xml
            mSvm.TrainPathXml(ymlfile,      //"../PhotoCV/cfg/SVM_SPEED.yml",
                              "../project/PhotoCV/cfg/POS/",
                              "../project/PhotoCV/cfg/NEG/");
        }
    int _type = 1;      //���ﱾ��Ӧ�ö�ȡ������
    if (_type == 0)
        CmdDetect(0, 0).SpeedSignCheck();
    else
    {
        CmdDetect(0, 0).SpeedSignCheck("../Resource/Speeddemo.jpg");
        CmdDetect(0, 0).SpeedSignCheck("../Resource/Speed.jpg");
    }
    return 1;
}

int TrainSample(int argc, char** argv)
{
    //=========��������������===========================
    StuOpDetect().man_GetPngPosImg("../Resource/SampleData/lanbuda.png", 1000, 15);
    //=========��������������===========================
    StuOpDetect().man_GetNegImg("../Resource/SampleData/group.jpg", 1000, 156);
    system("pause");
    StuSVM mSvm;
    //���óߴ�
    mSvm.SetHogWH(40, 40);
    //ѵ��xml
    mSvm.TrainPathXml("SVM_Test.yml", "./POS/", "./NEG/");
    //��λ����
    cout << "���������:";
    cout << mSvm.PredictTarget("SVM_Test.yml", imread("./POS/46.png")) << endl;
    cout << "��⸺����:";
    cout << mSvm.PredictTarget("SVM_Test.yml", imread("./NEG/1.jpg")) << endl;
    system("pause");
    return 1;
}

int test_gethist(int argc, char** argv)
{
    imshow("ԭͼ", imread("../Resource/lena.jpg"));
    Mat GrayHist, ColorHist, HSHist;
    MatHistogram().GetGrayHist(imread("../Resource/lena.jpg"), Mat(), GrayHist);
    imshow("GrayHist", GrayHist);
    MatHistogram().GetColorHist(imread("../Resource/lena.jpg"), Mat(), ColorHist);
    imshow("ColorHist", ColorHist);
    MatHistogram().GetHSHist(imread("../Resource/lena.jpg"), Mat(), HSHist);
    imshow("HSHist", HSHist);
    std::cout << "ͼ����:" << MatHistogram().CalcEntropy(imread("../Resource/lena.jpg")) << std::endl;
    imshow("ֱ��ͼ���⻯", MatHistogram().ColorHistEqualize(imread("../Resource/lena.jpg")));
    waitKey(0);
    return 1;
}

int test_CheckNum(int argc, char** argv)
{
    Mat InOut_img = imread("../Resource/numcolor2.jpg");
    imshow("ԭͼ", InOut_img);
    Mat numlogo = InOut_img.clone(), GrayMat, outMat;
    cvtColor(numlogo, GrayMat, CV_BGR2GRAY);
    cv::morphologyEx(GrayMat, GrayMat, cv::MORPH_OPEN, Mat());
    cv::morphologyEx(GrayMat, GrayMat, cv::MORPH_CLOSE, Mat());
    //��ֵ������
    cv::threshold(GrayMat, outMat, 80, 255, THRESH_BINARY_INV);
    Mat element(3, 3, CV_8U, cv::Scalar(1));
    cv::erode(outMat, outMat, element);
    //cv::erode(outMat, outMat, cv::Mat(), cv::Point(-1, -1), 3);
    //cv::dilate(outMat, outMat, cv::Mat(), cv::Point(-1, -1), 3);
    cv::dilate(outMat, outMat, element);
    imshow("��ֵͼ��ʾ", outMat);
    //����ͨ��
    Mat findObj = outMat.clone();
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(findObj, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
    cout << "Contours: " << contours.size() << endl;
    for (vector<vector<Point> >::const_iterator itContours = contours.begin();
         itContours != contours.end(); ++itContours)
    {
        //�������
        double tmparea = fabs(cv::contourArea(*itContours));
        //�����������
        Rect aRect = cv::boundingRect(*itContours);
        if (tmparea < 150 || aRect.height < 40)
            continue;
        string title = "�ָ�ͼ�� ";
        title += itContours->size();
        std::cout << "Size: " << itContours->size() << std::endl;
        cv::Rect tmpRect(0, 0, 0, 0);
        MatMath().GetPointsRect(*itContours, tmpRect);
        Mat cutMat;
        MatPixelsTool().CutRectMat(outMat, cutMat, tmpRect);
        //MatPixelsTool().MatResize(cutMat, cutMat, 3);
        imshow(title, cutMat);
    }
    // draw black contours on white image
    Mat result(outMat.size(), CV_8U, cv::Scalar(255));
    drawContours(result, contours, -1, cv::Scalar(0), 1);
    imshow("Contours", result);    //��ʾ����
    cout << "0 1 2 3 4 5 6 7 8 9" << endl;
    waitKey(0);
    return 1;
}











#ifndef WNDOPENCV_H
#define WNDOPENCV_H

#include "stdafx.h"
#include <windows.h>
#include "OpenCVConf.h"
#include "Imgcv.h"

//OpenCV
class WndOpenCV : public QWidget
{
    Q_OBJECT
public:
    WndOpenCV(QWidget* parent = 0);
    virtual ~WndOpenCV();
private:
    QFont Menuft;
    QGridLayout* ImageShowGrid;
    QScrollArea* scrollArea;
    QLabel* ImageShow;                          //显示图片
    QAction* Act_OpenImg, *Act_NavtImg,
             *Act_gray, *Act_blackw, *Act_Equalize,
             *Act_erode, *Act_dilate, *Act_opend, *Act_closed,
             *Act_Canny,
             *Act_Face, *Act_Pep, *Act_Speed, *Act_Car, *Act_Num,
             *Act_sift, *Act_suft, *Act_Hog, *Act_Haar,
             *Act_CutScreen, *Act_Video, *Act_Avi;
    cv::Mat Srcimage, Outimage;                 //处理数据
    cvRecord Op;

    int videoopend;
    VideoCapture cap;
    QTimer theTimer;
private:
    void CreateMenus(void);
public:
    //显示Mat数据
    void MatImgShow(const cv::Mat& _MatImg);
private slots:
    void OnBtnOpenImgClicked();                 //打开图片
    void OnBtnNavtImgClicked();                 //显示原图
    void OnBtnGrayClicked();
    void OnBtnBlackwClicked();
    void OnBtnEqualizeClicked();
    void OnBtnFaceClicked();
    void OnBtnPepClicked();
    void OnBtnSpeedClicked();
    void OnBtnCutScreenClicked();
    void OnBtnVideoClicked();
    void OnBtnAviClicked();
    void updateImage();
protected:
    void contextMenuEvent(QContextMenuEvent* ev);
    void paintEvent(QPaintEvent* ev);
};








#endif



#pragma once
#include "FaceOperateType.h"
//#include <opencv2/opencv.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/legacy/legacy.hpp>
//using namespace cv;

//定位到img中的一个或多个人像
class DetectObject
{
public:
    DetectObject() {};
    ~DetectObject() {};
public:
    //定位到img中的一个人像 (要查找的源数据,匹配模型,查找到的第一个人像,宽)
    void detectLargestObject(const Mat& img, CascadeClassifier& cascade, Rect& largestObject, int scaledWidth = 320);
    //定位到img中的多个人像 (要查找的源数据,匹配模型,查找到的多个人像,宽)
    void detectManyObjects(const Mat& img, CascadeClassifier& cascade, vector<Rect>& objects, int scaledWidth = 320);
private:
    //查找人像(查找源,模型,多个人像,宽,...)
    void detectObjectsCustom(const Mat& img, CascadeClassifier& cascade, vector<Rect>& objects,
                             int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors);
};

//训练模型
class Recognition
{
public:
    Recognition() {};
    ~Recognition() {};
public:
    //创建FaceRecognizer.Eigenfaces
    Ptr<FaceRecognizer> learnCollectedFaces(const vector<Mat> preprocessedFaces,
                                            const vector<int> faceLabels, const string facerecAlgorithm = "FaceRecognizer.Eigenfaces");
    //显示训练结果
    void showTrainingDebugData(const Ptr<FaceRecognizer> model, const int faceWidth, const int faceHeight);
    //判断人像在不在模型里面
    Mat reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace);
    //获取相似变化
    double getSimilarity(const Mat A, const Mat B);
private:
    //将image转成1d数据结构
    Mat getImageFrom1DFloatMat(const Mat matrixRow, int height);
};

//进行人像扫描和定位
class PrepocessFace
{
public:
    PrepocessFace();
    ~PrepocessFace();
public:
    //使用opencv获取满足要求的人像
    Mat getPreprocessedFace(Mat& srcImg,
                            int desiredFaceWidth,
                            CascadeClassifier& faceCascade,
                            CascadeClassifier& eyeCascade1,
                            CascadeClassifier& eyeCascade2,
                            bool doLeftAndRightSeparately,
                            Rect* storeFaceRect = NULL,
                            Point* storeLeftEye = NULL,
                            Point* storeRightEye = NULL,
                            Rect* searchedLeftEye = NULL,
                            Rect* searchedRightEye = NULL);
    //使用opencv获取满足要求的人像，重载
    void getPreprocessedCheckFace(Mat& srcImg,
                                  int desiredFaceWidth,
                                  CascadeClassifier& faceCascade,
                                  CascadeClassifier& eyeCascade1,
                                  CascadeClassifier& eyeCascade2,
                                  Rect* storeFaceRect = NULL,
                                  Point* storeLeftEye = NULL,
                                  Point* storeRightEye = NULL,
                                  Rect* searchedLeftEye = NULL,
                                  Rect* searchedRightEye = NULL);
    //使用opencv获取满足要求的人像
    Mat getFace(Mat& srcImg,
                int desiredFaceWidth,
                CascadeClassifier& faceCascade,
                CascadeClassifier& eyeCascade1,
                CascadeClassifier& eyeCascade2,
                Point* storeLeftEye = NULL,
                Point* storeRightEye = NULL,
                Rect* searchedLeftEye = NULL,
                Rect* searchedRightEye = NULL);
private:
    //确定眼睛的位置
    void detectBothEyes(const Mat& face,
                        CascadeClassifier& eyeCascade1,
                        CascadeClassifier& eyeCascade2,
                        Point& leftEye,
                        Point& rightEye,
                        Rect* searchedLeftEye = NULL,
                        Rect* searchedRightEye = NULL);
    //平均化左眼右眼的值
    void equalizeLeftAndRightHalves(Mat& faceImg);
private:
    const double DESIRED_LEFT_EYE_X;     // Controls how much of the face is visible after preprocessing.
    const double DESIRED_LEFT_EYE_Y;
    const double FACE_ELLIPSE_CY;
    const double FACE_ELLIPSE_W;         // Should be atleast 0.5
    const double FACE_ELLIPSE_H;         // Controls how tall the face mask is.
    boost::shared_ptr<DetectObject> mDetectObject;
};

//训练人像模型
class TrainFaceOpertae
{
    //依赖PrepocessFace,Recognition
public:
    TrainFaceOpertae();
    ~TrainFaceOpertae();
public:
    //初始化
    Face_Result_E initTrainFaceOpertae();
    //训练一张人像
    Face_Result_E facePretreatment(char* facePath, Mat& Face);
    Face_Result_E facePretreatment(Mat& soure, Mat& dec);
    //设置人像信息
    void setData(int fw, int fh);
    //使用opencv进行训练
    Ptr<FaceRecognizer> faceTrainlearnCollected(std::vector<Mat>& image, std::vector<int>& label, char* name);
private:
    //预处理人像
    bool FacePretreatment(Mat& sour, Mat& dec);
private:
    int facewidth;
    int faceheight;
    cv::CascadeClassifier mfaceCascade;
    cv::CascadeClassifier meyeCascade1;
    cv::CascadeClassifier meyeCascade2;

    const char* faceCascadeFilename;
    const char* eyeCascadeFilename1;
    const char* eyeCascadeFilename2;
    boost::shared_ptr<PrepocessFace> mTrainPrepocess;
    boost::shared_ptr<Recognition> mTrainRecognition;
    const double CHANGE_IN_IMAGE_FOR_COLLECTION;
    const double CHANGE_IN_SECONDS_FOR_COLLECTION;
};

//定位视频或者图片中的人像和模型的关系
class PositionFaceOperate
{
    //依赖Recognition, PrepocessFace;
public:
    PositionFaceOperate();
    ~PositionFaceOperate();
public:
    //初始化
    Face_Result_E initPositionFaceOperate();
    //定位人像，并返回结果
    Face_Result_E predictFace(Mat& predictmat, int& label, double& confidence);
    //获取人像
    Face_Result_E getFace(uint8_t* buf, int sizeo, Mat& face);
    //获取人像，重载
    Face_Result_E getFace(IplImage* image, Mat& face);
    //人像预处理
    Face_Result_E facePretreatmen(Mat& source, Mat& dec);
    //设置模型，视频，人像等信息
    void setDate(int fw, int fh, int vw, int vh);
    void loadModelDate(std::vector<ModelXmlInfo_t>& modeItem);
    void createModelDate();
private:
    //定位人像
    Face_Result_E positionFaces(Mat& predictmat, int& label, double& confidence);
    //获取视频buf中的人像
    bool Faces(Mat& sourFace, Mat& decFace);
    //对获取人像进行预处理
    bool Pretreatmen(Mat& source, Mat& dec);
    //将buf转成Mat类型
    void CharBufToCVMat(Mat& source, uint8_t* buf, int leng);
private:
    cv::CascadeClassifier mfaceCascade;
    cv::CascadeClassifier meyeCascade1;
    cv::CascadeClassifier meyeCascade2;

    const char* faceCascadeFilename;
    const char* eyeCascadeFilename1;
    const char* eyeCascadeFilename2;
    std::vector<Ptr<FaceRecognizer> > model;
    const float UNKNOWN_PERSON_THRESHOLD;
    boost::shared_ptr<Recognition> mRecognition;
    boost::shared_ptr<PrepocessFace> mPrepocessFace;
    int facewidth;
    int faceheight;
    int videowidth;
    int videoheight;
    const double CHANGE_IN_IMAGE_FOR_COLLECTION;
    const double CHANGE_IN_SECONDS_FOR_COLLECTION;
    int label;
    double confidence;
};

//检测视频或者图片中的人像
class CheckFaceOperatec
{
    //依赖PrepocessFace
public:
    CheckFaceOperatec();
    ~CheckFaceOperatec();
    //初始化
    Face_Result_E initCheckface();
    //将char类型转成Mat类型
    void CharBufToCVMat(Mat& source, uint8_t* buf, int leng);
    //设置视频宽高，face的宽高
    void setDate(int vw, int vh, int fw, int fh);
    //检测人像
    bool checkFace(uint8_t* buf, int leng, Mat& face, Check_Face_Model_t mod_);
    //检测人像，重载
    bool checkFace(IplImage* img, Mat& face, Check_Face_Model_t mod_);
    //检测人像，重载
    bool checkFace(char* path, Mat& face);
private:
    //检测人像
    bool FaceDetection(Mat& sour, Mat& dec, Check_Face_Model_t mod);
private:
    cv::CascadeClassifier mfaceCascade;
    cv::CascadeClassifier meyeCascade1;
    cv::CascadeClassifier meyeCascade2;

    const char* faceCascadeFilename;
    const char* eyeCascadeFilename1;
    const char* eyeCascadeFilename2;
    int faceWidth;
    int faceHeight;
    int VideoWidth;
    int VideoHeight;
    boost::shared_ptr<PrepocessFace> mPrepocessFacePtr;
};




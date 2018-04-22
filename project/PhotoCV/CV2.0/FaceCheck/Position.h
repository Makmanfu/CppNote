#pragma once
#include "FaceOperateType.h"
//#include <opencv2/opencv.hpp>
//#include <opencv2/nonfree/nonfree.hpp>
//#include <opencv2/nonfree/features2d.hpp>
//#include <opencv2/legacy/legacy.hpp>
//using namespace cv;

//��λ��img�е�һ����������
class DetectObject
{
public:
    DetectObject() {};
    ~DetectObject() {};
public:
    //��λ��img�е�һ������ (Ҫ���ҵ�Դ����,ƥ��ģ��,���ҵ��ĵ�һ������,��)
    void detectLargestObject(const Mat& img, CascadeClassifier& cascade, Rect& largestObject, int scaledWidth = 320);
    //��λ��img�еĶ������ (Ҫ���ҵ�Դ����,ƥ��ģ��,���ҵ��Ķ������,��)
    void detectManyObjects(const Mat& img, CascadeClassifier& cascade, vector<Rect>& objects, int scaledWidth = 320);
private:
    //��������(����Դ,ģ��,�������,��,...)
    void detectObjectsCustom(const Mat& img, CascadeClassifier& cascade, vector<Rect>& objects,
                             int scaledWidth, int flags, Size minFeatureSize, float searchScaleFactor, int minNeighbors);
};

//ѵ��ģ��
class Recognition
{
public:
    Recognition() {};
    ~Recognition() {};
public:
    //����FaceRecognizer.Eigenfaces
    Ptr<FaceRecognizer> learnCollectedFaces(const vector<Mat> preprocessedFaces,
                                            const vector<int> faceLabels, const string facerecAlgorithm = "FaceRecognizer.Eigenfaces");
    //��ʾѵ�����
    void showTrainingDebugData(const Ptr<FaceRecognizer> model, const int faceWidth, const int faceHeight);
    //�ж������ڲ���ģ������
    Mat reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace);
    //��ȡ���Ʊ仯
    double getSimilarity(const Mat A, const Mat B);
private:
    //��imageת��1d���ݽṹ
    Mat getImageFrom1DFloatMat(const Mat matrixRow, int height);
};

//��������ɨ��Ͷ�λ
class PrepocessFace
{
public:
    PrepocessFace();
    ~PrepocessFace();
public:
    //ʹ��opencv��ȡ����Ҫ�������
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
    //ʹ��opencv��ȡ����Ҫ�����������
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
    //ʹ��opencv��ȡ����Ҫ�������
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
    //ȷ���۾���λ��
    void detectBothEyes(const Mat& face,
                        CascadeClassifier& eyeCascade1,
                        CascadeClassifier& eyeCascade2,
                        Point& leftEye,
                        Point& rightEye,
                        Rect* searchedLeftEye = NULL,
                        Rect* searchedRightEye = NULL);
    //ƽ�����������۵�ֵ
    void equalizeLeftAndRightHalves(Mat& faceImg);
private:
    const double DESIRED_LEFT_EYE_X;     // Controls how much of the face is visible after preprocessing.
    const double DESIRED_LEFT_EYE_Y;
    const double FACE_ELLIPSE_CY;
    const double FACE_ELLIPSE_W;         // Should be atleast 0.5
    const double FACE_ELLIPSE_H;         // Controls how tall the face mask is.
    boost::shared_ptr<DetectObject> mDetectObject;
};

//ѵ������ģ��
class TrainFaceOpertae
{
    //����PrepocessFace,Recognition
public:
    TrainFaceOpertae();
    ~TrainFaceOpertae();
public:
    //��ʼ��
    Face_Result_E initTrainFaceOpertae();
    //ѵ��һ������
    Face_Result_E facePretreatment(char* facePath, Mat& Face);
    Face_Result_E facePretreatment(Mat& soure, Mat& dec);
    //����������Ϣ
    void setData(int fw, int fh);
    //ʹ��opencv����ѵ��
    Ptr<FaceRecognizer> faceTrainlearnCollected(std::vector<Mat>& image, std::vector<int>& label, char* name);
private:
    //Ԥ��������
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

//��λ��Ƶ����ͼƬ�е������ģ�͵Ĺ�ϵ
class PositionFaceOperate
{
    //����Recognition, PrepocessFace;
public:
    PositionFaceOperate();
    ~PositionFaceOperate();
public:
    //��ʼ��
    Face_Result_E initPositionFaceOperate();
    //��λ���񣬲����ؽ��
    Face_Result_E predictFace(Mat& predictmat, int& label, double& confidence);
    //��ȡ����
    Face_Result_E getFace(uint8_t* buf, int sizeo, Mat& face);
    //��ȡ��������
    Face_Result_E getFace(IplImage* image, Mat& face);
    //����Ԥ����
    Face_Result_E facePretreatmen(Mat& source, Mat& dec);
    //����ģ�ͣ���Ƶ���������Ϣ
    void setDate(int fw, int fh, int vw, int vh);
    void loadModelDate(std::vector<ModelXmlInfo_t>& modeItem);
    void createModelDate();
private:
    //��λ����
    Face_Result_E positionFaces(Mat& predictmat, int& label, double& confidence);
    //��ȡ��Ƶbuf�е�����
    bool Faces(Mat& sourFace, Mat& decFace);
    //�Ի�ȡ�������Ԥ����
    bool Pretreatmen(Mat& source, Mat& dec);
    //��bufת��Mat����
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

//�����Ƶ����ͼƬ�е�����
class CheckFaceOperatec
{
    //����PrepocessFace
public:
    CheckFaceOperatec();
    ~CheckFaceOperatec();
    //��ʼ��
    Face_Result_E initCheckface();
    //��char����ת��Mat����
    void CharBufToCVMat(Mat& source, uint8_t* buf, int leng);
    //������Ƶ��ߣ�face�Ŀ��
    void setDate(int vw, int vh, int fw, int fh);
    //�������
    bool checkFace(uint8_t* buf, int leng, Mat& face, Check_Face_Model_t mod_);
    //�����������
    bool checkFace(IplImage* img, Mat& face, Check_Face_Model_t mod_);
    //�����������
    bool checkFace(char* path, Mat& face);
private:
    //�������
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




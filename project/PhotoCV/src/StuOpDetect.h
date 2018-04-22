//***********************************************************************
//
//      基本检测识别框架
//
//
//***********************************************************************
#ifndef STUOPDETECT_H__
#define STUOPDETECT_H__
#include "StuOpDetect_typedef.h"

//svm训练与定位学习
class StuSVM
{
public:
    StuSVM(void);
    StuSVM(int hogw, int hogh);
    virtual ~StuSVM(void);
protected:
    vector<string> m_vecPos, m_vecNeg;  //正负样本路径+文件名
    int hogWidht, hogHeight;
    Size CurSize;                       //需设置的size
public:
    //训练模型生成xml[InPosPath为路径文件夹]
    void TrainPathXml(const char* OutXmlFile, const char* InPosPath, const char* InNegPath);
    //定位数据
    int PredictTarget(const char* OutXmlFile, Mat& f_checked) const;
    //设置尺寸[默认为行人检测的]
    void SetHogWH(int hogw = 64, int hogh = 128);
protected:
    //生成样本脚本文件
    void CreateSimpleFile(const char* InPath, vector<string>& InOutData);
    //加载样本数据训练模型生成xml
    void TrainXml(const char* OutXmlFile, vector<string>& vecPos, vector<string>& vecNeg);
private:
    //测试实例演示代码
    void DemoShow(void);
};

//行人检测定位框架
class OpPepDetection: public StuSVM
{
#if CV_MAJOR_VERSION<3
private:
    class PepSvm: public CvSVM
    {
    public:
        PepSvm(void) {}
        ~PepSvm(void) {}
    public:
        //获得SVM的决策函数中的alpha数组
        double* get_alpha_vector()
        {
            return this->decision_func->alpha;
        }
        //获得SVM的决策函数中的rho参数,即偏移量
        double get_rho()
        {
            return this->decision_func->rho;
        }
    } mSvm;                             //用于训练hog的训练器
    HOGDescriptor people_dectect;       //HOG特征检测器
public:
    OpPepDetection(void);
    OpPepDetection(int hogw, int hogh);
    virtual ~OpPepDetection();
public:
    //加载样本脚本训练xml模型生成xml
    void TrainPathXml(const char* OutXmlFile, const char* InPosPath, const char* InNegPath);
    //加载xml行人检测
    int PredictTarget(const char* OutXmlFile, Mat& f_checked);
    //opencv自带行人检测
    int PredictTarget(Mat f_img) const;
private:
    //加载样本数据训练模型生成xml
    void TrainXml(const char* OutXmlFile, vector<string>& vecPos, vector<string>& vecNeg);
#endif
};

//目标检测例子
class StuDetect
{
public:
    StuDetect(void);
    virtual ~StuDetect(void);
public:
    //级联检测人脸和眼睛demo[]
    void FindImgFaceEyes(Mat& f_img, CascadeClassifier& f_cascade,
                         CascadeClassifier& f_nestedCascade, double f_scale, bool f_tryflip) const;
    //检测人脸返回数量
    int FindMatFace(Mat& f_img, CascadeClassifier& f_cascade, double f_scale) const;
    //HOG行人检测opencv
    int FindMatHogPeople(Mat& f_img) const;
    //限速牌子检测
    int SpeedSignCheck(Mat& f_src, Check_Info& CFGinfo) const;
    //限速牌子检测2无svm
    int SpeedSignVec(Mat& f_src, vector<Rect>& f_faces) const;
private:
    //根据参数在图层画图形
    Mat DrawCircle(Mat& In_img, Point pt, int r, Scalar color) const;
    Mat DrawRect(Mat& In_img, Point pt, int r, Scalar color) const;
    Mat DrawRect(Mat& In_img, Rect rc, Scalar color) const;
public:
    //判断模型文件是否为空
    static bool CheckYmlFile(const char* ypath);
};

//车牌检测与识别
class DetectPlateCarNUM
{

};

//检测对象结果框架
class StuOpDetect : public StuDetect
{
public:
    StuOpDetect(void);
    virtual ~StuOpDetect(void);
private:
    //vector<Rect> facelist;
public:
    void DemoShow(void);
    //正样板生成[f_num张数,f_lengh为旋转度数]
    void man_GetPngPosImg(char* f_png, int f_num, int f_lengh) const;
    //负样本生成[f_num张数,f_lengh为宽度]
    void man_GetNegImg(char* f_file, int f_num, int f_lengh) const;
    //检测人脸
    void FaceCheck(Mat& f_img, vector<Rect>& f_faces) const;
    //检测标示牌
    void SpeedCheck(Mat& f_img, vector<Rect>& f_faces);
private:
    //透明处理
    void createAlphaMat(Mat& f_img);
    //旋转图片
    void corecv_rotateImage(Mat& f_img, double angle) const;
};

//仅仅命令行测试框架
class CmdDetect
{
public:
    CmdDetect(int argc, char** argv);
    virtual ~CmdDetect();
private:
    int argcl;
    char** argvl;
    //类的成员函数指针
    typedef void (CmdDetect::*func)(const char*);
public:
    void DemoShow(void);
    //命令行检测框架测试 图片/视频/摄像头 [仅供main使用]
    int FrameCheckMat(func fp);
    //图片/视频和摄像头检测人脸 [f_videoname空 调用摄像头]
    void FaceCheckShow(const char* f_file = "");
    //行人检测测试
    void PedestrianCheck(const char* f_file = "");
    //标识牌检测
    void SpeedSignCheck(const char* f_file = "");
private:
    string GetThisNowTime(void);
    Mat DrawCircle(Mat& f_img, Rect rt, float num = 0);
};


#endif

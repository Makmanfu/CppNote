//***********************************************************************
//
//      �������ʶ����
//
//
//***********************************************************************
#ifndef STUOPDETECT_H__
#define STUOPDETECT_H__
#include "StuOpDetect_typedef.h"

//svmѵ���붨λѧϰ
class StuSVM
{
public:
    StuSVM(void);
    StuSVM(int hogw, int hogh);
    virtual ~StuSVM(void);
protected:
    vector<string> m_vecPos, m_vecNeg;  //��������·��+�ļ���
    int hogWidht, hogHeight;
    Size CurSize;                       //�����õ�size
public:
    //ѵ��ģ������xml[InPosPathΪ·���ļ���]
    void TrainPathXml(const char* OutXmlFile, const char* InPosPath, const char* InNegPath);
    //��λ����
    int PredictTarget(const char* OutXmlFile, Mat& f_checked) const;
    //���óߴ�[Ĭ��Ϊ���˼���]
    void SetHogWH(int hogw = 64, int hogh = 128);
protected:
    //���������ű��ļ�
    void CreateSimpleFile(const char* InPath, vector<string>& InOutData);
    //������������ѵ��ģ������xml
    void TrainXml(const char* OutXmlFile, vector<string>& vecPos, vector<string>& vecNeg);
private:
    //����ʵ����ʾ����
    void DemoShow(void);
};

//���˼�ⶨλ���
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
        //���SVM�ľ��ߺ����е�alpha����
        double* get_alpha_vector()
        {
            return this->decision_func->alpha;
        }
        //���SVM�ľ��ߺ����е�rho����,��ƫ����
        double get_rho()
        {
            return this->decision_func->rho;
        }
    } mSvm;                             //����ѵ��hog��ѵ����
    HOGDescriptor people_dectect;       //HOG���������
public:
    OpPepDetection(void);
    OpPepDetection(int hogw, int hogh);
    virtual ~OpPepDetection();
public:
    //���������ű�ѵ��xmlģ������xml
    void TrainPathXml(const char* OutXmlFile, const char* InPosPath, const char* InNegPath);
    //����xml���˼��
    int PredictTarget(const char* OutXmlFile, Mat& f_checked);
    //opencv�Դ����˼��
    int PredictTarget(Mat f_img) const;
private:
    //������������ѵ��ģ������xml
    void TrainXml(const char* OutXmlFile, vector<string>& vecPos, vector<string>& vecNeg);
#endif
};

//Ŀ��������
class StuDetect
{
public:
    StuDetect(void);
    virtual ~StuDetect(void);
public:
    //��������������۾�demo[]
    void FindImgFaceEyes(Mat& f_img, CascadeClassifier& f_cascade,
                         CascadeClassifier& f_nestedCascade, double f_scale, bool f_tryflip) const;
    //���������������
    int FindMatFace(Mat& f_img, CascadeClassifier& f_cascade, double f_scale) const;
    //HOG���˼��opencv
    int FindMatHogPeople(Mat& f_img) const;
    //�������Ӽ��
    int SpeedSignCheck(Mat& f_src, Check_Info& CFGinfo) const;
    //�������Ӽ��2��svm
    int SpeedSignVec(Mat& f_src, vector<Rect>& f_faces) const;
private:
    //���ݲ�����ͼ�㻭ͼ��
    Mat DrawCircle(Mat& In_img, Point pt, int r, Scalar color) const;
    Mat DrawRect(Mat& In_img, Point pt, int r, Scalar color) const;
    Mat DrawRect(Mat& In_img, Rect rc, Scalar color) const;
public:
    //�ж�ģ���ļ��Ƿ�Ϊ��
    static bool CheckYmlFile(const char* ypath);
};

//���Ƽ����ʶ��
class DetectPlateCarNUM
{

};

//�����������
class StuOpDetect : public StuDetect
{
public:
    StuOpDetect(void);
    virtual ~StuOpDetect(void);
private:
    //vector<Rect> facelist;
public:
    void DemoShow(void);
    //����������[f_num����,f_lenghΪ��ת����]
    void man_GetPngPosImg(char* f_png, int f_num, int f_lengh) const;
    //����������[f_num����,f_lenghΪ���]
    void man_GetNegImg(char* f_file, int f_num, int f_lengh) const;
    //�������
    void FaceCheck(Mat& f_img, vector<Rect>& f_faces) const;
    //����ʾ��
    void SpeedCheck(Mat& f_img, vector<Rect>& f_faces);
private:
    //͸������
    void createAlphaMat(Mat& f_img);
    //��תͼƬ
    void corecv_rotateImage(Mat& f_img, double angle) const;
};

//���������в��Կ��
class CmdDetect
{
public:
    CmdDetect(int argc, char** argv);
    virtual ~CmdDetect();
private:
    int argcl;
    char** argvl;
    //��ĳ�Ա����ָ��
    typedef void (CmdDetect::*func)(const char*);
public:
    void DemoShow(void);
    //�����м���ܲ��� ͼƬ/��Ƶ/����ͷ [����mainʹ��]
    int FrameCheckMat(func fp);
    //ͼƬ/��Ƶ������ͷ������� [f_videoname�� ��������ͷ]
    void FaceCheckShow(const char* f_file = "");
    //���˼�����
    void PedestrianCheck(const char* f_file = "");
    //��ʶ�Ƽ��
    void SpeedSignCheck(const char* f_file = "");
private:
    string GetThisNowTime(void);
    Mat DrawCircle(Mat& f_img, Rect rt, float num = 0);
};


#endif

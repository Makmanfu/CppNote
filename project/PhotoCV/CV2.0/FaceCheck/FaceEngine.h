#pragma once
#include "FaceOperateType.h"
#include "Position.h"
#include "tinyxml/tinyxml.h"

//��xml���еĲ��������ȡ��������ɾ���ļ�����Ԫ��
class XmlOperate
{
public:
    XmlOperate();
    ~XmlOperate();
public:
    //�����ݽ��г�ʼ������
    Face_Result_E initXmlOperate();
    //����һ���ص������İ�
    void setBindXmlReadElement(FunXmlGetElement _fun);
    //����һ��Ҫ������xml�ļ�����һ��xmlBUF��load���ɹ���ʱ��ͻ���ý�������
    Face_Result_E loadXmlFile(char* pathOrBuf, int leng = 0);
    //����xml�е�Ԫ�ص��ı���e.<a>c</a>����c
    const char* getXMLElementText();
    //����Ԫ�ص�����ֵ e.<a c=2 b= 3>df</a>����2����3
    const char* getXMLAttributeText();
    //����xml�ļ���ͷ
    Face_Result_E createXmlDocHead(char* Version = "1.0", char* Encoding = "UTF-8", char* Standalone = "yes");
    //��ӽ��Ԫ��
    Face_Result_E addNode(char* strNodeName, char* strElementNodeName, char* strElementNodeValue);
    //��Ԫ���������
    Face_Result_E addNodeAttribut(char* strNodeName, char* strElementAttributName, char* strElementAttributValue);
    //��xml�ļ����浽ָ��λ��
    Face_Result_E saveXmlDoc(char* paht);
private:
    //��ʼ����������
    Face_Result_E ClearXmlData();
    //��ȡDocument���и����
    Face_Result_E GetDocRoodNode();
    //���ݸ���������б���xml�е�Ԫ�أ�������Ԫ�ػ�������֮�����ð󶨺õĺ���ָ��
    Face_Result_E ParaseUpdateXml(TiXmlNode* pParent);
    bool InsterElement(char* strNodeName, char* strElementNodeName, char* strElementNodeValue);
    bool InsterAttribut(char* strNodeName, char* strElementAttributName, char* strElementAttributValue);
    //���Ը������ֻ�ȡ������ֵĽ��
    bool GetNodePointerByName(TiXmlElement* pRootEle, char* strNodeName, TiXmlElement*& Node);
private:
    TiXmlDocument mXmlDoc;
    TiXmlElement* mRootXmlElement;
    TiXmlNode* mPchildXmlNode;
    TiXmlAttribute* mAttributXmlNode;
    TiXmlDeclaration* mdecl;
    FunXmlGetElement mGetElementCB;
};

//����opencv�Ľ�����Ƶ
class VideoOpenCVOperate
{
public:
    VideoOpenCVOperate();
    ~VideoOpenCVOperate();
public:
    //��ʼ�������������
    Face_Result_E initVideoOpenCVOperate();
    //����Ƶ�ļ��������Ƶ��Ϣ
    Face_Result_E openVideoOpenCVFile(char* paht);
    //������Ƶ��Ϣ
    VideoAndPictrueInfo_t   getVideoInfo()
    {
        return mVideoInfo;
    }
    //����ָ����֡
    IplImage* getVideoOpenCVFrame(int conut);
    void getVideoOpenCVFrame(int conut, Mat& f_out);
    void ReleaseOperCV();
private:
    //�����Ƶ��Ϣ
    bool FillVideoInfo(double duration, int width, int height, int Channels);
private:
    VideoAndPictrueInfo_t mVideoInfo;
    CvCapture* mCapture;
    int frame_count;
};

//����λ����
class PositionEngine
{
    //����PositionFaceOperate
public:
    PositionEngine();
    ~PositionEngine();
public:
    //��ʼ��
    Face_Result_E initPositionEngine(Local_Parameter_t& ptr);
    //��ʼ��λ
    Face_Result_E positionStart();
private:
    //���лص�
    void setCalBack(int a , double b, IplImage* src, int time_a, int id);
    Face_Result_E writePositionResultXml();
private:
    boost::shared_ptr<VideoOpenCVOperate> mOpenCVDecode;
    boost::shared_ptr<PositionFaceOperate> mPosition;
    boost::shared_ptr<XmlOperate> mPositionXmlOperate;
    Local_Parameter_t mLocalParam;
    std::vector<FaceInfo_t> mFaceInfo;
    int falg;
};

//ѵ������
class TrainEngine
{
    //����TrainFaceOpertae
public:
    TrainEngine();
    ~TrainEngine();
public:
    //��ʼ��
    Face_Result_E initTrainEngine(Train_Parameter_t& par);
    //��ʼѵ��
    Face_Result_E trainStart();
private:
    //ѵ������
    bool TrainFace();
    //����ѵ�����
    bool saveSequenceTrainXml();
private:
    Train_Parameter_t mTrainParameter;
    boost::shared_ptr<TrainFaceOpertae> mTrainFaceOperate;
    boost::shared_ptr<XmlOperate> mXmlOperate;
};

//������������
class CheckEngine
{
    //����CheckFaceOperatec
public:
    CheckEngine();
    ~CheckEngine();
public:
    //��ʼ������
    Face_Result_E initCheckEngine(Check_Parameter_t& ptr);
    //��ʼ���
    Face_Result_E checkStart();
private:
    //���video
    Face_Result_E checkVideo();
    //���picture
    Face_Result_E checkPicture();
    //����ffmpeg����Ƶ���н���
    Face_Result_E videoFFmpegDecipher();
    //����opencv����Ƶ����
    Face_Result_E videoOpencvDecipher();
    //��ʼʹ��opencv��������λ
    Face_Result_E FaceDetection(bool falgs);
    //����������ע��һ��������Ϣ
    void checkAddFaceItem(Mat& face, int64_t i, Mat& frame);
    //������������Ϣ
    Face_Result_E saveCheckResult();
private:
    Check_Parameter_t mCheckParam;
    /*  boost::shared_ptr<VideFFmpegOperate> mDecipherFFmpeg;*/
    boost::shared_ptr<VideoOpenCVOperate> mDecipherOpenCV;
    boost::shared_ptr<CheckFaceOperatec> mCheckFace;
    boost::shared_ptr<XmlOperate> mXmlOperate;
    bool Continuous;
};


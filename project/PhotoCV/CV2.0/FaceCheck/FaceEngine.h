#pragma once
#include "FaceOperateType.h"
#include "Position.h"
#include "tinyxml/tinyxml.h"

//对xml进行的操作，如读取，创建，删除文件或者元素
class XmlOperate
{
public:
    XmlOperate();
    ~XmlOperate();
public:
    //对数据进行初始化操作
    Face_Result_E initXmlOperate();
    //这是一个回调函数的绑定
    void setBindXmlReadElement(FunXmlGetElement _fun);
    //载入一个要解析的xml文件或者一个xmlBUF，load进成功的时候就会调用解析函数
    Face_Result_E loadXmlFile(char* pathOrBuf, int leng = 0);
    //返回xml中的元素的文本，e.<a>c</a>返回c
    const char* getXMLElementText();
    //返回元素的属性值 e.<a c=2 b= 3>df</a>返回2或者3
    const char* getXMLAttributeText();
    //创建xml文件的头
    Face_Result_E createXmlDocHead(char* Version = "1.0", char* Encoding = "UTF-8", char* Standalone = "yes");
    //添加结点元素
    Face_Result_E addNode(char* strNodeName, char* strElementNodeName, char* strElementNodeValue);
    //给元素添加属性
    Face_Result_E addNodeAttribut(char* strNodeName, char* strElementAttributName, char* strElementAttributValue);
    //将xml文件保存到指定位置
    Face_Result_E saveXmlDoc(char* paht);
private:
    //初始化类中数据
    Face_Result_E ClearXmlData();
    //获取Document的中根结点
    Face_Result_E GetDocRoodNode();
    //跟据根结点来进行遍历xml中的元素，遍历到元素或者属性之后会调用绑定好的函数指针
    Face_Result_E ParaseUpdateXml(TiXmlNode* pParent);
    bool InsterElement(char* strNodeName, char* strElementNodeName, char* strElementNodeValue);
    bool InsterAttribut(char* strNodeName, char* strElementAttributName, char* strElementAttributValue);
    //可以跟据名字获取这个名字的结点
    bool GetNodePointerByName(TiXmlElement* pRootEle, char* strNodeName, TiXmlElement*& Node);
private:
    TiXmlDocument mXmlDoc;
    TiXmlElement* mRootXmlElement;
    TiXmlNode* mPchildXmlNode;
    TiXmlAttribute* mAttributXmlNode;
    TiXmlDeclaration* mdecl;
    FunXmlGetElement mGetElementCB;
};

//这是opencv的解码视频
class VideoOpenCVOperate
{
public:
    VideoOpenCVOperate();
    ~VideoOpenCVOperate();
public:
    //初始化或者清理参数
    Face_Result_E initVideoOpenCVOperate();
    //打开视频文件并填充视频信息
    Face_Result_E openVideoOpenCVFile(char* paht);
    //返回视频信息
    VideoAndPictrueInfo_t   getVideoInfo()
    {
        return mVideoInfo;
    }
    //返回指定的帧
    IplImage* getVideoOpenCVFrame(int conut);
    void getVideoOpenCVFrame(int conut, Mat& f_out);
    void ReleaseOperCV();
private:
    //填充视频信息
    bool FillVideoInfo(double duration, int width, int height, int Channels);
private:
    VideoAndPictrueInfo_t mVideoInfo;
    CvCapture* mCapture;
    int frame_count;
};

//人像定位引擎
class PositionEngine
{
    //依赖PositionFaceOperate
public:
    PositionEngine();
    ~PositionEngine();
public:
    //初始化
    Face_Result_E initPositionEngine(Local_Parameter_t& ptr);
    //开始定位
    Face_Result_E positionStart();
private:
    //进行回调
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

//训练引擎
class TrainEngine
{
    //依赖TrainFaceOpertae
public:
    TrainEngine();
    ~TrainEngine();
public:
    //初始化
    Face_Result_E initTrainEngine(Train_Parameter_t& par);
    //开始训练
    Face_Result_E trainStart();
private:
    //训练人像
    bool TrainFace();
    //保存训练结果
    bool saveSequenceTrainXml();
private:
    Train_Parameter_t mTrainParameter;
    boost::shared_ptr<TrainFaceOpertae> mTrainFaceOperate;
    boost::shared_ptr<XmlOperate> mXmlOperate;
};

//检测人像的引擎
class CheckEngine
{
    //依赖CheckFaceOperatec
public:
    CheckEngine();
    ~CheckEngine();
public:
    //初始化引擎
    Face_Result_E initCheckEngine(Check_Parameter_t& ptr);
    //开始检测
    Face_Result_E checkStart();
private:
    //检测video
    Face_Result_E checkVideo();
    //检测picture
    Face_Result_E checkPicture();
    //进行ffmpeg对视频进行解码
    Face_Result_E videoFFmpegDecipher();
    //进行opencv对视频解码
    Face_Result_E videoOpencvDecipher();
    //开始使用opencv进行人像定位
    Face_Result_E FaceDetection(bool falgs);
    //向容器里面注册一个人像信息
    void checkAddFaceItem(Mat& face, int64_t i, Mat& frame);
    //保存人像结果信息
    Face_Result_E saveCheckResult();
private:
    Check_Parameter_t mCheckParam;
    /*  boost::shared_ptr<VideFFmpegOperate> mDecipherFFmpeg;*/
    boost::shared_ptr<VideoOpenCVOperate> mDecipherOpenCV;
    boost::shared_ptr<CheckFaceOperatec> mCheckFace;
    boost::shared_ptr<XmlOperate> mXmlOperate;
    bool Continuous;
};


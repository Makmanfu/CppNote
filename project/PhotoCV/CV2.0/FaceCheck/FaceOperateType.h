
#pragma once
//对外输出接口参数
#ifndef DefoutType
#define DefoutType
typedef long long  int64_t; //定义一个长整型
typedef unsigned char uint8_t;
typedef void (*CallBackSchedule)(int64_t , int64_t);      //定义进度函数指针
typedef void (*CallBackLocate)(char*, char*, char*, int, double);           //定义定位结果函数指针

//输入的类型
typedef enum Import_Type
{
    IMPORT_UNKNOWN = -1,//init
    IMPORT_PICTURE = 1,//picture
    IMPORT_VIDEO = 2,//video
    IMPORT_TRAIN_SAMPLE = 3,
    IMPORT_OPENCV_VIDEO = 5,
} Import_Type_e;
#endif

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/condition.hpp>

typedef boost::function<void (int64_t, int64_t) > FunPresent;
typedef boost::function<void (char*, char*, char*, int, double) > FunLocateInfo;

inline WCHAR* AnsiToUnicode(const char* szStr)
{
    int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, NULL, 0);
    if (nLen == 0)
        return NULL;
    WCHAR* pResult = new wchar_t[nLen];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szStr, -1, pResult, nLen);
    return pResult;
}

inline char* UnicodeToAnsi(const WCHAR* szStr)
{
    int nLen = WideCharToMultiByte(CP_ACP, 0, szStr, -1, NULL, 0, NULL, NULL);
    if (nLen == 0)
        return NULL;
    char* pResult = new char[nLen];
    WideCharToMultiByte(CP_ACP, 0, szStr, -1, pResult, nLen, NULL, NULL);
    return pResult;
}

typedef enum Face_Result_Type
{
    RESULT_UNKNOWN = -1, //未知
    RESULT_SUCCESS = 0,//成功
    RESULT_FAILURE = 1,//失败
    RESULT_PARAMET_FAILURE_NULL = 4,//失败-参数为空
    RESULT_PARAMET_FULL_FAILURE = 5,//参数填充失败
    RESULT_XML_LOAD_FAILURE = 100,//失败-xml问题_load文件文件
    RESULT_XML_ROOT_FAILURE = 101,//失败xml没有找到root节点（这个xml文件是空的）
    RESULT_XML_RESOLVE_FAILURE = 102,//失败xml解析出现问题
    RESULT_XML_SAVE_FAILURE = 103,//保存时失败
    RESULT_XML_INSTALL_ELEMENT_FALURE = 104,//插入元素失败
    RESULT_XML_INSTALL_ATTRIBUTE_FALURE = 105,//插入属性失败
    RESULT_VIDEO_OPENG_FALURE = 110,//视频开打失败
    RESULT_VIDEO_READ_END = 111,//视频读取完
    RESULT_VIDEO_DECOD_FALURE = 112,//视频解码失败
    RESULT_CHECK_INIT_FALURE = 120,//check初始化失败
} Face_Result_E;

typedef enum Check_Face_Model
{
    MODEL_UNKNOWN = -1,
    MODEL_FACE = 1,
    MODEL_FACEANDLEFTEYE = 2,
    MODEL_FACEANDRIGHTEYE = 3,
    MODEL_FACEANDEYES = 4
} Check_Face_Model_t;

typedef enum Xml_Element_Type
{
    XML_ELEMENT_UNKNOWN = -1,
    XML_ELEMENT = 0,
    XML_TEXT = 1,
    XML_ATTRIBUT = 2,
    XML_MAIN_ROOT_NODE = 3,
    XML_ROOT_NODE = 4,
    XML_ELEMENT_NODE = 5,
    XML_ELEMENT_ATTRIBUTE_NODE = 6,
    XML_ELEMENT_TEXT_NODE = 7,
} Xml_Element_Type_e;

typedef boost::function<void (int64_t, int64_t) > FunPresent;
typedef boost::function<void (char*, char*, char*, int, double) > FunLocateInfo;
typedef boost::function<void (const char*, Xml_Element_Type_e) > FunXmlGetElement;
//日志级别的提示信息
static const char* KEYINFOPREFIX   = " Key: \n";
static const char* ERRORPREFIX = " Error: \n";
static const char* WARNINGPREFIX   = " Warning: \n";
static const char* INFOPREFIX      = " Info: \n";
static const int MAX_STR_LEN = 1024;
//日志级别枚举
typedef enum EnumLogLevel_e
{
    LogLevelAll = 0,    //所有信息都写日志
    LogLevelMid,        //写错误、警告信息
    LogLevelNormal,     //只写错误信息
    LogLevelStop        //不写日志
} EnumLogLevel;

typedef struct CheckParameter
{
    int VideoReadMode;
    int VideoReadSetp;
    int faceWidht;
    int faceHeight;
    CheckParameter()
        : VideoReadMode(0)
        , VideoReadSetp(0)
        , faceWidht(0)
        , faceHeight(0)
    {
    }
    void cleanData()
    {
        VideoReadSetp = -1;
        VideoReadMode = -1;
        faceWidht = -1;
        faceHeight = -1;
    }

} CheckParameter_t;

typedef struct VideoAndPictrueInfo
{
    int width;
    int height;
    int64_t VIDuration;
    int Channels;
    int vfps;
    int fps;
    VideoAndPictrueInfo()
        : width(0)
        , height(0)
        , VIDuration(0)
        , Channels(-1)
        , vfps(0)
        , fps(0)
    {
    }
    void cleanData()
    {
        width = -1;
        height = -1;
        VIDuration = -1;
        Channels = -1;
        vfps = 0;
        fps = 0;
    }
} VideoAndPictrueInfo_t;

typedef struct FaceItemInfo
{
    int64_t Frames;
    std::string  Facename;
    std::string FrameName;
    FaceItemInfo()
        : Frames(0)
        , Facename("")
        , FrameName("")
    {
    }
    void cleanData()
    {
        Frames = -1;
        Facename = "";
        FrameName = "";
    }
} FaceItemInfo_t;

typedef struct FaceSequenceInfo
{
    std::string projectName;
    std::string VideoPaht;
    std::string faceSavePath;
    std::string SequenSaveXmlPaht;
    Import_Type Importmodel;
    Check_Face_Model CheckModel;
    std::vector<FaceItemInfo> FaceIetmSeque;
    FaceSequenceInfo()
        : projectName("")
        , VideoPaht("")
        , faceSavePath("")
        , SequenSaveXmlPaht("")
        , Importmodel(IMPORT_UNKNOWN)
        , CheckModel(MODEL_UNKNOWN)
    {
    }
    void cleanData()
    {
        projectName = "";
        VideoPaht = "";
        faceSavePath = "";
        SequenSaveXmlPaht = "";
        Importmodel = IMPORT_UNKNOWN;
        CheckModel = MODEL_UNKNOWN;
        FaceIetmSeque.clear();
    }
} FaceSequenceInfo_t;

typedef struct TrainFaceInfo
{
    bool FileRead;
    std::string modelName;
    std::string Samplepaht;
    std::string samplename;
    std::string name;
    int label;
    std::string remark;
    std::vector<std::string> facepaht;
    TrainFaceInfo()
        : FileRead(false)
        , modelName("")
        , Samplepaht("")
        , samplename("")
        , name("")
        , label(-1)
        , remark("")
    {
    }
    void cleanData()
    {
        FileRead = false;
        modelName = "";
        Samplepaht = "";
        samplename = "";
        name = "";
        label = -1;
        remark = "";
        facepaht.clear();
    }
} TrainFaceInfo_t;

typedef struct ModelItemInfo
{
    std::string name;
    int label;
    std::string remark;
    ModelItemInfo()
    {
        name = "";
        label = -1;
        remark = "";
    }
    void cleanData()
    {
        name = "";
        label = -1;
        remark = "";
    }
} ModelItemInfo_t;

typedef struct ModelXmlInfo
{
    std::vector<ModelItemInfo_t> _modelItem;
    std::string modelPath;
    ModelXmlInfo()
    {
        modelPath = "";
    }
    void cleanData()
    {
        modelPath = "";
        _modelItem.clear();
    }
} ModelXmlInfo_t;

typedef struct Check_Parameter
{
    CheckParameter_t checkParameterInfo;
    VideoAndPictrueInfo_t  OriginalSourceInfo;
    FaceSequenceInfo    SequenceInfo;
    std::vector<char*> inputFilePaht;
    FunPresent cb;
    Check_Parameter()
    {
        inputFilePaht.clear();
        cb = NULL;
    }
    void cleanData()
    {
        checkParameterInfo.cleanData();
        OriginalSourceInfo.cleanData();
        SequenceInfo.cleanData();
        inputFilePaht.clear();
        cb = NULL;
    }
} Check_Parameter_t;

typedef struct Train_Parameter
{
    char* saveModelPaht;
    std::vector<TrainFaceInfo_t> FaceTrainInfo;
    Train_Parameter()
    {
        FaceTrainInfo.clear();
    }
    void cleanData()
    {
        FaceTrainInfo.clear();
    }
} Train_Parameter_t;

typedef struct Local_Parameter
{
    Import_Type_e _type;
    std::vector<std::string> inputFilePath;
    std::vector<ModelXmlInfo_t> mdItemInfo;
    char outFileXmlPaht[256];
    char outFilePicPaht[256];
    char videoFileName[256];
    char outPicFile[256];
    int VideoReadMode;
    int VideoChoose;
    int VideoReadSetp;
    FunPresent cb;
    FunLocateInfo cb1;
    Local_Parameter()
    {
        _type = IMPORT_UNKNOWN;
        inputFilePath.clear();
        mdItemInfo.clear();
        VideoReadMode = -1;
        VideoReadSetp = -1;
        VideoChoose = -1;
        cb = NULL;
        cb1 = NULL;
        memset(videoFileName, '\0', sizeof(videoFileName));
        memset(outFileXmlPaht, '\0', sizeof(outFileXmlPaht));
        memset(outFilePicPaht, '\0', sizeof(outFilePicPaht));
        memset(outPicFile, '\0', sizeof(outPicFile));
    }
    void cleanData()
    {
        _type = IMPORT_UNKNOWN;
        inputFilePath.clear();
        mdItemInfo.clear();
        VideoReadSetp = -1;
        VideoReadMode = -1;
        VideoChoose = -1;
        memset(videoFileName, '\0', sizeof(videoFileName));
        memset(outFileXmlPaht, '\0', sizeof(outFileXmlPaht));
        memset(outFilePicPaht, '\0', sizeof(outFilePicPaht));
        memset(outPicFile, '\0', sizeof(outPicFile));
    }
} Local_Parameter_t;

typedef struct FaceInfo
{
    int id;
    int h;
    int m;
    int s;
    int sec;
    std::string framPicName;
    std::string XmlPicPaht;
    std::string frameTime;
    std::string name;
    std::string explain;
    int label;
    std::string modelPaht;
    std::string picFullPaht;
    double reliability;

    FaceInfo()
    {
        id = 0;
        h = 0;
        m = 0;
        s = 0;
        sec = 0;
        frameTime = "";
        framPicName = "";
        name = "";
        explain = "";
        label = 0;
        modelPaht = "";
        picFullPaht = "";
        reliability = 0.0;
    }
} FaceInfo_t;



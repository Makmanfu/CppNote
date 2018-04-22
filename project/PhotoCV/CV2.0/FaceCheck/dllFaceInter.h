//----------------------------------------------------------------------
//
//          人脸识别引擎框架dll部分 C++输出接口文件
//          目前识别率有待改进 框架可不变
//          注意pas中的void** 类型的声明
//                                  2014-06-10    @阿甘整理
//
//----------------------------------------------------------------------

#pragma once
#ifndef FACEEXP
    #define FACEEXP extern "C" __declspec(dllexport)
#endif

//对外输出接口参数
#ifndef DefoutType
#define DefoutType
typedef long long  int64_t;         //定义一个长整型
typedef unsigned char uint8_t;
typedef void (*CallBackSchedule)(int64_t , int64_t);        //定义进度回调函数指针
typedef void (*CallBackLocate)(char*, char*, char*, int, double);           //定义定位结果回调函数指针

//输入的类型
typedef enum Import_Type
{
    IMPORT_UNKNOWN = -1,    //init
    IMPORT_PICTURE = 1,           //picture
    IMPORT_VIDEO = 2,               //video
    IMPORT_TRAIN_SAMPLE = 3,
    IMPORT_OPENCV_VIDEO = 5,
} Import_Type_e;
#endif

//创建人像库对象，返回void*的一个指针，以更上层程序保存
FACEEXP void* Face_CreateOperateEx();
//上层应用要释放人像库对象
FACEEXP void Face_ReleaseOperateEx(void** facetrain);
//初始化人像库
FACEEXP bool Face_initOperateEx(void* facetrain);
//检测视频或图片中的人像
FACEEXP bool Face_CheckOperateEx(void* facetrain,           //创建好的人像库指针
                                 Import_Type_e _type,             //输入文件的类型 e.IMPORT_PICTURE，IMPORT_VIDEO
                                 char* CheckVideoFilePath,     //输入视频文件的路径 e.d:\\a.mp4
                                 char* ParamXmlBuf,              //输入的参数列表，是一个xml结构的内存buf
                                 int ParamBufLean,                 //参数列表的长度
                                 char* PicturePath,                 //输入的图片文件的路径 e.d:\\a.jpg
                                 char* OutPicturePath,           //将查找到人像输出到这个目录里 e.[d:\\temp\\a\\]
                                 char* OutXmlPath,                //人像检测完之后将结果统计成一个xml文件，将这个文件保存到这个里 e.d:\\temp\\a.xml
                                 CallBackSchedule cb);            //回调函数，返回检测进度

//对人像进行训练
FACEEXP bool Face_TrainOperateEx(void* facetrain,             //创建好的人像库指针
                                 char* xmlPath,                       //进行人工分类之后的结果的描述文件，可以是多个，中间有分号隔开
                                 char* outModelNamePaht);   //将模型和模型的描述文件，放到这个目录下

//定位视频中的人像，并返回定位结果。
FACEEXP bool Face_LocateOperateEx(void* facetrain,          //创建好的人像库指针
                                  Import_Type_e _type,            //输入文件的类型 e.IMPORT_PICTURE，IMPORT_VIDEO
                                  char* modlePath,                  //训练好的模型描述文件，可以是多个，中间用分号隔开
                                  char* videoPath,                   //要定位的视频路径
                                  char* picturePath,                //要定位的图片路径
                                  char* ParamXmlBuf,             //参数列表，xml文件结构的内存
                                  int ParamBufLean,                //参数列表的长度
                                  char* outFilePath,
                                  CallBackLocate cb,               //回调函数，返回定位结果
                                  CallBackSchedule cb1);


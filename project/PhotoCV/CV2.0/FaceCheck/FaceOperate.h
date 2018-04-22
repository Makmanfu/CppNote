#pragma once
#include "stdafx.h"
#include "FaceOperateType.h"
#include "FaceEngine.h"

//这个类是完成各各类之间的调度问题
class OperateEngine
{
public:
    OperateEngine(void);
    ~OperateEngine(void);
public:
    //初始化
    Face_Result_E initEngine();
    //检测
    Face_Result_E checkFace(Check_Parameter_t& Par);
    //训练
    Face_Result_E TrainFace(Train_Parameter_t& Par);
    //定位
    Face_Result_E LocateFae(Local_Parameter_t& Par);
protected:
    class imple;
    imple* _this;
};

//识别操作应用类
class FaceOperateClass
{
public:
    FaceOperateClass(void);
    ~FaceOperateClass(void);
    //进行初始化 返回RESULT_SUCCESS 表示成
    Face_Result_E initFaceOperate();
    //人像检测相关操作
    Face_Result_E FaceCheckOpertae(Import_Type_e _type, char* CheckVideoFilePath, char* ParamXmlBuf, int ParamBufLean, char* PicturePath, char* OutPicturePath, char* OutXmlPath, CallBackSchedule cb);
    //人像训练相关操作
    Face_Result_E TrainFaceOpertae(char* xmlPath, char* outModelNamePaht);
    //人像识别相关操作
    Face_Result_E LocateFaceOpertae(Import_Type_e _type, char* modlePath, char* videoPath, char* picturePaht, char* ParamXmlBuf, int ParamBufLean, char* outFilePath, CallBackLocate cb, CallBackSchedule cb1);
private:
    //检测回调函数
    void checkSchedule(int64_t a, int64_t b);
    //定位进度情况回调函数
    void localSchedule(int64_t a, int64_t b);
    //定位结果回调函数
    void local(char* name, char* remark, char* modelPath, int label, double confidence);
    //字符拆分
    vector<string> split(string str, string pattern);
    //检测参数列表封装
    Face_Result_E checkFullParameter(Import_Type_e _type, char* CheckVideoFilePath, char* ParamXmlBuf, int ParamBufLean, char* PicturePath, char* OutPicturePath, char* OutXmlPath);
    //读取xml文件
    bool readXmlParameter(char* xmlBuf, int lange);
    //xmloperate类的元素回调函数
    void checkXmlText(const char* text, Xml_Element_Type_e _type);
    //训练参数列表封装
    Face_Result_E trainFullParameter(char* xmlPath, char* outModelNamePath);
    //读取训练参数列表xml
    bool trainReadXmlParameter(char* xmlPath, int lange);
    //xml元素回调函数
    void trainXmlText(const char* text, Xml_Element_Type_e _type);
    //定位参数列表封装
    Face_Result_E localFullParameter(Import_Type_e _type, char* modlePath, char* videoPath, char* picturePaht, char* ParamXmlBuf, int ParamBufLean, char* outFilePath);
    //读取定位参数xml
    bool localReadXmlParameter(char* xmlPaht, int lange);
    //xml元素回调函数
    void localXmlText(const char* text, Xml_Element_Type_e _type);
private:
    vector<string> vec;
    OperateEngine* mEngine;
    CallBackSchedule mCheckSchedule;
    CallBackSchedule mLocateSchedule;
    CallBackLocate mLocate;
    Check_Parameter_t mCheckParameter;
    Train_Parameter_t mTrainParameter;
    Local_Parameter_t mLocalParameter;
    XmlOperate* mCheckXmlOperate;
    XmlOperate* mTrainXmlOperate;
    XmlOperate* mLocateXmlOperate;
};

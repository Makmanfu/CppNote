#pragma once
#include "stdafx.h"
#include "FaceOperateType.h"
#include "FaceEngine.h"

//���������ɸ�����֮��ĵ�������
class OperateEngine
{
public:
    OperateEngine(void);
    ~OperateEngine(void);
public:
    //��ʼ��
    Face_Result_E initEngine();
    //���
    Face_Result_E checkFace(Check_Parameter_t& Par);
    //ѵ��
    Face_Result_E TrainFace(Train_Parameter_t& Par);
    //��λ
    Face_Result_E LocateFae(Local_Parameter_t& Par);
protected:
    class imple;
    imple* _this;
};

//ʶ�����Ӧ����
class FaceOperateClass
{
public:
    FaceOperateClass(void);
    ~FaceOperateClass(void);
    //���г�ʼ�� ����RESULT_SUCCESS ��ʾ��
    Face_Result_E initFaceOperate();
    //��������ز���
    Face_Result_E FaceCheckOpertae(Import_Type_e _type, char* CheckVideoFilePath, char* ParamXmlBuf, int ParamBufLean, char* PicturePath, char* OutPicturePath, char* OutXmlPath, CallBackSchedule cb);
    //����ѵ����ز���
    Face_Result_E TrainFaceOpertae(char* xmlPath, char* outModelNamePaht);
    //����ʶ����ز���
    Face_Result_E LocateFaceOpertae(Import_Type_e _type, char* modlePath, char* videoPath, char* picturePaht, char* ParamXmlBuf, int ParamBufLean, char* outFilePath, CallBackLocate cb, CallBackSchedule cb1);
private:
    //���ص�����
    void checkSchedule(int64_t a, int64_t b);
    //��λ��������ص�����
    void localSchedule(int64_t a, int64_t b);
    //��λ����ص�����
    void local(char* name, char* remark, char* modelPath, int label, double confidence);
    //�ַ����
    vector<string> split(string str, string pattern);
    //�������б��װ
    Face_Result_E checkFullParameter(Import_Type_e _type, char* CheckVideoFilePath, char* ParamXmlBuf, int ParamBufLean, char* PicturePath, char* OutPicturePath, char* OutXmlPath);
    //��ȡxml�ļ�
    bool readXmlParameter(char* xmlBuf, int lange);
    //xmloperate���Ԫ�ػص�����
    void checkXmlText(const char* text, Xml_Element_Type_e _type);
    //ѵ�������б��װ
    Face_Result_E trainFullParameter(char* xmlPath, char* outModelNamePath);
    //��ȡѵ�������б�xml
    bool trainReadXmlParameter(char* xmlPath, int lange);
    //xmlԪ�ػص�����
    void trainXmlText(const char* text, Xml_Element_Type_e _type);
    //��λ�����б��װ
    Face_Result_E localFullParameter(Import_Type_e _type, char* modlePath, char* videoPath, char* picturePaht, char* ParamXmlBuf, int ParamBufLean, char* outFilePath);
    //��ȡ��λ����xml
    bool localReadXmlParameter(char* xmlPaht, int lange);
    //xmlԪ�ػص�����
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

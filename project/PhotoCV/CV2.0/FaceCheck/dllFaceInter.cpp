
#include "stdafx.h"
#include "dllFaceInter.h"
#include "FaceOperate.h"

//���д���
void* Face_CreateOperateEx()
{
    FaceOperateClass* _p = new FaceOperateClass();
    return _p;
}

//��������
void Face_ReleaseOperateEx(void** facetrain)
{
    FaceOperateClass* p = static_cast<FaceOperateClass*>(*facetrain);
    delete p;
    *facetrain = 0;
}

//���г�ʼ��
bool Face_initOperateEx(void* facetrain)
{
    FaceOperateClass* p = static_cast<FaceOperateClass*>(facetrain);
    if (p->initFaceOperate() == RESULT_SUCCESS)
        return true;
    else
        return false;
}

//���м��
bool Face_CheckOperateEx(void* facetrain, Import_Type_e _type, char* CheckVideoFilePath, char* ParamXmlBuf, int ParamBufLean, char* PicturePath, char* OutPicturePath, char* OutXmlPath, CallBackSchedule cb)
{
    FaceOperateClass* p = static_cast<FaceOperateClass*>(facetrain);
    if (p->FaceCheckOpertae(_type, CheckVideoFilePath, ParamXmlBuf, ParamBufLean, PicturePath, OutPicturePath, OutXmlPath, cb) == RESULT_SUCCESS)
        return true;
    else
        return false;
}

//����ѵ��
bool Face_TrainOperateEx(void* facetrain, char* xmlPath, char* outModelNamePaht)
{
    FaceOperateClass* p = static_cast<FaceOperateClass*>(facetrain);
    if (p->TrainFaceOpertae(xmlPath, outModelNamePaht) == RESULT_SUCCESS)
        return true;
    else
        return false;
}

//���ж�λ
bool Face_LocateOperateEx(void* facetrain, Import_Type_e _type, char* modlePath, char* videoPath, char* picturePaht, char* ParamXmlBuf, int ParamBufLean, char* outFilePath, CallBackLocate cb, CallBackSchedule cb1)
{
    FaceOperateClass* p = static_cast<FaceOperateClass*>(facetrain);
    if (p->LocateFaceOpertae(_type, modlePath, videoPath, picturePaht, ParamXmlBuf, ParamBufLean, outFilePath, cb, cb1) == RESULT_SUCCESS)
        return true;
    else
        return false;
}


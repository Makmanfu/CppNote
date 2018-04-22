//----------------------------------------------------------------------
//
//          ����ʶ��������dll���� C++����ӿ��ļ�
//          Ŀǰʶ�����д��Ľ� ��ܿɲ���
//          ע��pas�е�void** ���͵�����
//                                  2014-06-10    @��������
//
//----------------------------------------------------------------------

#pragma once
#ifndef FACEEXP
    #define FACEEXP extern "C" __declspec(dllexport)
#endif

//��������ӿڲ���
#ifndef DefoutType
#define DefoutType
typedef long long  int64_t;         //����һ��������
typedef unsigned char uint8_t;
typedef void (*CallBackSchedule)(int64_t , int64_t);        //������Ȼص�����ָ��
typedef void (*CallBackLocate)(char*, char*, char*, int, double);           //���嶨λ����ص�����ָ��

//���������
typedef enum Import_Type
{
    IMPORT_UNKNOWN = -1,    //init
    IMPORT_PICTURE = 1,           //picture
    IMPORT_VIDEO = 2,               //video
    IMPORT_TRAIN_SAMPLE = 3,
    IMPORT_OPENCV_VIDEO = 5,
} Import_Type_e;
#endif

//�����������󣬷���void*��һ��ָ�룬�Ը��ϲ���򱣴�
FACEEXP void* Face_CreateOperateEx();
//�ϲ�Ӧ��Ҫ�ͷ���������
FACEEXP void Face_ReleaseOperateEx(void** facetrain);
//��ʼ�������
FACEEXP bool Face_initOperateEx(void* facetrain);
//�����Ƶ��ͼƬ�е�����
FACEEXP bool Face_CheckOperateEx(void* facetrain,           //�����õ������ָ��
                                 Import_Type_e _type,             //�����ļ������� e.IMPORT_PICTURE��IMPORT_VIDEO
                                 char* CheckVideoFilePath,     //������Ƶ�ļ���·�� e.d:\\a.mp4
                                 char* ParamXmlBuf,              //����Ĳ����б���һ��xml�ṹ���ڴ�buf
                                 int ParamBufLean,                 //�����б�ĳ���
                                 char* PicturePath,                 //�����ͼƬ�ļ���·�� e.d:\\a.jpg
                                 char* OutPicturePath,           //�����ҵ�������������Ŀ¼�� e.[d:\\temp\\a\\]
                                 char* OutXmlPath,                //��������֮�󽫽��ͳ�Ƴ�һ��xml�ļ���������ļ����浽����� e.d:\\temp\\a.xml
                                 CallBackSchedule cb);            //�ص����������ؼ�����

//���������ѵ��
FACEEXP bool Face_TrainOperateEx(void* facetrain,             //�����õ������ָ��
                                 char* xmlPath,                       //�����˹�����֮��Ľ���������ļ��������Ƕ�����м��зֺŸ���
                                 char* outModelNamePaht);   //��ģ�ͺ�ģ�͵������ļ����ŵ����Ŀ¼��

//��λ��Ƶ�е����񣬲����ض�λ�����
FACEEXP bool Face_LocateOperateEx(void* facetrain,          //�����õ������ָ��
                                  Import_Type_e _type,            //�����ļ������� e.IMPORT_PICTURE��IMPORT_VIDEO
                                  char* modlePath,                  //ѵ���õ�ģ�������ļ��������Ƕ�����м��÷ֺŸ���
                                  char* videoPath,                   //Ҫ��λ����Ƶ·��
                                  char* picturePath,                //Ҫ��λ��ͼƬ·��
                                  char* ParamXmlBuf,             //�����б�xml�ļ��ṹ���ڴ�
                                  int ParamBufLean,                //�����б�ĳ���
                                  char* outFilePath,
                                  CallBackLocate cb,               //�ص����������ض�λ���
                                  CallBackSchedule cb1);


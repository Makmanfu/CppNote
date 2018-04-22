//===================================================
//          ����opencv����㷨�о��͹����ȫ�����
//===================================================
#ifndef CVEXPORTS_H__
#define CVEXPORTS_H__
//++++++++++++������÷�ʽ+++++++++++++++++++++++++++++++
#ifndef SQ_C_API
    #define SQ_C_API extern "C" __declspec(dllexport)
#endif
#ifndef SQ_CPP_API
    #define SQ_CPP_API __declspec(dllexport)
#endif

//++++++++++++������Ͷ���+++++++++++++++++++++++++++++++
#include "CVExports_typedef.h"
//#if defined(_WIN64)
//#pragma comment(lib,"SQCV64.lib")
//#else
//#pragma comment(lib,"SQCV.lib")
//#endif

//++++++++++++�����Ľӿ�+++++++++++++++++++++++++++++++
//�������������opencv����
SQ_CPP_API class CExpMatTestOut
{
public:
    //�������
    SQ_CPP_API void examCheckFace(int argc, char** argv);
    //���˼��
    SQ_CPP_API void examCheckPep(int argc, char** argv);
    //��ʶ�Ƽ��
    SQ_CPP_API void examCheckSpeed(int argc, char** argv);
    //����ͼ��Ƶ
    SQ_CPP_API void examLight(const char* f_name, int f_type = 1);
    //��ͷ���Ʋ�����ʾ ��ͶӰ�任ͼ
    SQ_CPP_API void LoopArrowShow(void);
    //��ͷ���ƶ���ģ����ʾ
    SQ_CPP_API void TestLoopArrowShow(const char* f_name);
};





#endif
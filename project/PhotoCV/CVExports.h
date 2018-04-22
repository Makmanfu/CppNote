//===================================================
//          基于opencv库的算法研究和管理的全部输出
//===================================================
#ifndef CVEXPORTS_H__
#define CVEXPORTS_H__
//++++++++++++输出调用方式+++++++++++++++++++++++++++++++
#ifndef SQ_C_API
    #define SQ_C_API extern "C" __declspec(dllexport)
#endif
#ifndef SQ_CPP_API
    #define SQ_CPP_API __declspec(dllexport)
#endif

//++++++++++++输出类型定义+++++++++++++++++++++++++++++++
#include "CVExports_typedef.h"
//#if defined(_WIN64)
//#pragma comment(lib,"SQCV64.lib")
//#else
//#pragma comment(lib,"SQCV.lib")
//#endif

//++++++++++++输出类的接口+++++++++++++++++++++++++++++++
//测试例子输出无opencv类型
SQ_CPP_API class CExpMatTestOut
{
public:
    //人脸检测
    SQ_CPP_API void examCheckFace(int argc, char** argv);
    //行人检测
    SQ_CPP_API void examCheckPep(int argc, char** argv);
    //标识牌检测
    SQ_CPP_API void examCheckSpeed(int argc, char** argv);
    //光流图视频
    SQ_CPP_API void examLight(const char* f_name, int f_type = 1);
    //箭头绘制播放演示 逆投影变换图
    SQ_CPP_API void LoopArrowShow(void);
    //箭头绘制动画模拟演示
    SQ_CPP_API void TestLoopArrowShow(const char* f_name);
};





#endif

#ifndef TARGETVER_H
#define TARGETVER_H
//警告宏处理--------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
//包含文件----------------------------------------------------------------
//#include <SDKDDKVer.h>

//平台运行模式(注意xp模式无效 依然需要手动设置)---------------------------
#ifdef GUIRUN
    #pragma comment (linker, "/subsystem:windows")
#else
    #pragma comment (linker, "/subsystem:console")
#endif

#ifdef _WIN64
    //优化作用: 指定节对齐为16字节
    #pragma comment(linker, "/align:16")
    //优化作用: 合并节 将.data节和.rdata节合并到.text节(代码节)
    #pragma comment(linker, "/MERGE:.data=.text")
    #pragma comment(linker, "/MERGE:.rdata=.text")
#else
    //#pragma comment(linker, "/align:16")     //注意32位目前没有解决
#endif



//---------输出宏定义-----------------------------------------------------
#ifndef C_API
    #define C_API extern "C" __declspec(dllexport)
#endif
#ifndef CPP_API
    #define CPP_API __declspec(dllexport)
#endif
#ifndef NATIVE_MATHEXP
    #define NATIVE_MATHEXP C_API
#endif

//开启win7后视觉效果 Copy from MSDN---------------------------------------
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' \
 version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#endif

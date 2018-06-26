//*********************************************************************
//
//                  全局判断宏整理
//                                          @2015-08-21阿甘整理
//*********************************************************************
#ifndef CDEFINE_HPP
#define CDEFINE_HPP

#define _CRT_SECURE_NO_WARNINGS

//万能输出宏--------------------------------------------------------------
#ifndef C_API
#define C_API extern "C" __declspec(dllexport)
#endif
#ifndef CPP_API
#define CPP_API __declspec(dllexport)
#endif
#ifndef DLL_API
#define DLL_API CPP_API
#endif

//平台运行模式(默认命令行,GUIRUNC 图形化,HIDECMDRUN 隐藏界面)--------------
#ifdef CONSOLE                          //命令行
#pragma comment (linker, "/subsystem:console")
#endif
#ifdef HIDECMD                          //隐藏命令行
#pragma comment (linker, "/subsystem:windows /entry:mainCRTStartup")
#endif
#ifdef GUIRUNC                          //WIN图形化
    #pragma comment (linker, "/subsystem:windows")
#endif
#ifdef NATIVEC                          //原生入口修改main入口
    #pragma comment (linker, "/ENTRY:\"SQMain\"")
#endif
//全都不定义会报错
#if !defined(GUIRUNC) && !defined(NATIVEC) && !defined(CONSOLE) && !defined(HIDECMD)
#pragma message("error:没有定义入口选项[GUIRUNC/NATIVEC/CONSOLE/HIDECMD]编个飞机啊？")
#endif

//输出lib的自动处理-------------------------------------------------------
#ifdef _WIN64
#define PLIB(name) name "64"
#else
#define PLIB(name) name
#endif
#ifdef _DEBUG
#define DLIB(name) name "d"
#else
#define DLIB(name) name
#endif
//#pragma comment( lib,DLIB(PLIB("XXX")))

//控件的视觉效果 Copy from MSDN-------------------------------------------
#if defined _M_IX86
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
//开启win7后视觉效果 Copy from MSDN---------------------------------------
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

//指定节对齐为512字节
//#pragma comment(linker, "/align:512")
//合并节将.data节和.rdata节合并到.text节(代码节)
//#pragma comment(linker, "/merge:.data=.text")
//#pragma comment(linker, "/merge:.rdata=.text")

#endif



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@宏笔记@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//编译器判断方法
//MS VC++ 14.1 _MSC_VER = 1912 (Visual C++ 2017)
//MS VC++ 14.0 _MSC_VER = 1900 (Visual C++ 2015)
//MS VC++ 12.0 _MSC_VER = 1800 (Visual C++ 2013)
//MS VC++ 11.0 _MSC_VER = 1700 (Visual C++ 2012)
//MS VC++ 10.0 _MSC_VER = 1600 (Visual C++ 2010)
//MS VC++ 9.0 _MSC_VER = 1500 (Visual C++ 2008)
//MS VC++ 8.0 _MSC_VER = 1400 (Visual C++ 2005)
//MS VC++ 7.1 _MSC_VER = 1310
//MS VC++ 7.0 _MSC_VER = 1300
//MS VC++ 6.0 _MSC_VER = 1200 (VC6)
//MS VC++ 5.0 _MSC_VER = 1100
//例如
//#if _MSC_VER==1200        //vc6
//#else if _MSC_VER>=1600   //vc10以上
//#endif

//消除警告
//#pragma warning(disable:4786)
//消除使用_s的函数
//#define _CRT_SECURE_NO_WARNINGS
//链接库文件
//#pragma comment( lib,"SQCore_X64.lib" )

//warning LNK4068: 未指定 /MACHINE；默认设置为 X86
//在目标计算机设置为MachineX64就解决。


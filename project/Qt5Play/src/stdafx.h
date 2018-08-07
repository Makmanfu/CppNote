
#ifndef STDAFX_H_
#define STDAFX_H_
#define _CRT_SECURE_NO_WARNINGS         // 屏蔽一些警告
#define __STDC_CONSTANT_MACROS
#define WIN32_LEAN_AND_MEAN             // 从Windows头文件中排除极少使用的信息
#define GUIRUNC                         //入口选项 GUIRUNC/NATIVEC/CONSOLE/HIDECMD

//#include "CDefine.hpp"
// Windows 头文件:
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
// QT支持
#include "QPublic.h"

//ffmpeg head
extern "C"
{
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libswresample\swresample.h>
#include <libavutil\channel_layout.h>
#include <libavutil\common.h>
#include <libavutil\imgutils.h>
#include <libswscale\swscale.h>
#include <libavutil\imgutils.h>
#include <libavutil\opt.h>
#include <libavutil\mathematics.h>
#include <libavutil\samplefmt.h>
#include <libavutil\log.h>
#include <libavutil\time.h>

//#include <SDL2/SDL.h>
};
//ffmpeg链接库
#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")
#pragma comment(lib,"avdevice.lib")
#pragma comment(lib,"avfilter.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"postproc.lib")
#pragma comment(lib,"swresample.lib")
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"SDL2main.lib")




#endif

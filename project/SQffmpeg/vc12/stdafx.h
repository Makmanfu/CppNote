
#ifndef STDAFX_H
#define STDAFX_H

//编译开关----------------------------------------------------------------
#define GUIRUNC                                 //入口选项 GUIRUNC/CONSOLE/NATIVEC/HIDECMD
//包含文件----------------------------------------------------------------
#define __STDC_CONSTANT_MACROS
#include "targetver.h"
//#include <winsock2.h>
//#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "resource.h"

//ffmpeg head
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavfilter/avfilter.h"
#include "libswresample/swresample.h"
#include "libavutil/imgutils.h"
#include "libavutil/time.h"
#include "SDL2/SDL.h"
};
//ffmpeg链接库
#pragma comment(lib,"ffmpeg_static.lib")
#pragma comment(lib,"SDL2.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Secur32.lib")
#pragma comment(lib,"Bcrypt.lib")
#pragma comment(lib,"Version.lib")
#pragma comment(lib,"WINMM.lib")
#pragma comment(lib,"IMM32.lib")
//#pragma comment(lib,"mpeg.lib")



#endif
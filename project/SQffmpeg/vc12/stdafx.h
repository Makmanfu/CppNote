
#ifndef STDAFX_H
#define STDAFX_H

//���뿪��----------------------------------------------------------------
#define GUIRUNC                                 //���ѡ�� GUIRUNC/NATIVEC/CONSOLE/HIDECMD
//�����ļ�----------------------------------------------------------------
#define __STDC_CONSTANT_MACROS
#include "targetver.h"
//#include <winsock2.h>
//#include <windows.h>
#include <stdio.h>
#include <tchar.h>
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
//ffmpeg���ӿ�
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
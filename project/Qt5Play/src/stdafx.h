
#ifndef STDAFX_H_
#define STDAFX_H_
#define _CRT_SECURE_NO_WARNINGS         // ����һЩ����
#define __STDC_CONSTANT_MACROS
#define WIN32_LEAN_AND_MEAN             // ��Windowsͷ�ļ����ų�����ʹ�õ���Ϣ
#define GUIRUNC                         //���ѡ�� GUIRUNC/NATIVEC/CONSOLE/HIDECMD

//#include "CDefine.hpp"
// Windows ͷ�ļ�:
#include <windows.h>
// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
// QT֧��
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

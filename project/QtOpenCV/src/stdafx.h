
#ifndef STDAFX_H_
#define STDAFX_H_
#define _CRT_SECURE_NO_WARNINGS         // 屏蔽一些警告
#define WIN32_LEAN_AND_MEAN             // 从Windows头文件中排除极少使用的信息
#define _OPENCV3_                       // OPENCV支持
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
//opencv
#define OPENCV_INDLL              //OPENCV发布版
#include "OpenCVConf.h"





#endif

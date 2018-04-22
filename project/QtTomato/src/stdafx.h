
#ifndef STDAFX_H
#define STDAFX_H

//全局宏定义
#define _CRT_SECURE_NO_WARNINGS         //屏蔽一些警告
#define WIN32_LEAN_AND_MEAN             //从Windows头文件中排除极少使用的信息
#define SQC_API
#define SQCPP_API
#define GUIRUN

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <math.h>

// QT支持
#include "QPublic.h"                    //qt支持

//程序配置文件
#include "AppCFG.h"







#endif

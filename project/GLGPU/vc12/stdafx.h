
#ifndef STDAFX_H
#define STDAFX_H
#define GUIRUNC                                //WINDOW窗口运行


#include "targetver.h"
#include <windows.h>
//C运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <iostream>
#include <memory>
//c++运行时头文件
#include <vector>
using namespace std;
#include "resource.h"
//opengl
//#include "Glconf.h"

char* G2U(const char* gb2312);

#ifndef SQC_API
#define SQC_API extern "C" __declspec(dllexport)
#endif
#ifndef SQCPP_API
#define SQCPP_API __declspec(dllexport)
#endif


#endif
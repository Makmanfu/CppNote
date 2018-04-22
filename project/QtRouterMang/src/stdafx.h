
#ifndef STDAFX_H_
#define STDAFX_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 屏蔽SOCK警告
#define _CRT_SECURE_NO_WARNINGS         // 屏蔽一些警告
#define WIN32_LEAN_AND_MEAN             // 从Windows头文件中排除极少使用的信息
#define SQC_API
#define SQCPP_API

#define GUIRUN
//#include "CDefine.hpp"
//Windows 头文件:  socket包含顺序
#include <WinSock2.h>
#include <Windows.h>
//C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>
#include <math.h>
//C++运行头文件
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
//QT支持
#include "QPublic.h"

//程序配置文件
#include "Cinicfg.h"


#endif

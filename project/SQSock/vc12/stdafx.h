
#ifndef STDAFX_H
#define STDAFX_H
//编译开关----------------------------------------------------------------
#define GUIRUN                                  //界面开关宏

//包含文件----------------------------------------------------------------
#include "targetver.h"
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
//SOCK通信支持------------------------------------------------------------
//#define _CRT_SECURE_NO_WARNINGS                 //消除使用_s的函数
//#define _WINSOCK_DEPRECATED_NO_WARNINGS         //消除socket问题
#include <iostream>
#include <WinSock2.h>                           //包含顺序可解决重定义问题
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")               //链接库
#include <windowsx.h>
#include <memory>

#include "coondef.h"



#endif
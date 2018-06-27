//*************************************************************************
//
//                   注意本库在exe模式调用有部分问题
//                                          @2018-06-26阿甘整理
//
//*************************************************************************

// 注意本库在exe模式调用有部分问题
#ifndef STDAFX_H
#define STDAFX_H
//编译开关----------------------------------------------------------------
#define CONSOLE                                 //入口选项 GUIRUNC/NATIVEC/CONSOLE/HIDECMD

//包含文件----------------------------------------------------------------
#include "CDefine.hpp"
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
//SOCK通信支持------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS                 //消除使用_s的函数
#define _WINSOCK_DEPRECATED_NO_WARNINGS         //消除socket问题
#include <iostream>
#include <WinSock2.h>                           //包含顺序可解决重定义问题
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")               //链接库
#include <windowsx.h>
#include <memory>
#include "coondef.h"                            //通信数据结构定
//输出宏定义
#ifndef SQC_API
#define SQC_API C_API
#endif
#ifndef SQCPP_API
#define SQCPP_API CPP_API
#endif




#endif




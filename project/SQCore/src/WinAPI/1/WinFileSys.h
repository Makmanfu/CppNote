//-----------------------------------------------------------------------------
//
//                  windows 文件系统api总结算法
//                                          @2015-05-10阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "WinFileSys_typedef.h"



//-------------------------------测试方便使用--------------------------------------
//显示char*
SQC_API void ShowBox(const char* f_txt);
//显示string
SQC_API void ShowBoxStr(string& f_txt);
//显示int
SQC_API void ShowBoxInt(int f_txt);
//---------------------------------------------------------------------------------
//得到系统所有驱动器
SQC_API void GetAllDisk(vector<string>& AllDrv);
//路径格式化成标准windows路径[c:\test]
SQC_API void PathFormat(string& path, int idtype = 0);
//检测路径或文件是否存在
SQC_API bool CheckPathFile(const string& pathfile);
//获取当前程序路径信息[idtype:0-全路径,1-程序名,2-路径c:\test ]
SQC_API string GetExePath(int idtype = 0);
//获取当前程序路径信息[idtype:0-全路径,1-程序名,2-路径c:\test ]
//SQC_API string GetExePathC(int idtype = 0);
//根据路径获得下所有内容[0递归全部,1递归文件夹,2递归文件,3当前文件夹,4当前文件]
SQC_API int GetDirFiles(const string& path, vector<string>& AllDir, DIRTYPE idtype = DALL);
//根据路径创建文件夹[绝对路径] 等价于MakeSureDirectoryPathExists
SQC_API bool CreatePathDir(const string& path);
//递归删除文件夹所有[路径用c:\test 速度已经优化]
SQC_API bool DelPathDir(const string& path);
//广度删除文件夹所有[路径用c:\test 速度已经优化]
SQC_API bool DelPathDirEx(const string& path);
//智能广度搜索文件
//智能广度搜索文件夹
//智能递归文件
SQC_API bool SmartFile(const string& path, FileProc fp = &ShowBoxStr);
//智能递归文件夹
SQC_API bool SmartDir(const string& path, FileProc fp = &ShowBoxStr);
//递归删除空文件夹
SQC_API bool DelEmptyDir(const string& path);
//修改属性
SQC_API void SetAtt(const string& pathfile);
//修改名称
SQC_API void ReNameDir(void);
//修改文件时间
SQC_API void SetFilesTime(void);

/*
//得到当前桌面路径
SQC_API BOOL GetDesktopPath(char* pszDesktopPath);
//创建快捷方式[对象全路径,生成path,生成名称,描述]

SQC_API BOOL ShortLink(LPCSTR lpszWorkDir, LPCSTR lpszDirName, int iShowCmd = SW_SHOWNORMAL);

//函数功能：对指定文件在指定的目录下创建其快捷方式
//函数参数：
//lpszFileName    指定文件，为NULL表示当前进程的EXE文件。
//lpszLnkFileDir  指定目录，不能为NULL。
//lpszLnkFileName 快捷方式名称，为NULL表示EXE文件名。
//wHotkey         为0表示不设置快捷键
//pszDescription  备注
//iShowCmd        运行方式，默认为常规窗口

SQC_API BOOL CreateFileShortcut(LPCSTR lpszFileName, LPCSTR lpszLnkFileDir, LPCSTR lpszLnkFileName,
                                LPCSTR lpszWorkDir, WORD wHotkey, LPCSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL);

*/


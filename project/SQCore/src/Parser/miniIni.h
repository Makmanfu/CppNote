//*************************************************************************
//
//                  windows 基本文件读写和日志测试操作
//
//                                       @阿甘整理于 2015-03-10
//
//*************************************************************************
#ifndef miniIni_H
#define miniIni_H
#include <string>
using namespace std;

//-------------------------------测试方便使用--------------------------------------
//显示char*
SQC_API void ShowBox(const char* f_txt);
//显示string
SQC_API void ShowBoxStr(string& f_txt);
//显示int
SQC_API void ShowBoxInt(int f_txt);
//---------------------------------------------------------------------------------
//读ini文件 返回字符[根,键,默认值,ini文件]
SQC_API string ReadINI(string inode, string ikey, string ivalue, string ipathfile = "./cfg.ini");
//读ini文件 返回int
SQC_API int RiniInt(string inode, string ikey, int ivalue, string ipathfile = "./cfg.ini");
//读ini文件 返回float
SQC_API float RiniFloat(string inode, string ikey, float ivalue, string ipathfile = "./cfg.ini");
//写INI文件[文件,根,键,值]
SQC_API void WriteINI(string inode, string ikey, string ivalue, string ipathfile = "./cfg.ini");
//创建ini文件
SQC_API bool CreateIni(string ipathfile = "./cfg.ini");
//追加写日志[cmd=1仅命令行 2都有]  (无锁 写日志)
SQC_API int LogFun(const char* f_txt, const char* f_logpath = "app.log", int cmd = 0);

//INI操作类
class SQCPP_API Ciniop
{
};





#endif
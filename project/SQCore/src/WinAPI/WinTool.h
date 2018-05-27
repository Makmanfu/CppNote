//-----------------------------------------------------------------------------
//
//                  windows 系统操作总结
//                  注册表 获得硬件信息 时间等
//                                          @2014-03-10阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

class SQCPP_API WinTool
{
    static WinTool* _instance;
    WinTool();
public:
    ~WinTool();
    static WinTool* Instance();
public:
    //程序唯一性检查
    bool RunCheckOnly(char* f_tname = "app");
    //程序自启动注册表
    void SetAutoRun(LPSTR RegName, bool fsetrun = true);
    //程序签名58字节 +52字节[ver-verr 00-000]
    void SignFlagEXE(const char* exename, int ver = 0, int verr = 0);
};





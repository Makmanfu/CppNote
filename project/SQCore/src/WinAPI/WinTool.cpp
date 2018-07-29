
#include "stdafx.h"
#include "WinTool.h"
#include <io.h>
#include <windows.h>
#include <winreg.h>
#include <ctime>
#include <fstream>
#include <TlHelp32.h>
#include <sys/timeb.h>
#include <climits>

WinTool* WinTool::_instance = 0;

WinTool::WinTool()
{
}

WinTool::~WinTool()
{
}

WinTool* WinTool::Instance()
{
    if (NULL == _instance)
        _instance = new WinTool();
    return _instance;
}

bool WinTool::RunCheckOnly(char* f_tname)
{
    //程序运行一个实例
    static HANDLE SQ_variable_hMutex;
    //程序实例检查
    SQ_variable_hMutex = CreateMutexA(NULL, true, f_tname);
    if ((GetLastError() == ERROR_ALREADY_EXISTS) || (GetLastError() == ERROR_INVALID_HANDLE))
        return false;   //ShowBox("该程序已在运行");
    return true;
}

void WinTool::SetAutoRun(LPSTR RegName, bool fsetrun /*= true*/)
{
    if (!strcmp(RegName, ""))
        RegName = "AutoRun";
    char bufname[260];
    GetModuleFileNameA(NULL, bufname, 260);  //获得路径 此时格式是c:\1\2.exe
    HKEY sub;
    if (fsetrun)
    {
        //用这个也行//HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run
        ::RegCreateKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &sub);
        ::RegSetValueExA(sub, RegName, NULL, REG_SZ, (BYTE*)bufname, 260);      //设置开机运行
    }
    else
    {
        ::RegCreateKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &sub);
        ::RegDeleteValueA(sub, RegName);                                        //取消开机运行
    }
}

void WinTool::SignFlagEXE(const char* exename, int ver, int verr)
{
    //printf("%s文件打开!\n", exename);
    std::fstream iofile(exename, ios::in | ios::out | ios::binary);    //二进制覆写
    if (iofile)   //判断打开成功
    {
        SYSTEMTIME CurTime;
        GetLocalTime(&CurTime);
        char* buf = (char*)malloc(58 * sizeof(char));
        sprintf(buf, "@BUILDING FOR AGAN!$MSVC%02d-%02d-%03d %04d-%02d-%02d %02d:%02d:%02d:%03d\n",
            _MSC_VER / 100, ver, verr, CurTime.wYear, CurTime.wMonth, CurTime.wDay, CurTime.wHour,
            CurTime.wMinute, CurTime.wSecond, CurTime.wMilliseconds);
        iofile.seekp(2, ios::beg);                  //寻找位置
        iofile.write(buf, 58/*sizeof(buf)*/);       //头部
        char* buf2 = (char*)malloc(52 * sizeof(char));
        sprintf(buf2, "WHATEVER IS WORTH DOING IS WORTH DOING WELL! PROGRAM\n");
        iofile.seekp(64, ios::beg);                 //寻找位置
        iofile.write(buf2, 52/*sizeof(buf)*/);      //头部
        iofile.close();
        free(buf);
        free(buf2);
    }
    else {
        printf("%s文件打开失败!\n", exename);
        //MessageBoxA(NULL, "error", "dll", MB_OK);
    }
    //main中调用
    //if (argc > 1)
    //    SignFlagEXE(argv[1], 0, 1);
}










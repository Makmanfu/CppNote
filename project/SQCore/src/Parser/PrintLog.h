//*************************************************************************
//
//  PrintLog适合全局保存日志 分析问题 可线程打印日志(IO处理过)
//  Logger提供写日志功能，支持多线程，支持可变形参数操作，支持写日志级别的设置
//  TextFile BinFile 文件操作例子
//                                       @阿甘整理于 2018-03-23
//
//*************************************************************************

#ifndef PRINTLOG_H
#define PRINTLOG_H

#include <string>
using namespace std;
#include <windows.h>

//日志工具
class SQCPP_API PrintLog
{
public:
    //PrintLog用法例子
    static int main(int argc, char** argv)
    {
        //使用方法
        PrintLog slog;
        char s_szBuffer[512];
        for (size_t i = 1; i < 101; i++)
        {
            sprintf(s_szBuffer, "showlog %d \n", i);
            slog.PrintToLog(s_szBuffer);
        }
        //PrintLog* slog2 = new PrintLog("./");
        //slog2->PrintToLog("showlog2 \n");
        //delete slog2;
        return 1;
        /*
            //使用方法
            PrintLog slog("c:\\");
            while (1)
                slog.PrintToLog("showlog \n");
            //简单打日志例子
            for (int i = 0; i < 100; i++)
            LogFun("简单日志", "logApp.log", 2);
            //打日志  注意PrintLog类要放在什么生命周期里
            PrintLog slogFast;
            slogFast.CreateLogFilePath(GetExeNamePath().c_str());
            for (int i = 0; i < 10000; i++)
            slogFast.PrintToLog(" 线程日志%d\n", i);
            slogFast.CreateLogFilePath(".\\Log\\one\\");
            slogFast.LogCmdOnOff(false);
            for (int i = 0; i < 10000; i++)
            slogFast.PrintToLog(" 目录中线程日志%d\n", i);
        */
    };
private:
    struct Log_File
    {
        enum { MAX_FILE_NAME = 256 };
        FILE* m_File;
        char m_FileName[MAX_FILE_NAME];         //char* m_FileName;
    } LogFile;                                  //日志文件
    /* 锁 */
    struct TraceMutex
    {
        HANDLE m_hMutex;
        DWORD m_timeout;
    } Loglocker;// = {0, 30000};
    char s_szBuffer[512];
    bool outtofile;                             //日志是否写入到文件
    bool ourtocmd;                              //日志是否打印到命令行
    bool traceInit;                             //锁
public:
    PrintLog(const char* f_pathfile = "./");
    virtual ~PrintLog(void);
public:
    //设置路径并创建 f_pathfile尾需带斜杠
    bool CreateLogFilePath(const char* f_pathfile);
    //写日志开关
    bool LogFileOnOff(bool f_enable = true);
    //命令行显示开关
    bool LogCmdOnOff(bool f_enable = true);
    //写日志
    void ToTerminal(const char* pszText);
    void PrintToLog(const char* fmt, ...);
private:
    //文件操作
    bool OpenLogFile(Log_File* pFile);
    bool WriteToLogFile(Log_File* pFile, const char* pTxt);
    bool CloseLogFile(Log_File* pFile);

    bool TraceMutexInit(TraceMutex* pMutex, wchar_t* pName);
    bool TraceMutexDestroy(TraceMutex* pMutex);
    bool TraceMutexLock(TraceMutex* pMutex);
    bool TraceMutexUnLock(TraceMutex* pMutex);
    void OutputLock(void);
    void OutputUnlock(void);
};

//日志打印工具 
class SQCPP_API Logger
{
public:
    //Logger用法例子
    static int main(int argc, char** argv)
    {
        char szAppPath[255];
        GetModuleFileNameA(NULL, szAppPath, 255);
        (strrchr(szAppPath, '\\'))[0] = 0;
        Logger logtest(szAppPath);
        for (size_t i = 1; i < 1001; i++)
        {
            logtest.TraceError("Error--------");
            logtest.TraceInfo("show-----");
            logtest.TraceKeyInfo("guanjian");
            logtest.TraceWarning("warning--------");
            //Sleep(100);
        }
        return 1;
    };
private:
    //日志级别的提示信息
    char* KEYINFOPREFIX, *ERRORPREFIX, *WARNINGPREFIX, *INFOPREFIX;
    static const int MAX_STR_LEN = 1024;
    //日志级别枚举
    typedef enum EnumLogLevel_e
    {
        LogLevelAll = 0,    //所有信息都写日志
        LogLevelMid,        //写错误、警告信息
        LogLevelNormal,     //只写错误信息
        LogLevelStop        //不写日志
    } EnumLogLevel;
    //写日志文件流
    FILE* m_pFileStream;
    //写日志级别
    EnumLogLevel m_nLogLevel;
    //日志的路径
    char m_strLogPath[MAX_STR_LEN];
    //日志的名称
    char m_strCurLogName[MAX_STR_LEN];
    //线程同步的临界区变量
    CRITICAL_SECTION m_cs;
public:
    Logger();
    //日志全路径,等级
    Logger(const char* strLogPath, EnumLogLevel nLogLevel = LogLevelAll);
    //析构函数
    virtual ~Logger();
public:
    //写关键信息 忽略日志级别
    void TraceKeyInfo(const char* strInfo, ...);
    //写错误信息
    void TraceError(const char* strInfo, ...);
    //写警告信息
    void TraceWarning(const char* strInfo, ...);
    //写一般信息
    void TraceInfo(const char* strInfo, ...);
    //设置写日志级别
    void SetLogLevel(EnumLogLevel nLevel);
private:
    //写文件操作
    void Trace(const char* strInfo);
    //获取当前系统时间
    char* GetCurrentTimeE(void);
    //创建日志文件名称
    void GenerateLogName();
    //创建日志路径
    void CreateLogPath();
};


#endif
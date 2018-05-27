//*************************************************************************
//
//  PrintLog�ʺ�ȫ�ֱ�����־ �������� ���̴߳�ӡ��־(IO�����)
//  Logger�ṩд��־���ܣ�֧�ֶ��̣߳�֧�ֿɱ��β���������֧��д��־���������
//  TextFile BinFile �ļ���������
//                                       @���������� 2018-03-23
//
//*************************************************************************

#ifndef PRINTLOG_H
#define PRINTLOG_H

#include <string>
using namespace std;
#include <windows.h>

//��־����
class SQCPP_API PrintLog
{
public:
    //PrintLog�÷�����
    static int main(int argc, char** argv)
    {
        //ʹ�÷���
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
            //ʹ�÷���
            PrintLog slog("c:\\");
            while (1)
                slog.PrintToLog("showlog \n");
            //�򵥴���־����
            for (int i = 0; i < 100; i++)
            LogFun("����־", "logApp.log", 2);
            //����־  ע��PrintLog��Ҫ����ʲô����������
            PrintLog slogFast;
            slogFast.CreateLogFilePath(GetExeNamePath().c_str());
            for (int i = 0; i < 10000; i++)
            slogFast.PrintToLog(" �߳���־%d\n", i);
            slogFast.CreateLogFilePath(".\\Log\\one\\");
            slogFast.LogCmdOnOff(false);
            for (int i = 0; i < 10000; i++)
            slogFast.PrintToLog(" Ŀ¼���߳���־%d\n", i);
        */
    };
private:
    struct Log_File
    {
        enum { MAX_FILE_NAME = 256 };
        FILE* m_File;
        char m_FileName[MAX_FILE_NAME];         //char* m_FileName;
    } LogFile;                                  //��־�ļ�
    /* �� */
    struct TraceMutex
    {
        HANDLE m_hMutex;
        DWORD m_timeout;
    } Loglocker;// = {0, 30000};
    char s_szBuffer[512];
    bool outtofile;                             //��־�Ƿ�д�뵽�ļ�
    bool ourtocmd;                              //��־�Ƿ��ӡ��������
    bool traceInit;                             //��
public:
    PrintLog(const char* f_pathfile = "./");
    virtual ~PrintLog(void);
public:
    //����·�������� f_pathfileβ���б��
    bool CreateLogFilePath(const char* f_pathfile);
    //д��־����
    bool LogFileOnOff(bool f_enable = true);
    //��������ʾ����
    bool LogCmdOnOff(bool f_enable = true);
    //д��־
    void ToTerminal(const char* pszText);
    void PrintToLog(const char* fmt, ...);
private:
    //�ļ�����
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

//��־��ӡ���� 
class SQCPP_API Logger
{
public:
    //Logger�÷�����
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
    //��־�������ʾ��Ϣ
    char* KEYINFOPREFIX, *ERRORPREFIX, *WARNINGPREFIX, *INFOPREFIX;
    static const int MAX_STR_LEN = 1024;
    //��־����ö��
    typedef enum EnumLogLevel_e
    {
        LogLevelAll = 0,    //������Ϣ��д��־
        LogLevelMid,        //д���󡢾�����Ϣ
        LogLevelNormal,     //ֻд������Ϣ
        LogLevelStop        //��д��־
    } EnumLogLevel;
    //д��־�ļ���
    FILE* m_pFileStream;
    //д��־����
    EnumLogLevel m_nLogLevel;
    //��־��·��
    char m_strLogPath[MAX_STR_LEN];
    //��־������
    char m_strCurLogName[MAX_STR_LEN];
    //�߳�ͬ�����ٽ�������
    CRITICAL_SECTION m_cs;
public:
    Logger();
    //��־ȫ·��,�ȼ�
    Logger(const char* strLogPath, EnumLogLevel nLogLevel = LogLevelAll);
    //��������
    virtual ~Logger();
public:
    //д�ؼ���Ϣ ������־����
    void TraceKeyInfo(const char* strInfo, ...);
    //д������Ϣ
    void TraceError(const char* strInfo, ...);
    //д������Ϣ
    void TraceWarning(const char* strInfo, ...);
    //дһ����Ϣ
    void TraceInfo(const char* strInfo, ...);
    //����д��־����
    void SetLogLevel(EnumLogLevel nLevel);
private:
    //д�ļ�����
    void Trace(const char* strInfo);
    //��ȡ��ǰϵͳʱ��
    char* GetCurrentTimeE(void);
    //������־�ļ�����
    void GenerateLogName();
    //������־·��
    void CreateLogPath();
};


#endif
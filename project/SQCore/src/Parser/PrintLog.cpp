
#include "stdafx.h"
#include "printlog.h"
#include <time.h>
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

//#include "WinFileSys.h"

PrintLog::PrintLog(void)
{
    //��ʼ��
    memset(&LogFile, 0, sizeof(Log_File));
    LogFile.m_File = NULL;
    outtofile = true;
    ourtocmd = true;
    traceInit = true;
    Loglocker.m_hMutex = 0;
    Loglocker.m_timeout = 30000;
}

PrintLog::PrintLog(const char* f_pathfile) : PrintLog()
{
    CreateLogFilePath(f_pathfile);
}

PrintLog::~PrintLog(void)
{
    if (NULL != LogFile.m_File)
        CloseLogFile(&LogFile);
}

bool PrintLog::OpenLogFile(Log_File* pFile)
{
    size_t fileLen = strlen(pFile->m_FileName);
    if ((fileLen <= 0) || (fileLen >= Log_File::MAX_FILE_NAME) || (NULL != pFile->m_File))
        return false;
    pFile->m_File = fopen(pFile->m_FileName, "w+t");
    if (NULL == pFile->m_File)
        return false;
    return true;
}

bool PrintLog::WriteToLogFile(Log_File* pFile, const char* pTxt)
{
    if ((NULL == pFile) || (NULL == pTxt) || (NULL == pFile->m_File))
        return false;
    return (fputs(pTxt, pFile->m_File)) == 1 ? true : false;
}

bool PrintLog::CloseLogFile(Log_File* pFile)
{
    if (NULL == pFile)    return false;
    if (NULL != pFile->m_File)
    {
        fclose(pFile->m_File);
        pFile->m_File = NULL;
    }
    return true;
}

bool PrintLog::CreateLogFilePath(const char* f_pathfile)
{
    if ((NULL == f_pathfile) || (strlen(f_pathfile) >= Log_File::MAX_FILE_NAME - 64))
        return false;
    if (NULL != LogFile.m_File)
        CloseLogFile(&LogFile);                 //ִ��һ�ιر�
    SYSTEMTIME st;
    GetLocalTime(&st);
    //CreatePathDir(f_pathfile);                  //�����༶Ŀ¼
    MakeSureDirectoryPathExists(f_pathfile);
    sprintf_s(LogFile.m_FileName, Log_File::MAX_FILE_NAME - 1,
              "%sLOG_%04d%02d%02d%02d%02d%02d.log", f_pathfile, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    if (!OpenLogFile(&LogFile))
        return false;
    return true;
}

bool PrintLog::LogFileOnOff(bool f_enable)
{
    outtofile = f_enable;
    return true;
    //if (f_enable)
    //{
    //    if (NULL == LogFile.m_File)
    //        if (!OpenLogFile(&LogFile))
    //            return false;
    //    outtofile = f_enable;
    //}else{
    //    outtofile = f_enable;
    //    CloseLogFile(&LogFile);
    //}
    //return true;
}

bool PrintLog::LogCmdOnOff(bool f_enable)
{
    ourtocmd = f_enable;
    return true;
}

void PrintLog::ToTerminal(const char* pszText)
{
    if (ourtocmd)
        printf(pszText);
    if (outtofile)
        WriteToLogFile(&LogFile, pszText);
}

void PrintLog::PrintToLog(const char* fmt, ...)
{
    va_list argptr;
    char* szBuffer = s_szBuffer;    //(char*)malloc(512*sizeof(char));
    if (NULL == fmt)
        return;
    if (!traceInit)
    {
        //��init
        ///???TraceMutexInit(&Loglocker, TEXT("MY_CAMERA_LOG_DEBUG_OUTPUT_LOCKER"));
        traceInit = true;
    }
    //��begin
    TraceMutexLock(&Loglocker);
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(&szBuffer[0], "%04d-%02d-%02d %02d:%02d:%02d:%03d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    va_start(argptr, fmt);
    _vsnprintf(&szBuffer[24], 512 - 24, fmt, argptr);
    va_end(argptr);
    szBuffer[512 - 1] = 0;
    ToTerminal(szBuffer);
    //��end
    TraceMutexUnLock(&Loglocker);
}

bool PrintLog::TraceMutexInit(TraceMutex* pMutex, wchar_t* pName)
{
    if (NULL == pMutex)
        return false;
    pMutex->m_hMutex = CreateMutexW(NULL, FALSE, pName);
    if (NULL == pMutex->m_hMutex)
        return false;
    return true;
}

bool PrintLog::TraceMutexDestroy(TraceMutex* pMutex)
{
    if (NULL == pMutex)
        return false;
    if (NULL != pMutex->m_hMutex)
        CloseHandle(pMutex->m_hMutex);
    return true;
}

bool PrintLog::TraceMutexLock(TraceMutex* pMutex)
{
    bool bRetVal = false;
    DWORD  dwRet = 0;
    if (NULL == pMutex)
        return false;
    if (NULL == pMutex->m_hMutex)
        return false;
    dwRet = WaitForSingleObject(pMutex->m_hMutex, pMutex->m_timeout);
    switch (dwRet)
    {
        case WAIT_OBJECT_0:
        case WAIT_ABANDONED:    // mutex only
            bRetVal = true;
            break;
        case WAIT_TIMEOUT:
        case WAIT_FAILED:
        default:
            bRetVal = false;
            break;
    };
    return bRetVal;
}

bool PrintLog::TraceMutexUnLock(TraceMutex* pMutex)
{
    if (NULL == pMutex)
        return false;
    if (NULL == pMutex->m_hMutex)
        return false;
    ReleaseMutex(pMutex->m_hMutex);
    return true;
}

void PrintLog::OutputLock(void)
{
    TraceMutexLock(&Loglocker);
}

void PrintLog::OutputUnlock(void)
{
    TraceMutexUnLock(&Loglocker);
}


Logger::Logger()
{
    KEYINFOPREFIX = " Key: \n";
    ERRORPREFIX = " Error: \n";
    WARNINGPREFIX = " Warning: \n";
    INFOPREFIX = " Info: \n";
    //��ʼ��
    memset(m_strLogPath, 0, 1024);
    memset(m_strCurLogName, 0, 1024);
    m_pFileStream = NULL;
    //����Ĭ�ϵ�д��־����
    m_nLogLevel = LogLevelNormal;
    //��ʼ���ٽ�������
    InitializeCriticalSection(&m_cs);
    //������־�ļ���
    GenerateLogName();
}

Logger::Logger(const char* strLogPath, EnumLogLevel nLogLevel /*= LogLevelNormal*/)
{
    KEYINFOPREFIX = " Key: ";
    ERRORPREFIX = " Error: ";
    WARNINGPREFIX = " Warning: ";
    INFOPREFIX = " Info: ";
    //��ʼ��
    m_pFileStream = NULL;
    //����Ĭ�ϵ�д��־����
    m_nLogLevel = nLogLevel;
    //???strLogPathע��ȫ·�����ԼӸ�����
    strcpy(m_strLogPath, strLogPath);
    InitializeCriticalSection(&m_cs);
    CreateLogPath();
    GenerateLogName();
}

Logger::~Logger()
{
    //�ͷ��ٽ���
    DeleteCriticalSection(&m_cs);
    //�ر��ļ���
    if (m_pFileStream)
        fclose(m_pFileStream);
}

void Logger::TraceKeyInfo(const char* strInfo, ...)
{
    if (!strInfo)
        return;
    char pTemp[MAX_STR_LEN] = { 0 };
    strcpy(pTemp, GetCurrentTimeE());
    strcat(pTemp, KEYINFOPREFIX);
    //��ȡ�ɱ��β�
    va_list arg_ptr = NULL;
    va_start(arg_ptr, strInfo);
    vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
    va_end(arg_ptr);
    //д��־�ļ�
    Trace(pTemp);
    arg_ptr = NULL;
}

void Logger::TraceError(const char* strInfo, ...)
{
    //�жϵ�ǰ��д��־����������Ϊ��д��־��������
    if (m_nLogLevel >= LogLevelStop)
        return;
    if (!strInfo)
        return;
    char pTemp[MAX_STR_LEN] = { 0 };
    strcpy(pTemp, GetCurrentTimeE());
    strcat(pTemp, ERRORPREFIX);
    va_list arg_ptr = NULL;
    va_start(arg_ptr, strInfo);
    vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
    va_end(arg_ptr);
    Trace(pTemp);
    arg_ptr = NULL;
}

void Logger::TraceWarning(const char* strInfo, ...)
{
    //�жϵ�ǰ��д��־����������Ϊֻд������Ϣ��������
    if (m_nLogLevel >= LogLevelNormal)
        return;
    if (!strInfo)
        return;
    char pTemp[MAX_STR_LEN] = { 0 };
    strcpy(pTemp, GetCurrentTimeE());
    strcat(pTemp, WARNINGPREFIX);
    va_list arg_ptr = NULL;
    va_start(arg_ptr, strInfo);
    vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
    va_end(arg_ptr);
    Trace(pTemp);
    arg_ptr = NULL;
}

void Logger::TraceInfo(const char* strInfo, ...)
{
    //�жϵ�ǰ��д��־����������ֻд����;�����Ϣ��������
    if (m_nLogLevel >= LogLevelMid)
        return;
    if (!strInfo)
        return;
    char pTemp[MAX_STR_LEN] = { 0 };
    strcpy(pTemp, GetCurrentTimeE());
    strcat(pTemp, INFOPREFIX);
    va_list arg_ptr = NULL;
    va_start(arg_ptr, strInfo);
    vsprintf(pTemp + strlen(pTemp), strInfo, arg_ptr);
    va_end(arg_ptr);
    Trace(pTemp);
    arg_ptr = NULL;
}

void Logger::SetLogLevel(EnumLogLevel nLevel)
{
    m_nLogLevel = nLevel;
}

void Logger::Trace(const char* strInfo)
{
    if (!strInfo)
        return;
    try
    {
        //�����ٽ���
        EnterCriticalSection(&m_cs);
        //���ļ���û�д򿪣������´�
        if (!m_pFileStream)
        {
            char temp[1024] = { 0 };
            strcat(temp, m_strLogPath);
            strcat(temp, m_strCurLogName);
            m_pFileStream = fopen(temp, "a+");
            if (!m_pFileStream)
                return;
        }
        //д��־��Ϣ���ļ���
        fprintf(m_pFileStream, "%s\n", strInfo);
        fflush(m_pFileStream);
        //�뿪�ٽ���
        LeaveCriticalSection(&m_cs);
    }
    //�������쳣�������뿪�ٽ�������ֹ����
    catch (...)
    {
        LeaveCriticalSection(&m_cs);
    }
}

void Logger::GenerateLogName()
{
    time_t curTime;
    struct tm* pTimeInfo = NULL;
    time(&curTime);
    pTimeInfo = localtime(&curTime);
    char tempLogName[1024] = { 0 };
    //��־�������磺20130101.log
    sprintf(tempLogName, "%04d%02d%02d.log", pTimeInfo->tm_year + 1900, pTimeInfo->tm_mon + 1, pTimeInfo->tm_mday);
    if (0 != strcmp(m_strCurLogName, tempLogName))
    {
        strcpy(m_strCurLogName, tempLogName);
        if (m_pFileStream)
            fclose(m_pFileStream);
        char temp[1024] = { 0 };
        strcat(temp, m_strLogPath);
        strcat(temp, m_strCurLogName);
        //��׷�ӵķ�ʽ���ļ���
        m_pFileStream = fopen(temp, "a+");  //a+       w+t
    }
}

char* Logger::GetCurrentTimeE()
{
    time_t curTime;
    struct tm* pTimeInfo = NULL;
    time(&curTime);
    pTimeInfo = localtime(&curTime);
    char temp[MAX_STR_LEN] = { 0 };
    sprintf(temp, "%02d:%02d:%02d", pTimeInfo->tm_hour, pTimeInfo->tm_min, pTimeInfo->tm_sec);
    char* pTemp = temp;
    return pTemp;
}

void Logger::CreateLogPath()
{
    if (0 != strlen(m_strLogPath))
        strcat(m_strLogPath, "\\");
    //CreatePathDir(m_strLogPath);        
    MakeSureDirectoryPathExists(m_strLogPath);
}



//*************************************************************************
//
//              串口通信日志接口
//                                              @2017-08-07阿甘
//
//*************************************************************************
#ifndef COMMON_LOG_H
#define COMMON_LOG_H

extern int g_nLogLevel;

#define DEBUG_LEVEL1 1
#define DEBUG_LEVEL2 2
#define DEBUG_LEVEL3 3
#define DEBUG_LEVEL4 4
#define COM_DEBUG(level, arg)                   \
    if(level <= g_nLogLevel) {                  \
        PrintToTerminal arg ;}

#define COM_WARN PrintToTerminal
#define COM_ERR  PrintToTerminal

typedef struct
{
    enum { MAX_FILE_NAME = 256 };
    FILE* m_pFile;
    char m_fileName[MAX_FILE_NAME];
}COMM_LOG_FILE;

//打开文件
int OpenLogFile(COMM_LOG_FILE* pFile);
//写入文件
int WriteToLogFile(COMM_LOG_FILE* pFile, const char* pInfo);
//关闭文件
int CloseLogFile(COMM_LOG_FILE* pFile);

int OpenLogRecord(const char* path_);
int EnableLogRecord(int enable_);
int CloseLogRecord(void);

//打印接口
void PrintToTerminal( const char* fmt, ... );

#endif



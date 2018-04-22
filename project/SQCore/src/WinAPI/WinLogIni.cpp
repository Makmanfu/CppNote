
#include "StdAfx.h"
#include "WinLogIni.h"
#include <fstream>
#include <sstream>
#include <windows.h>

string ReadINI(string inode, string ikey, string ivalue, string ipathfile)
{
    char szStr[256];            //  读文本
    memset(szStr, 0, sizeof(szStr));
    //GetPrivateProfileString("根", "键", "默认值", szStr, sizeof(szStr), ".//config.ini");
    if (GetPrivateProfileStringA(inode.c_str(), ikey.c_str(), ivalue.c_str(), szStr, sizeof(szStr), ipathfile.c_str()) > 0)
        return szStr;
    else
        return "0"; //读取失败
}

int RiniInt(string inode, string ikey, int ivalue, string ipathfile)
{
    return GetPrivateProfileIntA(inode.c_str(), ikey.c_str(), ivalue, ipathfile.c_str());
}

float RiniFloat(string inode, string ikey, float ivalue, string ipathfile/*="./cfg.ini"*/)
{
    std::stringstream tmp;
    tmp << ivalue;
    char szStr[256];            //  读文本
    memset(szStr, 0, sizeof(szStr));
    if (GetPrivateProfileStringA(inode.c_str(), ikey.c_str(), tmp.str().c_str(), szStr, sizeof(szStr), ipathfile.c_str()) > 0)
        return (float)atof(szStr);
    else
        return 0.0; //读取失败
}

void WriteINI(string inode, string ikey, string ivalue, string ipathfile)
{
    WritePrivateProfileStringA(inode.c_str(), ikey.c_str(), ivalue.c_str(), ipathfile.c_str());
    //  写一段
    //WritePrivateProfileSection("CLASS", "company=tq\0date=20090817\0number=48\0", ".//config.ini");
}

bool CreateIni(string ipathfile)
{
    return WritePrivateProfileStringA("ROOT", "key", "value", ipathfile.c_str()) == 1 ? true : false;
}

//得到写入字符
char* GetMulTik(const char* f_write)
{
    //得出时间
    char* szBuffer = (char*)malloc(23 * sizeof(char));
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(szBuffer, "%04d%02d%02d %02d:%02d:%02d:%03d: ",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    //时间值 cout<<szBuffer<<endl;
    char* tmpchar = new char[strlen(szBuffer) + strlen(f_write) + 1];
    strcpy(tmpchar, szBuffer);
    return strcat(tmpchar, f_write);
}

//得到写入字符,在字符后加\n
char* GetMulTikN(const char* f_write)
{
    //得出时间
    char* szBuffer = (char*)malloc((strlen(f_write) + 25) * sizeof(char));
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(szBuffer, "%04d%02d%02d %02d:%02d:%02d:%03d: %s\n",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, f_write);
    return szBuffer;
}

int LogFun(const char* f_txt, const char* f_logpath, int cmd)
{
    //判断字符合法
    if ((strlen(f_txt) <= 0) || (strlen(f_logpath) <= 0))  return 0;
    //判断命令行显示
    if (cmd == 1)
    {
        printf(GetMulTikN(f_txt));
        return 1;
    }
    //追加打开文件
    FILE* m_File = fopen(f_logpath, "a+t");
    if (NULL == m_File)  return 0;
    //得到字符
    f_txt = GetMulTikN(f_txt);
    //写入文件
    if (cmd == 2) printf(f_txt);
    fputs(f_txt, m_File);
    //关闭释放文件
    fclose(m_File);
    m_File = NULL;
    return 1;
}

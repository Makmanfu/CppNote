
#include "StdAfx.h"
#include "WinLogIni.h"
#include <fstream>
#include <sstream>
#include <windows.h>

string ReadINI(string inode, string ikey, string ivalue, string ipathfile)
{
    char szStr[256];            //  ���ı�
    memset(szStr, 0, sizeof(szStr));
    //GetPrivateProfileString("��", "��", "Ĭ��ֵ", szStr, sizeof(szStr), ".//config.ini");
    if (GetPrivateProfileStringA(inode.c_str(), ikey.c_str(), ivalue.c_str(), szStr, sizeof(szStr), ipathfile.c_str()) > 0)
        return szStr;
    else
        return "0"; //��ȡʧ��
}

int RiniInt(string inode, string ikey, int ivalue, string ipathfile)
{
    return GetPrivateProfileIntA(inode.c_str(), ikey.c_str(), ivalue, ipathfile.c_str());
}

float RiniFloat(string inode, string ikey, float ivalue, string ipathfile/*="./cfg.ini"*/)
{
    std::stringstream tmp;
    tmp << ivalue;
    char szStr[256];            //  ���ı�
    memset(szStr, 0, sizeof(szStr));
    if (GetPrivateProfileStringA(inode.c_str(), ikey.c_str(), tmp.str().c_str(), szStr, sizeof(szStr), ipathfile.c_str()) > 0)
        return (float)atof(szStr);
    else
        return 0.0; //��ȡʧ��
}

void WriteINI(string inode, string ikey, string ivalue, string ipathfile)
{
    WritePrivateProfileStringA(inode.c_str(), ikey.c_str(), ivalue.c_str(), ipathfile.c_str());
    //  дһ��
    //WritePrivateProfileSection("CLASS", "company=tq\0date=20090817\0number=48\0", ".//config.ini");
}

bool CreateIni(string ipathfile)
{
    return WritePrivateProfileStringA("ROOT", "key", "value", ipathfile.c_str()) == 1 ? true : false;
}

//�õ�д���ַ�
char* GetMulTik(const char* f_write)
{
    //�ó�ʱ��
    char* szBuffer = (char*)malloc(23 * sizeof(char));
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(szBuffer, "%04d%02d%02d %02d:%02d:%02d:%03d: ",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
    //ʱ��ֵ cout<<szBuffer<<endl;
    char* tmpchar = new char[strlen(szBuffer) + strlen(f_write) + 1];
    strcpy(tmpchar, szBuffer);
    return strcat(tmpchar, f_write);
}

//�õ�д���ַ�,���ַ����\n
char* GetMulTikN(const char* f_write)
{
    //�ó�ʱ��
    char* szBuffer = (char*)malloc((strlen(f_write) + 25) * sizeof(char));
    SYSTEMTIME st;
    GetLocalTime(&st);
    sprintf(szBuffer, "%04d%02d%02d %02d:%02d:%02d:%03d: %s\n",
            st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, f_write);
    return szBuffer;
}

int LogFun(const char* f_txt, const char* f_logpath, int cmd)
{
    //�ж��ַ��Ϸ�
    if ((strlen(f_txt) <= 0) || (strlen(f_logpath) <= 0))  return 0;
    //�ж���������ʾ
    if (cmd == 1)
    {
        printf(GetMulTikN(f_txt));
        return 1;
    }
    //׷�Ӵ��ļ�
    FILE* m_File = fopen(f_logpath, "a+t");
    if (NULL == m_File)  return 0;
    //�õ��ַ�
    f_txt = GetMulTikN(f_txt);
    //д���ļ�
    if (cmd == 2) printf(f_txt);
    fputs(f_txt, m_File);
    //�ر��ͷ��ļ�
    fclose(m_File);
    m_File = NULL;
    return 1;
}

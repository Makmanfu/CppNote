//*************************************************************************
//
//                          小工具集合
//
//
//*************************************************************************
#ifndef MINITOOLS_HPP
#define MINITOOLS_HPP
#include "WinTool.h"
#include <iostream>
#include <algorithm>
using namespace std;

//程序簽名器
struct SignFlagexe
{
    static int main(int argc, char** argv)
    {
        if (argc > 1)
        {
            SignFlagexe app;
            if (app.IsDirectory(argv[argc - 1]))
                app.SignAllFile(argv[argc - 1]);
            else
                WinTool::Instance()->SignFlagEXE(argv[argc - 1]);
            cout << "Sign  ok!" << endl;
        }
        return 0;
    };
public:
    string strexe, strdll, strso, strFile;

    SignFlagexe()
    {
        strexe = string("exe");
        strdll = string("dll");
        strso = string("so");
    };

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
        if ((strlen(f_txt) <= 0) || (strlen(f_logpath) <= 0))  return 0;
        //判断命令行显示
        if (cmd == 1)
        {
            printf(GetMulTikN(f_txt));
            return 1;
        }
        FILE* m_File = fopen(f_logpath, "a+t");
        if (NULL == m_File)  return 0;
        f_txt = GetMulTikN(f_txt);
        if (cmd == 2) printf(f_txt);
        fputs(f_txt, m_File);
        fclose(m_File);
        m_File = NULL;
        return 1;
    }

    bool IsDirectory(const string& pathfile)
    {
        DWORD dwAttr = ::GetFileAttributesA(pathfile.c_str());  //得到文件属性
        if (dwAttr == 0xFFFFFFFF)                   // 文件或目录不存在
            return false;
        else if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)   // 如果是目录
            return true;
        else
            return false;
    }

    void PathFormat(string& path, bool endflg)
    {
        char slash = '\\', backslash = '/'; //斜杠,反斜杠
        int flag = 0;
        for (string::iterator it = path.begin(); it != path.end();)
        {
            if (*it == backslash)
                *it = slash;                //将替换所有/为'\'
            if (1 == flag && *it == slash)  //除掉重复的
                it = path.erase(it);
            else
            {
                flag = (*it == slash) ? 1 : 0;
                ++it;
            }
        }
        if (endflg)                    //末尾判断加"\\"
        {
            if (*(path.end() - 1) != slash)
                path += slash;
        }
    }

    string rightstr(string f_src, int n)
    {
        if (n < 0 || n >(int(f_src.size()) - 1))
            return "0";
        return string(f_src.end() - n, f_src.end());
    }

    bool SignAllFile(const string& path)
    {
        WIN32_FIND_DATAA FindDTT;
        string Sear(path), format("*.*");
        PathFormat(Sear, 1);
        HANDLE hFind = FindFirstFileA((Sear + format).c_str(), &FindDTT);
        if (hFind == INVALID_HANDLE_VALUE)
            return false;
        do
        {
            if (!strcmp(FindDTT.cFileName, "..") || !strcmp(FindDTT.cFileName, "."))
                continue;
            //if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) //有BUG
            if (FindDTT.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) 
                SignAllFile(Sear + FindDTT.cFileName);
            else
            {
                strFile = string(FindDTT.cFileName);
                //转小写
                transform(strFile.begin(), strFile.end(), strFile.begin(), tolower);
                if (rightstr(strFile, 3) == strexe || rightstr(strFile, 3) == strdll || rightstr(strFile, 2) == strso)
                {
                    //LogFun((Sear + FindDTT.cFileName).c_str(), "app.log", 0);
                    WinTool::Instance()->SignFlagEXE((Sear + FindDTT.cFileName).c_str());
                }
            }
        } while (TRUE == FindNextFileA(hFind, &FindDTT));
        FindClose(hFind);
        return true;
    }
};








#endif

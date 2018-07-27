//*************************************************************************
//
//                          С���߼���
//
//
//*************************************************************************
#ifndef MINITOOLS_HPP
#define MINITOOLS_HPP
#include "WinTool.h"
#include <iostream>
using namespace std;

//��������
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
    bool IsDirectory(const string& pathfile)
    {
        DWORD dwAttr = ::GetFileAttributesA(pathfile.c_str());  //�õ��ļ�����
        if (dwAttr == 0xFFFFFFFF)                   // �ļ���Ŀ¼������
            return false;
        else if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)   // �����Ŀ¼
            return true;
        else
            return false;
    }

    void PathFormat(string& path, bool endflg)
    {
        char slash = '\\', backslash = '/'; //б��,��б��
        int flag = 0;
        for (string::iterator it = path.begin(); it != path.end();)
        {
            if (*it == backslash)
                *it = slash;                //���滻����/Ϊ'\'
            if (1 == flag && *it == slash)  //�����ظ���
                it = path.erase(it);
            else
            {
                flag = (*it == slash) ? 1 : 0;
                ++it;
            }
        }
        if (endflg)                    //ĩβ�жϼ�"\\"
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
            if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                SignAllFile(Sear + FindDTT.cFileName);
            else
            {
                if (rightstr(string(FindDTT.cFileName), 3) == string("exe") || rightstr(string(FindDTT.cFileName), 3) == string("dll"))
                {
                    WinTool::Instance()->SignFlagEXE((Sear + FindDTT.cFileName).c_str());
                }
            }
        } while (TRUE == FindNextFileA(hFind, &FindDTT));
        FindClose(hFind);
        return true;
    }
};








#endif

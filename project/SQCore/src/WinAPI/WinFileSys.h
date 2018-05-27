//-----------------------------------------------------------------------------
//
//                  windows �ļ�ϵͳapi�ܽ��㷨
//                                          @2015-05-10��������
//
//-----------------------------------------------------------------------------
#pragma once
#include "WinFileSys_typedef.h"


class SQCPP_API WinFileSys
{
#define BUFSIZE 1024
public:
    WinFileSys();
    ~WinFileSys();
public:
    //��ʽ���ɱ�׼win·��[c:\test] endflgĩβ��"\\"
    void PathFormat(string& path, bool endflg = false);
    //���·�����ļ��Ƿ�Ϸ� ????
    bool CheckPathFile(const string& pathfile);
    //��ȡ��ǰ����·����Ϣ[idtype:0-ȫ·��,1-������,2-·��c:\test ]
    string GetExePath(EXETYPE idtype = PATHALL);
    //�õ�ϵͳ����������
    void GetAllDisk(vector<string>& AllDrv);
    //����·�������ļ���[����·��] �ȼ���MakeSureDirectoryPathExists
    bool CreatePathDir(const string& path);
    //�޸��ļ�����
    void SetAtt(const string& pathfile);
public:
    //����·���������������[0�ݹ�ȫ��,1�ݹ��ļ���,2�ݹ��ļ�,3��ǰ�ļ���,4��ǰ�ļ�]
    int GetDirFiles(const string& path, vector<string>& AllDir, DIRTYPE idtype = DALL);
    //�ݹ�ɾ���ļ�������[·����c:\test �ٶ��Ѿ��Ż�]
    bool DelPathDir(const string& path);
    //�ݹ�ɾ�����ļ���
public:
    //���ܵݹ��ļ�
    bool SmartFile(const string& path, FileProc fp);
    //���ܵݹ��ļ���
    bool SmartDir(const string& path, FileProc fp);
};






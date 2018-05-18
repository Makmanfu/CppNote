//-----------------------------------------------------------------------------
//
//                  windows �ļ�ϵͳapi�ܽ��㷨
//                                          @2015-05-10��������
//-----------------------------------------------------------------------------
#pragma once
#include "WinFileSys_typedef.h"



//-------------------------------���Է���ʹ��--------------------------------------
//��ʾchar*
SQC_API void ShowBox(const char* f_txt);
//��ʾstring
SQC_API void ShowBoxStr(string& f_txt);
//��ʾint
SQC_API void ShowBoxInt(int f_txt);
//---------------------------------------------------------------------------------
//�õ�ϵͳ����������
SQC_API void GetAllDisk(vector<string>& AllDrv);
//·����ʽ���ɱ�׼windows·��[c:\test]
SQC_API void PathFormat(string& path, int idtype = 0);
//���·�����ļ��Ƿ����
SQC_API bool CheckPathFile(const string& pathfile);
//��ȡ��ǰ����·����Ϣ[idtype:0-ȫ·��,1-������,2-·��c:\test ]
SQC_API string GetExePath(int idtype = 0);
//��ȡ��ǰ����·����Ϣ[idtype:0-ȫ·��,1-������,2-·��c:\test ]
//SQC_API string GetExePathC(int idtype = 0);
//����·���������������[0�ݹ�ȫ��,1�ݹ��ļ���,2�ݹ��ļ�,3��ǰ�ļ���,4��ǰ�ļ�]
SQC_API int GetDirFiles(const string& path, vector<string>& AllDir, DIRTYPE idtype = DALL);
//����·�������ļ���[����·��] �ȼ���MakeSureDirectoryPathExists
SQC_API bool CreatePathDir(const string& path);
//�ݹ�ɾ���ļ�������[·����c:\test �ٶ��Ѿ��Ż�]
SQC_API bool DelPathDir(const string& path);
//���ɾ���ļ�������[·����c:\test �ٶ��Ѿ��Ż�]
SQC_API bool DelPathDirEx(const string& path);
//���ܹ�������ļ�
//���ܹ�������ļ���
//���ܵݹ��ļ�
SQC_API bool SmartFile(const string& path, FileProc fp = &ShowBoxStr);
//���ܵݹ��ļ���
SQC_API bool SmartDir(const string& path, FileProc fp = &ShowBoxStr);
//�ݹ�ɾ�����ļ���
SQC_API bool DelEmptyDir(const string& path);
//�޸�����
SQC_API void SetAtt(const string& pathfile);
//�޸�����
SQC_API void ReNameDir(void);
//�޸��ļ�ʱ��
SQC_API void SetFilesTime(void);

/*
//�õ���ǰ����·��
SQC_API BOOL GetDesktopPath(char* pszDesktopPath);
//������ݷ�ʽ[����ȫ·��,����path,��������,����]

SQC_API BOOL ShortLink(LPCSTR lpszWorkDir, LPCSTR lpszDirName, int iShowCmd = SW_SHOWNORMAL);

//�������ܣ���ָ���ļ���ָ����Ŀ¼�´������ݷ�ʽ
//����������
//lpszFileName    ָ���ļ���ΪNULL��ʾ��ǰ���̵�EXE�ļ���
//lpszLnkFileDir  ָ��Ŀ¼������ΪNULL��
//lpszLnkFileName ��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����
//wHotkey         Ϊ0��ʾ�����ÿ�ݼ�
//pszDescription  ��ע
//iShowCmd        ���з�ʽ��Ĭ��Ϊ���洰��

SQC_API BOOL CreateFileShortcut(LPCSTR lpszFileName, LPCSTR lpszLnkFileDir, LPCSTR lpszLnkFileName,
                                LPCSTR lpszWorkDir, WORD wHotkey, LPCSTR lpszDescription, int iShowCmd = SW_SHOWNORMAL);

*/


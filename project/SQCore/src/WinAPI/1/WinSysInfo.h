//-----------------------------------------------------------------------------
//
//                  windows ϵͳ�����ܽ�
//                  ע��� ���Ӳ����Ϣ ʱ���
//                                          @2014-03-10��������
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;


//����Ψһ�Լ��
SQC_API bool RunCheckOnly(char* f_tname = "app");
//��̬����dll����
SQC_API void LoadDllPro(char* Dllname, char* ProName);
//����������ע���
SQC_API void SetAutoRun(LPSTR RegName, bool fsetrun = true);
//���ϵͳ�汾
SQC_API void GetSysVersion(void);
//���ϵͳcpu��Ϣcpuid
SQC_API void GetCPUInfo(void);
//��ȡCPUid
SQC_API string GetCPUid(void);
//���ϵͳ���Ƶ���Ϣ
SQC_API void GetSysInfo(void);
//����û����û���
SQC_API void GetUsers(void);
//����û����û���
SQC_API void AddUsers(void);
//ɾ���û����û���
SQC_API void DelUser(void);
//���ϵͳʱ��(ʱ���ʽ)
SQC_API void GetTime(void);
//������Ļ����[����0-390]
SQC_API int SetBrightness(int f_bright);
//������ʾ����ʾ״̬
SQC_API int SetDisplayState(int fState = 2);
//�޸ķֱ���[�����ɫ]
SQC_API bool ChngDsplyMd(int f_WidthP, int f_HeightP, int f_intBitsPer);
//����ϵͳ[0����ػ�/4��������/1�ػ�/2����/3ע��]
SQC_API void SetSysShut(int f_State = 1);
//��ȡʱ��
SQC_API string GetThisNowTime(void);
//��ȡΨһID
SQC_API string GetMilliNum(void);
//GUID(num=��ʱ��һ����)
SQC_API char* GetGUID(bool f_num = false);
//exe�ڲ���Դ�ͷŷ���
SQC_API bool UPackExeRes(char* f_strFileName, WORD f_wResID, char* f_strFileType);
//��̬����Dll�ͷ���Դ����
SQC_API bool UPackDllRes(char* f_dllname, char* f_strFileName, WORD f_wResID, char* f_strFileType);
//�����㷨ʱ�䷽��
SQC_API void CalcRunTime(void(*FUNProc)(void));
//����ǩ��58�ֽ� +52�ֽ�[ver-verr 00-000]
SQC_API void SignFlagEXE(const char* exename, int ver = 0, int verr = 0);
//��ȡDebugȨ��
SQC_API void RaiseToDebug(void);
//�����ⲿexe����
SQC_API int RunShell(char* f_exeName);
//ǿ�ƽ�������
SQC_API int KillProcess(char* f_exeName, bool f_Must = false);
//��ȡ�����б�
SQC_API void ProcessList(vector<string>& f_prolist);



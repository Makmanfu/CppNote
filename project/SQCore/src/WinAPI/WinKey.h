//-----------------------------------------------------------------------------
//
//          windows hook �ͼ��̲���
//                                          @2017-03-10��������
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//dll�о��������
BOOL APIENTRY KeyDllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
//��ʼ���μ��̰���
SQC_API bool StartMaskKey(LPDWORD lpdwVirtualKey, int nLength, bool bDisableKeyboard = false);
//ֹͣ���μ��̰���
SQC_API bool StopMaskKey(void);
//���� A, B, C, ��, ��, ��, �Ҽ�����win��
SQC_API void OnStartMaskkey(void);
//���ΰ���
SQC_API void AllStartMaskkey(void);
//����Ctrl+alt+del
SQC_API void MaskCtrlAltDel(void);
//��������
SQC_API void GetPressKeyValue(void);













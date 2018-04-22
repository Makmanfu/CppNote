//-----------------------------------------------------------------------------
//
//          windows hook 和键盘操作
//                                          @2017-03-10阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//dll中句柄的问题
BOOL APIENTRY KeyDllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
//开始屏蔽键盘按键
SQC_API bool StartMaskKey(LPDWORD lpdwVirtualKey, int nLength, bool bDisableKeyboard = false);
//停止屏蔽键盘按键
SQC_API bool StopMaskKey(void);
//屏蔽 A, B, C, 上, 下, 左, 右及两个win键
SQC_API void OnStartMaskkey(void);
//屏蔽按键
SQC_API void AllStartMaskkey(void);
//屏蔽Ctrl+alt+del
SQC_API void MaskCtrlAltDel(void);
//按键触发
SQC_API void GetPressKeyValue(void);













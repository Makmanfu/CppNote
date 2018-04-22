//----------------------------------------------------------------------
//
//      lpk劫持库的框架
//
//                                          @阿甘  20160923
//----------------------------------------------------------------------

#include "stdafx.h"
#include "process.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
#pragma comment(linker, "/EXPORT:LpkInitialize=_AheadLib_LpkInitialize,@1")
#pragma comment(linker, "/EXPORT:LpkTabbedTextOut=_AheadLib_LpkTabbedTextOut,@2")
#pragma comment(linker, "/EXPORT:LpkDllInitialize=_AheadLib_LpkDllInitialize,@3")
#pragma comment(linker, "/EXPORT:LpkDrawTextEx=_AheadLib_LpkDrawTextEx,@4")
//#pragma comment(linker, "/EXPORT:LpkEditControl=_AheadLib_LpkEditControl,@5")
#pragma comment(linker, "/EXPORT:LpkExtTextOut=_AheadLib_LpkExtTextOut,@6")
#pragma comment(linker, "/EXPORT:LpkGetCharacterPlacement=_AheadLib_LpkGetCharacterPlacement,@7")
#pragma comment(linker, "/EXPORT:LpkGetTextExtentExPoint=_AheadLib_LpkGetTextExtentExPoint,@8")
#pragma comment(linker, "/EXPORT:LpkPSMTextOut=_AheadLib_LpkPSMTextOut,@9")
#pragma comment(linker, "/EXPORT:LpkUseGDIWidthCache=_AheadLib_LpkUseGDIWidthCache,@10")
#pragma comment(linker, "/EXPORT:ftsWordBreak=_AheadLib_ftsWordBreak,@11")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LpkEditControl导出的是数组，不是单一的函数（by Backer）
EXTERNC void __cdecl AheadLib_LpkEditControl(void);
EXTERNC __declspec(dllexport) void (*LpkEditControl[14])() = { AheadLib_LpkEditControl };
////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AheadLib 命名空间
namespace AheadLib
{
    HMODULE m_hModule = NULL;   // 原始模块句柄
    // 加载原始模块
    inline BOOL WINAPI Load()
    {
        TCHAR tzPath[MAX_PATH];
        TCHAR tzTemp[MAX_PATH * 2];
        GetSystemDirectory(tzPath, MAX_PATH);
        lstrcat(tzPath, TEXT("\\lpk"));
        m_hModule = LoadLibrary(tzPath);
        if (m_hModule == NULL)
        {
            wsprintf(tzTemp, TEXT("无法加载 %s，程序无法正常运行。"), tzPath);
            MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
        };
        return (m_hModule != NULL);
    }
    // 释放原始模块
    inline VOID WINAPI Free()
    {
        if (m_hModule)
            FreeLibrary(m_hModule);
    }
    // 获取原始函数地址
    FARPROC WINAPI GetAddress(PCSTR pszProcName)
    {
        FARPROC fpAddress;
        CHAR szProcName[16];
        TCHAR tzTemp[MAX_PATH];
        fpAddress = GetProcAddress(m_hModule, pszProcName);
        if (fpAddress == NULL)
        {
            if (HIWORD(pszProcName) == 0)
            {
                wsprintfA(szProcName, "%d", pszProcName);
                pszProcName = szProcName;
            }
            wsprintf(tzTemp, TEXT("无法找到函数 %hs，程序无法正常运行。"), pszProcName);
            MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
            ExitProcess(-2);
        }
        return fpAddress;
    }
}
using namespace AheadLib;

void WINAPIV Init(LPVOID pParam)
{
    //在这里添加DLL加载代码
    return;
}

// 入口函数
BOOL WINAPI LpkDllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        if (Load())
        {
            //LpkEditControl这个数组有14个成员，必须将其复制过来
            memcpy((LPVOID)(LpkEditControl + 1), (LPVOID)((int*) GetAddress("LpkEditControl") + 1), 52);
            _beginthread(Init, NULL, NULL);
        }
        else
            return FALSE;
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        Free();
    return TRUE;
}

ALCDECL AheadLib_LpkInitialize(void)
{
    GetAddress("LpkInitialize");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkTabbedTextOut(void)
{
    GetAddress("LpkTabbedTextOut");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkDllInitialize(void)
{
    GetAddress("LpkDllInitialize");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkDrawTextEx(void)
{
    GetAddress("LpkDrawTextEx");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkEditControl(void)
{
    GetAddress("LpkEditControl");
    __asm jmp DWORD ptr[EAX];//这里的LpkEditControl是数组，eax存的是函数指针
}

ALCDECL AheadLib_LpkExtTextOut(void)
{
    GetAddress("LpkExtTextOut");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkGetCharacterPlacement(void)
{
    GetAddress("LpkGetCharacterPlacement");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkGetTextExtentExPoint(void)
{
    GetAddress("LpkGetTextExtentExPoint");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkPSMTextOut(void)
{
    GetAddress("LpkPSMTextOut");
    __asm JMP EAX;
}

ALCDECL AheadLib_LpkUseGDIWidthCache(void)
{
    GetAddress("LpkUseGDIWidthCache");
    __asm JMP EAX;
}

ALCDECL AheadLib_ftsWordBreak(void)
{
    GetAddress("ftsWordBreak");
    __asm JMP EAX;
}



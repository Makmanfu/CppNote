
#include "stdafx.h"
#include "UIlib.h"

#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )
extern "C" {
//#include "stb_image.c"
}

BOOL APIENTRY DUIDllMain(HANDLE hModule, DWORD  dwReason, LPVOID /*lpReserved*/)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            ::DisableThreadLibraryCalls((HMODULE)hModule);
            break;
    }
    return TRUE;
}

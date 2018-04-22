
#include "stdafx.h"
#include "Glconf.h"
#include <io.h>


bool UPackExeRes(char* f_strFileName, WORD f_wResID, LPCSTR lpType)
{
    if (!_access(f_strFileName, 0))
        return false;
    DWORD dwWrite = 0;
    HANDLE hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
                               CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;
    HRSRC hrsc = FindResourceA(NULL, MAKEINTRESOURCEA(f_wResID), lpType);
    HGLOBAL hG = LoadResource(NULL, hrsc);
    DWORD dwSize = SizeofResource(NULL, hrsc);
    WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
    CloseHandle(hFile);
    return TRUE;
}




#include "stdafx.h"
#include "NaFile.h"
#include <io.h>
#include <conio.h>
#include <dos.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

void test(void)
{
    //if (_open("data,dat",O_CREAT,S_IREAD|S_IWRITE))
}

int DeleteDemo(void)
{
    //HINSTANCE hNtDll;
    //ZWDELETEFILE ZwDeleteFile;
    //RTLINITUNICODESTRING RtlInitUnicodeString;
    //ZWCREATEFILE ZwCreateFile;
    //ZWWRITEFILE ZwWriteFile;
    //ZWCLOSE ZwClose;
    //hNtDll = LoadLibraryA ( "NTDLL.dll" );
    //if ( !hNtDll )
    //    return 0;
    //ZwDeleteFile = ( ZWDELETEFILE ) GetProcAddress ( hNtDll, "ZwDeleteFile" );
    //RtlInitUnicodeString = ( RTLINITUNICODESTRING ) GetProcAddress ( hNtDll, "RtlInitUnicodeString" );
    //ZwCreateFile = ( ZWCREATEFILE ) GetProcAddress ( hNtDll, "ZwCreateFile" );
    //ZwWriteFile = ( ZWWRITEFILE ) GetProcAddress ( hNtDll, "ZwWriteFile" );
    //ZwClose = ( ZWCLOSE ) GetProcAddress ( hNtDll, "ZwClose" );
    //UNICODE_STRING ObjectName;
    //RtlInitUnicodeString ( &ObjectName, L"//??//C://1.rar" );
    ////RtlInitUnicodeString(&ObjectName,L"//??//C://ForZwFileTest.txt");//記得這裏是要有//??//在前面的,DDK說的.
    //OBJECT_ATTRIBUTES ObjectAttributes =
    //{
    //    sizeof ( OBJECT_ATTRIBUTES ),       // Length
    //    NULL,                               // RootDirectory
    //    &ObjectName,                        // ObjectName
    //    OBJ_CASE_INSENSITIVE,               // Attributes
    //    0,                                  // SecurityDescriptor
    //    NULL,                               // SecurityQualityOfService
    //};
    //HANDLE hFile = NULL;
    //PVOID content = "ForZwFileTest";
    //IO_STATUS_BLOCK IoStatusBlock;
    ////创建文件
    //ZwCreateFile(&hFile,
    //    GENERIC_WRITE | SYNCHRONIZE | GENERIC_READ,
    //    &ObjectAttributes,
    //    &IoStatusBlock,
    //    0,
    //    FILE_ATTRIBUTE_NORMAL,
    //    FILE_SHARE_DELETE,
    //    FILE_OPEN_IF,
    //    FILE_SYNCHRONOUS_IO_NONALERT,
    //    NULL,
    //    0);
    ////写入文件
    //ZwWriteFile ( hFile, 0, 0, 0, &IoStatusBlock, content, 12, NULL, NULL );
    //ZwClose ( hFile );
    ////删除文件
    //ZwDeleteFile ( &ObjectAttributes );
    //FreeLibrary ( hNtDll );
    return 0;
}

void NaSysShut(int f_State)
{
    //const unsigned int SE_SHUTDOWN_PRIVILEGE = 0x13;
    HMODULE hDll = ::LoadLibraryA("ntdll.dll");
    typedef int(*type_RtlAdjustPrivilege)(int, bool, bool, int*);
    typedef int(*type_ZwShutdownSystem)(int);
    type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");
    type_ZwShutdownSystem ZwShutdownSystem = (type_ZwShutdownSystem)GetProcAddress(hDll, "ZwShutdownSystem");
    int nEn = 0;
    int nResult = RtlAdjustPrivilege(0x13, true, true, &nEn);
    if (nResult == 0x0c000007c)
        nResult = RtlAdjustPrivilege(0x13, true, false, &nEn);
    nResult = ZwShutdownSystem(f_State); //1 reboot 2 shutdown
    FreeLibrary(hDll);
}







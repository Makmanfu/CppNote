
#include "StdAfx.h"
#include "WinFileSys.h"

#define BUFSIZE 1024

void ShowBox(const char* f_txt)
{
    MessageBoxA(NULL, f_txt, "提示", MB_OK);
}

void ShowBoxStr(string& f_txt)
{
    MessageBoxA(NULL, f_txt.c_str(), "提示", MB_OK);
}

void ShowBoxInt(int f_txt)
{
    //方法一 比较靠谱
    char buf[255] = { 0 };
    sprintf(buf, "%d", f_txt);
    MessageBoxA(NULL, buf, "提示", MB_OK);
    //方法二
    //MessageBox(NULL,AIntToStr(f_txt).c_str(),"提示",MB_OK);
}

void GetAllDisk(vector<string>& AllDrv)
{
    CHAR szLogicalDriveStrings[BUFSIZE];
    PCHAR szDrive;
    ZeroMemory(szLogicalDriveStrings, BUFSIZE);
    //获取逻辑驱动器卷标名
    GetLogicalDriveStringsA(BUFSIZE - 1, szLogicalDriveStrings);
    szDrive = (PCHAR)szLogicalDriveStrings;
    //循环处理所有驱动器
    do
    {
        AllDrv.push_back(szDrive);
        szDrive += (lstrlenA(szDrive) + 1);
    }
    while (*szDrive != '\x00');
}

void PathFormat(string& path, int idtype)
{
    char slash = '\\', backslash = '/'; //斜杠,反斜杠
    int flag = 0;
    for (string::iterator it = path.begin(); it != path.end();)
    {
        if (*it == backslash)
            *it = slash;                //将替换所有/为'\'
        if (1 == flag && *it == slash)  //除掉重复的
            it = path.erase(it);
        else
        {
            flag = (*it == slash) ? 1 : 0;
            ++it;
        }
    }
    if (idtype == 1)                    //末尾判断加"\\"
    {
        if (*(path.end() - 1) != slash)
            path += slash;
    }
}

bool CheckPathFile(const string& pathfile)
{
    string pathfiletmp(pathfile);
    //路径格式化
    PathFormat(pathfiletmp);
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind = FindFirstFileA(pathfiletmp.c_str(), &FindFileData);
    bool rValue(false);
    //判断文件夹
    if ((hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        rValue = true;
    if (hFind != INVALID_HANDLE_VALUE)
        rValue = true;
    if (GetDriveTypeA(pathfiletmp.c_str()) != DRIVE_NO_ROOT_DIR)
        rValue = true;
    FindClose(hFind);
    return rValue;
}

string GetExePath(int idtype)
{
    char szAppPath[MAX_PATH];
    GetModuleFileNameA(NULL, szAppPath, MAX_PATH);
    //GetCurrentDirectoryA
    string ret(szAppPath);
    size_t pos0 = ret.find_last_of("\\");
    switch (idtype)
    {
        default:                                        //"c:\1\curr.exe"
            break;
        case 1:                                         //"curr.exe"
            ret = ret.substr(pos0 + 1, ret.size() - pos0 - 1);
            break;
        case 2:                                         //"c:\1"
            ret = ret.substr(0, pos0);                  //或者(strrchr(szAppPath, '\\'))[0] = 0; ret = szAppPath
            break;
        case 3:                                         //"c:\1\"
            ret = ret.substr(0, pos0 + 1);              //或者(strrchr(szAppPath, '\\'))[1] = 0; ret = szAppPath
            break;
        case 4:                                         //curr  &(strrchr(szAppPath, '\\'))[1];
            ret = ret.substr(pos0 + 1, ret.size() - pos0 - 5);
            break;
    }
    return ret;
}

int GetDirFiles(const string& path, vector<string>& AllDir, DIRTYPE idtype)
{
    WIN32_FIND_DATAA FindDTT;
    string Sear(path), format("*.*");                               //扩展名格式
    PathFormat(Sear, 1);                                            //路径格式化
    HANDLE hFind = FindFirstFileA((Sear + format).c_str(), &FindDTT);
    if (hFind == INVALID_HANDLE_VALUE)
        return -1;                                                  //什么都没找到
    do
    {
        if (!strcmp(FindDTT.cFileName, "..") || !strcmp(FindDTT.cFileName, "."))
            continue;                                               //此级没有文件
        switch (idtype)
        {
            case DALL:
            default:                                          //全递归
                AllDir.push_back(Sear + FindDTT.cFileName);
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALL);
                break;
            case DALLDIR:                                               //仅递归文件夹
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    AllDir.push_back(Sear + FindDTT.cFileName);
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALLDIR);
                }
                break;
            case DALLFILE:                                              //仅递归文件
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALLFILE);
                else
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
            case DCURRDIR:                                              //当前文件夹
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
            case DCURRFILE:                                             //当前文件
                if (FindDTT.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
        }
    }
    while (TRUE == FindNextFileA(hFind, &FindDTT));
    FindClose(hFind);
    return AllDir.size();                                           //返回查找数量
#ifdef GetDirFiles_old     //方法二 老算法
    struct _finddata_t filefind;
    string curr = ChkPachSkew(f_path, false) + "*.*"; //搜索文件
    int pResultNum(0);                               //搜索结果数计数
    intptr_t done = 0, handle;
    if ((handle = _findfirst(curr.c_str(), &filefind)) == -1)
        return 0;                                       //什么都没找到
    while (!(done = _findnext(handle, &filefind)))
    {
        if (!strcmp(filefind.name, "..")) continue; //此级没有文件
        switch (f_type)
        {
            default:
            case 0:  /*递归所有文件夹和文件列表*/
                if ((filefind.attrib & _A_SUBDIR))      //文件是否为文件夹
                {
                    curr = ChkPachSkew(f_path + filefind.name, false);
                    pResultNum += GetDirFileList(curr, 0); //递归此文件夹
                }
                else
                {
                    ++pResultNum;
                    cout << f_path + filefind.name << endl;
                    //LogFun( (f_path + filefind.name).c_str(),"filelist.log" );
                }
                break;
            case 1:         /*得到此级目录下所有文件(包括文件夹)列表*/
                if ((filefind.attrib & _A_SUBDIR)) //文件是否为文件夹
                    continue;
                else
                {
                    ++pResultNum;
                    //pResult += f_path + filefind.name;
                }
                break;
            case 2:         /*递归记下所有文件夹列表*/
                if ((filefind.attrib & _A_SUBDIR))      //文件是否为文件夹
                {
                    cout << (curr = ChkPachSkew(f_path + filefind.name, false)) << endl; //测试写到文件中去
                    LogFun((f_path + filefind.name).c_str(), "filelist.log", 2);
                    ++pResultNum;
                    pResultNum += GetDirFileList(curr, 2); //记下递归文件夹和当前文件夹数量
                }
                break;
        }
    }
    _findclose(handle);
    return pResultNum;
#endif
}

//string实现splite
vector<string> split(string str, string pattern)
{
    string::size_type pos;
    vector<string> result;
    str += pattern;   //扩展字符窜以方便操作
    size_t size = str.size();
    for (size_t i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}

bool CreatePathDir(const string& path)
{
    //记下原来当前path
    CHAR szCurrentDir[BUFSIZE];
    ZeroMemory(szCurrentDir, BUFSIZE);
    GetCurrentDirectoryA(BUFSIZE - 1, szCurrentDir);
    string pathtmp(path), pathnew;
    //路径格式化
    PathFormat(pathtmp);
    //路径分割
    vector<string> dirs = split(pathtmp, "\\");
    for (vector<string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
    {
        pathnew += *it + "\\";
        if (!CheckPathFile(pathnew))
            if (!CreateDirectoryA(it->c_str(), NULL)) //创建文件夹
                return false;
        SetCurrentDirectoryA(pathnew.c_str());
    }
    //还原原来当前path
    SetCurrentDirectoryA((PCHAR)szCurrentDir);
    return true;
}

bool DelPathDir(const string& path)
{
    WIN32_FIND_DATAA FindDTT;
    string Sear(path + "\\"), format("*.*");                //扩展名格式
    HANDLE hFind = FindFirstFileA((Sear + format).c_str(), &FindDTT);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;                                       //什么都没找到
    do
    {
        if (!strcmp(FindDTT.cFileName, "..") || !strcmp(FindDTT.cFileName, "."))
            continue;                                       //此级没有文件
        if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            DelPathDir(Sear + FindDTT.cFileName);           //递归删除文件夹
        else
            DeleteFileA((Sear + FindDTT.cFileName).c_str());
    }
    while (TRUE == FindNextFileA(hFind, &FindDTT));
    FindClose(hFind);
    return RemoveDirectoryA(path.c_str());                  //完后删除此空文件夹
}

// 广度遍历目录
//BOOL TraverseDirBF(const string& strDir)
//{
//    deque<string> dequePath;
//    string strTemp = strDir;
//    int iFileCount(0),iDirCount(0);
//    ofstream ofs("out.txt");
//    while (TRUE)
//    {
//        if (!PathFileExists(strTemp.c_str()))
//            return FALSE;
//        string strDirFind = strTemp + string("\\*");
//        WIN32_FIND_DATA ffd = { 0 };
//        HANDLE hFind = FindFirstFile(strDirFind.c_str(), &ffd);
//        if (INVALID_HANDLE_VALUE == hFind)
//        {
//            if (dequePath.empty())
//                break;
//            else
//            {
//                strTemp = dequePath.front();
//                dequePath.pop_front();
//                continue;
//            }
//        }
//        // List all the files in the directory with some info about them.
//        do
//        {
//            if ((0 == strcmp(ffd.cFileName, ".")) || (0 == strcmp(ffd.cFileName, "..")))
//                continue;
//            string strFilePathName = strTemp + "\\" + string(ffd.cFileName);
//            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
//            {
//                //_tprintf(TEXT("  %s   <DIR>\n"), strFilePathName.c_str());
//                string str111 = strFilePathName + "\n";
//                ofs.write(str111.c_str(), str111.size());
//                dequePath.push_back(strFilePathName);
//                ++iDirCount;
//            } else {
//                //_tprintf(TEXT("  %s   <FILE>\n"), strFilePathName.c_str());
//                string str111 = strFilePathName + "\n";
//                ofs.write(str111.c_str(), str111.size());
//                ++iFileCount;
//            }
//        } while (FindNextFile(hFind, &ffd) != 0);
//        DWORD dwError = GetLastError();
//        if (dwError != ERROR_NO_MORE_FILES)
//        {
//            FindClose(hFind);
//            return FALSE;
//        }
//
//        FindClose(hFind);
//        if (dequePath.empty())
//            break;
//        else
//        {
//            strTemp = dequePath.front();
//            dequePath.pop_front();
//        }
//    }
//    ofs.close();
//    return TRUE;
//}

bool DelPathDirEx(const string& path)
{
    return true;
}

bool SmartFile(const string& path, FileProc fp)
{
    WIN32_FIND_DATAA FindDTT;
    string Sear(path), format("*.*");
    PathFormat(Sear, 1);
    HANDLE hFind = FindFirstFileA((Sear + format).c_str(), &FindDTT);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;
    do
    {
        if (!strcmp(FindDTT.cFileName, "..") || !strcmp(FindDTT.cFileName, "."))
            continue;
        if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            SmartFile(Sear + FindDTT.cFileName, fp);
        else
            fp(Sear + FindDTT.cFileName);
    }
    while (TRUE == FindNextFileA(hFind, &FindDTT));
    FindClose(hFind);
    return true;
}

bool SmartDir(const string& path, FileProc fp)
{
    WIN32_FIND_DATAA FindDTT;
    string Sear(path), format("*.*");
    PathFormat(Sear, 1);
    HANDLE hFind = FindFirstFileA((Sear + format).c_str(), &FindDTT);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;
    do
    {
        if (!strcmp(FindDTT.cFileName, "..") || !strcmp(FindDTT.cFileName, "."))
            continue;
        if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            SmartDir(Sear + FindDTT.cFileName, fp);
    }
    while (TRUE == FindNextFileA(hFind, &FindDTT));
    FindClose(hFind);
    fp(Sear);
    return true;
}

bool DelEmptyDir(const string& path)
{
    return true;
}

void SetAtt(const string& pathfile)
{
    //修改创建和修改时间
    SYSTEMTIME t;
    FILETIME ft;
    HANDLE file;
    GetSystemTime(&t);
    t.wYear = 2009;
    SystemTimeToFileTime(&t, &ft);
    file = CreateFileA("2.exe", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetFileTime(file, &ft, &ft, &ft);
    CloseHandle(file);
    return;
    //修改文件属性
    //SetFileAttributesA("MyGUI64.exe", FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
    SetFileAttributesA("MyGUI64.exe", !FILE_ATTRIBUTE_READONLY | !FILE_ATTRIBUTE_HIDDEN | !FILE_ATTRIBUTE_SYSTEM);
}

void ReNameDir(void)
{
}

void SetFilesTime(void)
{
}

/*
BOOL GetDesktopPath(char* pszDesktopPath)
{
    LPITEMIDLIST  ppidl = NULL;
    if (SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &ppidl) == S_OK)
    {
        BOOL flag = SHGetPathFromIDListA(ppidl, pszDesktopPath);
        CoTaskMemFree(ppidl);
        return flag;
    }
    return FALSE;
}

//得到快速启动栏的路径
BOOL GetIEQuickLaunchPath(char *pszIEQueickLaunchPath)
{
    LPITEMIDLIST  ppidl;

    if (SHGetSpecialFolderLocation(NULL, CSIDL_APPDATA, &ppidl) == S_OK)
    {
        BOOL flag = SHGetPathFromIDListA(ppidl, pszIEQueickLaunchPath);
        strcat(pszIEQueickLaunchPath, "\\Microsoft\\Internet Explorer\\Quick Launch");
        CoTaskMemFree(ppidl);
        return flag;
    }

    return FALSE;
}

//得到 开始->程序组 的路径
BOOL GetProgramsPath(char *pszProgramsPath)
{
    LPITEMIDLIST  ppidl;
    if (SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &ppidl) == S_OK)
    {
        BOOL flag = SHGetPathFromIDListA(ppidl, pszProgramsPath);
        CoTaskMemFree(ppidl);
        return flag;
    }
    return FALSE;
}


BOOL ShortLink(LPCSTR lpszWorkDir, LPCSTR lpszDirName, int iShowCmd )
{
    WCHAR wsz[MAX_PATH];   //定义Unicode字符串
    IShellLinkW* pLink;     //IShellLink对象指针
    IPersistFile* ppf;      //IPersisFil对象指针
    //创建IShellLink对象
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
    if (FAILED(hr))
        return FALSE;
    //从IShellLink对象中获取IPersistFile接口
    hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
    if (FAILED(hr))
    {
        pLink->Release();
        return FALSE;
    }
    //对象全路径
    MultiByteToWideChar(CP_ACP, 0, lpszWorkDir, -1, wsz, MAX_PATH);
    if (lpszWorkDir != NULL)
        pLink->SetPath(wsz);
    //快捷键
    WORD wHotkey = 0;
    if (wHotkey != 0)
        pLink->SetHotkey(wHotkey);
    //备注
    LPCSTR lpszDescription = "备注";
    MultiByteToWideChar(CP_ACP, 0, lpszDescription, -1, wsz, MAX_PATH);
    if (lpszDescription != NULL)
        pLink->SetDescription(wsz);
    //显示方式
    pLink->SetShowCmd(iShowCmd);
    //修改扩展名 lpszDirName
    //char szBuffer[MAX_PATH];
    //sprintf(szBuffer, "%s", lpszDirName);
    //int nLen = strlen(szBuffer);
    //szBuffer[nLen - 3] = 'l';
    //szBuffer[nLen - 2] = 'n';
    //szBuffer[nLen - 1] = 'k';
    MultiByteToWideChar(CP_ACP, 0, lpszDirName, -1, wsz, MAX_PATH);
    //保存文件
    hr = ppf->Save(wsz, TRUE);
    ppf->Release();
    pLink->Release();
    return SUCCEEDED(hr);
}

BOOL CreateFileShortcut(LPCSTR lpszFileName, LPCSTR lpszLnkFileDir, LPCSTR lpszLnkFileName,
                        LPCSTR lpszWorkDir, WORD wHotkey, LPCSTR lpszDescription, int iShowCmd)
{
    WCHAR wsz[MAX_PATH];        //定义Unicode字符串
    IShellLink* pLink;          //IShellLink对象指针
    IPersistFile* ppf;          //IPersisFil对象指针
    //创建IShellLink对象

    // 建立合适的COM服务器  
    HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,IID_IShellLink, reinterpret_cast<LPVOID*>(&pLink));
    //HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&pLink);
    if (FAILED(hr))
        return FALSE;
    //从IShellLink对象中获取IPersistFile接口
    hr = pLink->QueryInterface(IID_IPersistFile, (void**)&ppf);
    if (FAILED(hr))
    {
        pLink->Release();
        return FALSE;
    }
    //目标
    //MultiByteToWideChar(CP_ACP, 0, lpszFileName, -1, wsz, MAX_PATH);
    //pLink->SetPath(lpszFileName == NULL ? NULL : wsz);
    //工作目录
    MultiByteToWideChar(CP_ACP, 0, lpszWorkDir, -1, wsz, MAX_PATH);
    if (lpszWorkDir != NULL)
        pLink->SetPath(wsz);
    //快捷键
    if (wHotkey != 0)
        pLink->SetHotkey(wHotkey);
    //备注
    MultiByteToWideChar(CP_ACP, 0, lpszDescription, -1, wsz, MAX_PATH);
    if (lpszDescription != NULL)
        pLink->SetDescription(wsz);
    //显示方式
    pLink->SetShowCmd(iShowCmd);
    //保存文件
    WCHAR* wsz2 = TEXT("c:/TEST.lnk");
    hr = ppf->Save(wsz2, TRUE);
    ppf->Release();
    pLink->Release();
    return SUCCEEDED(hr);
}

*/





//测试遍历文件夹
//vector<string> AllDir;
//string path = "c:/";
//PathFormat(path);
//GetDirFiles(path, AllDir,4);
//for (vector<string>::const_iterator it = AllDir.begin();
//    it != AllDir.end();++it)
//    LogFun(it->c_str());

//测试删除文件夹
//if (DelPathDir("D:/SYSDOC/down/sd"))
//    MessageBoxA(NULL, "DelPathDir", "BOX", MB_OK);
//if (SmartFile("D:/SYSDOC/down/sd"))
//    MessageBoxA(NULL, "SmartFile", "BOX", MB_OK);
//if (SmartDir("D:/SYSDOC/down/sd"))
//    MessageBoxA(NULL, "SmartDir", "BOX", MB_OK);

//待考察
/*
    bool UpPackExeRes(char* f_strFileName, unsigned short  f_wResID, char* f_strFileType)
    {
    //仅exe有效 dll获取不到句柄
    //************************************************************************
    //*  函数说明：释放资源中某类型的文件
    //* 参    数：新文件名、资源ID、资源类型
    //* 返 回 值：成功返回TRUE，否则返回FALSE
    //************************************************************************
    // 资源大小
    DWORD   dwWrite = 0;
    // 创建文件
    HANDLE  hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)    return FALSE;
    // 查找资源文件中、加载资源到内存、得到资源大小
    HRSRC   hrsc = FindResourceA(NULL, MAKEINTRESOURCEA(f_wResID), f_strFileType);
    HGLOBAL hG = LoadResource(NULL, hrsc);
    DWORD   dwSize = SizeofResource(NULL, hrsc);
    // 写入文件
    WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
    CloseHandle(hFile);
    return TRUE;
    }

    bool UpPackDllRes(char* f_dllname, char* f_strFileName, WORD f_wResID, char* f_strFileType)
    {
    //判断文件存在就不再释放
    if (!_access(f_strFileName, 0))  return false;
    //dll句柄
    HINSTANCE m_DLLhandle;
    //加载dll
    m_DLLhandle = ::LoadLibraryA(f_dllname);
    // 资源大小
    DWORD   dwWrite = 0;
    // 创建文件
    HANDLE  hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)    return FALSE;
    // 查找资源文件中、加载资源到内存、得到资源大小
    HRSRC   hrsc = FindResourceA(m_DLLhandle, MAKEINTRESOURCEA(f_wResID), f_strFileType);
    HGLOBAL hG = LoadResource(m_DLLhandle, hrsc);
    DWORD   dwSize = SizeofResource(m_DLLhandle, hrsc);
    // 写入文件
    WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
    CloseHandle(hFile);
    //释放资源
    FreeLibrary(m_DLLhandle);
    return false;
    }

    bool BitmapWriteFile(char* filename, int width, int height, unsigned char* bitmapData)
    {
    //WriteBitmapFile
    //根据bitmapData的（RGB）数据，保存bitmap
    //filename是要保存到物理硬盘的文件名（包括路径）
    //填充BITMAPFILEHEADER
    BITMAPFILEHEADER bitmapFileHeader;
    memset(&bitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
    bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
    bitmapFileHeader.bfType = 0x4d42;   //BM
    bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    //填充BITMAPINFOHEADER
    BITMAPINFOHEADER bitmapInfoHeader;
    memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
    bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfoHeader.biWidth = width;
    bitmapInfoHeader.biHeight = height;
    bitmapInfoHeader.biPlanes = 1;
    bitmapInfoHeader.biBitCount = 24;
    bitmapInfoHeader.biCompression = BI_RGB;
    bitmapInfoHeader.biSizeImage = width * abs(height) * 3;
    //////////////////////////////////////////////////////////////////////////
    FILE* filePtr;          //连接要保存的bitmap文件用
    unsigned char tempRGB;  //临时色素
    size_t imageIdx;
    //交换R、B的像素位置,bitmap的文件放置的是BGR,内存的是RGB
    for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3)
    {
    tempRGB = bitmapData[imageIdx];
    bitmapData[imageIdx] = bitmapData[imageIdx + 2];
    bitmapData[imageIdx + 2] = tempRGB;
    }
    filePtr = fopen(filename, "wb");
    if (NULL == filePtr)
    return false;
    fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
    fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
    fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, filePtr);
    fclose(filePtr);
    return true;
    }

    bool CopyBinFile(const char* f_src, const char* f_out)
    {
    if (f_out == NULL)
    return false;
    if (f_src == NULL)
    return false;
    bool bRet = true;
    std::ofstream fout(f_out, std::ios::binary | std::ios::app);
    std::ifstream fin(f_src, std::ios::binary);  //需要检查文件
    if (fin.bad())
    bRet = false;
    else
    {
    while (!fin.eof())
    {
    char szBuf[256] = { 0 };
    fin.read(szBuf, sizeof(char) * 256);
    if (fout.bad())
    {
    bRet = false;
    break;
    }
    fout.write(szBuf, sizeof(char) * 256);
    }
    }
    fin.close();
    fout.close();
    return bRet;
    }

    void Md5File(char* f_infile, char* f_outfile, char* f_pwd, bool f_flag)
    {
    FILE* fp1, *fp2;
    register char ch;
    int j = 0, j0 = 0;
    fp1 = fopen(f_infile, "r");      //打开要加密/解密的文件
    if (fp1 == NULL)
    return;
    fp2 = fopen(f_outfile, "w");
    if (fp2 == NULL)
    return;
    //--------------------------------------------------
    while (f_pwd[++j0]);
    ch = fgetc(fp1);
    while (!feof(fp1))
    {
    if (j0 > 7) j0 = 0;
    if (f_flag)
    ch += f_pwd[j0++];      //加密
    else
    ch -= f_pwd[j0++];      //解密
    fputc(ch, fp2);
    ch = fgetc(fp1);
    }
    //--------------------------------------------------
    fclose(fp1); //关闭源文件
    fclose(fp2); //关闭目标文件
    }

    bool WriteBinFile(const char* Filename)
    {
    std::ofstream fout("a.dat", std::ios::binary);
    int nNum = 20;
    std::string str("Helloworld");
    fout.write((char*)&nNum, sizeof(int));
    fout.write(str.c_str(), sizeof(char) * (str.size()));
    fout.close();
    return true;
    //写文本文件则比较简单，如下：
    std::ofstream fout2("b.dat");
    int nNum1 = 20;
    std::string str1("Hello, world");
    fout2 << nNum1 << "," << str1 << std::endl;
    fout2.close();
    //读二进制文件
    //读取二进制文件可以使用ifstream 类来进行，文件的打开模式一定要是 binary，如果传入的不是 binary, 文件将以ASCII方式打开。
    std::ifstream fin1("a.dat", std::ios::binary);
    int nNum2;
    char szBuf[256] = { 0 };
    fin1.read((char*)&nNum2, sizeof(int));
    fin1.read(szBuf, sizeof(char) * 256);
    //std::cout << "int = " << nNum << std::endl;
    //std::cout << "str = " << szBuf << std::endl;
    fin1.close();
    //而读取文本文件则比较简单：
    std::ifstream fin2("b.dat");
    int nNum3;
    char szBuf2[256] = { 0 };
    fin2 >> nNum3 >> szBuf2;
    //std::cout << "int = " << nNum << std::endl;
    //std::cout << "str = " << szBuf << std::endl;
    fin2.close();
    //  文件的打开模式
    //文件操作时，如果不显示指定打开模式，文件流类将使用默认值。
    //在<fstream> 中定义了如下打开模式和文件属性:
    //ios::app // 从后面添加
    //ios::ate // 打开并找到文件尾
    //ios::binary // 二进制模式I/O（与文本模式相对)
    //ios::in // 只读打开
    //ios::out // 写打开
    //ios::trunc // 将文件截为 0 长度
    //可以使用位操作符 OR 组合这些标志，比如
    //ofstream logFile("log.dat", ios::binary | ios::app);
    }

    void binWriteFile(const char* Filename)
    {
    //下面能实现向二进制文件重写数据
    //std::ofstream fout(Filename, std::ios::binary);
    //int nNum = 20;
    //std::string str("Hello, world");
    //fout.write((char*)&nNum, sizeof(int));
    //fout.write(str.c_str(), sizeof(char) * (str.size()));
    //fout.close();
    //写入二进制文件
    std::fstream iofile(Filename, ios::in | ios::out | ios::binary); //二进制覆写
    if (iofile)
    {
    const char* txt = "test";
    iofile.seekp(198, ios::beg);      //定位位置
    //iofile.read((char *)&stud1[i/2],sizeof(stud1[0]));    //读数据
    for (int i = 0; i < 2; i++)
    iofile.write(txt, sizeof(txt));  //写入值
    iofile.seekp(598, ios::beg);      //定位位置
    for (int i = 0; i < 3; i++)
    iofile.write(txt, sizeof(txt));  //写入值
    iofile.close();
    }
    //http://blog.csdn.net/sxhelijian/article/details/29557915
    }


*/
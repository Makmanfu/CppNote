
#include "StdAfx.h"
#include "WinFileSys.h"

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

WinFileSys::WinFileSys()
{
}

WinFileSys::~WinFileSys()
{
}

void WinFileSys::PathFormat(string& path, bool endflg)
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
    if (endflg)                    //末尾判断加"\\"
    {
        if (*(path.end() - 1) != slash)
            path += slash;
    }
}

bool WinFileSys::CheckPathFile(const string& pathfile)
{
    string pathfiletmp(pathfile);
    PathFormat(pathfiletmp);          //路径格式化
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind = FindFirstFileA(pathfiletmp.c_str(), &FindFileData);
    bool rValue(false);
    //判断可能是文件夹
    if ((hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        rValue = true;
    //判断可能是文件
    if (hFind != INVALID_HANDLE_VALUE)
        rValue = true;
    //判断可能是盘符
    if (GetDriveTypeA(pathfiletmp.c_str()) != DRIVE_NO_ROOT_DIR)
        rValue = true;
    FindClose(hFind);
    return rValue;
}

bool WinFileSys::IsDirectory(const string& pathfile)
{
    DWORD dwAttr = ::GetFileAttributesA(pathfile.c_str());  //得到文件属性
    if (dwAttr == 0xFFFFFFFF)                   // 文件或目录不存在
        return false;
    else if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)   // 如果是目录
        return true;
    else
        return false;
}

string WinFileSys::GetExePath(EXETYPE idtype)
{
    char szAppPath[MAX_PATH];
    GetModuleFileNameA(NULL, szAppPath, MAX_PATH);    //GetCurrentDirectoryA
    string ret(szAppPath);
    size_t pos0 = ret.find_last_of("\\");
    switch (idtype)
    {
    case PATHALL: default:                          //"c:\1\curr.exe"
        break;
    case PATHEXE:                                   //"curr.exe"
        ret = ret.substr(pos0 + 1, ret.size() - pos0 - 1);
        break;
    case PATHBF:                                    //"c:\1"
        ret = ret.substr(0, pos0);                  //或者(strrchr(szAppPath, '\\'))[0] = 0; ret = szAppPath
        break;
    case PATHBFA:                                   //"c:\1\"
        ret = ret.substr(0, pos0 + 1);              //或者(strrchr(szAppPath, '\\'))[1] = 0; ret = szAppPath
        break;
    case PATHEXENAME:                               //curr   &(strrchr(szAppPath, '\\'))[1];
        ret = ret.substr(pos0 + 1, ret.size() - pos0 - 5);
        break;
    }
    return ret;
}

void WinFileSys::GetAllDisk(vector<string>& AllDrv)
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

bool WinFileSys::CreatePathDir(const string& path)
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

void WinFileSys::SetAtt(const string& pathfile)
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

int WinFileSys::GetDirFiles(const string& path, vector<string>& AllDir, DIRTYPE idtype)
{
    WIN32_FIND_DATAA FindDTT;
    string Sear(path), format("*.*");                               //扩展名格式
    PathFormat(Sear, true);                                         //路径格式化
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
            default:                                                //全递归
                AllDir.push_back(Sear + FindDTT.cFileName);
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALL);
                break;
            case DALLDIR:                                           //仅递归文件夹
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    AllDir.push_back(Sear + FindDTT.cFileName);
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALLDIR);
                }
                break;
            case DALLFILE:                                          //仅递归文件
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALLFILE);
                else
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
            case DCURRDIR:                                          //当前文件夹
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
            case DCURRFILE:                                         //当前文件
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

bool WinFileSys::DelPathDir(const string& path)
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

bool WinFileSys::SmartFile(const string& path, FileProc fp)
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

bool WinFileSys::SmartDir(const string& path, FileProc fp)
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







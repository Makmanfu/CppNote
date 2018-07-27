
#include "StdAfx.h"
#include "WinFileSys.h"

//stringʵ��splite
vector<string> split(string str, string pattern)
{
    string::size_type pos;
    vector<string> result;
    str += pattern;   //��չ�ַ����Է������
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
    char slash = '\\', backslash = '/'; //б��,��б��
    int flag = 0;
    for (string::iterator it = path.begin(); it != path.end();)
    {
        if (*it == backslash)
            *it = slash;                //���滻����/Ϊ'\'
        if (1 == flag && *it == slash)  //�����ظ���
            it = path.erase(it);
        else
        {
            flag = (*it == slash) ? 1 : 0;
            ++it;
        }
    }
    if (endflg)                    //ĩβ�жϼ�"\\"
    {
        if (*(path.end() - 1) != slash)
            path += slash;
    }
}

bool WinFileSys::CheckPathFile(const string& pathfile)
{
    string pathfiletmp(pathfile);
    PathFormat(pathfiletmp);          //·����ʽ��
    WIN32_FIND_DATAA FindFileData;
    HANDLE hFind = FindFirstFileA(pathfiletmp.c_str(), &FindFileData);
    bool rValue(false);
    //�жϿ������ļ���
    if ((hFind != INVALID_HANDLE_VALUE) && (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        rValue = true;
    //�жϿ������ļ�
    if (hFind != INVALID_HANDLE_VALUE)
        rValue = true;
    //�жϿ������̷�
    if (GetDriveTypeA(pathfiletmp.c_str()) != DRIVE_NO_ROOT_DIR)
        rValue = true;
    FindClose(hFind);
    return rValue;
}

bool WinFileSys::IsDirectory(const string& pathfile)
{
    DWORD dwAttr = ::GetFileAttributesA(pathfile.c_str());  //�õ��ļ�����
    if (dwAttr == 0xFFFFFFFF)                   // �ļ���Ŀ¼������
        return false;
    else if (dwAttr&FILE_ATTRIBUTE_DIRECTORY)   // �����Ŀ¼
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
        ret = ret.substr(0, pos0);                  //����(strrchr(szAppPath, '\\'))[0] = 0; ret = szAppPath
        break;
    case PATHBFA:                                   //"c:\1\"
        ret = ret.substr(0, pos0 + 1);              //����(strrchr(szAppPath, '\\'))[1] = 0; ret = szAppPath
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
    //��ȡ�߼������������
    GetLogicalDriveStringsA(BUFSIZE - 1, szLogicalDriveStrings);
    szDrive = (PCHAR)szLogicalDriveStrings;
    //ѭ����������������
    do
    {
        AllDrv.push_back(szDrive);
        szDrive += (lstrlenA(szDrive) + 1);
    }
    while (*szDrive != '\x00');
}

bool WinFileSys::CreatePathDir(const string& path)
{
    //����ԭ����ǰpath
    CHAR szCurrentDir[BUFSIZE];
    ZeroMemory(szCurrentDir, BUFSIZE);
    GetCurrentDirectoryA(BUFSIZE - 1, szCurrentDir);
    string pathtmp(path), pathnew;
    //·����ʽ��
    PathFormat(pathtmp);
    //·���ָ�
    vector<string> dirs = split(pathtmp, "\\");
    for (vector<string>::const_iterator it = dirs.begin(); it != dirs.end(); ++it)
    {
        pathnew += *it + "\\";
        if (!CheckPathFile(pathnew))
            if (!CreateDirectoryA(it->c_str(), NULL)) //�����ļ���
                return false;
        SetCurrentDirectoryA(pathnew.c_str());
    }
    //��ԭԭ����ǰpath
    SetCurrentDirectoryA((PCHAR)szCurrentDir);
    return true;
}

void WinFileSys::SetAtt(const string& pathfile)
{
    //�޸Ĵ������޸�ʱ��
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
    //�޸��ļ�����
    //SetFileAttributesA("MyGUI64.exe", FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
    SetFileAttributesA("MyGUI64.exe", !FILE_ATTRIBUTE_READONLY | !FILE_ATTRIBUTE_HIDDEN | !FILE_ATTRIBUTE_SYSTEM);
}

int WinFileSys::GetDirFiles(const string& path, vector<string>& AllDir, DIRTYPE idtype)
{
    WIN32_FIND_DATAA FindDTT;
    string Sear(path), format("*.*");                               //��չ����ʽ
    PathFormat(Sear, true);                                         //·����ʽ��
    HANDLE hFind = FindFirstFileA((Sear + format).c_str(), &FindDTT);
    if (hFind == INVALID_HANDLE_VALUE)
        return -1;                                                  //ʲô��û�ҵ�
    do
    {
        if (!strcmp(FindDTT.cFileName, "..") || !strcmp(FindDTT.cFileName, "."))
            continue;                                               //�˼�û���ļ�
        switch (idtype)
        {
            case DALL:
            default:                                                //ȫ�ݹ�
                AllDir.push_back(Sear + FindDTT.cFileName);
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALL);
                break;
            case DALLDIR:                                           //���ݹ��ļ���
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                {
                    AllDir.push_back(Sear + FindDTT.cFileName);
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALLDIR);
                }
                break;
            case DALLFILE:                                          //���ݹ��ļ�
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    GetDirFiles(Sear + FindDTT.cFileName, AllDir, DALLFILE);
                else
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
            case DCURRDIR:                                          //��ǰ�ļ���
                if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
            case DCURRFILE:                                         //��ǰ�ļ�
                if (FindDTT.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
                    AllDir.push_back(Sear + FindDTT.cFileName);
                break;
        }
    }
    while (TRUE == FindNextFileA(hFind, &FindDTT));
    FindClose(hFind);
    return AllDir.size();                                           //���ز�������
#ifdef GetDirFiles_old     //������ ���㷨
    struct _finddata_t filefind;
    string curr = ChkPachSkew(f_path, false) + "*.*"; //�����ļ�
    int pResultNum(0);                               //�������������
    intptr_t done = 0, handle;
    if ((handle = _findfirst(curr.c_str(), &filefind)) == -1)
        return 0;                                       //ʲô��û�ҵ�
    while (!(done = _findnext(handle, &filefind)))
    {
        if (!strcmp(filefind.name, "..")) continue; //�˼�û���ļ�
        switch (f_type)
        {
            default:
            case 0:  /*�ݹ������ļ��к��ļ��б�*/
                if ((filefind.attrib & _A_SUBDIR))      //�ļ��Ƿ�Ϊ�ļ���
                {
                    curr = ChkPachSkew(f_path + filefind.name, false);
                    pResultNum += GetDirFileList(curr, 0); //�ݹ���ļ���
                }
                else
                {
                    ++pResultNum;
                    cout << f_path + filefind.name << endl;
                    //LogFun( (f_path + filefind.name).c_str(),"filelist.log" );
                }
                break;
            case 1:         /*�õ��˼�Ŀ¼�������ļ�(�����ļ���)�б�*/
                if ((filefind.attrib & _A_SUBDIR)) //�ļ��Ƿ�Ϊ�ļ���
                    continue;
                else
                {
                    ++pResultNum;
                    //pResult += f_path + filefind.name;
                }
                break;
            case 2:         /*�ݹ���������ļ����б�*/
                if ((filefind.attrib & _A_SUBDIR))      //�ļ��Ƿ�Ϊ�ļ���
                {
                    cout << (curr = ChkPachSkew(f_path + filefind.name, false)) << endl; //����д���ļ���ȥ
                    LogFun((f_path + filefind.name).c_str(), "filelist.log", 2);
                    ++pResultNum;
                    pResultNum += GetDirFileList(curr, 2); //���µݹ��ļ��к͵�ǰ�ļ�������
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
    string Sear(path + "\\"), format("*.*");                //��չ����ʽ
    HANDLE hFind = FindFirstFileA((Sear + format).c_str(), &FindDTT);
    if (hFind == INVALID_HANDLE_VALUE)
        return false;                                       //ʲô��û�ҵ�
    do
    {
        if (!strcmp(FindDTT.cFileName, "..") || !strcmp(FindDTT.cFileName, "."))
            continue;                                       //�˼�û���ļ�
        if (FindDTT.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
            DelPathDir(Sear + FindDTT.cFileName);           //�ݹ�ɾ���ļ���
        else
            DeleteFileA((Sear + FindDTT.cFileName).c_str());
    }
    while (TRUE == FindNextFileA(hFind, &FindDTT));
    FindClose(hFind);
    return RemoveDirectoryA(path.c_str());                  //���ɾ���˿��ļ���
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







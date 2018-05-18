
#include "StdAfx.h"
#include "WinSysInfo.h"
#include <io.h>
#include <winreg.h>
#include <ctime>
#include <fstream>
#include <TlHelp32.h>
#include <sys/timeb.h>
#include <climits>


bool RunCheckOnly(char* f_tname /*= "app"*/)
{
    //程序运行一个实例
    static HANDLE SQ_variable_hMutex;
    //程序实例检查
    SQ_variable_hMutex = CreateMutexA(NULL, true, f_tname);
    if ((GetLastError() == ERROR_ALREADY_EXISTS) || (GetLastError() == ERROR_INVALID_HANDLE))
        return false;   //ShowBox("该程序已在运行");
    return true;
}

void LoadDllPro(char* Dllname, char* ProName)
{
    HINSTANCE hInst;
    hInst = LoadLibraryA(Dllname);
    //定义函数指针
    typedef void(*FunProc)(void);
    //强制类型转换
    FunProc NewFun = (FunProc)GetProcAddress(hInst, ProName);
    char SZBUF[255] = { 0 };
    sprintf(SZBUF, "没有发现%d", Dllname);
    //通过上面函数指针 得到函数就是NewFun
    if (!NewFun)
        MessageBoxA(NULL, SZBUF, "ERROR", MB_OK);
    else
        NewFun();
}

void SetAutoRun(LPSTR RegName, bool fsetrun)
{
    if (!strcmp(RegName, ""))
        RegName = "AutoRun";
    char bufname[260];
    GetModuleFileNameA(NULL, bufname, 260);  //获得路径 此时格式是c:\1\2.exe
    HKEY sub;
    if (fsetrun)
    {
        //用这个也行//HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run
        ::RegCreateKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &sub);
        ::RegSetValueExA(sub, RegName, NULL, REG_SZ, (BYTE*)bufname, 260);      //设置开机运行
    }
    else
    {
        ::RegCreateKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &sub);
        ::RegDeleteValueA(sub, RegName);                                        //取消开机运行
    }
}

void GetSysVersion(void)
{
    OSVERSIONINFOEX osver;
    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    //获取版本信息
    if (!GetVersionExA((LPOSVERSIONINFOA)&osver))
        printf("Error:%d", GetLastError());
    //打印版本信息
    printf("System info:%s,%s", osver.dwMajorVersion, osver.dwMinorVersion);
    printf("Build:%s", osver.dwBuildNumber);
    printf("Service Pack:%s,%s", osver.wServicePackMajor, osver.wServicePackMinor);
}

/*
    //获取CPU厂商
    string GetCpuVendor(void)
    {
    char* pvendor = new char[64];
    INT32 dwBuf[4];
    if (NULL == pvendor)    return 0;
    // Function 0: Vendor-ID and Largest Standard Function
    __cpuid(dwBuf, 0);
    // save. 保存到pvendor
     (INT32*)&pvendor[0] = dwBuf[1];  // ebx: 前四个字符
     (INT32*)&pvendor[4] = dwBuf[3];  // edx: 中间四个字符
     (INT32*)&pvendor[8] = dwBuf[2];  // ecx: 最后四个字符
    pvendor[12] = '\0';
    return pvendor;
    }

    //获取CPU商标
    string GetCpuBrand(void)
    {
    char* pbrand = new char[64];
    INT32 dwBuf[4];
    if (NULL == pbrand) return 0;
    // Function 0x80000000: Largest Extended Function Number
    __cpuid(dwBuf, 0x80000000);
    if (dwBuf[0] < 0x80000004)  return 0;
    // Function 80000002h,80000003h,80000004h: Processor Brand String
    __cpuid((INT32*)&pbrand[0], 0x80000002);  // 前16个字符
    __cpuid((INT32*)&pbrand[16], 0x80000003);  // 中间16个字符
    __cpuid((INT32*)&pbrand[32], 0x80000004);  // 最后16个字符
    pbrand[48] = '\0';
    return pbrand;
    }
*/

string GetCPUid(void)
{
    char cputmpid[255];
    //char szBuf[64];
    INT32 dwBuf[4];
    __cpuidex(dwBuf, 0, 0);
    __cpuid(dwBuf, 0);
    sprintf(cputmpid, "%.8X%.8X%.8X%.8X", dwBuf[0], dwBuf[1], dwBuf[2], dwBuf[3]);
    return cputmpid;
    //x86下
    //int s1,s2;
    //std::string f_out;
    //char cputmpid[255];
    //__asm{
    //    mov eax,01h
    //        xor edx,edx
    //        cpuid
    //        mov s1,edx
    //        mov s2,eax
    //}
    //sprintf(cputmpid,"%08X%08X",s1,s2);
    //cout<<cputmpid<<endl;
    //f_out = string(cputmpid);
    //__asm{
    //    mov eax,03h
    //        xor ecx,ecx
    //        xor edx,edx
    //        cpuid
    //        mov s1,edx
    //        mov s2,ecx
    //}
    //sprintf(cputmpid,"%08X%08X",s1,s2);
    //cout<<cputmpid<<endl;
    //f_out += string(cputmpid);
    //return f_out;
    //++++++++cpuid++++++++++++++++++++++++++++++++++++
    //#if defined(_WIN64)
    //    // 64位下不支持内联汇编. 应使用__cpuid、__cpuidex等Intrinsics函数。
    //#else
    //#if _MSC_VER < 1600 // VS2010. 据说VC2008 SP1之后才支持__cpuidex
    //    void __cpuidex(INT32 CPUInfo[4], INT32 InfoType, INT32 ECXValue)
    //    {
    //        if (NULL == CPUInfo)    return;
    //        _asm
    //        {
    //            // load. 读取参数到寄存器
    //            mov edi, CPUInfo;   // 准备用edi寻址CPUInfo
    //            mov eax, InfoType;
    //            mov ecx, ECXValue;
    //            // CPUID
    //            cpuid;
    //            // save. 将寄存器保存到CPUInfo
    //            mov[edi], eax;
    //            mov[edi + 4], ebx;
    //            mov[edi + 8], ecx;
    //            mov[edi + 12], edx;
    //        }
    //    }
    //#endif  // #if _MSC_VER < 1600  // VS2010. 据说VC2008 SP1之后才支持__cpuidex
    //
    //#if _MSC_VER < 1400 // VC2005才支持__cpuid
    //    void __cpuid(INT32 CPUInfo[4], INT32 InfoType)
    //    {
    //        __cpuidex(CPUInfo, InfoType, 0);
    //    }
    //#endif  // #if _MSC_VER < 1400  // VC2005才支持__cpuid
    //
    //#endif  // #if defined(_WIN64)
    //========cpuid====================================
}

void GetCPUInfo(void)
{
}

void GetSysInfo(void)
{
    ////获取网络用户名称
    //char username[20];
    //unsigned long size1 = 20;
    //GetUserName(username, &size1);
    ////获得计算机名称
    //WORD wVersionRequested;
    //WSADATA wsaData;
    //char name[255];
    //wVersionRequested = MAKEWORD(2, 0);
    //if (WSAStartup(wVersionRequested, &wsaData) == 0)
    //{
    //    gethostname(name, sizeof(name));
    //    WSACleanup();
    //}
}

void GetUsers(void)
{
}

void AddUsers(void)
{
}

void DelUser(void)
{
}

void GetTime(void)
{
}

int SetBrightness(int f_bright)
{
    //调整亮度 参数0-390    成功返回0  失败返回负数
    if (f_bright > 390)
        f_bright = 390;
    void* lpGamma = NULL;
    int iArrayValue;
    WORD gMap[3][256] = { 0 };
    lpGamma = &gMap;
    HDC hdc = ::GetDC(NULL);
    if (NULL == hdc)
        return -1;
    for (int i = 0; i < 256; i++)
    {
        iArrayValue = i * (f_bright + 128);
        if (iArrayValue > 65535)
            iArrayValue = 65535;
        gMap[0][i] = gMap[1][i] = gMap[2][i] = (WORD)iArrayValue;
    }
    if (FALSE == SetDeviceGammaRamp(hdc, lpGamma))
        return -2;
    return 0;
}

int SetDisplayState(int fState /*= 2*/)
{
    //关闭显示器[1低能耗状态2直接关闭-1打开显示器]
    return PostMessage(HWND_BROADCAST, WM_SYSCOMMAND, SC_MONITORPOWER, fState);
}

bool ChngDsplyMd(int f_WidthP, int f_HeightP, int f_intBitsPer)
{
    DEVMODE lpDevMode;
    lpDevMode.dmBitsPerPel = f_intBitsPer;
    lpDevMode.dmPelsWidth = f_WidthP;
    lpDevMode.dmPelsHeight = f_HeightP;
    lpDevMode.dmSize = sizeof(lpDevMode);
    lpDevMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
    LONG result = ChangeDisplaySettings(&lpDevMode, 0);
    if (result == DISP_CHANGE_SUCCESSFUL)
    {
        //AfxMessageBox("修改成功");
        ChangeDisplaySettings(&lpDevMode, CDS_UPDATEREGISTRY);
        //使用CDS_UPDATEREGISTRY表示次修改是持久的,
        //并在注册表中写入了相关的数据
        return true;
    }
    else
    {
        //AfxMessageBox("修改失败，恢复原有设置");
        ChangeDisplaySettings(NULL, 0);
        return false;
    }
}

void SetSysShut_GetNT(void)
{
    DWORD dwVersion = GetVersion();
    if (dwVersion < 0x80000000)             //是NT系列的系统
    {
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;
        OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
        LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    }
}

void SetSysShut(int f_State /*= 1*/)
{
    switch (f_State)
    {
        case 0:
        default:                                 //闪电关机
        {
            const unsigned int SE_SHUTDOWN_PRIVILEGE = 0x13;
            HMODULE hDll = ::LoadLibraryA("ntdll.dll");
            typedef int(*type_RtlAdjustPrivilege)(int, bool, bool, int*);
            typedef int(*type_ZwShutdownSystem)(int);
            type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");
            type_ZwShutdownSystem ZwShutdownSystem = (type_ZwShutdownSystem)GetProcAddress(hDll, "ZwShutdownSystem");
            int nEn = 0;
            int nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, true, &nEn);
            if (nResult == 0x0c000007c)
                nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &nEn);
            nResult = ZwShutdownSystem(2); //2 shutdown
            FreeLibrary(hDll);
        }
        break;
        case 4:
        {
            const unsigned int SE_SHUTDOWN_PRIVILEGE = 0x13;
            HMODULE hDll = ::LoadLibraryA("ntdll.dll");
            typedef int(*type_RtlAdjustPrivilege)(int, bool, bool, int*);
            typedef int(*type_ZwShutdownSystem)(int);
            type_RtlAdjustPrivilege RtlAdjustPrivilege = (type_RtlAdjustPrivilege)GetProcAddress(hDll, "RtlAdjustPrivilege");
            type_ZwShutdownSystem ZwShutdownSystem = (type_ZwShutdownSystem)GetProcAddress(hDll, "ZwShutdownSystem");
            int nEn = 0;
            int nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, true, &nEn);
            if (nResult == 0x0c000007c)
                nResult = RtlAdjustPrivilege(SE_SHUTDOWN_PRIVILEGE, true, false, &nEn);
            nResult = ZwShutdownSystem(1); //reboot
            FreeLibrary(hDll);
        }
        break;
        case 1:                                         //系统关机
        {
            SetSysShut_GetNT();
            ExitWindowsEx(EWX_SHUTDOWN, 0);
        }
        break;
        case 2:                                         //Reboot 系统重启
        {
            SetSysShut_GetNT();
            ExitWindowsEx(EWX_REBOOT, 0);
        }
        break;
        case 3:     //Logoff  系统注销
        {
            SetSysShut_GetNT();
            ExitWindowsEx(EWX_LOGOFF, 0);
        }
        break;
    }
}

string GetThisNowTime()
{
    time_t t = time(0);
    char buffer[9] = { 0 };
    strftime(buffer, 9, "%H:%M:%S", localtime(&t));
    //strftime(buffer, 9, "%H:%M", localtime(&t));
    return string(buffer);
}

string GetMilliNum(void)
{
    SYSTEMTIME CurTime;
    GetLocalTime(&CurTime);
    //得出时间
    char* szBuffer = (char*)malloc(17 * sizeof(char));
    sprintf(szBuffer, "%04d%02d%02d%02d%02d%02d%03d",
            CurTime.wYear, CurTime.wMonth, CurTime.wDay, CurTime.wHour, CurTime.wMinute, CurTime.wSecond, CurTime.wMilliseconds);
    //stringstream tmp;
    //tmp<<CurTime.wYear<<CurTime.wMonth<<CurTime.wDay<<CurTime.wHour<<CurTime.wMinute<<CurTime.wSecond<<CurTime.wMilliseconds;
    //return tmp.str();
    return (string)szBuffer;
}

char* GetGUID(bool f_num)
{
    static char buf[64] = { 0 };
    GUID guid;
    if (S_OK == ::CoCreateGuid(&guid))
    {
        if (f_num)
        {
            //_snprintf_s
            printf(buf, sizeof(buf), "%08X%04X%04x%02X%02X%02X%02X%02X%02X%02X%02X"
                   , guid.Data1, guid.Data2, guid.Data3
                   , guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
                   , guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        }
        else
        {
            printf(buf, sizeof(buf), "{%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X}"
                   , guid.Data1, guid.Data2, guid.Data3
                   , guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3]
                   , guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
        }
    }
    return (char*)buf;                   //如果要转换成string      return std::string(buf);
}

bool UPackExeRes(char* f_strFileName, WORD f_wResID, char* f_strFileType)
{
    //仅exe有效 dll获取不到句柄
    /************************************************************************/
    /*  函数说明：释放资源中某类型的文件
        /* 参    数：新文件名、资源ID、资源类型
        /* 返 回 值：成功返回TRUE，否则返回FALSE
        /************************************************************************/
    //if (!_access(f_strFileName, 0))
    //    return false;
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

bool UPackDllRes(char* f_dllname, char* f_strFileName, WORD f_wResID, char* f_strFileType)
{
#ifdef _WIN64
    string str1(f_dllname);
    string str2(str1, 0, str1.size() - 4);
    str2 += "64.dll";
    //cout<<str2.c_str()<<endl;
    f_dllname = const_cast<char*>(str2.c_str());
    //char* tmpchar = new char[strlen(f_dllname)+2]; //算出长
    //cout<<tmpchar<<endl;
    ////strcpy(tmpchar,f_dllname);
    //strncpy(tmpchar,f_dllname,4);
    //cout<<tmpchar<<endl;
    //cout<< strcat(tmpchar,"64.dll")<<endl;
#endif
    //判断当前文件存在就不再释放
    if (!_access(f_strFileName, 0))  return false;
    //dll句柄
    HINSTANCE m_DLLhandle;
    //加载dll
    m_DLLhandle = ::LoadLibraryA(f_dllname);
    //资源大小
    DWORD   dwWrite = 0;
    //创建文件
    HANDLE  hFile = CreateFileA(f_strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)    return FALSE;
    //查找资源文件中、加载资源到内存、得到资源大小
    HRSRC   hrsc = FindResourceA(m_DLLhandle, MAKEINTRESOURCEA(f_wResID), f_strFileType);
    HGLOBAL hG = LoadResource(m_DLLhandle, hrsc);
    DWORD   dwSize = SizeofResource(m_DLLhandle, hrsc);
    //写入文件
    WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
    CloseHandle(hFile);
    //释放资源
    FreeLibrary(m_DLLhandle);
    return false;
}

void CalcRunTime(void(*FUNProc)(void))
{
#define PLANFUN2
#ifdef PLANFUN1
    //计时方式一  精确到秒
    time_t start = 0, end = 0;
    time(&start);
    FUNProc();
    time(&end);
    printf("运行了：%d 秒\n", (end - start));
    //std::cout << "运行了：" << (end - start) << "秒" << std::endl;
#endif

#ifdef PLANFUN2
    //计时方式二  精确到毫秒
    struct timeb startTime, endTime;
    ftime(&startTime);
    FUNProc();             //FUNProc();
    ftime(&endTime);
    printf("运行了：%d 毫秒\n", (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm));
    //std::cout << "运行了：" << (endTime.time - startTime.time) * 1000 + (endTime.millitm - startTime.millitm) << "毫秒" << std::endl;
#endif

#ifdef PLANFUN3
    //计时方式三
    clock_t startCTime, endCTime;
    startCTime = clock();   //clock函数返回CPU时钟计时单元（clock tick）数，还有一个常量表示一秒钟有多少个时钟计时单元，可以用clock()/CLOCKS_PER_SEC来求取时间
    FUNProc();
    endCTime = clock();
    printf("运行了：%d 秒\n", double((endCTime - startCTime) / CLOCKS_PER_SEC));
    //std::cout << "运行了：" << double((endCTime - startCTime) / CLOCKS_PER_SEC) << "秒" << std::endl;
#endif
}

void SignFlagEXE(const char* exename, int ver, int verr)
{
    std::fstream iofile(exename, ios::in | ios::out | ios::binary);    //二进制覆写
    if (iofile)   //判断打开成功
    {
        SYSTEMTIME CurTime;
        GetLocalTime(&CurTime);
        char* buf = (char*)malloc(58 * sizeof(char));
        sprintf(buf, "@BUILDING FOR AGAN!$MSVC%02d-%02d-%03d %04d-%02d-%02d %02d:%02d:%02d:%03d\n",
            _MSC_VER / 100, ver, verr, CurTime.wYear, CurTime.wMonth, CurTime.wDay, CurTime.wHour,
            CurTime.wMinute, CurTime.wSecond, CurTime.wMilliseconds);
        iofile.seekp(2, ios::beg);                  //寻找位置
        iofile.write(buf, 58/*sizeof(buf)*/);       //头部
        free(buf);
        char* buf2 = (char*)malloc(52 * sizeof(char));
        sprintf(buf2, "WHATEVER IS WORTH DOING IS WORTH DOING WELL! PROGRAM\n");
        iofile.seekp(64, ios::beg);                 //寻找位置
        iofile.write(buf2, 52/*sizeof(buf)*/);      //头部
        free(buf2);
        iofile.close();
    }
    //main中调用
    //if (argc > 1)
    //    SignFlagEXE(argv[1], 0, 1);
}

void RaiseToDebug(void)
{
    HANDLE hToken, hProcess = GetCurrentProcess(); // 获取当前进程句柄
    // 打开当前进程的Token，就是一个权限令牌，第二个参数可以用TOKEN_ALL_ACCESS
    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        TOKEN_PRIVILEGES tkp;
        if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid))
        {
            tkp.PrivilegeCount = 1;
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            //通知系统修改进程权限
            BOOL bREt = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
        }
        CloseHandle(hToken);
    }
}

int RunShell(char* f_exeName)
{
    WinExec(f_exeName, SW_SHOW);
    return 1;
}

char* KillProcess_WidetoAnsi(wchar_t* pWideChar)
{
    if (NULL == pWideChar)
        return NULL;
    char* pAnsi = NULL;
    int needBytes = WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, NULL, 0, NULL, NULL);
    if (needBytes > 0)
    {
        pAnsi = new char[needBytes + 1];
        ZeroMemory(pAnsi, needBytes + 1);
        WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, pAnsi, needBytes, NULL, NULL);
    }
    return pAnsi;
}

int KillProcess(char* f_exeName, bool f_Must)
{
    if (f_Must)
        RaiseToDebug(); //提高权限
    int rc = 0;
    HANDLE hSysSnapshot = NULL;
    PROCESSENTRY32 proc;
    hSysSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSysSnapshot == (HANDLE)-1) return 1;
    proc.dwSize = sizeof(proc);
    if (Process32First(hSysSnapshot, &proc))
    {
        do
        {
#ifdef UNICODE
            if (_stricmp(KillProcess_WidetoAnsi(proc.szExeFile), f_exeName) == 0)
#else
            if (_stricmp(proc.szExeFile, f_exeName) == 0)
#endif
            {
                HANDLE Proc_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, proc.th32ProcessID);
                rc = (Proc_handle == NULL || !TerminateProcess(Proc_handle, 0)) ? 1 : 0;
            }
        } while (Process32Next(hSysSnapshot, &proc));
    }
    CloseHandle(hSysSnapshot);
    return rc;
}

string ProcessList_WChar2Ansi(LPCWSTR pwszSrc)
{
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen <= 0) return std::string("");
    char* pszDst = new char[nLen];
    if (NULL == pszDst) return std::string("");
    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    pszDst[nLen - 1] = 0;
    std::string strTemp(pszDst);
    delete[] pszDst;
    return strTemp;
}

void ProcessList(vector<string>& f_prolist)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return;
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    //遍历进程快照。轮流显示每个进程的信息
    BOOL bMore = Process32First(hProcessSnap, &pe32);
    //printf("PID\t线程数\t进程名称\n");
    while (bMore)
    {
        bMore = Process32Next(hProcessSnap, &pe32);
        //打印到命令行
        //wprintf(L"%u\t%u\t%s\n", pe32.th32ProcessID, pe32.cntThreads, pe32.szExeFile);
        //cout<<WChar2Ansi(pe32.szExeFile)<<endl;
        f_prolist.push_back(ProcessList_WChar2Ansi(pe32.szExeFile));
    }
    CloseHandle(hProcessSnap);   //清除snapshot对象
}



























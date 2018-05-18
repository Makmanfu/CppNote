//-----------------------------------------------------------------------------
//
//                  windows 系统操作总结
//                  注册表 获得硬件信息 时间等
//                                          @2014-03-10阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;


//程序唯一性检查
SQC_API bool RunCheckOnly(char* f_tname = "app");
//动态调用dll方法
SQC_API void LoadDllPro(char* Dllname, char* ProName);
//程序自启动注册表
SQC_API void SetAutoRun(LPSTR RegName, bool fsetrun = true);
//获得系统版本
SQC_API void GetSysVersion(void);
//获得系统cpu信息cpuid
SQC_API void GetCPUInfo(void);
//获取CPUid
SQC_API string GetCPUid(void);
//获得系统名称等信息
SQC_API void GetSysInfo(void);
//获得用户或用户组
SQC_API void GetUsers(void);
//添加用户或用户组
SQC_API void AddUsers(void);
//删除用户或用户组
SQC_API void DelUser(void);
//获得系统时间(时间格式)
SQC_API void GetTime(void);
//设置屏幕亮度[参数0-390]
SQC_API int SetBrightness(int f_bright);
//设置显示器显示状态
SQC_API int SetDisplayState(int fState = 2);
//修改分辨率[宽高颜色]
SQC_API bool ChngDsplyMd(int f_WidthP, int f_HeightP, int f_intBitsPer);
//设置系统[0闪电关机/4闪电重启/1关机/2重启/3注销]
SQC_API void SetSysShut(int f_State = 1);
//获取时间
SQC_API string GetThisNowTime(void);
//获取唯一ID
SQC_API string GetMilliNum(void);
//GUID(num=真时是一串数)
SQC_API char* GetGUID(bool f_num = false);
//exe内部资源释放方法
SQC_API bool UPackExeRes(char* f_strFileName, WORD f_wResID, char* f_strFileType);
//动态调用Dll释放资源方法
SQC_API bool UPackDllRes(char* f_dllname, char* f_strFileName, WORD f_wResID, char* f_strFileType);
//计算算法时间方法
SQC_API void CalcRunTime(void(*FUNProc)(void));
//程序签名58字节 +52字节[ver-verr 00-000]
SQC_API void SignFlagEXE(const char* exename, int ver = 0, int verr = 0);
//获取Debug权限
SQC_API void RaiseToDebug(void);
//运行外部exe程序
SQC_API int RunShell(char* f_exeName);
//强制结束进程
SQC_API int KillProcess(char* f_exeName, bool f_Must = false);
//获取进程列表
SQC_API void ProcessList(vector<string>& f_prolist);



//Native C 原生C++应用程序例子
//1.去掉crt编译  C/C++代码生成---运行库---/mt
//2.再在link中加上/nodefaultlib:msvcrt.lib
//3.链接器---高级---入口点   main
//4.C/C++代码生成---缓冲区安全检查---否
//#pragma comment (linker, "/nodefaultlib")
//#pragma comment (linker, "/nodefaultlib:msvcrt.lib")
//#pragma comment (linker, "/entry:main")


#include "stdafx.h"
#include <stdio.h>
#include "NaMath.h"
#include "NaFile.h"

#define IDB_BOX     110
#define IDB_REBOOT  111
#define IDB_SHUT    112
HWND hwndbtn1, hwndbtn2;

INT_PTR CALLBACK DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:      //点击X时执行
            //WM_DESTROY最后执行 if (uMsg == WM_SYSCOMMAND && wParam == SC_CLOSE)
            EndDialog(hwndDlg, 0);
            break;
        case WM_LBUTTONDOWN:
            SendMessage(hwndDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            break;
        case WM_INITDIALOG:
            CreateWindowA("Button", "帮助", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 10, 130, 60, hwndDlg, (HMENU)IDB_BOX, NULL, NULL);
            CreateWindowA("Button", "闪电重启", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 90, 130, 60, hwndDlg, (HMENU)IDB_REBOOT, NULL, NULL);
            CreateWindowA("Button", "闪电关机", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 170, 130, 60, hwndDlg, (HMENU)IDB_SHUT, NULL, NULL);
            break;
        //case WM_KEYDOWN:
        //case WM_KEYUP:
        //case WM_CHAR:
        //case WM_DEADCHAR:
        //case WM_SYSKEYDOWN:
        //case WM_SYSKEYUP:
        //case WM_SYSCHAR:
        //case WM_SYSDEADCHAR:
        //    //int KeyID = LOWORD(wParam);
        //    MessageBoxA(hwndDlg, "WM_KEYDOWN", "Hello Native C++!", MB_OK);
        //    switch (wParam)
        //    {
        //    case VK_UP:
        //        MessageBoxA(hwndDlg, "VK_UP", "Hello Native C++!", MB_OK);
        //        break;
        //    case VK_DOWN:
        //        MessageBoxA(hwndDlg, "VK_DOWN", "Hello Native C++!", MB_OK);
        //        break;
        //    case VK_LEFT:
        //        MessageBoxA(hwndDlg, "VK_LEFT", "Hello Native C++!", MB_OK);
        //        break;
        //    case VK_RIGHT: default:
        //        MessageBoxA(hwndDlg, "VK_RIGHT", "Hello Native C++!", MB_OK);
        //        break;
        //    }
        //    break;
        case WM_COMMAND:
            int ctrlID = LOWORD(wParam);
            switch (ctrlID)
            {
                case IDB_BOX:
                    MessageBoxA(hwndDlg, "Windows 原生关机功能函数调用。", "Hello Native C++!", MB_OK);
                    break;
                case IDB_REBOOT:
                    NaSysShut(1);               //NT重启
                    break;
                case IDB_SHUT:
                    NaSysShut(2);               //NT关机
                    break;
                default:
                    break;
            }
            break;
            //default:
            //    return DefWindowProc(hwndDlg, uMsg, wParam, lParam);
    }
    return FALSE;
}

//exe入口函数
extern "C" __declspec(dllexport) void NativeMain(int argc, char** argv)
{
#ifdef GUIRUN
    //程序运行一个实例
    static HANDLE SQ_variable_hMutex;
    //程序实例检查
    SQ_variable_hMutex = CreateMutexA(NULL, true, "SQNativeC");
    if ((GetLastError() == ERROR_ALREADY_EXISTS) || (GetLastError() == ERROR_INVALID_HANDLE))
        return ;   //ShowBox("该程序已在运行");
    ExitProcess(DialogBoxParamA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(IDD_DIALOG1), NULL, &DlgProc, 0));
#else
    //设置标题
    SetConsoleTitleA("NativeC++");      //const char szDlgTitle[] = "NativeC++";
    //打印字符
    NaCout("Native C++ program run !\n");
    DeleteDemo();
    for (int i = 0; i < 3; ++i)
        NaCout("for !\n");
    RecvMessage();
    char* str = new char[250];
    sprintf(str, "当前系统已运行了%d毫秒!", GetTickCount());
    MessageBoxA(NULL, str, str, MB_ICONINFORMATION);
    delete[] str;
#endif
}

//dll的入口函数
bool __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:        //当这个DLL被映射到了进程的地址空间时
        case DLL_THREAD_ATTACH:         //一个线程正在被创建
        case DLL_THREAD_DETACH:         //线程终结
        case DLL_PROCESS_DETACH:        //这个DLL从进程的地址空间中解除映射
            break;
    }
    return true;
}





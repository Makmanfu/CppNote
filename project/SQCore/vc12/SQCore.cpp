
#include "stdafx.h"
#include "BootLoad.h"
#include "WinTool.h"

//窗口界面程序
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    //BootLoad::WINMAIN(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    //BootLoad::WIN32MAIN(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return WinTool::Instance()->RunCheckOnly("DlgWin") ? BootLoad::WIN32MAIN(hInstance, hPrevInstance, lpCmdLine, nCmdShow) : 0;
}

//控制台程序
int main(int argc, char** argv)
{
#ifdef _WIN64
    SetConsoleTitleA("SQCore64");
#else
    SetConsoleTitleA("SQCore");
#endif
    system("color e0");
    return WinTool::Instance()->RunCheckOnly("DlgWin") ? BootLoad::CMDMAIN(argc, argv) : 0;
}

//dll的入口函数
bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:        //当这个DLL被映射到了进程的地址空间时
        case DLL_THREAD_ATTACH:         //一个线程正在被创建
        case DLL_PROCESS_DETACH:        //这个DLL从进程的地址空间中解除映射
            break;
        case DLL_THREAD_DETACH:         //线程终结
            ::DisableThreadLibraryCalls((HMODULE)hModule);
            break;
    }
    return true;
}













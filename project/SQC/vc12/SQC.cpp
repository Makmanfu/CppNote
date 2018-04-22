

#include "stdafx.h"

//exe的入口函数
int main(int argc, char** argv)
{
    return 0;
}

//窗口界面程序
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    return 0;
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
            break;
    }
    return true;
}





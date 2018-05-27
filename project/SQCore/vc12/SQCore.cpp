
#include "stdafx.h"
#include "BootLoad.h"
#include "WinTool.h"

//���ڽ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    //BootLoad::WINMAIN(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    //BootLoad::WIN32MAIN(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return WinTool::Instance()->RunCheckOnly("DlgWin") ? BootLoad::WIN32MAIN(hInstance, hPrevInstance, lpCmdLine, nCmdShow) : 0;
}

//����̨����
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

//dll����ں���
bool APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:        //�����DLL��ӳ�䵽�˽��̵ĵ�ַ�ռ�ʱ
        case DLL_THREAD_ATTACH:         //һ���߳����ڱ�����
        case DLL_PROCESS_DETACH:        //���DLL�ӽ��̵ĵ�ַ�ռ��н��ӳ��
            break;
        case DLL_THREAD_DETACH:         //�߳��ս�
            ::DisableThreadLibraryCalls((HMODULE)hModule);
            break;
    }
    return true;
}













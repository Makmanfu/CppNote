

#include "stdafx.h"

//exe����ں���
int main(int argc, char** argv)
{
    return 0;
}

//���ڽ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    return 0;
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
            break;
    }
    return true;
}





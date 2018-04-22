//Native C ԭ��C++Ӧ�ó�������
//1.ȥ��crt����  C/C++��������---���п�---/mt
//2.����link�м���/nodefaultlib:msvcrt.lib
//3.������---�߼�---��ڵ�   main
//4.C/C++��������---��������ȫ���---��
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
        case WM_CLOSE:      //���Xʱִ��
            //WM_DESTROY���ִ�� if (uMsg == WM_SYSCOMMAND && wParam == SC_CLOSE)
            EndDialog(hwndDlg, 0);
            break;
        case WM_LBUTTONDOWN:
            SendMessage(hwndDlg, WM_NCLBUTTONDOWN, HTCAPTION, 0);
            break;
        case WM_INITDIALOG:
            CreateWindowA("Button", "����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 10, 130, 60, hwndDlg, (HMENU)IDB_BOX, NULL, NULL);
            CreateWindowA("Button", "����", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 90, 130, 60, hwndDlg, (HMENU)IDB_REBOOT, NULL, NULL);
            CreateWindowA("Button", "�ػ�", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                10, 170, 130, 60, hwndDlg, (HMENU)IDB_SHUT, NULL, NULL);
            break;
        case WM_COMMAND:
            int ctrlID = LOWORD(wParam);
            switch (ctrlID)
            {
                case IDB_BOX:
                    MessageBoxA(hwndDlg, "Hello Native C++!", "C++", MB_OK);
                    break;
                case IDB_REBOOT:
                    NaSysShut(1);
                    break;
                case IDB_SHUT:
                    NaSysShut(2);            //NT�ػ�
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

//exe��ں���
extern "C" __declspec(dllexport) void NativeMain(int argc, char** argv)
{
#ifdef GUIRUN
    //��������һ��ʵ��
    static HANDLE SQ_variable_hMutex;
    //����ʵ�����
    SQ_variable_hMutex = CreateMutexA(NULL, true, "SQNativeC");
    if ((GetLastError() == ERROR_ALREADY_EXISTS) || (GetLastError() == ERROR_INVALID_HANDLE))
        return ;   //ShowBox("�ó�����������");
    ExitProcess(DialogBoxParamA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(IDD_DIALOG1), NULL, &DlgProc, 0));
#else
    //���ñ���
    SetConsoleTitleA("NativeC++");      //const char szDlgTitle[] = "NativeC++";
    //��ӡ�ַ�
    NaCout("Native C++ program run !\n");
    DeleteDemo();
    for (int i = 0; i < 3; ++i)
        NaCout("for !\n");
    RecvMessage();
    char* str = new char[250];
    sprintf(str, "��ǰϵͳ��������%d����!", GetTickCount());
    MessageBoxA(NULL, str, str, MB_ICONINFORMATION);
    delete[] str;
#endif
}

//dll����ں���
bool __stdcall DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:        //�����DLL��ӳ�䵽�˽��̵ĵ�ַ�ռ�ʱ
        case DLL_THREAD_ATTACH:         //һ���߳����ڱ�����
        case DLL_THREAD_DETACH:         //�߳��ս�
        case DLL_PROCESS_DETACH:        //���DLL�ӽ��̵ĵ�ַ�ռ��н��ӳ��
            break;
    }
    return true;
}





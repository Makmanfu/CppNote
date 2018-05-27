
#include "stdafx.h"
#include <windows.h>
#include <thread>
#include "CToolClass.hpp"
#include "tcp_sync_echo.hpp"
#include "tcp_async_echo.hpp"
#include "udp_sync_echo.hpp"
#include "tcp_sync_server.hpp"
#include "tcp_async_server.hpp"
//#include "NativeSocket.hpp"

static int boosthelp(void)
{
    cout << "��ͬ���ͻ���  --tcp_sync_echo_client \n"
         "��ͬ�������  --tcp_sync_echo_server \n"
         "���첽�ͻ���  --tcp_async_echo_client \n"
         "���첽�����  --tcp_async_echo_server \n"
         "UDP �ͻ���  --udp_sync_echo_client \n"
         "UDP �����  --udp_sync_echo_server \n"
         "TCP �첽�ͻ���  --async_client \n"
         "TCP �첽������  --async_server \n"
         "TCP ͬ���ͻ���  --sync_client \n"
         "TCP ͬ��������  --sync_server \n"
         "ԭ��Socket�ͻ��� --NSocketclient \n"
         "ԭ��Socket������ --NSocketserver " << endl;
    system("pause");
    return 1;
}

int mainboost(int argc, char** argv)
{
    SetConsoleTitleA("boost");
    system("color de");
    FUNProc fp = NULL;
    CmdProc<FUNProc> cmds(argv[0]);                             //����������
    cmds.DefaultFunAdd(boosthelp);
    cmds.ComandAdd(string("tcp_sync_echo_client"), tcp_sync_echo_client);     //ͬ���ͻ���
    cmds.ComandAdd(string("tcp_sync_echo_server"), tcp_sync_echo_server);     //ͬ�������
    cmds.ComandAdd(string("tcp_async_echo_client"), tcp_async_echo_client);   //�첽�ͻ���
    cmds.ComandAdd(string("tcp_async_echo_server"), tcp_async_echo_server);   //�첽�����
    cmds.ComandAdd(string("udp_sync_echo_client"), udp_sync_echo_client);     //udp�ͻ���
    cmds.ComandAdd(string("udp_sync_echo_server"), udp_sync_echo_server);     //udp�����
    cmds.ComandAdd(string("sync_client"), sync_client);
    cmds.ComandAdd(string("sync_server"), sync_server);
    cmds.ComandAdd(string("async_client"), async_client);
    cmds.ComandAdd(string("async_server"), async_server);
    //cmds.ComandAdd(string("NSocketclient"), NSocketclient);     //ԭ���ͻ���
    //cmds.ComandAdd(string("NSocketserver"), NSocketserver);     //ԭ�������
    string strcmd = argc > 1 ? argv[1] : "default";             //Ĭ��ָ��(argc == 1ʱ) help
    cmds.GetCmdFunProc(strcmd, &fp);
    if (fp != NULL)
        fp();
    system("pause");
    return 0;
}

extern "C" __declspec(dllexport) int OutMain(int argc, char** argv)
{
    return mainboost(argc, argv);
}


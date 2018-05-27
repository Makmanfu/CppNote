
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
    cout << "简单同步客户端  --tcp_sync_echo_client \n"
         "简单同步服务端  --tcp_sync_echo_server \n"
         "简单异步客户端  --tcp_async_echo_client \n"
         "简单异步服务端  --tcp_async_echo_server \n"
         "UDP 客户端  --udp_sync_echo_client \n"
         "UDP 服务端  --udp_sync_echo_server \n"
         "TCP 异步客户端  --async_client \n"
         "TCP 异步服务器  --async_server \n"
         "TCP 同步客户端  --sync_client \n"
         "TCP 同步服务器  --sync_server \n"
         "原生Socket客户器 --NSocketclient \n"
         "原生Socket服务器 --NSocketserver " << endl;
    system("pause");
    return 1;
}

int mainboost(int argc, char** argv)
{
    SetConsoleTitleA("boost");
    system("color de");
    FUNProc fp = NULL;
    CmdProc<FUNProc> cmds(argv[0]);                             //程序名传入
    cmds.DefaultFunAdd(boosthelp);
    cmds.ComandAdd(string("tcp_sync_echo_client"), tcp_sync_echo_client);     //同步客户端
    cmds.ComandAdd(string("tcp_sync_echo_server"), tcp_sync_echo_server);     //同步服务端
    cmds.ComandAdd(string("tcp_async_echo_client"), tcp_async_echo_client);   //异步客户端
    cmds.ComandAdd(string("tcp_async_echo_server"), tcp_async_echo_server);   //异步服务端
    cmds.ComandAdd(string("udp_sync_echo_client"), udp_sync_echo_client);     //udp客户端
    cmds.ComandAdd(string("udp_sync_echo_server"), udp_sync_echo_server);     //udp服务端
    cmds.ComandAdd(string("sync_client"), sync_client);
    cmds.ComandAdd(string("sync_server"), sync_server);
    cmds.ComandAdd(string("async_client"), async_client);
    cmds.ComandAdd(string("async_server"), async_server);
    //cmds.ComandAdd(string("NSocketclient"), NSocketclient);     //原生客户端
    //cmds.ComandAdd(string("NSocketserver"), NSocketserver);     //原生服务端
    string strcmd = argc > 1 ? argv[1] : "default";             //默认指令(argc == 1时) help
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


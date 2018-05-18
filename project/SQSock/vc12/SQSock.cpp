

#include "stdafx.h"
#include "CToolClass.hpp"
#include "OpenThread.h"
//cmd
#include "SocketNative.hpp"                 //����ͨ������
#include "WinSocket.h"
#include "ThreadConcurrency.h"              //C11����
#include "MyThread.h"                       //�����ܽ�
//win32
#include "WinSockDlg.h"                     //��ܽ���
#include "ThreadCookbook.h"                 //Cookbook����
#include "WSockFileDlg.h"


int main(int argc, char** argv)
{
    CMDProc fp = NULL, CmdMain = NULL, DefaultFun = &TCPSocket::TCPServer;
    CCMDParam cmds(argv[0]);               //����������
    cmds.DefaultFunAdd(DefaultFun);
    //��������
    cmds.ComandAdd("TCPServer", &SocketNative::TCPServer);
    cmds.ComandAdd("TCPClient", &SocketNative::TCPClient);
    cmds.ComandAdd("UDPServer", &SocketNative::UDPServer);
    cmds.ComandAdd("UDPClient", &SocketNative::UDPClient);
    //��TCPͨ�Ź���
    cmds.ComandAdd("TCPSVR", &TCPSocket::TCPServer);
    cmds.ComandAdd("TCPCEN", &TCPSocket::TCPClient);

    //cmds.ComandAdd("SimpleThread", &ThreadDemo::SimpleThread::main);
    //cmds.ComandAdd("CBankAccount", &ThreadPools::CBankAccount::main);
    //cmds.ComandAdd("CBankAccount2", &ThreadPools::CBankAccount::main2);
    //WINAPI�̼߳�¼
    //cmds.ComandAdd("ThreadTest1", &ThreadStudy::ThreadTest1::main);
    //cmds.ComandAdd("ThreadTest2", &ThreadStudy::ThreadTest2::main);
    //cmds.ComandAdd("ThreadTest3", &ThreadStudy::ThreadTest3::main);
    //cmds.ComandAdd("ThreadTest4", &ThreadStudy::ThreadTest4::main);
    //cmds.ComandAdd("ThreadTest5", &ThreadStudy::ThreadTest5::main);
    //cmds.ComandAdd("ThreadTest6", &ThreadStudy::ThreadTest6::main);
    //C11�̼߳�¼
    //cmds.ComandAdd("C11ThreadTest1", &C11ThreadDemo::C11ThreadTest1::main);
    //cmds.ComandAdd("C11ThreadTest2", &C11ThreadDemo::C11ThreadTest2::main);

    string strcmd = argc > 1 ? argv[1] : "default";    //default  help
    cmds.GetCmdFunProc(strcmd, &fp);
    if (fp != NULL)
        fp(argc,argv);
    cmds.Loop();
    //system("pause");
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    WINProc fp = NULL;
    CWINParam cmds;
    cmds.DefaultFunAdd(&WinSockDlg::WINMAIN);
    cmds.ComandAdd("WinSockDlg", &WinSockDlg::WINMAIN);
    cmds.ComandAdd("WSockFileDlg", &WSockFileDlg::WINMAIN);






    //���涼�ǲ���
    cmds.ComandAdd("IPCDemo", &IPCDemo::IPCGui::WINMAIN);
    cmds.ComandAdd("IPCDemoMuti", &IPCDemoMuti::IPCGui::WINMAIN);
    cmds.ComandAdd("ConcurrentFileCopy", &ThreadDemo::ConcurrentFileCopy::WINMAIN);
    cmds.ComandAdd("AsyncThreadGUI", &Threads::AsyncThreadGUI::WINMAIN);
    cmds.ComandAdd("SyncThreadGUI", &Threads::SyncThreadGUI::WINMAIN);
    cmds.ComandAdd("MessagesGUI", &Messages::MessagesGUI::WINMAIN);
    cmds.ComandAdd("concurrent_operations", &Concurrent::concurrent_operations::WINMAIN);
    cmds.ComandAdd("AsyncGUI", &ThreadStudy::AsyncGUI::WINMAIN);    //�첽
    cmds.ComandAdd("SyncGUI", &ThreadStudy::SyncGUI::WINMAIN);      //ͬ��

    cmds.GetCmdFunProc(string(lpCmdLine), &fp);
    if (fp != NULL)
        fp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}







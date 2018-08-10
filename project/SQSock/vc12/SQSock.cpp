

#include "stdafx.h"
#include "CToolClass.hpp"
#include "OpenThread.h"
//cmd
#include "SocketNative.hpp"                 //����ͨ������
#include "WinSocket.h"
#include "ThreadConcurrency.h"              //C11����
#include "MyThread.h"                       //�����ܽ�
#include "WinThread.h"                      //�߳�ѧϰ
//win32
#include "WinSockDlg.h"                     //��ܽ���
#include "ThreadCookbook.h"                 //Cookbook����
#include "WSockFileDlg.h"
//ZMQ
#include "styzmq.h"


int main(int argc, char** argv)
{
    string strcmd = argc > 1 ? argv[1] : "default";
    CCMDParam cmds(argv[0], &StyZMQ::zmq_hwclient);
    cmds.NativeARGC(argc, argv);
    //ZMQ
    cmds.ComandAdd("zmq_hwserverc", &StyZMQ::zmq_hwserverc);
    cmds.ComandAdd("zmq_hwservercpp", &StyZMQ::zmq_hwservercpp);
    cmds.ComandAdd("zmq_hwclient", &StyZMQ::zmq_hwclient);
    cmds.ComandAdd("zmq_wuserverPub", &StyZMQ::zmq_wuserverPub);
    cmds.ComandAdd("zmq_wuclientSub", &StyZMQ::zmq_wuclientSub);

    //�����߳�ѧϰ
    cmds.ComandAdd("GlobalThread", &GlobalThread::main);        //�޲�������ȫ���߳�
    //����ԭ��ͨ�Ż�������
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

    CMDProc fp = NULL;
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







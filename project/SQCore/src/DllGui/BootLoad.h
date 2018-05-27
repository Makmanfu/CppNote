//*************************************************************************
//
//                      程序启动器界面
//                                          @2017-03-10阿甘整理
//
//*************************************************************************
#ifndef BOOTLOAD_H
#define BOOTLOAD_H

#include <windows.h>
#include "WinGUI.h"                 //界面
#include "CToolClass.hpp"           //启动器模板
#include "CTemplate.hpp"            //工具模板

//运行模式
enum ModeRunWork { EnumWIN = 0, EnumCMD };

//程序启动器
class BootLoad : public XqWindowDlg, public noncopyable
{
public:
    //windows界面入口
    static int WIN32MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    //windows控制台入口
    static int CMDMAIN(int argc, char** argv);
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    BootLoad();
public:
    //将上面WIN32MAIN转到下面控制台
    static int CWinMain(int argc, char** argv);
    //阻塞式消息循环
    static int MSGLOOP(MSG& msg);
    //添加命令FUN指针
    static void FunAddCMDAll(void);
    static void FunAddWINAll(void);
public:
    static CWINParam* mWIN;         //win函数指针
    static CCMDParam* mCMD;         //cmd函数指针
    static WINProc WINfp;
    static CMDProc CMDfp;
public:
    //存储传进来的参数
    static int cmdargc;
    static char** cmdargv;
    static HINSTANCE guihInst;
    static HINSTANCE guihPrevInst;
    static char* guilpCmd;
    static int guinCmd;
    ModeRunWork modenum;        //当前运行模式
private:
    HWND hWCBoxMode, hWCBoxLINK, hWLISTCMD, hWLISTEXE;
public:
    //初始化
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //关闭
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //消息事件
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //处理模式下拉框
    void CBoxModeSelect(ModeRunWork id);
    //显示程序列表
    void ListViewShow(vector<string>& lst);

    void ListCurrGetText(char* exename);
};















#endif
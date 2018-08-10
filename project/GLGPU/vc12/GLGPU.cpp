
#include "stdafx.h"
#include <stdio.h>
#include "GLWindow.h"
#include "CToolClass.hpp"
#include "Harkscr.hpp"

//freeglut
extern int GL_freeglut_demo(int argc, char** argv);
//四元数旋转
extern int TrackBall_demo(int argc, char** argv);
extern int TrackBall_Test(int argc, char** argv);
extern int TrackBallExample(int argc, char** argv);

int main(int argc, char** argv)
{
    string strcmd = argc > 1 ? argv[1] : "default";
    CmdProc<CMDProc> cmds(argv[0], &TrackBall_Test);
    cmds.NativeARGC(argc, argv);
    cmds.ComandAdd(string("GLfreeglutdemo"), GL_freeglut_demo);      //freeglut
    cmds.ComandAdd(string("TrackBall_Test"), TrackBall_Test);        //四元数旋转
    cmds.ComandAdd(string("TrackBallExample"), TrackBallExample);    //四元数旋转
    CMDProc fp = NULL;
    cmds.GetCmdFunProc(strcmd, &fp);
    if (fp != NULL)
        fp(argc, argv);
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow) /*LPTSTR*/
{
    WINProc fp = NULL;
    CmdProc<WINProc> cmds;
    cmds.DefaultFunAdd(&TrackBallWnd::WINMAIN);
    cmds.ComandAdd(string("GLWnd"), &GLWnd::WINMAIN);
    cmds.ComandAdd(string("WndTrackBall"), &TrackBallWnd::WINMAIN);
    cmds.ComandAdd(string("WinHarkscr"), &WinHarkscr::WINMAIN);
    cmds.GetCmdFunProc(string(lpCmdLine), &fp);
    if (fp != NULL)
        fp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}

C_API int dmain(int argc, char** argv)
{
    return main(argc, argv);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}





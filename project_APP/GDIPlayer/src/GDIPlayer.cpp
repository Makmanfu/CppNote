
#include "stdafx.h"
#include <stdio.h>
#include "CToolClass.hpp"
#include "WinPlayer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
{
    WINProc fp = NULL;
    CmdProc<WINProc> cmds;
    cmds.DefaultFunAdd(&WinPlayer::WINMAIN);
    cmds.ComandAdd(string("WinPlayer"), &WinPlayer::WINMAIN);
    cmds.GetCmdFunProc(string(lpCmdLine), &fp);
    if (fp != NULL)
        fp(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
    return 0;
}








#include "stdafx.h"
#include "Parser\PrintLog.h"
#include "WinKey.h"

int main(int argc, char** argv)
{
    Logger logtest("log");      //"c:\\log"
    for (size_t i = 1; i < 1001; i++)
    {
        logtest.TraceError("Error--------");
        logtest.TraceInfo("show-----");
        logtest.TraceKeyInfo("guanjian");
        logtest.TraceWarning("warning--------");
        //Sleep(100);
    }

    MSG msg;
    HACCEL hAccelTable = NULL;
    printf("message loop!\n");
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }


    return 0;
}















#include "stdafx.h"
#include "Parser\PrintLog.h"

int main(int argc, char** argv)
{
    Logger logtest("c:\\log");
    for (size_t i = 1; i < 1001; i++)
    {
        logtest.TraceError("Error--------");
        logtest.TraceInfo("show-----");
        logtest.TraceKeyInfo("guanjian");
        logtest.TraceWarning("warning--------");
        //Sleep(100);
    }
    return 0;
}












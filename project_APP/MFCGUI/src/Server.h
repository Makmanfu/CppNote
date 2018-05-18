#pragma once

#include "resource.h"       // main symbols

class CServerApp : public CWinApp
{
public:
    CServerApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

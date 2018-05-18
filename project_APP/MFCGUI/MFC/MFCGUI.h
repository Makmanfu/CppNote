

#ifndef MFCGUI_H
#define MFCGUI_H

class CMFCGUIApp : public CWinApp
{
public:
    CMFCGUIApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
};

#ifndef _LIB
    extern CMFCGUIApp theApp;
#endif



#endif
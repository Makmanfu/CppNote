
#ifndef MFCGUI_H
#define MFCGUI_H

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

class CMFCGUIApp : public CWinApp
{
public:
    CMFCGUIApp();

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CMFCGUIApp)
public:
    virtual BOOL InitInstance();
    //}}AFX_VIRTUAL

    // Implementation

    //{{AFX_MSG(CMFCGUIApp)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


#endif



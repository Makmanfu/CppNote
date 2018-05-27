

#include "stdafx.h"
#include "MFCGUI.h"
#include "MFCGUIDlg.h"


BEGIN_MESSAGE_MAP(CMFCGUIApp, CWinApp)
    //{{AFX_MSG_MAP(CMFCGUIApp)
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CMFCGUIApp theApp;

CMFCGUIApp::CMFCGUIApp()
{
}


BOOL CMFCGUIApp::InitInstance()
{
    // Standard initialization
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
    CMFCGUIDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
    }
    else if (nResponse == IDCANCEL)
    {
    }
    return FALSE;
}

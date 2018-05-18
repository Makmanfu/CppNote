

#include "stdafx.h"
#include "GuiMan.h"
#include "MFCGUIDlg.h"

GuiMan::GuiMan()
{
    //initliaze();
}

GuiMan::~GuiMan()
{
}

void GuiMan::initliaze()
{
    // 切记，此处需要加下面这行代码，不然对话框显示不出来。
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CMFCGUIDlg dlg;
    INT_PTR nResponse = dlg.DoModal();
    //if (nResponse == IDOK)
    //{
    //    // TODO:  在此放置处理何时用
    //    //  “确定”来关闭对话框的代码
    //}
    //else if (nResponse == IDCANCEL)
    //{
    //    // TODO:  在此放置处理何时用
    //    //  “取消”来关闭对话框的代码
    //}
    //else if (nResponse == -1)
    //{
    //    TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
    //    TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
    //}
}

void GUIMFC(void)
{
    // 切记，此处需要加下面这行代码，不然对话框显示不出来。
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CMFCGUIDlg dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO:  在此放置处理何时用
        //  “确定”来关闭对话框的代码
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO:  在此放置处理何时用
        //  “取消”来关闭对话框的代码
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
        TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
    }
}



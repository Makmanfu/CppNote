

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
    // �мǣ��˴���Ҫ���������д��룬��Ȼ�Ի�����ʾ��������
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CMFCGUIDlg dlg;
    INT_PTR nResponse = dlg.DoModal();
    //if (nResponse == IDOK)
    //{
    //    // TODO:  �ڴ˷��ô����ʱ��
    //    //  ��ȷ�������رնԻ���Ĵ���
    //}
    //else if (nResponse == IDCANCEL)
    //{
    //    // TODO:  �ڴ˷��ô����ʱ��
    //    //  ��ȡ�������رնԻ���Ĵ���
    //}
    //else if (nResponse == -1)
    //{
    //    TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
    //    TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
    //}
}

void GUIMFC(void)
{
    // �мǣ��˴���Ҫ���������д��룬��Ȼ�Ի�����ʾ��������
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CMFCGUIDlg dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        // TODO:  �ڴ˷��ô����ʱ��
        //  ��ȷ�������رնԻ���Ĵ���
    }
    else if (nResponse == IDCANCEL)
    {
        // TODO:  �ڴ˷��ô����ʱ��
        //  ��ȡ�������رնԻ���Ĵ���
    }
    else if (nResponse == -1)
    {
        TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
        TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
    }
}



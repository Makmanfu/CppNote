//*************************************************************************
//
//              WINDOWS��Ϣcopydata ����ͨ�ŷ����ܽ�
//                                              @2017-02-13����
//
//*************************************************************************

#ifndef CopyDATA_h
#define CopyDATA_h

//�������ݰ�
typedef struct tagCopyDATA
{
    int cmdid;                   //����
    char data[256];              //����
    //ds.data[sizeof(ds.data) - 1] = '\0';
    tagCopyDATA()
    {
        cmdid = 0;
        memset(data, 0, sizeof(data));
    };
} MsgCDATA;

//copydataͨ��ģʽ
class CopyDATA
{
public:
    //��������
    int SendRecv(const char* szDlgTitle, MsgCDATA& ds);
    //�ŵ�WINGUI�е�WM_COPYDATA����
    int RecvCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, MsgCDATA& ds);

private:
    //��ȡ������
    inline int GetHWND(const char* szDlgTitle, HWND& hWindow);
    //��ȡcmd���
    inline int GetHWND(HWND& hWindow);
private:
    COPYDATASTRUCT CopyData;
    HWND hSendWindow, hRecvWindow;         //��������,���ܷ����
};



#endif



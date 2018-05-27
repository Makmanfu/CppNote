//*************************************************************************
//
//              WINDOWS消息copydata 数据通信方法总结
//                                              @2017-02-13阿甘
//
//*************************************************************************

#ifndef CopyDATA_h
#define CopyDATA_h

//发送数据包
typedef struct tagCopyDATA
{
    int cmdid;                   //命令
    char data[256];              //数据
    //ds.data[sizeof(ds.data) - 1] = '\0';
    tagCopyDATA()
    {
        cmdid = 0;
        memset(data, 0, sizeof(data));
    };
} MsgCDATA;

//copydata通信模式
class CopyDATA
{
public:
    //发送数据
    int SendRecv(const char* szDlgTitle, MsgCDATA& ds);
    //放到WINGUI中的WM_COPYDATA里面
    int RecvCOPYDATA(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, MsgCDATA& ds);

private:
    //获取程序句柄
    inline int GetHWND(const char* szDlgTitle, HWND& hWindow);
    //获取cmd句柄
    inline int GetHWND(HWND& hWindow);
private:
    COPYDATASTRUCT CopyData;
    HWND hSendWindow, hRecvWindow;         //本程序句柄,接受方句柄
};



#endif



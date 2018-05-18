

#ifndef CTRAY_H
#define CTRAY_H

//托盘控件类
class CTray
{
#define WM_SHOWTASK WM_USER+1
    //注册托盘消息
    //static UINT const WM_TASKBAR_CREATED;
#define WM_TASKBAR_CREATED RegisterWindowMessageA("TaskbarCreated")
    //if ( uMsg == WM_TASKBAR_CREATED )//系统Explorer崩溃重启时，重新加载托盘
    //    MP.myTrayClass.ShowTray ( true );
    //break;
public:
    CTray();
    CTray(HWND hWnd, size_t IconID, HMENU hMenu);
    virtual ~CTray();
private:
    NOTIFYICONDATAA nid;        //最小化到托盘用到的记录图标信息
    HWND hDlgWnd;               //窗口句柄
    size_t TaskIconID;          //托盘ID
    HMENU hTrayMenu;            //菜单
public:
    //托盘初始化
    void SetMenu(HMENU hMenu);
    //设置图标
    void SetIcon(size_t IconID);

    //显示托盘
    void ShowTray(bool _state);
    //将窗口显示出来
    void ShowTask(HWND hWnd, WPARAM IconID, LPARAM iMouseMsg);
};


#endif


#ifndef GDIPLAYER_H
#define GDIPLAYER_H
#include "stdafx.h"
#include <windows.h>
#include "WinGUI.h"
#include "Music.h"
#include "CTray.h"

#define WM_INVALIDATE WM_USER + 1

//WinAPI播放器
class GDIPlayer :public XqWindowDlg
{
public:
    //测试 主程序入口
    static int WIN_MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    GDIPlayer(int tResDlgID = 0);     //IDD_DLLDLG
    virtual ~GDIPlayer();
private:
    //绘制类型
    enum DRAWTYPE { DGPI, DGPIBMP};

    HWND hProgress, hSound, hWndList, hWndShow;     //控件
    HMENU hMainMenu, hPopMenu, hTrayMenu;           //菜单
    Music m_mp3;                                    //播放器框架
    CTray* myTrayClass;                             //托盘
    int iVolume, iExitFlag, iMenuFlag;              //音量大小,托盘,主菜单
    DRAWTYPE eDrawMode;                             //默认绘制
    RECT rect;                                      //GPI绘制区域使用
private:
    //将长整型的时间转换成00:00:00形式
    void ConvLTime2Str(LONG LTime, char* StrTime);
    //函数作用：得到a,b之间的随机数
    int GetRandNum(int a, int b);
    //添加目录时用到、选出指定路径的音乐文件
    BOOL SearchFileAddtoList(HWND hWnd, char path[], char format[]);
    //获取路径
    void GetExeP(char* exePath, int iLen);
    //初始化OPENFILENAME结构体
    void InitOFN(HWND hWnd, OPENFILENAMEA* Pofn, char OpenSaveFileName[]);
private:
    void initGui(void);
    //初始化音量调节的滚动条
    void Initsound(void);
    //从配置文件读取信息，并把歌曲添加到列表
    void ReadSettingInfo(HWND hWnd);
    //欢迎声音
    void InitWelcomSound(void);
    //获取菜单
    void GetMyMenu(HWND hWnd);
protected:
    //将配置信息写入文件
    void WriteSettingInfo(HWND hWnd);
    //添加目录(非递归)
    BOOL AddDirMusic(HWND hWnd);
    //添加文件(支持多选)
    BOOL AddMusicFileToList(HWND hWnd);
    //将曲目移出列表
    void RemoveSelectedMusic(HWND hWnd);
    //清空播放列表
    void ClearMusicList(HWND hWnd);
    //播放列表中索引为index的音乐
    void PlayMusicInList(HWND hWnd, int index);
public:
    //GPI绘制播放器logo
    void GDIPaint(HWND DrawHWnd);
    //GDI贴图BMP
    void GDIBMPPaint(HINSTANCE hInstance, HWND DrawHWnd);
public:
    virtual void WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMSHOWTASK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};







#endif


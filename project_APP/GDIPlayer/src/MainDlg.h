
#ifndef MAINDLG_H
#define MAINDLG_H

#include <windows.h>
#include <shellapi.h>   // 拖拽文件时用到的HDROP定义在此文件(要写在windowsx.h之前，HANDLE_WM_DROPFILES才能被识别)
#include <windowsx.h>
#include "Music.h"
#include "CTray.h"
#include "CSingleton.hpp"
#include "OpenGLView.h"

#define WM_INVALIDATE           WM_USER + 1

//播放器
class MusicFrm : public noncopyable
{
#ifdef _WIN64
    static void CALLBACK AdjustProgessPos(HWND hWnd, UINT uMsg, UINT_PTR iTimeID, DWORD dwTime);
#else
    static void CALLBACK AdjustProgessPos(HWND hWnd, UINT uMsg, UINT iTimeID, DWORD dwTime);
#endif
public:
    //绘制类型
    enum DRAWTYPE {DGPI, DGPIBMP, DGL, DDX9, DNULL };

    HINSTANCE hInstance;                //本程序实例
    //主窗口,声音控件,list,show
    HWND hWndDlg, hProgress, hSound, hWndList, hWndShow;
    Song CurPlayingSong;                //当前播放的歌曲
    HMENU hMainMenu, hPopMenu, hTrayMenu;
    RECT rect;                          //GPI绘制区域使用
    PLAYMODE iPlayMode;                 //播放模式,可取Music的宏定义
    DRAWTYPE eDrawMode;                 //默认绘制
    //当前播放在列表中的序号
    int PlayingIndex;
    //最小化到托盘的类的对象
    //CTray myTrayClass;
    //音量大小,托盘,主菜单
    int iVolume, iExitFlag, iMenuFlag;
protected:
    MusicFrm();
    virtual ~MusicFrm();
protected:
    //获得Instance和其他句柄
    void GethInstance(HWND hWnd);
    //获取菜单
    void GetMyMenu(HWND hWnd);
    //初始化音量调节的滚动条
    void Initsound(void);
    //设置标题和图标
    void InitTitIcon(HWND hWnd);
    //欢迎声音
    void InitWelcomSound(void);

    //从配置文件读取信息，并把歌曲添加到列表
    void ReadSettingInfo(HWND hWnd);
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
};

//管理消息回调
class MainMsg : public MusicFrm
{
    DEFINE_SINGLETON(MainMsg);
public:
    OpenGLView m_OpenGL;
public:
    //其他消息
    void WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //初始化
    void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //关闭
    void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //重绘制
    void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //事件
    void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //listbox右键点击
    void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //wParam接收的是图标的ID，而lParam接收的是鼠标的行为
    void WMSHOWTASK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //水平滚动条
    void WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //窗口改变时
    void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //拖放文件
    void WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //GPI绘制播放器logo
    void GDIPaint(HWND DrawHWnd);
    //GDI贴图BMP
    void GDIBMPPaint(HWND DrawHWnd);
    //opengl
    void OpenGlPaint(HWND hDrawWnd);
public:
    //------------------测试代码-------------------------

    //添加listbox测试数据
    void addlist(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

extern "C" MainMsg* GetMP3();
















#endif
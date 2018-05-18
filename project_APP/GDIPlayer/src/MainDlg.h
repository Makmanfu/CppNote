
#ifndef MAINDLG_H
#define MAINDLG_H

#include <windows.h>
#include <shellapi.h>   // ��ק�ļ�ʱ�õ���HDROP�����ڴ��ļ�(Ҫд��windowsx.h֮ǰ��HANDLE_WM_DROPFILES���ܱ�ʶ��)
#include <windowsx.h>
#include "Music.h"
#include "CTray.h"
#include "CSingleton.hpp"
#include "OpenGLView.h"

#define WM_INVALIDATE           WM_USER + 1

//������
class MusicFrm : public noncopyable
{
#ifdef _WIN64
    static void CALLBACK AdjustProgessPos(HWND hWnd, UINT uMsg, UINT_PTR iTimeID, DWORD dwTime);
#else
    static void CALLBACK AdjustProgessPos(HWND hWnd, UINT uMsg, UINT iTimeID, DWORD dwTime);
#endif
public:
    //��������
    enum DRAWTYPE {DGPI, DGPIBMP, DGL, DDX9, DNULL };

    HINSTANCE hInstance;                //������ʵ��
    //������,�����ؼ�,list,show
    HWND hWndDlg, hProgress, hSound, hWndList, hWndShow;
    Song CurPlayingSong;                //��ǰ���ŵĸ���
    HMENU hMainMenu, hPopMenu, hTrayMenu;
    RECT rect;                          //GPI��������ʹ��
    PLAYMODE iPlayMode;                 //����ģʽ,��ȡMusic�ĺ궨��
    DRAWTYPE eDrawMode;                 //Ĭ�ϻ���
    //��ǰ�������б��е����
    int PlayingIndex;
    //��С�������̵���Ķ���
    //CTray myTrayClass;
    //������С,����,���˵�
    int iVolume, iExitFlag, iMenuFlag;
protected:
    MusicFrm();
    virtual ~MusicFrm();
protected:
    //���Instance���������
    void GethInstance(HWND hWnd);
    //��ȡ�˵�
    void GetMyMenu(HWND hWnd);
    //��ʼ���������ڵĹ�����
    void Initsound(void);
    //���ñ����ͼ��
    void InitTitIcon(HWND hWnd);
    //��ӭ����
    void InitWelcomSound(void);

    //�������ļ���ȡ��Ϣ�����Ѹ�����ӵ��б�
    void ReadSettingInfo(HWND hWnd);
    //��������Ϣд���ļ�
    void WriteSettingInfo(HWND hWnd);
    //���Ŀ¼(�ǵݹ�)
    BOOL AddDirMusic(HWND hWnd);
    //����ļ�(֧�ֶ�ѡ)
    BOOL AddMusicFileToList(HWND hWnd);
    //����Ŀ�Ƴ��б�
    void RemoveSelectedMusic(HWND hWnd);
    //��ղ����б�
    void ClearMusicList(HWND hWnd);
    //�����б�������Ϊindex������
    void PlayMusicInList(HWND hWnd, int index);

private:
    //�������͵�ʱ��ת����00:00:00��ʽ
    void ConvLTime2Str(LONG LTime, char* StrTime);
    //�������ã��õ�a,b֮��������
    int GetRandNum(int a, int b);
    //���Ŀ¼ʱ�õ���ѡ��ָ��·���������ļ�
    BOOL SearchFileAddtoList(HWND hWnd, char path[], char format[]);
    //��ȡ·��
    void GetExeP(char* exePath, int iLen);
    //��ʼ��OPENFILENAME�ṹ��
    void InitOFN(HWND hWnd, OPENFILENAMEA* Pofn, char OpenSaveFileName[]);
};

//������Ϣ�ص�
class MainMsg : public MusicFrm
{
    DEFINE_SINGLETON(MainMsg);
public:
    OpenGLView m_OpenGL;
public:
    //������Ϣ
    void WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��ʼ��
    void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ر�
    void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ػ���
    void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�¼�
    void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //listbox�Ҽ����
    void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //wParam���յ���ͼ���ID����lParam���յ���������Ϊ
    void WMSHOWTASK(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //ˮƽ������
    void WMHSCROLL(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //���ڸı�ʱ
    void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�Ϸ��ļ�
    void WMDROPFILES(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //GPI���Ʋ�����logo
    void GDIPaint(HWND DrawHWnd);
    //GDI��ͼBMP
    void GDIBMPPaint(HWND DrawHWnd);
    //opengl
    void OpenGlPaint(HWND hDrawWnd);
public:
    //------------------���Դ���-------------------------

    //���listbox��������
    void addlist(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

extern "C" MainMsg* GetMP3();
















#endif

#ifndef GDIPLAYER_H
#define GDIPLAYER_H
#include "stdafx.h"
#include <windows.h>
#include "WinGUI.h"
#include "Music.h"
#include "CTray.h"

#define WM_INVALIDATE WM_USER + 1

//WinAPI������
class GDIPlayer :public XqWindowDlg
{
public:
    //���� ���������
    static int WIN_MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    GDIPlayer(int tResDlgID = 0);     //IDD_DLLDLG
    virtual ~GDIPlayer();
private:
    //��������
    enum DRAWTYPE { DGPI, DGPIBMP};

    HWND hProgress, hSound, hWndList, hWndShow;     //�ؼ�
    HMENU hMainMenu, hPopMenu, hTrayMenu;           //�˵�
    Music m_mp3;                                    //���������
    CTray* myTrayClass;                             //����
    int iVolume, iExitFlag, iMenuFlag;              //������С,����,���˵�
    DRAWTYPE eDrawMode;                             //Ĭ�ϻ���
    RECT rect;                                      //GPI��������ʹ��
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
private:
    void initGui(void);
    //��ʼ���������ڵĹ�����
    void Initsound(void);
    //�������ļ���ȡ��Ϣ�����Ѹ�����ӵ��б�
    void ReadSettingInfo(HWND hWnd);
    //��ӭ����
    void InitWelcomSound(void);
    //��ȡ�˵�
    void GetMyMenu(HWND hWnd);
protected:
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
public:
    //GPI���Ʋ�����logo
    void GDIPaint(HWND DrawHWnd);
    //GDI��ͼBMP
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


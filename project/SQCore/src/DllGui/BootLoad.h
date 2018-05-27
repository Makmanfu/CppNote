//*************************************************************************
//
//                      ��������������
//                                          @2017-03-10��������
//
//*************************************************************************
#ifndef BOOTLOAD_H
#define BOOTLOAD_H

#include <windows.h>
#include "WinGUI.h"                 //����
#include "CToolClass.hpp"           //������ģ��
#include "CTemplate.hpp"            //����ģ��

//����ģʽ
enum ModeRunWork { EnumWIN = 0, EnumCMD };

//����������
class BootLoad : public XqWindowDlg, public noncopyable
{
public:
    //windows�������
    static int WIN32MAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
    //windows����̨���
    static int CMDMAIN(int argc, char** argv);
public:
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    BootLoad();
public:
    //������WIN32MAINת���������̨
    static int CWinMain(int argc, char** argv);
    //����ʽ��Ϣѭ��
    static int MSGLOOP(MSG& msg);
    //�������FUNָ��
    static void FunAddCMDAll(void);
    static void FunAddWINAll(void);
public:
    static CWINParam* mWIN;         //win����ָ��
    static CCMDParam* mCMD;         //cmd����ָ��
    static WINProc WINfp;
    static CMDProc CMDfp;
public:
    //�洢�������Ĳ���
    static int cmdargc;
    static char** cmdargv;
    static HINSTANCE guihInst;
    static HINSTANCE guihPrevInst;
    static char* guilpCmd;
    static int guinCmd;
    ModeRunWork modenum;        //��ǰ����ģʽ
private:
    HWND hWCBoxMode, hWCBoxLINK, hWLISTCMD, hWLISTEXE;
public:
    //��ʼ��
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ر�
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��Ϣ�¼�
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //����ģʽ������
    void CBoxModeSelect(ModeRunWork id);
    //��ʾ�����б�
    void ListViewShow(vector<string>& lst);

    void ListCurrGetText(char* exename);
};















#endif
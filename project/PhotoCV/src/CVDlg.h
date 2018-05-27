
#ifndef WINSOCKDLG_H
#define WINSOCKDLG_H

#include "../../../../CppNote/project/GLGPU/src/WinGUI.h"
#include "../../../../CppNote/project/GLGPU/src/GDIView.h"

class CVDlg : public XqWindowDlg
{
public:
    //���������
    static int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    CVDlg();
    virtual ~CVDlg();
public:
    //��ʼ��
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //��Ϣ�¼�
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ػ���
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ر�
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //�򿪶Ի���
    void OpenDlg(HWND hWnd);
private:
    string pinname1;

    HWND hWPICIN, hWPICOUT, hWCBOXCMD;
    cv::Mat mPicL, mPicR;
    GDIView mPicView1, mPicView2;
};




#endif

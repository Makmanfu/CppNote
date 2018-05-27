
#ifndef WINSOCKDLG_H
#define WINSOCKDLG_H

#include "../../../../CppNote/project/GLGPU/src/WinGUI.h"
#include "../../../../CppNote/project/GLGPU/src/GDIView.h"

class CVDlg : public XqWindowDlg
{
public:
    //主程序入口
    static int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow);
public:
    CVDlg();
    virtual ~CVDlg();
public:
    //初始化
    virtual void WMINITDIALOG(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //消息事件
    virtual void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //重绘制
    virtual void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //关闭
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    //打开对话框
    void OpenDlg(HWND hWnd);
private:
    string pinname1;

    HWND hWPICIN, hWPICOUT, hWCBOXCMD;
    cv::Mat mPicL, mPicR;
    GDIView mPicView1, mPicView2;
};




#endif

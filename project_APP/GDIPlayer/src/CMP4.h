//-----------------------------------------------------------------------------
//
//          ��װproc����
//
//-----------------------------------------------------------------------------

#ifndef CMP4_H
#define CMP4_H

#include "CTemplate.hpp"
#include "CSingleton.hpp"


//������Ϣ�ص�
class CMP4Msg: public noncopyable
{
    DEFINE_SINGLETON(CMP4Msg);
#define MAX_LOADSTRING 100
public:
    HINSTANCE hInst;                            //��ǰʵ��
    TCHAR szTitle[MAX_LOADSTRING];              //�������ı�
    TCHAR szWindowClass[MAX_LOADSTRING];        //����������
    RECT rect;                                  //��������С
public:
    CMP4Msg();
    ~CMP4Msg();
private:
    void DrawGDITest(HWND hWnd);
    //GPI���Ʋ�����logo
    void GDIPaint(HWND hWnd);
    //GDI��ͼBMP
    void GDIBMPPaint(HWND hWnd);
public:
    void InitData(HINSTANCE hInstance);
    //������Ϣ
    void WMOTHER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //���ڴ���ʱ�����Ϣ.
    void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�����Ϣ
    void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�Ҽ��¼�
    void WMCONTEXTMENU(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ı�ߴ�ʱ
    void WMSIZE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�ػ���
    void WMPAINT(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //�����¼�
    void WMCOMMAND(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    //���ڸı�ʱ
    void WMSIZING(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//����ָ��
extern "C" CMP4Msg* GetMP4();




#endif
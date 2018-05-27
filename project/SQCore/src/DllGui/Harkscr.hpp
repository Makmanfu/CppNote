//*************************************************************************
//
//                      �ڿ͵۹��ַ�������
//                                             @2017-09-03��������
//
//*************************************************************************
#ifndef HARKSCR_HPP
#define HARKSCR_HPP
#include "WinGUI.h"

#define ID_TIMER 1
#define STRMAXLEN 25    //һ����ʾ�е���󳤶�
#define STRMINLEN 8     //һ����ʾ�е���С����

//����������Ļ��һ����ʾ��,���Ǹ�˫���б�
typedef struct tagCharChain
{
    struct tagCharChain* prev;      //�����ǰ��Ԫ��
    TCHAR ch;                       //һ����ʾ���е�һ���ַ�
    struct tagCharChain* next;      //����ĺ��Ԫ��
} CharChain, *pCharChain;

typedef struct tagCharColumn
{
    CharChain* head, *current, *point;
    int x, y, iStrLen;              //��ʾ�еĿ�ʼ��ʾ��x,y����,iStrLen������еĳ���
    int iStopTimes, iMustStopTimes; //�Ѿ�ͣ�͵Ĵ����ͱ���ͣ�͵Ĵ���,����ͣ�͵Ĵ����������
} CharColumn, *pCharColumn;

//�ڿ͵۹�����
class WinHarkscr : public XqWindowEx
{
public:
    //�򵥷�ģ̬�Ի������ ���������
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        WinHarkscr WND(hInstance);
        WND.Create();
        return WND.ShowDLG();     //��Ϣѭ��
    }
private:
    HDC hdc;
    //ctn ����ȷ��һ����ʾ���Ƿ� ����ǰ��,����ȴ�������������ȴ��Ĵ���,ctn�ʹ���Ҫ����ǰ��
    int i, j, temp, ctn; //jΪһ����ʾ���г�����ͷ�������Ļ����ʾ��y����,temp��ɫ���ȵ���ɫ֮��
    HDC hdcMem;
    HFONT hFont;
    HBITMAP hBitmap;
    int cxScreen, cyScreen;                     //��Ļ�Ŀ�� �߶�.
    int iFontWidth, iFontHeight, iColumnCount;  //����Ŀ�� �߶�, ����
    CharColumn* ccChain;
public:
    WinHarkscr(HINSTANCE hInst) : XqWindowEx(hInst)
        , iFontWidth(10 + 5)
        , iFontHeight(15 + 5)
        , iColumnCount(0)
    {};
    ~WinHarkscr() {};
public:
    virtual void WMCREATE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        cxScreen = GetSystemMetrics(SM_CXSCREEN); //��Ļ���
        cyScreen = GetSystemMetrics(SM_CYSCREEN);
        SetTimer(hWnd, ID_TIMER, 1, NULL);
        hdc = GetDC(hWnd);
        hdcMem = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, cxScreen, cyScreen);
        SelectObject(hdcMem, hBitmap);
        ReleaseDC(hWnd, hdc);
        //��������
        hFont = CreateFont(iFontHeight, iFontWidth - 5, 0, 0, FW_BOLD, 0, 0, 0,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DRAFT_QUALITY, FIXED_PITCH | FF_SWISS, TEXT("Consolas"));    //Fixedsys
        SelectObject(hdcMem, hFont);
        DeleteObject(hFont);
        SetBkMode(hdcMem, TRANSPARENT); //���ñ���ģʽΪ ͸��
        iColumnCount = cxScreen / (iFontWidth * 3 / 2); //��Ļ����ʾ��ĸ�������
        ccChain = (pCharColumn)calloc(iColumnCount, sizeof(CharColumn));
        for (i = 0; i < iColumnCount; i++)
            init(ccChain + i, cyScreen, (iFontWidth * 3 / 2)*i);
    };
    virtual void WMLBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        KillTimer(hWnd, ID_TIMER);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        for (i = 0; i < iColumnCount; i++)
            free((ccChain + i)->current);
        free(ccChain);
        EndDialog(hWnd, 0);
        //SendMessage(hWnd, WM_CLOSE, 0, 0);
        ::DestroyWindow(hWnd); //PostQuitMessage(0);
        hWnd = NULL;
    };
    virtual void WMRBUTTONDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        KillTimer(hWnd, ID_TIMER);
    };
    virtual void WMRBUTTONUP(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        SetTimer(hWnd, ID_TIMER, 10, NULL);
    };
    virtual void WMCLOSE(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        EndDialog(hWnd, 0);
        hWnd = NULL;
    };
    virtual void WMTIMER(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        hdc = GetDC(hWnd);
        //���ڴ��豸ӳ��ˢ�ɺ�ɫ
        PatBlt(hdcMem, 0, 0, cxScreen, cyScreen, BLACKNESS);
        for (i = 0; i < iColumnCount; i++)
        {
            ctn = (ccChain + i)->iStopTimes++ > (ccChain + i)->iMustStopTimes;
            (ccChain + i)->point = (ccChain + i)->head; //point���ڱ���������ʾ��
            //��һ���ַ���ʾΪ ��ɫ
            SetTextColor(hdcMem, RGB(255, 255, 255));
            TextOut(hdcMem, (ccChain + i)->x, (ccChain + i)->y, &((ccChain + i)->point->ch), 1);
            j = (ccChain + i)->y;
            (ccChain + i)->point = (ccChain + i)->point->next;
            //����������ʾ��,�������ʾ������ַ�����������ʾ
            temp = 0; //temp��ɫ���ȵ���ɫ֮��
            while ((ccChain + i)->point != (ccChain + i)->head && (ccChain + i)->point->ch)
            {
                SetTextColor(hdcMem, RGB(0, 255 - (255 * (temp++) / (ccChain + i)->iStrLen), 0));
                TextOut(hdcMem, (ccChain + i)->x, j -= iFontHeight, &((ccChain + i)->point->ch), 1);
                (ccChain + i)->point = (ccChain + i)->point->next;
            }
            if (ctn)
                (ccChain + i)->iStopTimes = 0;
            else continue;
            (ccChain + i)->y += iFontHeight; //�´ο�ʼ��ʾ��y���� Ϊ��ǰ��y������� һ���ַ��ĸ߶�
            //�����ʼ��ʾ��y�����ȥ ������ʾ�еĳ��ȳ�������Ļ�ĸ߶�
            if ((ccChain + i)->y - (ccChain + i)->iStrLen * iFontHeight > cyScreen)
            {
                free((ccChain + i)->current);
                init(ccChain + i, cyScreen, (iFontWidth * 3 / 2)*i);
            }
            //�����ͷ Ϊ�������ǰ��Ԫ��,��Ϊ�´ο�ʼ��ʾ��ʱ�� ���൱����������ʾ�еĿ�ͷ��Ӹ�Ԫ��,Ȼ���ڿ�ʼ������ʾ
            (ccChain + i)->head = (ccChain + i)->head->prev;
            (ccChain + i)->head->ch = randomChar();
        }
        BitBlt(hdc, 0, 0, cxScreen, cyScreen, hdcMem, 0, 0, SRCCOPY);
        ReleaseDC(hWnd, hdc);
    };
    virtual void WMKEYDOWN(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        WMLBUTTONDOWN(hWnd, uMsg, wParam, lParam);
    };
public:
    void Create(void)
    {
        XqWindowEx::Create();
        //ShowCursor(FALSE);                            //���������
        srand((int)GetCurrentTime());                   //��ʼ�������������
        ::SetWindowLongPtr(GetHandle(), GWL_STYLE, WS_POPUP);
        ShowWindow(GetHandle(), SW_SHOWMAXIMIZED);      //�����ʾ
    }
private:
    //����ַ���������
    TCHAR randomChar(void)
    {
        //33��126֮��
        return (TCHAR)(rand() % (126 - 33) + 33);
    };
    //��ʼ��
    int init(CharColumn* cc, int cyScreen, int x)
    {
        int j;
        //��ʾ�еĳ���
        cc->iStrLen = rand() % (STRMAXLEN - STRMINLEN) + STRMINLEN;
        //��ʾ�еĿ�ʼ��ʾ��x����
        cc->x = x + 3;
        //��ʾ�еĿ�ʼ��ʾ��y����
        cc->y = rand() % 3 ? rand() % cyScreen : 0;
        cc->iMustStopTimes = rand() % 6;
        cc->iStopTimes = 0;
        //������ʾ��
        cc->head = cc->current = (pCharChain)calloc(cc->iStrLen, sizeof(CharChain));
        for (j = 0; j < cc->iStrLen - 1; j++)
        {
            cc->current->prev = cc->point;          //cc->pointһ����ʾ�е�ǰ��Ԫ��
            cc->current->ch = ' ';
            cc->current->next = cc->current + 1;    //cc->current+1һ����ʾ�еĺ��Ԫ��
            cc->point = cc->current++;              //cc->point = cc->current; cc->current++;
        }
        cc->current->prev = cc->point;              //���һ���ڵ�
        cc->current->ch = ' ';
        cc->current->next = cc->head;
        cc->head->prev = cc->current;               //ͷ�ڵ��ǰһ��Ϊ���������һ��Ԫ��
        cc->current = cc->point = cc->head;         //free��������ڴ�Ҫ��current������
        cc->head->ch = randomChar();                //������ͷ�� Ԫ�����
        return 0;
    };
};















#endif

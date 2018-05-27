//*************************************************************************
//
//                      黑客帝国字符雨屏保
//                                             @2017-09-03阿甘整理
//
//*************************************************************************
#ifndef HARKSCR_HPP
#define HARKSCR_HPP
#include "WinGUI.h"

#define ID_TIMER 1
#define STRMAXLEN 25    //一个显示列的最大长度
#define STRMINLEN 8     //一个显示列的最小长度

//整个当作屏幕的一个显示列,这是个双向列表
typedef struct tagCharChain
{
    struct tagCharChain* prev;      //链表的前个元素
    TCHAR ch;                       //一个显示列中的一个字符
    struct tagCharChain* next;      //链表的后个元素
} CharChain, *pCharChain;

typedef struct tagCharColumn
{
    CharChain* head, *current, *point;
    int x, y, iStrLen;              //显示列的开始显示的x,y坐标,iStrLen是这个列的长度
    int iStopTimes, iMustStopTimes; //已经停滞的次数和必须停滞的次数,必须停滞的次数是随机的
} CharColumn, *pCharColumn;

//黑客帝国屏保
class WinHarkscr : public XqWindowEx
{
public:
    //简单非模态对话框测试 主程序入口
    static int WINMAIN(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nCmdShow)
    {
        WinHarkscr WND(hInstance);
        WND.Create();
        return WND.ShowDLG();     //消息循环
    }
private:
    HDC hdc;
    //ctn 用来确定一个显示链是否 向下前进,如果等待次数超过必须等待的次数,ctn就代表要向下前进
    int i, j, temp, ctn; //j为一个显示链中除链表头外的在屏幕上显示的y坐标,temp绿色过度到黑色之用
    HDC hdcMem;
    HFONT hFont;
    HBITMAP hBitmap;
    int cxScreen, cyScreen;                     //屏幕的宽度 高度.
    int iFontWidth, iFontHeight, iColumnCount;  //字体的宽度 高度, 列数
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
        cxScreen = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度
        cyScreen = GetSystemMetrics(SM_CYSCREEN);
        SetTimer(hWnd, ID_TIMER, 1, NULL);
        hdc = GetDC(hWnd);
        hdcMem = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, cxScreen, cyScreen);
        SelectObject(hdcMem, hBitmap);
        ReleaseDC(hWnd, hdc);
        //创建字体
        hFont = CreateFont(iFontHeight, iFontWidth - 5, 0, 0, FW_BOLD, 0, 0, 0,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                           DRAFT_QUALITY, FIXED_PITCH | FF_SWISS, TEXT("Consolas"));    //Fixedsys
        SelectObject(hdcMem, hFont);
        DeleteObject(hFont);
        SetBkMode(hdcMem, TRANSPARENT); //设置背景模式为 透明
        iColumnCount = cxScreen / (iFontWidth * 3 / 2); //屏幕所显示字母雨的列数
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
        //将内存设备映像刷成黑色
        PatBlt(hdcMem, 0, 0, cxScreen, cyScreen, BLACKNESS);
        for (i = 0; i < iColumnCount; i++)
        {
            ctn = (ccChain + i)->iStopTimes++ > (ccChain + i)->iMustStopTimes;
            (ccChain + i)->point = (ccChain + i)->head; //point用于遍历整个显示列
            //第一个字符显示为 白色
            SetTextColor(hdcMem, RGB(255, 255, 255));
            TextOut(hdcMem, (ccChain + i)->x, (ccChain + i)->y, &((ccChain + i)->point->ch), 1);
            j = (ccChain + i)->y;
            (ccChain + i)->point = (ccChain + i)->point->next;
            //遍历整个显示列,将这个显示列里的字符从下往上显示
            temp = 0; //temp绿色过度到黑色之用
            while ((ccChain + i)->point != (ccChain + i)->head && (ccChain + i)->point->ch)
            {
                SetTextColor(hdcMem, RGB(0, 255 - (255 * (temp++) / (ccChain + i)->iStrLen), 0));
                TextOut(hdcMem, (ccChain + i)->x, j -= iFontHeight, &((ccChain + i)->point->ch), 1);
                (ccChain + i)->point = (ccChain + i)->point->next;
            }
            if (ctn)
                (ccChain + i)->iStopTimes = 0;
            else continue;
            (ccChain + i)->y += iFontHeight; //下次开始显示的y坐标 为当前的y坐标加上 一个字符的高度
            //如果开始显示的y坐标减去 整个显示列的长度超过了屏幕的高度
            if ((ccChain + i)->y - (ccChain + i)->iStrLen * iFontHeight > cyScreen)
            {
                free((ccChain + i)->current);
                init(ccChain + i, cyScreen, (iFontWidth * 3 / 2)*i);
            }
            //链表的头 为此链表的前个元素,因为下次开始显示的时候 就相当与在整个显示列的开头添加个元素,然后在开始往上显示
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
        //ShowCursor(FALSE);                            //隐藏鼠标光标
        srand((int)GetCurrentTime());                   //初始化随机数发生器
        ::SetWindowLongPtr(GetHandle(), GWL_STYLE, WS_POPUP);
        ShowWindow(GetHandle(), SW_SHOWMAXIMIZED);      //最大化显示
    }
private:
    //随机字符产生函数
    TCHAR randomChar(void)
    {
        //33到126之间
        return (TCHAR)(rand() % (126 - 33) + 33);
    };
    //初始化
    int init(CharColumn* cc, int cyScreen, int x)
    {
        int j;
        //显示列的长度
        cc->iStrLen = rand() % (STRMAXLEN - STRMINLEN) + STRMINLEN;
        //显示列的开始显示的x坐标
        cc->x = x + 3;
        //显示列的开始显示的y坐标
        cc->y = rand() % 3 ? rand() % cyScreen : 0;
        cc->iMustStopTimes = rand() % 6;
        cc->iStopTimes = 0;
        //生成显示列
        cc->head = cc->current = (pCharChain)calloc(cc->iStrLen, sizeof(CharChain));
        for (j = 0; j < cc->iStrLen - 1; j++)
        {
            cc->current->prev = cc->point;          //cc->point一个显示列的前个元素
            cc->current->ch = ' ';
            cc->current->next = cc->current + 1;    //cc->current+1一个显示列的后个元素
            cc->point = cc->current++;              //cc->point = cc->current; cc->current++;
        }
        cc->current->prev = cc->point;              //最后一个节点
        cc->current->ch = ' ';
        cc->current->next = cc->head;
        cc->head->prev = cc->current;               //头节点的前一个为此链的最后一个元素
        cc->current = cc->point = cc->head;         //free掉申请的内存要用current当参数
        cc->head->ch = randomChar();                //对链表头的 元素填充
        return 0;
    };
};















#endif

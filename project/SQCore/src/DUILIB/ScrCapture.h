#pragma once
#include <UIlib.h>
using namespace DuiLib;

#define  DUI_CTR_CANVAS                          (_T("Canvas"))
#define  DUI_CTR_CANVASCONTAINER                 (_T("CanvasContainer"))

#define MENUCMD_ADDRECTANGE     200
#define MENUCMD_ADDELLIPSE      201
#define MENUCMD_ADDARROW        202
#define MENUCMD_ADDBRUSH        203
#define MENUCMD_ADDTEXT         204
#define MENUCMD_UNDO            205
#define MENUCMD_RECHOICE        206
#define MENUCMD_SAVEANDQUIT     207
#define MENUCMD_SAVE            208
#define MENUCMD_HIDETOOLBAR     209
#define MENUCMD_QUIT            210

HBITMAP CreateDesktopBitmap(HWND hWnd);
HBITMAP CreateDesktopMaskBitmap(HWND hWnd);
HWND SmallestWindowFromCursor(RECT& rcWindow);

class CCanvasUI : public CControlUI
{
public:
    CCanvasUI();
    ~CCanvasUI();
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    void DoEvent(TEventUI& event);
};

class CCanvasContainerUI : public CContainerUI
{
public:
    CCanvasContainerUI();
    LPCTSTR GetClass() const;
    LPVOID GetInterface(LPCTSTR pstrName);
    UINT GetControlFlags() const;

    // 0    1    2
    // 3    4    5
    // 6    7    8
    RECT GetSizerRect(int iIndex);
    LPTSTR GetSizerCursor(POINT& pt, int& iCursor);
    void PaintBorder(HDC hDC);
    void DoEvent(TEventUI& event);

protected:
    int m_iCursor;
    UINT m_uButtonState;
    POINT m_ptClipBasePoint; // 调整基准点
    POINT m_ptLastMouse; //
};

class CScrCaptureWnd : public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
private:
    CScrCaptureWnd();
    ~CScrCaptureWnd();

public:
    static CScrCaptureWnd* Instance();

    LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND /*hWnd*/);

    void Init();
    void OnPrepare();
    void Notify(TNotifyUI& msg);

    RECT GetWindowRect() const;
    RECT GetClipPadding() const;
    void SetClipPadding(RECT rc);
    RECT GetClipRect() const;
    RECT GetCanvasContainerRect() const;

    LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    static CScrCaptureWnd* m_pInstance;
    CPaintManagerUI m_pm;
    HMENU m_hEditMenu;

    CControlUI* m_pDesktopImage;
    CControlUI* m_pDesktopMask;
    CControlUI* m_pDesktopMaskBorder;
    CContainerUI* m_pDesktopCanvasContainer;

    RECT m_rcWindow;
    RECT m_rcClipPadding;
    POINT m_ptClipBasePoint;                // 调整基准点
    bool m_bClipChoiced;                    // 是否已选择截图区域
    TCHAR m_sClipDrawStringBuf[MAX_PATH];
};


#include "stdafx.h"
#include "CTray.h"


CTray::CTray() : CTray(NULL, 0, NULL)
{
}

CTray::CTray(HWND hWnd, size_t IconID, HMENU hMenu) : hDlgWnd(hWnd)
    , TaskIconID(IconID)
    , hTrayMenu(hMenu)
{
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hDlgWnd;
    nid.uID = TaskIconID;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_SHOWTASK; //回调信息
    lstrcpyA(nid.szTip, "asds");
    SetIcon(TaskIconID);
    ShowWindow(hWnd, SW_HIDE);
    //显示托盘图标
    ShowTray(true);
}

CTray::~CTray()
{
    ShowTray(false);
}

void CTray::SetMenu(HMENU hMenu)
{
    hTrayMenu = hMenu;
}

void CTray::SetIcon(size_t IconID)
{
#ifdef _WIN64
    nid.hIcon = LoadIconA((HINSTANCE)GetWindowLongPtr(hDlgWnd, GWLP_HINSTANCE), MAKEINTRESOURCEA(IconID));
#else
    nid.hIcon = LoadIconA((HINSTANCE)GetWindowLongA(hDlgWnd, GWLP_HINSTANCE), MAKEINTRESOURCEA(IconID));
#endif
}

void CTray::ShowTask(HWND hWnd, WPARAM IconID, LPARAM iMouseMsg)
{
    MessageBoxA(NULL, "ShowTask", "BOX", MB_OK);
    if (IconID != TaskIconID)
        return;
    switch (iMouseMsg)
    {
        case WM_LBUTTONDBLCLK:
        {
            ShowWindow(hWnd, SW_SHOWNORMAL);
            SetForegroundWindow(hWnd);
            ShowTray(false);
        }
        break;
        case WM_RBUTTONDOWN:
        {
            POINT pos;
            GetCursorPos(&pos);
            SetForegroundWindow(hWnd);
            TrackPopupMenu(hTrayMenu, TPM_CENTERALIGN | TPM_RIGHTBUTTON, pos.x + 65, pos.y - 9, 0, hWnd, NULL);
        }
        break;
        default:
            break;
    }
}

void CTray::ShowTray(bool _state)
{
    //添加删除托盘图标
    Shell_NotifyIconA(_state ? NIM_ADD : NIM_DELETE, &nid);
}






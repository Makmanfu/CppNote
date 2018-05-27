

#include <Windows.h>
#include <stdio.h>
#include "CAStar.h"

#define CELL 20                 //20·½¸ñ
extern int map[20][20];         //20x20µØÍ¼
int nGameState = 1;

CAStar astar;
HBRUSH BeginBrush, EndBrush, PathBrush, WallBrush, SearchBrush;

LRESULT CALLBACK WndProcAStar(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
    stNode* pNode;
    int xPos, yPos;
    static POINT ptBegin;
    static POINT ptEnd;
    switch (message)
    {
        case WM_CREATE:
            BeginBrush = CreateSolidBrush(RGB(255, 255, 0));
            EndBrush = CreateSolidBrush(RGB(255, 0, 0));
            PathBrush = CreateSolidBrush(RGB(0, 255, 255));
            WallBrush = CreateSolidBrush(RGB(0, 0, 0));
            SearchBrush = CreateSolidBrush(RGB(0, 255, 0));
            return 0;
        case WM_TIMER:
            hdc = GetDC(hWnd);
            if ((pNode = astar.GetNext()) != NULL)
            {
                SelectObject(hdc, SearchBrush);
                Rectangle(hdc, pNode->yPos * CELL, pNode->xPos * CELL, (pNode->yPos + 1) *CELL, (pNode->xPos + 1) *CELL);
            }
            else
            {
                KillTimer(hWnd, 1);
                nGameState = 2;
                InvalidateRect(hWnd, NULL, false);
            }
            ReleaseDC(hWnd, hdc);
            return 0;
        case WM_PAINT:
            hdc = BeginPaint(hWnd, &ps);
            for (int i = 0; i < 20; ++i)
            {
                for (int j = 0; j < 20; ++j)
                {
                    if (map[i][j] == 1)
                        SelectObject(hdc, WallBrush);
                    else
                        SelectObject(hdc, PathBrush);
                    Rectangle(hdc, j * CELL, i * CELL, (j + 1) *CELL, (i + 1) *CELL);
                }
            }
            if (nGameState == 2)
            {
                ptBegin = ptEnd;
                SelectObject(hdc, BeginBrush);
                Rectangle(hdc, ptBegin.x * CELL, ptBegin.y * CELL, (ptBegin.x + 1) *CELL, (ptBegin.y + 1) *CELL);
            }
            EndPaint(hWnd, &ps);
            return 0;
        case WM_LBUTTONDOWN:
            hdc = GetDC(hWnd);
            if (nGameState == 1)
            {
                xPos = LOWORD(lParam);
                yPos = HIWORD(lParam);
                if (map[yPos / CELL][xPos / CELL] == 1)
                {
                    MessageBoxA(NULL, "×²Ç½ÁË", 0, 0);
                    return 0;
                }
                SelectObject(hdc, BeginBrush);
                nGameState = 2;
                ptBegin.x = xPos / CELL;
                ptBegin.y = yPos / CELL;
                Rectangle(hdc, ptBegin.x * CELL, ptBegin.y * CELL, (ptBegin.x + 1) *CELL, (ptBegin.y + 1) *CELL);
                return 0;
            }
            else if (nGameState == 2)
            {
                xPos = LOWORD(lParam);
                yPos = HIWORD(lParam);
                if (map[yPos / CELL][xPos / CELL] == 1)
                {
                    MessageBoxA(NULL, "×²Ç½ÁË", 0, 0);
                    return 0;
                }
                SelectObject(hdc, EndBrush);
                ptEnd.x = xPos / CELL;
                ptEnd.y = yPos / CELL;
                Rectangle(hdc, ptEnd.x * CELL, ptEnd.y * CELL, (ptEnd.x + 1) *CELL, (ptEnd.y + 1) *CELL);
                nGameState = 3;
                astar.Search(ptEnd.y, ptEnd.x, ptBegin.y, ptBegin.x);
                SetTimer(hWnd, 1, 200, NULL);
                return 0;
            }
            ReleaseDC(hWnd, hdc);
            return 0;
        case WM_DESTROY:
            DeleteObject(BeginBrush);
            DeleteObject(EndBrush);
            DeleteObject(PathBrush);
            DeleteObject(WallBrush);
            DeleteObject(SearchBrush);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nShowCmd)
{
    char* szName = /*TEXT*/ ("Hello");
    WNDCLASSA wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProcAStar;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szName;
    if (!RegisterClassA(&wndclass))
    {
        MessageBoxA(NULL, "×¢²áÊ§°Ü", "A*", 0);
        return 0;
    }
    HWND hWnd = CreateWindowA(szName, /*TEXT*/ ("A*STAR"), WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT
                              , 400, 430, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nShowCmd);
    UpdateWindow(hWnd);
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}





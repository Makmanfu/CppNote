
#include "stdafx.h"
#include "WinKey.h"
#include <iostream>
using namespace std;

class GohanConsoleHelper
{
    HANDLE _hIn;
    HANDLE _hOut;
    INPUT_RECORD _InRec;
    DWORD _NumRead;
public:
    WORD VKey;
    GohanConsoleHelper(void)
    {
        _hIn = GetStdHandle(STD_INPUT_HANDLE);
        _hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        VKey = 0;
    }
    ~GohanConsoleHelper(void) {}
    bool ReadOneInput()
    {
        return 0 != ReadConsoleInput(_hIn, &_InRec, 1, &_NumRead);
    }
    bool ReadOneInput(INPUT_RECORD& InRec)
    {
        return 0 != ReadConsoleInput(_hIn, &InRec, 1, &_NumRead);
    }
    DWORD ReadKeyDown()
    {
        if (!ReadConsoleInput(_hIn, &_InRec, 1, &_NumRead))
            return 0;
        if (_InRec.EventType != KEY_EVENT)
            return 0;
        if (_InRec.Event.KeyEvent.bKeyDown > 0)
            return 0;
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;
        return VKey;
    }
    DWORD ReadKeyPush()
    {
        if (!ReadConsoleInput(_hIn, &_InRec, 1, &_NumRead))
            return 0;
        if (_InRec.EventType != KEY_EVENT)
            return 0;
        if (_InRec.Event.KeyEvent.bKeyDown == 0)
            return 0;
        VKey = _InRec.Event.KeyEvent.wVirtualKeyCode;
        return VKey;
    }
};

CkeyHook* CkeyHook::pKey = NULL;

LRESULT CALLBACK CkeyHook::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // ���ü��̵�ĳ������, ��� g_bDisableKeyboard Ϊ TRUE �������������
    if (nCode == HC_ACTION)
    {
        if (pKey->KEY_bDisableKeyboard)
            return TRUE;
        KBDLLHOOKSTRUCT* pStruct = (KBDLLHOOKSTRUCT*)lParam;
        LPDWORD tmpVirtualKey = pKey->KEY_lpdwVirtualKey;
        for (int i = 0; i < pKey->KEY_nLength; i++)
            if (pStruct->vkCode == *tmpVirtualKey++)
                return TRUE;
    }
    // ����ϵͳ�е���һ������
    return CallNextHookEx(pKey->KEY_hHook, nCode, wParam, lParam);
}

CkeyHook::CkeyHook(void) :KEY_lpdwVirtualKey(NULL)
, KEY_nLength(0)
, KEY_bDisableKeyboard(false)
, KEY_hInstance(NULL)
, KEY_hHook(NULL)
{
    CkeyHook::pKey = this;
}

CkeyHook::~CkeyHook(void)
{
    StopMaskKey();
}

void CkeyHook::sethInstance(HINSTANCE HD)
{
    if (HD == NULL)
    {
        static HINSTANCE hInstance = GetModuleHandle(NULL);
        KEY_hInstance = hInstance;
    }
    // ����ģ��ʵ�����
    //KEY_hInstance = (HINSTANCE)hModule;
    KEY_hInstance = HD;
}

bool CkeyHook::StartMaskKey(LPDWORD lpdwVirtualKey, int nLength, bool bDisableKeyboard)
{
    if (KEY_hInstance == NULL)
        sethInstance(NULL);
    /********************************************************************
    ��ʼ���μ��̰���
    ����:
    lpdwVirtualKey          Keycode �����ָ��
    nLength                 Keycode ����Ĵ�С
    bDisableKeyboard        �Ƿ�������������
    ����ֵ:    TRUE �ɹ�, FALSE ʧ��
    *********************************************************************/
    // ����Ѿ���װ���̹����򷵻� FALSE
    if (KEY_hHook != NULL)
        return false;
    // ���û������� keycode ���鱣����ȫ�ֱ�����
    KEY_lpdwVirtualKey = (LPDWORD)malloc(sizeof(DWORD) * nLength);
    LPDWORD tmpVirtualKey = KEY_lpdwVirtualKey;
    for (int i = 0; i < nLength; i++)
        *tmpVirtualKey++ = *lpdwVirtualKey++;
    KEY_nLength = nLength;
    KEY_bDisableKeyboard = bDisableKeyboard;
    // ��װ�ײ���̹���
    KEY_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, KEY_hInstance, NULL);
    if (KEY_hHook == NULL)
        return false;
    return true;
}

bool CkeyHook::StopMaskKey(void)
{
    // �ڽ��̽������߳̽���ʱж�ع���
    if (KEY_hHook != NULL)
    {
        if (KEY_lpdwVirtualKey != NULL)
            delete KEY_lpdwVirtualKey;
        // ж�ع���
        if (UnhookWindowsHookEx(KEY_hHook) == 0)
        {
            return false;
        } else {
            KEY_hHook = NULL;
            return true;
        }
    }
    else{
        return true;
    }
}

void CkeyHook::MaskkeyABC(void)
{
    // ���� A, B, C, ��, ��, ��, �Ҽ�����win��
    DWORD dwVK[] = { 'A', 'B', 'C', 'L', VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_LWIN, VK_RWIN };
    int nLength = sizeof(dwVK) / sizeof(DWORD);
    StartMaskKey(dwVK, nLength, TRUE);
}

void CkeyHook::MaskAllkey(void)
{
    DWORD dwVK[] =
    {
        VK_ESCAPE, VK_CONVERT, VK_NONCONVERT, VK_ACCEPT, VK_MODECHANGE, VK_SPACE,
        VK_PRIOR, VK_NEXT, VK_END, VK_HOME, VK_LEFT, VK_UP, VK_RIGHT, VK_DOWN, VK_ESCAPE,
        VK_SELECT, VK_PRINT, VK_EXECUTE, VK_SNAPSHOT, VK_INSERT, VK_DELETE, VK_HELP,
        VK_BACK, VK_TAB, VK_CLEAR, VK_RETURN, VK_SHIFT, VK_CONTROL, VK_MENU, VK_PAUSE,
        VK_CAPITAL, VK_KANA, VK_HANGEUL, VK_HANGUL, VK_JUNJA, VK_FINAL, VK_HANJA, VK_KANJI,
        VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN, VK_LWIN, VK_RWIN
    };
    int nLength = sizeof(dwVK) / sizeof(DWORD);
    StartMaskKey(dwVK, nLength, TRUE);
}

void CkeyHook::MaskCtrlAltDel(void)
{
    //�Ϸ����Ѿ��������� ��Ҫ���·���
}

void CkeyHook::GetPressKeyValue(void)
{
    GohanConsoleHelper gch;
    while (true)
    {
        //ʹ��ReadKeyDown()���񰴼������VK_CODE
        if (gch.ReadKeyPush() != 0) 
        {
            if (gch.VKey == VK_ESCAPE)
            {
                cout << "Bye~~" << endl;
                break;
            }
            else
                cout << "VK_CODE == " << gch.VKey << endl;
        }
    }
}


//��������
void testkey(void)
{
    //EXE���÷���
    static HINSTANCE hInstance = GetModuleHandle(NULL);
    CkeyHook key;
    //����ʵ�����
    key.sethInstance(hInstance);
    //�����ȼ�
    key.MaskAllkey();
    //key.GetPressKeyValue();
}





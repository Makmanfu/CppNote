//-----------------------------------------------------------------------------
//
//          windows hook �ͼ��̲���
//                                          @2013-08-20��������
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//���̲���
class SQCPP_API CkeyHook
{
    static int main(int argc, char** argv)
    {
        CkeyHook key;
        key.MaskAllkey();               //GetPressKeyValue
        return 0;
    };
public:
    LPDWORD KEY_lpdwVirtualKey;             // Keycode �����ָ��
    int KEY_nLength;                        // Keycode ����Ĵ�С
    bool KEY_bDisableKeyboard;              // �Ƿ�������������
    HINSTANCE KEY_hInstance;                // ģ��ʵ�����
    HHOOK KEY_hHook;                        // ���Ӿ��
    static CkeyHook* pKey;                  // ��̬����ָ��
public:
    //�ײ���̹��Ӻ���
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
public:
    CkeyHook(void);
    ~CkeyHook(void);
    void sethInstance(HINSTANCE HD);
public:
    //��װ���Ӻ���
    bool StartMaskKey(LPDWORD lpdwVirtualKey, int nLength, bool bDisableKeyboard);
    //ж�ع��Ӻ��� 
    bool StopMaskKey(void);
    //���� A, B, C, ��, ��, ��, �Ҽ�����win��
    void MaskkeyABC(void);
    //���ΰ���
    void MaskAllkey(void);
    //����Ctrl+alt+del���̹�����
    void MaskCtrlAltDel(void);
    //����������ʾ
    void GetPressKeyValue(void);
};












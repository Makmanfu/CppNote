//-----------------------------------------------------------------------------
//
//          windows hook 和键盘操作
//                                          @2013-08-20阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>

//键盘操作
class SQCPP_API CkeyHook
{
    static int main(int argc, char** argv)
    {
        CkeyHook key;
        key.MaskAllkey();               //GetPressKeyValue
        return 0;
    };
public:
    LPDWORD KEY_lpdwVirtualKey;             // Keycode 数组的指针
    int KEY_nLength;                        // Keycode 数组的大小
    bool KEY_bDisableKeyboard;              // 是否屏蔽整个键盘
    HINSTANCE KEY_hInstance;                // 模块实例句柄
    HHOOK KEY_hHook;                        // 钩子句柄
    static CkeyHook* pKey;                  // 静态对象指针
public:
    //底层键盘钩子函数
    static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
public:
    CkeyHook(void);
    ~CkeyHook(void);
    void sethInstance(HINSTANCE HD);
public:
    //安装钩子函数
    bool StartMaskKey(LPDWORD lpdwVirtualKey, int nLength, bool bDisableKeyboard);
    //卸载钩子函数 
    bool StopMaskKey(void);
    //屏蔽 A, B, C, 上, 下, 左, 右及两个win键
    void MaskkeyABC(void);
    //屏蔽按键
    void MaskAllkey(void);
    //屏蔽Ctrl+alt+del进程管理器
    void MaskCtrlAltDel(void);
    //按键触发显示
    void GetPressKeyValue(void);
};












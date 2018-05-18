//*************************************************************************
//
//                  MFC事件回调消息事件到派发封装
//
//
//*************************************************************************


#pragma once
#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;

typedef enum
{
    Key_None = 0,
    // unicode keys
    Key_0,
    Key_1,
    Key_2,
    Key_3,
    Key_4,
    Key_5,
    Key_6,
    Key_7,
    Key_8,
    Key_9,

    Key_A,
    Key_B,
    Key_C,
    Key_D,
    Key_E,
    Key_F,
    Key_G,
    Key_H,
    Key_I,
    Key_J,
    Key_K,
    Key_L,
    Key_M,
    Key_N,
    Key_O,
    Key_P,
    Key_Q,
    Key_R,
    Key_S,
    Key_T,
    Key_U,
    Key_V,
    Key_W,
    Key_X,
    Key_Y,
    Key_Z,

    Key_Return,
    Key_BackSpace,
    Key_Tab,
    Key_Space,

    Key_Clear,
    Key_Escape,
    Key_Exclam,
    Key_QuoteDbl,
    Key_Hash,
    Key_Dollar,
    Key_Ampersand,
    Key_Quote,
    Key_LeftParen,
    Key_RightParen,
    Key_Asterisk,
    Key_Plus,
    Key_Comma,
    Key_Minus,
    Key_Period,
    Key_Slash,
    Key_Colon,
    Key_Semicolon,
    Key_Less,
    Key_Equal,
    Key_Greater,
    Key_Question,
    Key_At,
    Key_LeftBracket,
    Key_BackSlash,
    Key_RightBracket,
    Key_Caret,
    Key_Underscore,
    Key_QuoteLeft,

    // non unicode keys

    Key_Ctrl,
    Key_LeftCtrl,
    Key_RightCtrl,
    Key_Alt,
    Key_LeftAlt,
    Key_RightAlt,
    Key_Shift,
    Key_LeftShift,
    Key_RightShift,
    Key_Insert,
    Key_Delete,
    Key_Home,
    Key_End,
    Key_Print,
    Key_Pause,
    Key_PageUp,
    Key_PageDown,
    Key_Left,
    Key_Right,
    Key_Up,
    Key_Down,
    Key_F1,
    Key_F2,
    Key_F3,
    Key_F4,
    Key_F5,
    Key_F6,
    Key_F7,
    Key_F8,
    Key_F9,
    Key_F10,
    Key_F11,
    Key_F12,

    Key_Unknown,

    Key_NumberOfKeys

} EKey;

typedef enum
{
    NoButton = 0,
    LeftButton   = 1,
    RightButton  = 2,
    MiddleButton = 4,
    UnknownButton,
} EMouseButton;

template <typename UI>
class EventListener
{
    friend typename UI;
public:
    EventListener() : ui_(0), mRank(0), mEnabled(true) {};
    virtual ~EventListener() {};
public:
    virtual void initEvent() {};
    virtual void mouseMoveEvent(int, int) {};
    virtual void mouseUpEvent(EMouseButton, int, int) {};
    virtual void mouseDownEvent(EMouseButton, int, int) {};
    virtual void mouseWheelEvent(int) {};
    virtual void keyPressEvent(unsigned short, EKey) {};
    virtual void keyReleaseEvent(unsigned short, EKey) {};
    virtual void resizeEvent(int, int) {};
    virtual void destroyEvent() {};
    virtual void runEvent() {};
    virtual void fileDroppedEvent(const std::vector<string>&) {};
    virtual void prepareToReconnect() {};

    void setRank(int rank)
    {
        mRank = rank;
    };

    int rank() const
    {
        return mRank;
    };

    UI* wnd()
    {
        return ui_;
    };

    virtual void setEnabled(bool enabled)
    {
        mEnabled = enabled;
    };

    bool isEnabled() const
    {
        return mEnabled;
    };

    template <class UI>
    bool operator<(const EventListener<UI>& other) const
    {
        return rank() < other.rank();
    };

    UI* ui_;

protected:
    int mRank;
    bool mEnabled;
};

template <class UI>
class UIEventDispatcher : virtual public UI
{
public:
    typedef std::shared_ptr<EventListener<UI> > UIEventListener;

    UIEventDispatcher()
    {
        mMouseDownCount = 0;
    }

    virtual ~UIEventDispatcher()
    {
        dispatchDestroyEvent();
        eraseAllEventListeners();
    }

    void sortEventListeners()
    {
        std::sort(mEventListeners.begin(), mEventListeners.end());
    }

    void addEventListener(UIEventListener el)
    {
        el->ui_ = this;
        mEventListeners.push_back(el);
        sortEventListeners();
    }

    void removeEventListener(UIEventListener el)
    {
        std::vector< UIEventListener >::iterator pos =
            std::find(mEventListeners.begin(), mEventListeners.end(), el);
        if (pos != mEventListeners.end())
            mEventListeners.erase(pos);
    }

    void eraseAllEventListeners()
    {
        mEventListeners.clear();
    }

    std::vector< UIEventListener >& eventListeners()
    {
        return mEventListeners;
    }

    UIEventListener eventListener(int i) const
    {
        return mEventListeners[i].get();
    }

    int eventListenerCount() const
    {
        return (int)mEventListeners.size();
    }

    void dispatchResizeEvent(int w, int h)
    {
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->resizeEvent(w, h);
    }

    void dispatchMouseMoveEvent(int x, int y)
    {
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->mouseMoveEvent(x, y);
    }

    void dispatchMouseUpEvent(EMouseButton button, int x, int y)
    {
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->mouseUpEvent(button, x, y);
    }

    void dispatchMouseDownEvent(EMouseButton button, int x, int y)
    {
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->mouseDownEvent(button, x, y);
    }

    void dispatchMouseWheelEvent(int n)
    {
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->mouseWheelEvent(n);
    }

    void dispatchKeyPressEvent(unsigned short unicode_ch, EKey key)
    {
        keyPress(key);
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->keyPressEvent(unicode_ch, key);
    }

    void dispatchKeyReleaseEvent(unsigned short unicode_ch, EKey key)
    {
        keyRelease(key);
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->keyReleaseEvent(unicode_ch, key);
    }

    void dispatchDestroyEvent()
    {
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            //if ( temp_clients[i]->isEnabled() )
            temp_clients[i]->destroyEvent();
    }

    void dispatchRunEvent()
    {
        std::vector< UIEventListener > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            //if ( temp_clients[i]->isEnabled() )
            temp_clients[i]->runEvent();
    }

    void dispatchInitEvent()
    {
        std::vector< std::shared_ptr<UIEventListener> > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->initEvent();
    }

    void dispatchFileDroppedEvent(const std::vector<string>& files)
    {
        std::vector< std::shared_ptr<UIEventListener> > temp_clients = eventListeners();
        for (unsigned i = 0; i < temp_clients.size(); ++i)
            if (temp_clients[i]->isEnabled())
                temp_clients[i]->fileDroppedEvent(files);
    }

    const std::set<EKey>& keyboard() const
    {
        return mKeyboard;
    }

    bool isKeyPressed(EKey key) const
    {
        return mKeyboard.find(key) != mKeyboard.end();
    }

    void keyPress(EKey key)
    {
        mKeyboard.insert(key);
    }

    void keyRelease(EKey key)
    {
        mKeyboard.erase(key);
    }
protected:
    std::vector< UIEventListener > mEventListeners;
    std::set<EKey> mKeyboard;
    int mMouseDownCount;

    void CountAndCapture()
    {
        mMouseDownCount++;
        if (mMouseDownCount == 1)
            ::SetCapture(hwnd());
    }

    void CountAndRelease()
    {
        mMouseDownCount--;
        if (mMouseDownCount <= 0)
        {
            ReleaseCapture();
            mMouseDownCount = 0;
        }
    }

    void translateKeyEvent(WPARAM wParam, LPARAM lParam, unsigned short& unicode_out, EKey& key_out)
    {
        // translate non unicode characters
        key_out    = Key_None;
        unicode_out = 0;
        switch (wParam)
        {
            case VK_CLEAR:
                key_out = Key_Clear;
                break;
            case VK_CONTROL:
                key_out = Key_Ctrl;
                break;
            case VK_LCONTROL:
                key_out = Key_LeftCtrl;
                break;
            case VK_RCONTROL:
                key_out = Key_RightCtrl;
                break;
            case VK_MENU:
                key_out = Key_Alt;
                break;
            case VK_LMENU:
                key_out = Key_LeftAlt;
                break;
            case VK_RMENU:
                key_out = Key_RightAlt;
                break;
            case VK_SHIFT:
                key_out = Key_Shift;
                break;
            case VK_LSHIFT:
                key_out = Key_LeftShift;
                break;
            case VK_RSHIFT:
                key_out = Key_RightShift;
                break;
            case VK_INSERT:
                key_out = Key_Insert;
                break;
            case VK_DELETE:
                key_out = Key_Delete;
                break;
            case VK_HOME:
                key_out = Key_Home;
                break;
            case VK_END:
                key_out = Key_End;
                break;
            case VK_PRINT:
                key_out = Key_Print;
                break;
            case VK_PAUSE:
                key_out = Key_Pause;
                break;
            case VK_PRIOR:
                key_out = Key_PageUp;
                break;
            case VK_NEXT:
                key_out = Key_PageDown;
                break;
            case VK_LEFT:
                key_out = Key_Left;
                break;
            case VK_RIGHT:
                key_out = Key_Right;
                break;
            case VK_UP:
                key_out = Key_Up;
                break;
            case VK_DOWN:
                key_out = Key_Down;
                break;
            case VK_F1:
                key_out = Key_F1;
                break;
            case VK_F2:
                key_out = Key_F2;
                break;
            case VK_F3:
                key_out = Key_F3;
                break;
            case VK_F4:
                key_out = Key_F4;
                break;
            case VK_F5:
                key_out = Key_F5;
                break;
            case VK_F6:
                key_out = Key_F6;
                break;
            case VK_F7:
                key_out = Key_F7;
                break;
            case VK_F8:
                key_out = Key_F8;
                break;
            case VK_F9:
                key_out = Key_F9;
                break;
            case VK_F10:
                key_out = Key_F10;
                break;
            case VK_F11:
                key_out = Key_F11;
                break;
            case VK_F12:
                key_out = Key_F12;
                break;
            /*
                VK_0-VK_9 are the same as ASCII '0'-'9' (0x30-0x39)
                0x40 : is unassigned
                VK_A-VK_Z are the same as ASCII 'A'-'Z' (0x41-0x5A)
            */
            case L'0':
                key_out = Key_0;
                break;
            case L'1':
                key_out = Key_1;
                break;
            case L'2':
                key_out = Key_2;
                break;
            case L'3':
                key_out = Key_3;
                break;
            case L'4':
                key_out = Key_4;
                break;
            case L'5':
                key_out = Key_5;
                break;
            case L'6':
                key_out = Key_6;
                break;
            case L'7':
                key_out = Key_7;
                break;
            case L'8':
                key_out = Key_8;
                break;
            case L'9':
                key_out = Key_9;
                break;
            case L'A':
                key_out = Key_A;
                break;
            case L'B':
                key_out = Key_B;
                break;
            case L'C':
                key_out = Key_C;
                break;
            case L'D':
                key_out = Key_D;
                break;
            case L'E':
                key_out = Key_E;
                break;
            case L'F':
                key_out = Key_F;
                break;
            case L'G':
                key_out = Key_G;
                break;
            case L'H':
                key_out = Key_H;
                break;
            case L'I':
                key_out = Key_I;
                break;
            case L'J':
                key_out = Key_J;
                break;
            case L'K':
                key_out = Key_K;
                break;
            case L'L':
                key_out = Key_L;
                break;
            case L'M':
                key_out = Key_M;
                break;
            case L'N':
                key_out = Key_N;
                break;
            case L'O':
                key_out = Key_O;
                break;
            case L'P':
                key_out = Key_P;
                break;
            case L'Q':
                key_out = Key_Q;
                break;
            case L'R':
                key_out = Key_R;
                break;
            case L'S':
                key_out = Key_S;
                break;
            case L'T':
                key_out = Key_T;
                break;
            case L'U':
                key_out = Key_U;
                break;
            case L'V':
                key_out = Key_V;
                break;
            case L'W':
                key_out = Key_W;
                break;
            case L'X':
                key_out = Key_X;
                break;
            case L'Y':
                key_out = Key_Y;
                break;
            case L'Z':
                key_out = Key_Z;
                break;
        }
        // fill unicode
        BYTE mskeys[256];
        memset(mskeys, 0, sizeof(BYTE) * 256);
        WCHAR unicode[4] = { 0, 0 };
        if (ToUnicode((UINT)wParam, (UINT)((lParam >> 16) & 0xFF), mskeys, unicode, 4, 0) == 1)
        {
            unicode_out = unicode[0];
            // fill key
            switch (unicode_out)
            {
                case L'0':
                    key_out = Key_0;
                    break;
                case L'1':
                    key_out = Key_1;
                    break;
                case L'2':
                    key_out = Key_2;
                    break;
                case L'3':
                    key_out = Key_3;
                    break;
                case L'4':
                    key_out = Key_4;
                    break;
                case L'5':
                    key_out = Key_5;
                    break;
                case L'6':
                    key_out = Key_6;
                    break;
                case L'7':
                    key_out = Key_7;
                    break;
                case L'8':
                    key_out = Key_8;
                    break;
                case L'9':
                    key_out = Key_9;
                    break;
                case L'A':
                    key_out = Key_A;
                    break;
                case L'B':
                    key_out = Key_B;
                    break;
                case L'C':
                    key_out = Key_C;
                    break;
                case L'D':
                    key_out = Key_D;
                    break;
                case L'E':
                    key_out = Key_E;
                    break;
                case L'F':
                    key_out = Key_F;
                    break;
                case L'G':
                    key_out = Key_G;
                    break;
                case L'H':
                    key_out = Key_H;
                    break;
                case L'I':
                    key_out = Key_I;
                    break;
                case L'J':
                    key_out = Key_J;
                    break;
                case L'K':
                    key_out = Key_K;
                    break;
                case L'L':
                    key_out = Key_L;
                    break;
                case L'M':
                    key_out = Key_M;
                    break;
                case L'N':
                    key_out = Key_N;
                    break;
                case L'O':
                    key_out = Key_O;
                    break;
                case L'P':
                    key_out = Key_P;
                    break;
                case L'Q':
                    key_out = Key_Q;
                    break;
                case L'R':
                    key_out = Key_R;
                    break;
                case L'S':
                    key_out = Key_S;
                    break;
                case L'T':
                    key_out = Key_T;
                    break;
                case L'U':
                    key_out = Key_U;
                    break;
                case L'V':
                    key_out = Key_V;
                    break;
                case L'W':
                    key_out = Key_W;
                    break;
                case L'X':
                    key_out = Key_X;
                    break;
                case L'Y':
                    key_out = Key_Y;
                    break;
                case L'Z':
                    key_out = Key_Z;
                    break;
                case L'a':
                    key_out = Key_A;
                    break;
                case L'b':
                    key_out = Key_B;
                    break;
                case L'c':
                    key_out = Key_C;
                    break;
                case L'd':
                    key_out = Key_D;
                    break;
                case L'e':
                    key_out = Key_E;
                    break;
                case L'f':
                    key_out = Key_F;
                    break;
                case L'g':
                    key_out = Key_G;
                    break;
                case L'h':
                    key_out = Key_H;
                    break;
                case L'i':
                    key_out = Key_I;
                    break;
                case L'j':
                    key_out = Key_J;
                    break;
                case L'k':
                    key_out = Key_K;
                    break;
                case L'l':
                    key_out = Key_L;
                    break;
                case L'm':
                    key_out = Key_M;
                    break;
                case L'n':
                    key_out = Key_N;
                    break;
                case L'o':
                    key_out = Key_O;
                    break;
                case L'p':
                    key_out = Key_P;
                    break;
                case L'q':
                    key_out = Key_Q;
                    break;
                case L'r':
                    key_out = Key_R;
                    break;
                case L's':
                    key_out = Key_S;
                    break;
                case L't':
                    key_out = Key_T;
                    break;
                case L'u':
                    key_out = Key_U;
                    break;
                case L'v':
                    key_out = Key_V;
                    break;
                case L'w':
                    key_out = Key_W;
                    break;
                case L'x':
                    key_out = Key_X;
                    break;
                case L'y':
                    key_out = Key_Y;
                    break;
                case L'z':
                    key_out = Key_Z;
                    break;
                case 13:
                    key_out = Key_Return;
                    break;
                case 8:
                    key_out = Key_BackSpace;
                    break;
                case 9:
                    key_out = Key_Tab;
                    break;
                case L' ':
                    key_out = Key_Space;
                    break;
                case 27:
                    key_out = Key_Escape;
                    break;
                case L'!':
                    key_out = Key_Exclam;
                    break;
                case L'"':
                    key_out = Key_QuoteDbl;
                    break;
                case L'#':
                    key_out = Key_Hash;
                    break;
                case L'$':
                    key_out = Key_Dollar;
                    break;
                case L'&':
                    key_out = Key_Ampersand;
                    break;
                case L'\'':
                    key_out = Key_Quote;
                    break;
                case L'(':
                    key_out = Key_LeftParen;
                    break;
                case L')':
                    key_out = Key_RightParen;
                    break;
                case L'*':
                    key_out = Key_Asterisk;
                    break;
                case L'+':
                    key_out = Key_Plus;
                    break;
                case L',':
                    key_out = Key_Comma;
                    break;
                case L'-':
                    key_out = Key_Minus;
                    break;
                case L'.':
                    key_out = Key_Period;
                    break;
                case L'\\':
                    key_out = Key_Slash;
                    break;
                case L':':
                    key_out = Key_Colon;
                    break;
                case L';':
                    key_out = Key_Semicolon;
                    break;
                case L'<':
                    key_out = Key_Less;
                    break;
                case L'=':
                    key_out = Key_Equal;
                    break;
                case L'>':
                    key_out = Key_Greater;
                    break;
                case L'?':
                    key_out = Key_Question;
                    break;
                case L'@':
                    key_out = Key_At;
                    break;
                case L'[':
                    key_out = Key_LeftBracket;
                    break;
                case L'/':
                    key_out = Key_BackSlash;
                    break;
                case L']':
                    key_out = Key_RightBracket;
                    break;
                case L'|':
                    key_out = Key_Caret;
                    break;
                case L'_':
                    key_out = Key_Underscore;
                    break;
                case L'`':
                    key_out = Key_QuoteLeft;
                    break;
            }
        }
    }
};

#define DECLARE_UIEvent_Entry(theCalss,baseClass) \
    BEGIN_MESSAGE_MAP(theCalss, baseClass) \
        ON_WM_ERASEBKGND() \
        ON_WM_KEYDOWN() \
        ON_WM_KEYUP() \
        ON_WM_LBUTTONDBLCLK() \
        ON_WM_LBUTTONDOWN() \
        ON_WM_LBUTTONUP() \
        ON_WM_MBUTTONDBLCLK() \
        ON_WM_MBUTTONDOWN() \
        ON_WM_MBUTTONUP() \
        ON_WM_MOUSEMOVE() \
        ON_WM_MOUSEWHEEL() \
        ON_WM_RBUTTONDBLCLK() \
        ON_WM_RBUTTONDOWN() \
        ON_WM_RBUTTONUP() \
        ON_WM_SIZE() \
    END_MESSAGE_MAP() \
    \
    BOOL theCalss::OnEraseBkgnd(CDC* pDC) { \
        return TRUE; \
        return __super::OnEraseBkgnd(pDC); \
    } \
    \
    void theCalss::OnSize(UINT nType, int cx, int cy) {\
        __super::OnSize(nType, cx, cy);\
        \
        resize(cx,cy);\
        \
        this->dispatchResizeEvent(cx,cy);\
    }\
    \
    void theCalss::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {\
        unsigned short unicode_out=0;\
        EKey       key_out    =Key_None;\
        translateKeyEvent(nChar, nFlags, unicode_out, key_out);\
        \
        dispatchKeyPressEvent(unicode_out, key_out);\
    }\
    \
    void theCalss::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {\
        unsigned short unicode_out=0;\
        EKey       key_out    =Key_None;\
        translateKeyEvent(nChar, nFlags, unicode_out, key_out);\
        dispatchKeyReleaseEvent(unicode_out, key_out);\
    }\
    \
    void theCalss::OnLButtonDblClk(UINT nFlags, CPoint point) {\
        \
    }\
    \
    void theCalss::OnLButtonDown(UINT nFlags, CPoint point) {\
        CountAndCapture();\
        dispatchMouseDownEvent(LeftButton,point.x, point.y );\
        CView::OnLButtonDown(nFlags, point);\
    }\
    \
    void theCalss::OnLButtonUp(UINT nFlags, CPoint point) {\
        \
        CountAndRelease();\
        dispatchMouseUpEvent(LeftButton, point.x, point.y );\
    }\
    \
    void theCalss::OnMButtonDblClk(UINT nFlags, CPoint point) {\
    }\
    \
    void theCalss::OnMButtonDown(UINT nFlags, CPoint point) {\
    }\
    \
    void theCalss::OnMButtonUp(UINT nFlags, CPoint point) {\
    }\
    \
    void theCalss::OnMouseMove(UINT nFlags, CPoint point) {\
        dispatchMouseMoveEvent(point.x, point.y );\
    }\
    \
    BOOL theCalss::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) {\
        dispatchMouseWheelEvent (zDelta);\
        return TRUE;\
    }\
    \
    void theCalss::OnRButtonDblClk(UINT nFlags, CPoint point) {\
    }\
    \
    void theCalss::OnRButtonDown(UINT nFlags, CPoint point) {\
        dispatchMouseDownEvent( RightButton, point.x, point.y );\
    }\
    \
    void theCalss::OnRButtonUp(UINT nFlags, CPoint point) {\
        dispatchMouseUpEvent( RightButton, point.x, point.y );\
    }\
    \
    void theCalss::translateKeyEvent(WPARAM wParam, LPARAM lParam, unsigned short& unicode_out, EKey& key_out) {\
        key_out    =Key_None;\
        unicode_out=0;\
        \
        switch(wParam) {\
            case VK_CLEAR:\
                key_out=Key_Clear;\
                break;\
            case VK_CONTROL:\
                key_out=Key_Ctrl;\
                break;\
            case VK_LCONTROL:\
                key_out=Key_LeftCtrl;\
                break;\
            case VK_RCONTROL:\
                key_out=Key_RightCtrl;\
                break;\
            case VK_MENU:\
                key_out=Key_Alt;\
                break;\
            case VK_LMENU:\
                key_out=Key_LeftAlt;\
                break;\
            case VK_RMENU:\
                key_out=Key_RightAlt;\
                break;\
            case VK_SHIFT:\
                key_out=Key_Shift;\
                break;\
            case VK_LSHIFT:\
                key_out=Key_LeftShift;\
                break;\
            case VK_RSHIFT:\
                key_out=Key_RightShift;\
                break;\
            case VK_INSERT:\
                key_out=Key_Insert;\
                break;\
            case VK_DELETE:\
                key_out=Key_Delete;\
                break;\
            case VK_HOME:\
                key_out=Key_Home;\
                break;\
            case VK_END:\
                key_out=Key_End;\
                break;\
            case VK_PRINT:\
                key_out=Key_Print;\
                break;\
            case VK_PAUSE:\
                key_out=Key_Pause;\
                break;\
            case VK_PRIOR:\
                key_out=Key_PageUp;\
                break;\
            case VK_NEXT:\
                key_out=Key_PageDown;\
                break;\
            case VK_LEFT:\
                key_out=Key_Left;\
                break;\
            case VK_RIGHT:\
                key_out=Key_Right;\
                break;\
            case VK_UP:\
                key_out=Key_Up;\
                break;\
            case VK_DOWN:\
                key_out=Key_Down;\
                break;\
            case VK_F1:\
                key_out=Key_F1;\
                break;\
            case VK_F2:\
                key_out=Key_F2;\
                break;\
            case VK_F3:\
                key_out=Key_F3;\
                break;\
            case VK_F4:\
                key_out=Key_F4;\
                break;\
            case VK_F5:\
                key_out=Key_F5;\
                break;\
            case VK_F6:\
                key_out=Key_F6;\
                break;\
            case VK_F7:\
                key_out=Key_F7;\
                break;\
            case VK_F8:\
                key_out=Key_F8;\
                break;\
            case VK_F9:\
                key_out=Key_F9;\
                break;\
            case VK_F10:\
                key_out=Key_F10;\
                break;\
            case VK_F11:\
                key_out=Key_F11;\
                break;\
            case VK_F12:\
                key_out=Key_F12;\
                break;\
            case L'0':\
                key_out=Key_0;\
                break;\
            case L'1':\
                key_out=Key_1;\
                break;\
            case L'2':\
                key_out=Key_2;\
                break;\
            case L'3':\
                key_out=Key_3;\
                break;\
            case L'4':\
                key_out=Key_4;\
                break;\
            case L'5':\
                key_out=Key_5;\
                break;\
            case L'6':\
                key_out=Key_6;\
                break;\
            case L'7':\
                key_out=Key_7;\
                break;\
            case L'8':\
                key_out=Key_8;\
                break;\
            case L'9':\
                key_out=Key_9;\
                break;\
            case L'A':\
                key_out=Key_A;\
                break;\
            case L'B':\
                key_out=Key_B;\
                break;\
            case L'C':\
                key_out=Key_C;\
                break;\
            case L'D':\
                key_out=Key_D;\
                break;\
            case L'E':\
                key_out=Key_E;\
                break;\
            case L'F':\
                key_out=Key_F;\
                break;\
            case L'G':\
                key_out=Key_G;\
                break;\
            case L'H':\
                key_out=Key_H;\
                break;\
            case L'I':\
                key_out=Key_I;\
                break;\
            case L'J':\
                key_out=Key_J;\
                break;\
            case L'K':\
                key_out=Key_K;\
                break;\
            case L'L':\
                key_out=Key_L;\
                break;\
            case L'M':\
                key_out=Key_M;\
                break;\
            case L'N':\
                key_out=Key_N;\
                break;\
            case L'O':\
                key_out=Key_O;\
                break;\
            case L'P':\
                key_out=Key_P;\
                break;\
            case L'Q':\
                key_out=Key_Q;\
                break;\
            case L'R':\
                key_out=Key_R;\
                break;\
            case L'S':\
                key_out=Key_S;\
                break;\
            case L'T':\
                key_out=Key_T;\
                break;\
            case L'U':\
                key_out=Key_U;\
                break;\
            case L'V':\
                key_out=Key_V;\
                break;\
            case L'W':\
                key_out=Key_W;\
                break;\
            case L'X':\
                key_out=Key_X;\
                break;\
            case L'Y':\
                key_out=Key_Y;\
                break;\
            case L'Z':\
                key_out=Key_Z;\
                break;\
        }\
        \
        BYTE mskeys[256];\
        memset( mskeys, 0, sizeof(BYTE)*256 );\
        WCHAR unicode[4]= { 0, 0 };\
        if ( ToUnicode( (UINT)wParam, (UINT)((lParam >> 16) & 0xFF), mskeys, unicode, 4, 0 ) == 1 ) {\
            unicode_out=unicode[0];\
            \
            switch(unicode_out) {\
                case L'0':\
                    key_out=Key_0;\
                    break;\
                case L'1':\
                    key_out=Key_1;\
                    break;\
                case L'2':\
                    key_out=Key_2;\
                    break;\
                case L'3':\
                    key_out=Key_3;\
                    break;\
                case L'4':\
                    key_out=Key_4;\
                    break;\
                case L'5':\
                    key_out=Key_5;\
                    break;\
                case L'6':\
                    key_out=Key_6;\
                    break;\
                case L'7':\
                    key_out=Key_7;\
                    break;\
                case L'8':\
                    key_out=Key_8;\
                    break;\
                case L'9':\
                    key_out=Key_9;\
                    break;\
                case L'A':\
                    key_out=Key_A;\
                    break;\
                case L'B':\
                    key_out=Key_B;\
                    break;\
                case L'C':\
                    key_out=Key_C;\
                    break;\
                case L'D':\
                    key_out=Key_D;\
                    break;\
                case L'E':\
                    key_out=Key_E;\
                    break;\
                case L'F':\
                    key_out=Key_F;\
                    break;\
                case L'G':\
                    key_out=Key_G;\
                    break;\
                case L'H':\
                    key_out=Key_H;\
                    break;\
                case L'I':\
                    key_out=Key_I;\
                    break;\
                case L'J':\
                    key_out=Key_J;\
                    break;\
                case L'K':\
                    key_out=Key_K;\
                    break;\
                case L'L':\
                    key_out=Key_L;\
                    break;\
                case L'M':\
                    key_out=Key_M;\
                    break;\
                case L'N':\
                    key_out=Key_N;\
                    break;\
                case L'O':\
                    key_out=Key_O;\
                    break;\
                case L'P':\
                    key_out=Key_P;\
                    break;\
                case L'Q':\
                    key_out=Key_Q;\
                    break;\
                case L'R':\
                    key_out=Key_R;\
                    break;\
                case L'S':\
                    key_out=Key_S;\
                    break;\
                case L'T':\
                    key_out=Key_T;\
                    break;\
                case L'U':\
                    key_out=Key_U;\
                    break;\
                case L'V':\
                    key_out=Key_V;\
                    break;\
                case L'W':\
                    key_out=Key_W;\
                    break;\
                case L'X':\
                    key_out=Key_X;\
                    break;\
                case L'Y':\
                    key_out=Key_Y;\
                    break;\
                case L'Z':\
                    key_out=Key_Z;\
                    break;\
                case L'a':\
                    key_out=Key_A;\
                    break;\
                case L'b':\
                    key_out=Key_B;\
                    break;\
                case L'c':\
                    key_out=Key_C;\
                    break;\
                case L'd':\
                    key_out=Key_D;\
                    break;\
                case L'e':\
                    key_out=Key_E;\
                    break;\
                case L'f':\
                    key_out=Key_F;\
                    break;\
                case L'g':\
                    key_out=Key_G;\
                    break;\
                case L'h':\
                    key_out=Key_H;\
                    break;\
                case L'i':\
                    key_out=Key_I;\
                    break;\
                case L'j':\
                    key_out=Key_J;\
                    break;\
                case L'k':\
                    key_out=Key_K;\
                    break;\
                case L'l':\
                    key_out=Key_L;\
                    break;\
                case L'm':\
                    key_out=Key_M;\
                    break;\
                case L'n':\
                    key_out=Key_N;\
                    break;\
                case L'o':\
                    key_out=Key_O;\
                    break;\
                case L'p':\
                    key_out=Key_P;\
                    break;\
                case L'q':\
                    key_out=Key_Q;\
                    break;\
                case L'r':\
                    key_out=Key_R;\
                    break;\
                case L's':\
                    key_out=Key_S;\
                    break;\
                case L't':\
                    key_out=Key_T;\
                    break;\
                case L'u':\
                    key_out=Key_U;\
                    break;\
                case L'v':\
                    key_out=Key_V;\
                    break;\
                case L'w':\
                    key_out=Key_W;\
                    break;\
                case L'x':\
                    key_out=Key_X;\
                    break;\
                case L'y':\
                    key_out=Key_Y;\
                    break;\
                case L'z':\
                    key_out=Key_Z;\
                    break;\
                case 13:\
                    key_out=Key_Return;\
                    break;\
                case 8:\
                    key_out=Key_BackSpace;\
                    break;\
                case 9:\
                    key_out=Key_Tab;\
                    break;\
                case L' ':\
                    key_out=Key_Space;\
                    break;\
                case 27:\
                    key_out=Key_Escape;\
                    break;\
                case L'!':\
                    key_out=Key_Exclam;\
                    break;\
                case L'"':\
                    key_out=Key_QuoteDbl;\
                    break;\
                case L'#':\
                    key_out=Key_Hash;\
                    break;\
                case L'$':\
                    key_out=Key_Dollar;\
                    break;\
                case L'&':\
                    key_out=Key_Ampersand;\
                    break;\
                case L'\'':\
                    key_out=Key_Quote;\
                    break;\
                case L'(':\
                    key_out=Key_LeftParen;\
                    break;\
                case L')':\
                    key_out=Key_RightParen;\
                    break;\
                case L'*':\
                    key_out=Key_Asterisk;\
                    break;\
                case L'+':\
                    key_out=Key_Plus;\
                    break;\
                case L',':\
                    key_out=Key_Comma;\
                    break;\
                case L'-':\
                    key_out=Key_Minus;\
                    break;\
                case L'.':\
                    key_out=Key_Period;\
                    break;\
                case L'\\':\
                    key_out=Key_Slash;\
                    break;\
                case L':':\
                    key_out=Key_Colon;\
                    break;\
                case L';':\
                    key_out=Key_Semicolon;\
                    break;\
                case L'<':\
                    key_out=Key_Less;\
                    break;\
                case L'=':\
                    key_out=Key_Equal;\
                    break;\
                case L'>':\
                    key_out=Key_Greater;\
                    break;\
                case L'?':\
                    key_out=Key_Question;\
                    break;\
                case L'@':\
                    key_out=Key_At;\
                    break;\
                case L'[':\
                    key_out=Key_LeftBracket;\
                    break;\
                case L'/':\
                    key_out=Key_BackSlash;\
                    break;\
                case L']':\
                    key_out=Key_RightBracket;\
                    break;\
                case L'|':\
                    key_out=Key_Caret;\
                    break;\
                case L'_':\
                    key_out=Key_Underscore;\
                    break;\
                case L'`':\
                    key_out=Key_QuoteLeft;\
                    break;\
            }\
        }\
    }

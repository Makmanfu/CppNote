

#ifndef __FILET__
    #define __DUILIB_STR2WSTR(str)  L##str
    #define _DUILIB_STR2WSTR(str)   __DUILIB_STR2WSTR(str)
    #ifdef _UNICODE
        #define __FILET__   _DUILIB_STR2WSTR(__FILE__)
        #define __FUNCTIONT__   _DUILIB_STR2WSTR(__FUNCTION__)
    #else
        #define __FILET__   __FILE__
        #define __FUNCTIONT__   __FUNCTION__
    #endif
#endif

#define _CRT_SECURE_NO_DEPRECATE

// Remove pointless warning messages
#ifdef _MSC_VER
    #pragma warning (disable : 4511) // copy operator could not be generated
    #pragma warning (disable : 4512) // assignment operator could not be generated
    #pragma warning (disable : 4702) // unreachable code (bugs in Microsoft's STL)
    #pragma warning (disable : 4786) // identifier was truncated
    #pragma warning (disable : 4996) // function or variable may be unsafe (deprecated)
    #ifndef _CRT_SECURE_NO_WARNINGS
        #define _CRT_SECURE_NO_WARNINGS // eliminate deprecation warnings for VS2005
    #endif
#endif // _MSC_VER
#ifdef __BORLANDC__
    #pragma option -w-8027         // function not expanded inline
#endif

// Required for VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0500
#endif

#ifdef UILIB_STATIC
    #define DUILIB_API
#else
    #if defined(UILIB_EXPORTS)
        #define DUILIB_API __declspec(dllexport)
    #else
        #define DUILIB_API __declspec(dllimport)
    #endif
#endif

#define UILIB_COMDAT __declspec(selectany)

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stddef.h>
#include <richedit.h>
#include <tchar.h>
#include <assert.h>
#include <crtdbg.h>
#include <malloc.h>
#include <stdio.h>

#include "Utils.h"
#include "UIDelegate.h"
//Core-------------------------------------------------
#include "UIDefine.h"
#include "UIManager.h"
#include "UIBase.h"
#include "UIControl.h"
#include "UIContainer.h"
#include "UIMarkup.h"
#include "UIDlgBuilder.h"
#include "UIRender.h"
//-----------------------------------------------------
#include "WinImplBase.h"
#include "WndShadow.h"

//Layout
#include "UIVerticalLayout.h"
#include "UIHorizontalLayout.h"
#include "UITileLayout.h"
#include "UITabLayout.h"
#include "UIChildLayout.h"

//Control
#include "UIList.h"
#include "UICombo.h"
#include "UIScrollBar.h"
#include "UITreeView.h"

#include "UILabel.h"
#include "UIText.h"
#include "UIEdit.h"

#include "UIButton.h"
#include "UIOption.h"
#include "UICheckBox.h"

#include "UIProgress.h"
#include "UISlider.h"

#include "UIRichEdit.h"
#include "UIDateTime.h"

#include "UIActiveX.h"
#include "UIWebBrowser.h"
#include "UIGifAnim.h"
//#include "UIFlash.h"

#include <olectl.h>
#define lengthof(x) (sizeof(x)/sizeof(*x))
#define MAX max
#define MIN min
#define CLAMP(x,a,b) (MIN(b,MAX(a,x)))
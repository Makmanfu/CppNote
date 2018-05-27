//*************************************************************************
//
//                  MFC头文件支持
//
//
//
//*************************************************************************
#ifndef MFCDEF_H
#define MFCDEF_H

#ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN                            //从 Windows 头中排除极少使用的资料
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      //某些 CString 构造函数将是显式的
#define _AFX_ALL_WARNINGS                       //关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#include <afxwin.h>                             //MFC 核心组件和标准组件
#include <afxext.h>                             //MFC 扩展
#include <afxdisp.h>                            //MFC 自动化类
#ifndef _AFX_NO_OLE_SUPPORT
    #include <afxdtctl.h>                           //MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
    #include <afxcmn.h>                             //MFC 对 Windows 公共控件的支持
#endif
#if (_MSC_VER > 1400)
    #include <afxcontrolbars.h>                     //功能区和控件条的 MFC 支持
#endif

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif






#endif



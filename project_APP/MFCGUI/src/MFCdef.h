//*************************************************************************
//
//                  MFCͷ�ļ�֧��
//
//
//
//*************************************************************************
#ifndef MFCDEF_H
#define MFCDEF_H

#ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN                            //�� Windows ͷ���ų�����ʹ�õ�����
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      //ĳЩ CString ���캯��������ʽ��
#define _AFX_ALL_WARNINGS                       //�ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#include <afxwin.h>                             //MFC ��������ͱ�׼���
#include <afxext.h>                             //MFC ��չ
#include <afxdisp.h>                            //MFC �Զ�����
#ifndef _AFX_NO_OLE_SUPPORT
    #include <afxdtctl.h>                           //MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
    #include <afxcmn.h>                             //MFC �� Windows �����ؼ���֧��
#endif
#if (_MSC_VER > 1400)
    #include <afxcontrolbars.h>                     //�������Ϳؼ����� MFC ֧��
#endif

#ifdef _DEBUG
    #define new DEBUG_NEW
#endif






#endif



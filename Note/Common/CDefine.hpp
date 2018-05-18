//*********************************************************************
//
//                  ȫ���жϺ�����
//                                          @2017-03-21��������
//*********************************************************************
#ifndef CDEFINE_HPP
#define CDEFINE_HPP


//���������--------------------------------------------------------------
#ifndef C_API
#define C_API extern "C" __declspec(dllexport)
#endif
#ifndef CPP_API
#define CPP_API __declspec(dllexport)
#endif
#ifndef DLL_API
#define DLL_API CPP_API
#endif

//ƽ̨����ģʽ(Ĭ��������,GUIRUN ͼ�λ�,HIDECMDRUN ���ؽ���)--------------
#ifdef GUIRUN
    #pragma comment (linker, "/subsystem:windows")
#else
    #ifdef HIDECMDRUN
        #pragma comment (linker, "/subsystem:windows /entry:mainCRTStartup")    //����������
    #else
        #pragma comment (linker, "/subsystem:console")
    #endif
    #ifdef KILLMAIN
        #pragma comment (linker, "/ENTRY:\"SQMain\"")       //�޸�main���
    #endif
#endif

//���lib���Զ�����-------------------------------------------------------
#ifdef _WIN64
#define PLIB(name) name "64"
#else
#define PLIB(name) name
#endif
#ifdef _DEBUG
#define DLIB(name) name "d"
#else
#define DLIB(name) name
#endif
//#pragma comment( lib,DLIB(PLIB("XXX")))

//�ؼ����Ӿ�Ч�� Copy from MSDN-------------------------------------------
#if defined _M_IX86
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
//����win7���Ӿ�Ч�� Copy from MSDN---------------------------------------
#   pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' \
version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

//ָ���ڶ���Ϊ512�ֽ�
//#pragma comment(linker, "/align:512")
//�ϲ��ڽ�.data�ں�.rdata�ںϲ���.text��(�����)
//#pragma comment(linker, "/merge:.data=.text")
//#pragma comment(linker, "/merge:.rdata=.text")

#endif



//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@��ʼ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//�������жϷ���
//MS VC++ 14.1 _MSC_VER = 1912 (Visual C++ 2017)
//MS VC++ 14.0 _MSC_VER = 1900 (Visual C++ 2015)
//MS VC++ 12.0 _MSC_VER = 1800 (Visual C++ 2013)
//MS VC++ 11.0 _MSC_VER = 1700 (Visual C++ 2012)
//MS VC++ 10.0 _MSC_VER = 1600 (Visual C++ 2010)
//MS VC++ 9.0 _MSC_VER = 1500 (Visual C++ 2008)
//MS VC++ 8.0 _MSC_VER = 1400 (Visual C++ 2005)
//MS VC++ 7.1 _MSC_VER = 1310
//MS VC++ 7.0 _MSC_VER = 1300
//MS VC++ 6.0 _MSC_VER = 1200 (VC6)
//MS VC++ 5.0 _MSC_VER = 1100
//����
//#if _MSC_VER==1200        //vc6
//#else if _MSC_VER>=1600   //vc10����
//#endif

//��������
//#pragma warning(disable:4786)
//����ʹ��_s�ĺ���
//#define _CRT_SECURE_NO_WARNINGS
//���ӿ��ļ�
//#pragma comment( lib,"SQCore_X64.lib" )

//warning LNK4068: δָ�� /MACHINE��Ĭ������Ϊ X86
//��Ŀ����������ΪMachineX64�ͽ����


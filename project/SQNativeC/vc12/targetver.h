
#ifndef TARGETVER_H
#define TARGETVER_H
//����괦��--------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
//�����ļ�----------------------------------------------------------------
//#include <SDKDDKVer.h>

//ƽ̨����ģʽ(ע��xpģʽ��Ч ��Ȼ��Ҫ�ֶ�����)---------------------------
#ifdef GUIRUN
    #pragma comment (linker, "/subsystem:windows")
#else
    #pragma comment (linker, "/subsystem:console")
#endif

#ifdef _WIN64
    //�Ż�����: ָ���ڶ���Ϊ16�ֽ�
    #pragma comment(linker, "/align:16")
    //�Ż�����: �ϲ��� ��.data�ں�.rdata�ںϲ���.text��(�����)
    #pragma comment(linker, "/MERGE:.data=.text")
    #pragma comment(linker, "/MERGE:.rdata=.text")
#else
    //#pragma comment(linker, "/align:16")     //ע��32λĿǰû�н��
#endif



//---------����궨��-----------------------------------------------------
#ifndef C_API
    #define C_API extern "C" __declspec(dllexport)
#endif
#ifndef CPP_API
    #define CPP_API __declspec(dllexport)
#endif
#ifndef NATIVE_MATHEXP
    #define NATIVE_MATHEXP C_API
#endif

//����win7���Ӿ�Ч�� Copy from MSDN---------------------------------------
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' \
 version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#endif

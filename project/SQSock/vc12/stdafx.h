//*************************************************************************
//
//                   ע�Ȿ����exeģʽ�����в�������
//                                          @2018-06-26��������
//
//*************************************************************************

// ע�Ȿ����exeģʽ�����в�������
#ifndef STDAFX_H
#define STDAFX_H
//���뿪��----------------------------------------------------------------
#define CONSOLE                                 //���ѡ�� GUIRUNC/NATIVEC/CONSOLE/HIDECMD

//�����ļ�----------------------------------------------------------------
#include "CDefine.hpp"
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
//SOCKͨ��֧��------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS                 //����ʹ��_s�ĺ���
#define _WINSOCK_DEPRECATED_NO_WARNINGS         //����socket����
#include <iostream>
#include <WinSock2.h>                           //����˳��ɽ���ض�������
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")               //���ӿ�
#include <windowsx.h>
#include <memory>
#include "coondef.h"                            //ͨ�����ݽṹ��
//����궨��
#ifndef SQC_API
#define SQC_API C_API
#endif
#ifndef SQCPP_API
#define SQCPP_API CPP_API
#endif




#endif





#ifndef STDAFX_H_
#define STDAFX_H_
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ����SOCK����
#define _CRT_SECURE_NO_WARNINGS         // ����һЩ����
#define WIN32_LEAN_AND_MEAN             // ��Windowsͷ�ļ����ų�����ʹ�õ���Ϣ
#define SQC_API
#define SQCPP_API

#define GUIRUN
//#include "CDefine.hpp"
//Windows ͷ�ļ�:  socket����˳��
#include <WinSock2.h>
#include <Windows.h>
//C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
//#include <tchar.h>
#include <math.h>
//C++����ͷ�ļ�
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;
//QT֧��
#include "QPublic.h"

//���������ļ�
#include "Cinicfg.h"


#endif

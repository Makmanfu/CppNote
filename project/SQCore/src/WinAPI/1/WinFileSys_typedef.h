//-----------------------------------------------------------------------------
//
//              windows �ļ�ϵͳ���Ͷ����ͷ�ļ�
//
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <shlobj.h>
//#pragma comment(lib, "shell32.lib")
#include <iostream>
#include <string>
#include <vector>
using namespace std;

//�ļ�������ʽ[0�ݹ�ȫ��,1�ݹ��ļ���,2�ݹ��ļ�,3��ǰ�ļ���,4��ǰ�ļ�]
enum DIRTYPE { DALL = 0, DALLDIR, DALLFILE, DCURRDIR, DCURRFILE };

//�ļ�(��)������ָ��
typedef void(*FileProc)(string& pathfile);



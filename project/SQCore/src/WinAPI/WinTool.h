//-----------------------------------------------------------------------------
//
//                  windows ϵͳ�����ܽ�
//                  ע��� ���Ӳ����Ϣ ʱ���
//                                          @2014-03-10��������
//-----------------------------------------------------------------------------
#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

class SQCPP_API WinTool
{
    static WinTool* _instance;
    WinTool();
public:
    ~WinTool();
    static WinTool* Instance();
public:
    //����Ψһ�Լ��
    bool RunCheckOnly(char* f_tname = "app");
    //����������ע���
    void SetAutoRun(LPSTR RegName, bool fsetrun = true);
    //����ǩ��58�ֽ� +52�ֽ�[ver-verr 00-000]
    void SignFlagEXE(const char* exename, int ver = 0, int verr = 0);
};





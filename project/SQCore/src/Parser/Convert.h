//*************************************************************************
//
//                  ��������ת��
//
//
//
//*************************************************************************

#ifndef CONVERT_H
#define CONVERT_H

#include <string>
#include <sstream>
#include <iostream>
using namespace std;
#include <windows.h>

//---------------------�ڲ�����----------------------
SQC_API void TcharToChar(const TCHAR* tchar, char* _char);
SQC_API void CharToTchar(const char* _char, TCHAR* tchar);

//------------------------------------------------------------------------------------------------
//              char*������¼
//------------------------------------------------------------------------------------------------
//intתchar*
SQC_API char* InttoChar(int f_txt);
//char*תint
SQC_API int ChartoInt(const char* f_txt);
//const char*תchar*
SQC_API char* ConstCov(const char* f_txt);
//char*���
SQC_API char* CharAddChar(char* A, char* B);
//�ַ����Ƚ�
SQC_API int CompareChar(char* A, char* B);
//bug?? char* ת���� LPCTSTR
SQC_API LPCTSTR CharCovLpctstr(char* dst);
//bug?? LPCTSTR ת���� char *
SQC_API char* LpctstrCovChar(LPCTSTR dst);
//UTF-8��GB2312��ת��
SQC_API char* U2G(const char* utf8);
//GB2312��UTF-8��ת��
SQC_API char* G2U(const char* gb2312);
//wcharתchar
SQC_API char* WideChartoAnsi(wchar_t* pWideChar);
//�ַ������ȡ
SQC_API char* LeftChar(char* src, int f_n);
//���ַ���������м��ȡn���ַ�[nΪ���ȣ�mΪ���λ��]
SQC_API char* MidChar(char* src, int f_m, int f_n);
//�ַ����ҽ�ȡ
SQC_API char* RightChar(char* src, int f_n);

//------------------------------------------------------------------------------------------------
//              string������¼
//------------------------------------------------------------------------------------------------
//string�Ƚ���ͬ���ַ�ͷ
SQC_API int CompareStr(string new_str, string old_str);
//WCHAR ת�� string
SQC_API string WChar2Ansi(LPCWSTR pwszSrc);
//intתstring
SQC_API string AIntToStr(int f_txt);
//stringתint
SQC_API int AStrToInt(string f_txt);
//char*תstring
SQC_API string CharToStr(char* f_txt);
//stringתchar*
SQC_API char* StrToChar(string f_txt);
// ���ַ�תUTF8
SQC_API string EncodeUtf8(wstring in);
// UTF8 ת���ַ�
SQC_API wstring DecodeUtf8(string in);
//�ַ�����߽�ȡn���ַ�
SQC_API string leftstr(string f_src, int n);
//�ַ����м��ȡn���ַ�[nΪ���ȣ�mΪ���λ��]
SQC_API string midstr(string f_src, int n, int m);
//�ַ����ұ߽�ȡn���ַ�
SQC_API string rightstr(string f_src, int n);
//utf8תstd::string
SQC_API string UTF8_ToString(const string& f_str);
//std::stringתutf8�ַ���
SQC_API string StringTo_UTF8(const string& f_str);


class SQCPP_API CChineseCode
{
public:
    CChineseCode();
    ~CChineseCode();
public:
    static void UTF_8ToUnicode(wchar_t* pOut, char *pText);             //��UTF-8ת����Unicode
    static void UnicodeToUTF_8(char* pOut, wchar_t* pText);             //Unicode ת����UTF-8
    static void UnicodeToGB2312(char* pOut, wchar_t uData);             //��Unicode ת���� GB2312
    static void Gb2312ToUnicode(wchar_t* pOut, char *gbBuffer);         //��GB2312 ת���� Unicode
    static void GB2312ToUTF_8(string& pOut, char *pText, int pLen);     //GB2312 תΪ UTF-8
    static void UTF_8ToGB2312(string &pOut, char *pText, int pLen);     //UTF-8 תΪ GB2312
};



#endif
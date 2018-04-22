//*************************************************************************
//
//                  数据类型转换
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

//---------------------内部函数----------------------
SQC_API void TcharToChar(const TCHAR* tchar, char* _char);
SQC_API void CharToTchar(const char* _char, TCHAR* tchar);

//------------------------------------------------------------------------------------------------
//              char*操作记录
//------------------------------------------------------------------------------------------------
//int转char*
SQC_API char* InttoChar(int f_txt);
//char*转int
SQC_API int ChartoInt(const char* f_txt);
//const char*转char*
SQC_API char* ConstCov(const char* f_txt);
//char*相加
SQC_API char* CharAddChar(char* A, char* B);
//字符串比较
SQC_API int CompareChar(char* A, char* B);
//bug?? char* 转换成 LPCTSTR
SQC_API LPCTSTR CharCovLpctstr(char* dst);
//bug?? LPCTSTR 转换成 char *
SQC_API char* LpctstrCovChar(LPCTSTR dst);
//UTF-8到GB2312的转换
SQC_API char* U2G(const char* utf8);
//GB2312到UTF-8的转换
SQC_API char* G2U(const char* gb2312);
//wchar转char
SQC_API char* WideChartoAnsi(wchar_t* pWideChar);
//字符串左截取
SQC_API char* LeftChar(char* src, int f_n);
//从字符串数组的中间截取n个字符[n为长度，m为起点位置]
SQC_API char* MidChar(char* src, int f_m, int f_n);
//字符串右截取
SQC_API char* RightChar(char* src, int f_n);

//------------------------------------------------------------------------------------------------
//              string操作记录
//------------------------------------------------------------------------------------------------
//string比较相同的字符头
SQC_API int CompareStr(string new_str, string old_str);
//WCHAR 转成 string
SQC_API string WChar2Ansi(LPCWSTR pwszSrc);
//int转string
SQC_API string AIntToStr(int f_txt);
//string转int
SQC_API int AStrToInt(string f_txt);
//char*转string
SQC_API string CharToStr(char* f_txt);
//string转char*
SQC_API char* StrToChar(string f_txt);
// 宽字符转UTF8
SQC_API string EncodeUtf8(wstring in);
// UTF8 转宽字符
SQC_API wstring DecodeUtf8(string in);
//字符串左边截取n个字符
SQC_API string leftstr(string f_src, int n);
//字符串中间截取n个字符[n为长度，m为起点位置]
SQC_API string midstr(string f_src, int n, int m);
//字符串右边截取n个字符
SQC_API string rightstr(string f_src, int n);
//utf8转std::string
SQC_API string UTF8_ToString(const string& f_str);
//std::string转utf8字符串
SQC_API string StringTo_UTF8(const string& f_str);


class SQCPP_API CChineseCode
{
public:
    CChineseCode();
    ~CChineseCode();
public:
    static void UTF_8ToUnicode(wchar_t* pOut, char *pText);             //把UTF-8转换成Unicode
    static void UnicodeToUTF_8(char* pOut, wchar_t* pText);             //Unicode 转换成UTF-8
    static void UnicodeToGB2312(char* pOut, wchar_t uData);             //把Unicode 转换成 GB2312
    static void Gb2312ToUnicode(wchar_t* pOut, char *gbBuffer);         //把GB2312 转换成 Unicode
    static void GB2312ToUTF_8(string& pOut, char *pText, int pLen);     //GB2312 转为 UTF-8
    static void UTF_8ToGB2312(string &pOut, char *pText, int pLen);     //UTF-8 转为 GB2312
};



#endif
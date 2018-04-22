
#include "stdafx.h"
#include "Convert.h"
#include <fstream>
#include <sstream>
using namespace std;
#include <windows.h>


void TcharToChar(const TCHAR* tchar, char* _char)
{
    //��ȡ�ֽڳ���
    int iLength = WideCharToMultiByte(CP_ACP, 0, (LPCWCH)tchar, -1, NULL, 0, NULL, NULL);
    //��tcharֵ����_char
    WideCharToMultiByte(CP_ACP, 0, (LPCWCH)tchar, -1, _char, iLength, NULL, NULL);
}

void CharToTchar(const char* _char, TCHAR* tchar)
{
    int iLength = MultiByteToWideChar(CP_ACP, 0, _char, int(strlen(_char) + 1), NULL, 0);
    MultiByteToWideChar(CP_ACP, 0, _char, int(strlen(_char) + 1), (LPWSTR)tchar, iLength);
}


char* InttoChar(int f_txt)
{
    static char buf[255] = { 0 };
    sprintf(buf, "%d", f_txt);
    return buf;
}

int ChartoInt(const char* f_txt)
{
    return atoi(f_txt);
}

char* ConstCov(const char* f_txt)
{
    return const_cast<char*>(f_txt);
}

char* CharAddChar(char* A, char* B)
{
    char* tmpchar = new char[strlen(A) + strlen(B) + 1];       //�����
    strcpy(tmpchar, A);                 //��A�Ž�ȥ
    return strcat(tmpchar, B);          //����B
#if _MSC_VER == 1600 //string��Ҳ��
    string str1(A), str2(B);
    return ConstCov((str1 + str2).c_str());
#endif
}

int CompareChar(char* A, char* B)
{
    if (0 == strcmp(A, B))
        return 1;
    else
        return 0;
}

LPCTSTR CharCovLpctstr(char* dst)
{
    int num = MultiByteToWideChar(0, 0, dst, -1, NULL, 0);
    wchar_t* wide = new wchar_t[num];
    MultiByteToWideChar(0, 0, dst, -1, wide, num);
    return (LPCTSTR)wide;
    //bug
}

char* LpctstrCovChar(LPCTSTR dst)
{
    int num = WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)dst, -1, NULL, 0, NULL, FALSE);
    char* pchar = new char[num];
    WideCharToMultiByte(CP_OEMCP, NULL, (LPCWCH)dst, -1, (LPSTR)pchar, num, NULL, FALSE);
    return pchar;
    //bug
}

//UTF-8��GB2312��ת��
char* U2G(const char* utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

//GB2312��UTF-8��ת��
char* G2U(const char* gb2312)
{
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

char* WideChartoAnsi(wchar_t* pWideChar)
{
    if (NULL == pWideChar)
        return NULL;
    char* pAnsi = NULL;
    int needBytes = WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, NULL, 0, NULL, NULL);
    if (needBytes > 0)
    {
        pAnsi = new char[needBytes + 1];
        ZeroMemory(pAnsi, needBytes + 1);
        WideCharToMultiByte(CP_ACP, 0, pWideChar, -1, pAnsi, needBytes, NULL, NULL);
    }
    return pAnsi;
}

char* LeftChar(char* src, int f_n)
{
    return "0";
    //char *p = src;
    //char *q = dst;
    //int len = strlen(src);
    //if(n>len) n = len;
    //while(n--) *(q++) = *(p++);
    //*(q++)='\0'; /*�б�Ҫ�𣿺��б�Ҫ*/
    //return dst;
}

char* MidChar(char* src, int f_m, int f_n)
{
    return "0";
    //char *p = src;
    //char *q = dst;
    //int len = strlen(src);
    //if(n>len) n = len-m;    /*�ӵ�m�������*/
    //if(m<0) m=0;    /*�ӵ�һ����ʼ*/
    //if(m>len) return NULL;
    //p += m;
    //while(n--) *(q++) = *(p++);
    //*(q++)='\0'; /*�б�Ҫ�𣿺��б�Ҫ*/
    //return dst;
}

char* RightChar(char* src, int f_n)
{
    return &src[strlen(src) - f_n];
}


int CompareStr(string new_str, string old_str)
{
    //��new_str����old_str�ַ�ͷ
    //����new_str = "123456789"
    //old_str="1234" ��������
    if (new_str.size() >= old_str.size())
        if (new_str.substr(0, old_str.size()) == old_str)
            return 1;
    return 0;
}

string WChar2Ansi(LPCWSTR pwszSrc)
{
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen <= 0) return std::string("");
    char* pszDst = new char[nLen];
    if (NULL == pszDst) return std::string("");
    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    pszDst[nLen - 1] = 0;
    std::string strTemp(pszDst);
    delete[] pszDst;
    return strTemp;
}

string AIntToStr(int f_txt)
{
    //��׼�ⷨ
    stringstream tmp;
    tmp << f_txt;
    return tmp.str();       //string text = tmp.str();
    //sprintf����
    //char buf[255]={0};
    //sprintf(buf,"%d",f_txt);
    //return buf;
}

int AStrToInt(string f_txt)
{
    stringstream tmp;
    tmp << f_txt;
    int out;
    tmp >> out;
    return out;
}

string CharToStr(char* f_txt)
{
    //ʹ�ñ�׼��Ĺ��췽��ʵ��
    std::string pp(f_txt);
    return pp;
}

char* StrToChar(string f_txt)
{
    //return ConstCov(txt.c_str());
    //Ϊ��ͳһ�������������
    return const_cast<char*>(f_txt.c_str());
    //1.data(),����û�С�\0�����ַ�������
    //2.c_str()�������С�\0�����ַ�������
}

// �� �ַ�תUTF8
string EncodeUtf8(wstring in)
{
    string s(in.length() * 3 + 1, ' ');
    int len = ::WideCharToMultiByte(CP_UTF8, 0,
                                    in.c_str(), int (in.length()), &s[0], int (s.length()), NULL, NULL);
    s.resize(len);
    return s;
}

// UTF8 ת���ַ�
wstring DecodeUtf8(string in)
{
    //wstring s(in.length(), _T(' '));
    wstring s(in.length(), ' ');
    int len = ::MultiByteToWideChar(CP_UTF8, 0,
                                    in.c_str(), int (in.length()), &s[0], int (s.length()));
    s.resize(len);
    return s;
}

string leftstr(string f_src, int n)
{
    if (n < 0 || n > (int (f_src.size()) - 1))
        return "0";
    return string(f_src.begin(), f_src.begin() + n);
}

string midstr(string f_src, int n, int m)
{
    return "2";
}

string rightstr(string f_src, int n)
{
    if (n < 0 || n > (int (f_src.size()) - 1))
        return "0";
    return string(f_src.end() - n, f_src.end());
}

string UTF8_ToString(const string& f_str)
{
    //ת�����̣��Ƚ�utf8ת˫�ֽ�Unicode���룬��ͨ��WideCharToMultiByte�����ַ�ת��Ϊ���ֽڡ�
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, f_str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1]; //һ��Ҫ��1����Ȼ�����β��
    memset(pwBuf, 0, nwLen * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, f_str.c_str(), (int) f_str.length(), pwBuf, nwLen);
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    memset(pBuf, 0, nLen + 1);
    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    string retStr = pBuf;
    delete[]pBuf;
    delete[]pwBuf;
    pBuf = NULL;
    pwBuf = NULL;
    return retStr;
}

string StringTo_UTF8(const string& f_str)
{
    int nwLen = MultiByteToWideChar(CP_ACP, 0, f_str.c_str(), -1, NULL, 0);
    wchar_t* pwBuf = new wchar_t[nwLen + 1]; //һ��Ҫ��1����Ȼ�����β��
    ZeroMemory(pwBuf, nwLen * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, f_str.c_str(), (int) f_str.length(), pwBuf, nwLen);
    int nLen = WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
    char* pBuf = new char[nLen + 1];
    ZeroMemory(pBuf, nLen + 1);
    WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
    string retStr(pBuf);
    delete[]pwBuf;
    delete[]pBuf;
    pwBuf = NULL;
    pBuf = NULL;
    return retStr;
}






CChineseCode::CChineseCode()
{
}

CChineseCode::~CChineseCode()
{
}

void CChineseCode::UTF_8ToUnicode(wchar_t* pOut, char *pText)
{
    char* uchar = (char *)pOut;
    uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
    uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
    return;
}

void CChineseCode::UnicodeToUTF_8(char* pOut, wchar_t* pText)
{
    // ע�� WCHAR�ߵ��ֵ�˳��,���ֽ���ǰ�����ֽ��ں�    
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
    return;
}

void CChineseCode::UnicodeToGB2312(char* pOut, wchar_t uData)
{
    WideCharToMultiByte(CP_ACP, NULL, &uData, 1, pOut, sizeof(wchar_t), NULL, NULL);
    return;
}

void CChineseCode::Gb2312ToUnicode(wchar_t* pOut, char *gbBuffer)
{
    ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
    return;
}

void CChineseCode::GB2312ToUTF_8(string& pOut, char *pText, int pLen)
{
    char buf[4];
    int nLength = pLen * 3;
    char* rst = new char[nLength];
    memset(buf, 0, 4);
    memset(rst, 0, nLength);
    int i = 0;
    int j = 0;
    while (i < pLen)
    {
        //�����Ӣ��ֱ�Ӹ��ƾ���        
        if (*(pText + i) >= 0)
        {
            rst[j++] = pText[i++];
        }
        else
        {
            wchar_t pbuffer;
            Gb2312ToUnicode(&pbuffer, pText + i);
            UnicodeToUTF_8(buf, &pbuffer);
            unsigned short int tmp = 0;
            tmp = rst[j] = buf[0];
            tmp = rst[j + 1] = buf[1];
            tmp = rst[j + 2] = buf[2];
            j += 3;
            i += 2;
        }
    }
    rst[j] = ' ';
    pOut = rst;          //���ؽ��         
    delete[]rst;
    return;
}

void CChineseCode::UTF_8ToGB2312(string &pOut, char *pText, int pLen)
{
    char * newBuf = new char[pLen];
    char Ctemp[4];
    memset(Ctemp, 0, 4);
    int i = 0;
    int j = 0;
    while (i < pLen)
    {
        if (pText > 0)
        {
            newBuf[j++] = pText[i++];
        }
        else
        {
            WCHAR Wtemp;
            UTF_8ToUnicode(&Wtemp, pText + i);
            UnicodeToGB2312(Ctemp, Wtemp);
            newBuf[j] = Ctemp[0];
            newBuf[j + 1] = Ctemp[1];
            i += 3;
            j += 2;
        }
    }
    newBuf[j] = ' ';
    pOut = newBuf;
    delete[]newBuf;
    return;
}





















































































//16����ת10����(����A >> 10,ע��ֻ��תһ���ַ�)
int hexCharToInt(char c)
{
    if (c >= '0' && c <= '9') return (c - '0');
    if (c >= 'A' && c <= 'F') return (c - 'A' + 10);
    if (c >= 'a' && c <= 'f') return (c - 'a' + 10);
    return 0;
}

//�ַ�ת��Ϊ ansii�� (ע��ֱ����ʾ�ı� һ��������,��16���ƴ򿪿�)
char* hexstringToBytes(string s)
{
    size_t sz = s.length();
    char* ret = new char[sz / 2];
    for (size_t i = 0; i < sz; i += 2)
        ret[i / 2] = (char)((hexCharToInt(s.at(i)) << 4) | hexCharToInt(s.at(i + 1)));
    return ret;
}


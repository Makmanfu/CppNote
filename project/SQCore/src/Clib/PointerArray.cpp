
#include "stdafx.h"
#include "PointerArray.h"
#include <stddef.h>
#include <iostream>
#include <string.h>
using namespace std;

CPPSTUDY::CPPSTUDY()
{
}

CPPSTUDY::~CPPSTUDY()
{
}

void CPPSTUDY::ArrayInit(void)
{
    //����ĳ�ʼ�����μǲ���ֱ�Ӹ�ֵ
    //����һ
    char recvBuf[1024] = {0};
    //������
    memset(recvBuf, 0, 1024);
    //������
    for (int i = 0; i < 1024; i++)
        recvBuf[i] = 0;
}

void CPPSTUDY::Iplusplus(void)
{
    //�ھ��� ++��ǰ��+��=   ++�ں������=��+
    int a, b, c, d;
    a = b = 10;
    c = ++a;
    cout << "a(�ȼ���a=a+1;c=a) " << a << "  c(Ӧ����11)" << c << endl;
    d = b++;
    cout << "d(�ȼ���d=b;b=b+1) " << b << "  d(Ӧ����10)" << b << endl;
    //�ص� ����ʹ��++i ��Ϊ������ԭֵ���ǽ��
}

size_t CPPSTUDY::strlength(char const* str)
{
    int length;
    for (length = 0; *str++ != '\0';)
        length += 1;
    return length;
}

char* CPPSTUDY::strtstr(char const* s1, char const* s2)
{
    register char* last;
    register char* current;
    //��ָ���ʼ��Ϊ�����Ѿ��ҵ���ǰһ��ƥ��λ��
    last = NULL;
    //ֻ�ڵ�2���ַ�����Ϊ��ʱ�Ž��в���
    if (*s2 != '\0')
    {
        //����s2��s1�е�1�γ��ֵ�λ��
        current = const_cast<char*>(strstr(s1, s2));
        //����ÿ���ҵ��ַ���,��ָ��ָ��������ʼλ��,Ȼ����Ҹ��ַ�����һƥ��λ��
        while (current != NULL)
        {
            last = current;
            current = strstr(last + 1, s2);
        }
    }
    return last;
}

int CPPSTUDY::find_char1(char** strs, int value)
{
    //���ǵ�ǰ���ڲ��ҵ��ַ���
    char* str;
    //�����б��ÿ���ַ�������
    while ((str = *strs++) != NULL)
    {
        //�۲��ַ�����ÿ���ַ��Ƚ��ǲ��ǲ��ҵ�
        while (**strs != '\0')
        {
            if (*(*strs)++ == value)
                return 1;
        }
        strs++;
    }
    return 0;
}

int CPPSTUDY::find_char2(char** strs, int value)
{
    //assert(strings != NULL);
    //�����б��ÿ���ַ�������
    while (*strs != NULL)
    {
        //�۲��ַ�����ÿ���ַ��Ƚ��ǲ��ǲ��ҵ�
        while (**strs != '\0')
        {
            if (*(*strs)++ == value)
                return 1;
        }
        strs++;
    }
    return 0;
}

void CPPSTUDY::PrintfAllUser(void)
{
    //��intת��char*��ȥ
    char* buf = (char*)malloc(255 * sizeof(char));
    int num = 3;
    sprintf(buf, "The int print %d", num);
    cout << buf << endl;
    //��floatת��char*ȥ
    float num2 = 123.1;
    sprintf(buf, "The float print %06.2f", num2);
    cout << buf << endl;
    free(buf);
}

void CPPSTUDY::ArrayCopy(int* x, int* y, size_t SIZEN)
{
#define FUN_ArrayCopy5
#ifdef FUN_ArrayCopy1
    for (int i = 0; i < SIZEN; i++)
        x[i] = y[i];
#endif
#ifdef FUN_ArrayCopy2
    for (int* p1 = x, *p2 = y; p1 - x < SIZEN;)
        *p1++ = *p2++;
#endif
#ifdef FUN_ArrayCopy3
    for (int i = 0, *p1 = x, *p2 = y; i < SIZEN; i++)
        *p1++ = *p2++;
#endif
#ifdef FUN_ArrayCopy4
    register int* p1, *p2;
    register int i;
    for (i = 0, p1 = x, p2 = y; i < SIZEN; i++)
        *p1++ = *p2++;
#endif
#ifdef FUN_ArrayCopy5   //Ч�����
    register int* p1, *p2;
    for (p1 = x, p2 = y; p1 < &x[SIZEN];)
        *p1++ = *p2++;
#endif
}

long CPPSTUDY::factorial1(int n)
{
    return n <= 0 ? 1 : factorial1(n - 1) * n;
}

long CPPSTUDY::factorial2(int n)
{
    int result = 1;
    while (n > 1)
    {
        result *= n;
        n--;
    }
    return result;
}

long CPPSTUDY::fibonacci1(int n)
{
    return n < 2 ? 1 : fibonacci1(n - 1) + fibonacci1(n - 2);
}

long CPPSTUDY::fibonacci2(int n)
{
    long result, previous_result, next_older_result;
    result = previous_result = 1;
    while (n > 2)
    {
        n -= 1;
        next_older_result = previous_result;
        previous_result = result;
        result = previous_result + next_older_result;
    }
    return result;
}

float CPPSTUDY::average(int n_values, ...)
{
    va_list var_arg;
    int count;
    float sum = 0;
    //׼�����ʿɱ����
    va_start(var_arg, n_values);
    //���ȡ�Կɱ��������ֵ
    for (count = 0; count < n_values; count += 1)
        sum += va_arg(var_arg, int);
    //��ɴ���ɱ����
    va_end(var_arg);
    return sum / n_values;
}


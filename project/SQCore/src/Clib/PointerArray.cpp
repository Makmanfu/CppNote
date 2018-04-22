
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
    //数组的初始化请牢记不能直接赋值
    //方法一
    char recvBuf[1024] = {0};
    //方法二
    memset(recvBuf, 0, 1024);
    //方法三
    for (int i = 0; i < 1024; i++)
        recvBuf[i] = 0;
}

void CPPSTUDY::Iplusplus(void)
{
    //口诀是 ++在前先+后=   ++在后就是先=后+
    int a, b, c, d;
    a = b = 10;
    c = ++a;
    cout << "a(等价于a=a+1;c=a) " << a << "  c(应该是11)" << c << endl;
    d = b++;
    cout << "d(等价于d=b;b=b+1) " << b << "  d(应该是10)" << b << endl;
    //特点 优先使用++i 因为拷贝和原值都是结果
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
    //把指针初始化为我们已经找到的前一次匹配位置
    last = NULL;
    //只在第2个字符串不为空时才进行查找
    if (*s2 != '\0')
    {
        //查找s2在s1中第1次出现的位置
        current = const_cast<char*>(strstr(s1, s2));
        //我们每次找到字符串,让指针指向它的起始位置,然后查找该字符串下一匹配位置
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
    //我们当前正在查找的字符串
    char* str;
    //对于列表的每个字符串处理
    while ((str = *strs++) != NULL)
    {
        //观察字符串的每个字符比较是不是查找的
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
    //对于列表的每个字符串处理
    while (*strs != NULL)
    {
        //观察字符串的每个字符比较是不是查找的
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
    //将int转到char*里去
    char* buf = (char*)malloc(255 * sizeof(char));
    int num = 3;
    sprintf(buf, "The int print %d", num);
    cout << buf << endl;
    //将float转到char*去
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
#ifdef FUN_ArrayCopy5   //效率最高
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
    //准备访问可变参数
    va_start(var_arg, n_values);
    //添加取自可变参数李彪的值
    for (count = 0; count < n_values; count += 1)
        sum += va_arg(var_arg, int);
    //完成处理可变参数
    va_end(var_arg);
    return sum / n_values;
}


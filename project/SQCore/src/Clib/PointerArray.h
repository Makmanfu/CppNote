//*************************************************************************
//
//                      指针和数组
//
//
//
//*************************************************************************
#pragma once
#include <stdarg.h>         //可变参数支持

class CPPSTUDY
{
public:
    CPPSTUDY();
    ~CPPSTUDY();
private:
    //数组的初始化
    void ArrayInit(void);
    //指针中 ++i和 i++
    void Iplusplus(void);
    //计算字符串长度
    size_t strlength(char const* str);
    //s1中找s2最右出现的位置返回指向的指针
    char* strtstr(char const* s1, char const* s2);
    //字符串组中查找
    int find_char1(char** strs, int value);
    int find_char2(char** strs, int value);
    //printf常用方法
    void PrintfAllUser(void);
    //拷贝数组中指针的优化
    void ArrayCopy(int* x, int* y, size_t SIZEN);

private:    //递归的讲解
    //递归计算阶乘
    long factorial1(int n);
    //迭代计算阶乘
    long factorial2(int n);
    //递归计算斐波那契数列
    long fibonacci1(int n);
    //迭代计算斐波那契数列
    long fibonacci2(int n);
private:
    //可变参数的使用
    float average(int n_values, ...);
};



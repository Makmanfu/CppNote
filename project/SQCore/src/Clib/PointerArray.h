//*************************************************************************
//
//                      ָ�������
//
//
//
//*************************************************************************
#pragma once
#include <stdarg.h>         //�ɱ����֧��

class CPPSTUDY
{
public:
    CPPSTUDY();
    ~CPPSTUDY();
private:
    //����ĳ�ʼ��
    void ArrayInit(void);
    //ָ���� ++i�� i++
    void Iplusplus(void);
    //�����ַ�������
    size_t strlength(char const* str);
    //s1����s2���ҳ��ֵ�λ�÷���ָ���ָ��
    char* strtstr(char const* s1, char const* s2);
    //�ַ������в���
    int find_char1(char** strs, int value);
    int find_char2(char** strs, int value);
    //printf���÷���
    void PrintfAllUser(void);
    //����������ָ����Ż�
    void ArrayCopy(int* x, int* y, size_t SIZEN);

private:    //�ݹ�Ľ���
    //�ݹ����׳�
    long factorial1(int n);
    //��������׳�
    long factorial2(int n);
    //�ݹ����쳲���������
    long fibonacci1(int n);
    //��������쳲���������
    long fibonacci2(int n);
private:
    //�ɱ������ʹ��
    float average(int n_values, ...);
};



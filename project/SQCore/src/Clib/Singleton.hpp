//-----------------------------------------------------------------------------
//
//                  Singleton���ģʽ����
//                                          @2017-02-14��������
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

class Singleton
{
private:
    //��������Ψһ�Ķ�����
    static Singleton* _instance;
protected:
    //ע�ⵥ����Ҫ��ʵ����
    Singleton()
    {
        printf("Singleton\n");
    };
public:
    ~Singleton() {};
    static Singleton* Instance()
    {
        if (NULL == _instance)
            _instance = new Singleton();
        return _instance;
    };
};

Singleton* Singleton::_instance = 0;

int Mode_Singleton(int argc, char** argv)
{
    //������������
    Singleton* sgn = Singleton::Instance();
    return 1;
}




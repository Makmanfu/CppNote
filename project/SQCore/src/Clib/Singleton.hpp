//-----------------------------------------------------------------------------
//
//                  Singleton设计模式例子
//                                          @2017-02-14阿甘整理
//-----------------------------------------------------------------------------
#pragma once
#include "stdafx.h"

class Singleton
{
private:
    //这样就有唯一的对象了
    static Singleton* _instance;
protected:
    //注意单键不要能实例化
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
    //单例创建过程
    Singleton* sgn = Singleton::Instance();
    return 1;
}




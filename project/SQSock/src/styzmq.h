//*************************************************************************
//
//              ZMQ
//                                              @2018-07-29阿甘
//
//*************************************************************************
#ifndef STYZMQ_H
#define STYZMQ_H

#include <zmq.h>
#include <zmq.hpp>
#include <stdio.h>
#include <string.h>
#include <zhelpers.h>

//#define ZMQ_STATIC

struct StyZMQ
{
    //zmq服务端C版例子
    static int zmq_hwserverc(int argc, char** argv);
    //zmq服务端Cpp版例子
    static int zmq_hwservercpp(int argc, char** argv);
    //zmq客户端例子
    static int zmq_hwclient(int argc, char** argv);
    //publisher 发布
    static int zmq_wuserverPub(int argc, char** argv);
    //Subscriber 订阅
    static int zmq_wuclientSub(int argc, char** argv);



};









#endif
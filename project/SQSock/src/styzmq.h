//*************************************************************************
//
//              ZMQ
//                                              @2018-07-29����
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
    //zmq�����C������
    static int zmq_hwserverc(int argc, char** argv);
    //zmq�����Cpp������
    static int zmq_hwservercpp(int argc, char** argv);
    //zmq�ͻ�������
    static int zmq_hwclient(int argc, char** argv);
    //publisher ����
    static int zmq_wuserverPub(int argc, char** argv);
    //Subscriber ����
    static int zmq_wuclientSub(int argc, char** argv);



};









#endif
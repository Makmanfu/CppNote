//-----------------------------------------------------------------------------
//
//      udp������
//
//-----------------------------------------------------------------------------

#ifndef UDP_SYNC_ECHO_HPP
#define  UDP_SYNC_ECHO_HPP

#include "stdafx.h"
#include "BoostConfig.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/lexical_cast.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
io_service udp_sync_service;

//�󶨵�ַ�Ͷ˿�
ip::udp::endpoint udp_sync_ep(ip::address::from_string("127.0.0.1"), 2001);

void udp_sync_echo(std::string msg)
{
    ip::udp::socket sock(udp_sync_service, ip::udp::endpoint(ip::udp::v4(), 0));
    //��������
    sock.send_to(buffer(msg), udp_sync_ep);
    char buff[1024];
    ip::udp::endpoint sender_ep;
    int bytes(0);
    //�ȴ�����˷���
    try
    {
        bytes = sock.receive_from(buffer(buff), sender_ep);
    }
    catch (...)
    {
        std::cerr << "receive_from error" << endl;
    }
    std::string copy(buff, bytes);
    std::cout << "server echoed our " << msg << ": "
              << (copy == msg ? "OK" : "FAIL") << std::endl;
    sock.close();
}

//udp�ͻ���
int udp_sync_echo_client(void)
{
    // connect several clients
    char* messages[] = { "John says hi", "so does James", "Lucy just got home", 0 };
    boost::thread_group threads;
    //Ϊ�˱�֤����������ʾ ѭ��30Ƭ
    for (int i = 0; i < 30; i++)
        for (char** message = messages; *message; ++message)
        {
            //�̷߳�������
            threads.create_thread(boost::bind(udp_sync_echo, *message));
            boost::this_thread::sleep(boost::posix_time::millisec(100));
        }
    threads.join_all();
    return 1;
}

//udp������
int udp_sync_echo_server(void)
{
    char buff[1024];
    ip::udp::socket sock(udp_sync_service, ip::udp::endpoint(ip::udp::v4(), 2001));
    while (true)
    {
        ip::udp::endpoint sender_ep;
        //������������
        int bytes = sock.receive_from(buffer(buff), sender_ep);
        std::string msg(buff, bytes);
        std::cout << sender_ep.address() << ":" << ""
                  << ":" << msg.c_str() << std::endl;
        //�ظ�����
        sock.send_to(buffer(msg), sender_ep);
    }
    return 1;
}




#endif
//-----------------------------------------------------------------------------
//
//      TCP��ͬ��������
//
//-----------------------------------------------------------------------------

#ifndef TCP_SYNC_ECHO_HPP
#define TCP_SYNC_ECHO_HPP

#include "stdafx.h"
#include "BoostConfig.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
using boost::system::error_code;

io_service tcp_sync_service;

size_t read_complete(char* buff, const boost::system::error_code& err, size_t bytes)
{
    if (err)
        return 0;
    bool found = std::find(buff, buff + bytes, '\n') < buff + bytes;
    // we read one-by-one until we get to enter, no buffering
    return found ? 0 : 1;
}

//ͬ�������demo
int tcp_sync_echo_server()
{
    //handle_connections
    ip::tcp::acceptor acceptor(tcp_sync_service, ip::tcp::endpoint(ip::tcp::v4(), 2001));
    char buff[1024];
    cout << "server start!" << endl;
    while (true)
    {
        //����sock
        ip::tcp::socket sock(tcp_sync_service);
        //����sock ����ֱ���ͻ������� ******************
        acceptor.accept(sock);
        //��ȡ��������
        int bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));
        std::string msg(buff, bytes);
        std::cout << msg << std::endl;
        //��������
        sock.write_some(buffer(msg));
        sock.close();
    }
    return 1;
}

void tcp_sync_echo(std::string msg)
{
    static ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    msg += "\n";
    //����sock
    ip::tcp::socket sock(tcp_sync_service);
    //����ip����
    try
    {
        sock.connect(ep);
        cout << "net ok" << endl;
    }
    catch (...)
    {
        cerr << "not net" << endl;
        sock.close();
        return ;
    }
    //��������
    sock.write_some(buffer(msg));
    cout << "send:" << msg << endl;
    char buf[1024];
    //��ȡ��������
    int bytes = read(sock, buffer(buf), boost::bind(read_complete, buf, _1, _2));
    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);
    std::cout << "server echoed our " << msg << ": "
              << (copy == msg ? "OK" : "FAIL") << std::endl;
    sock.close();
    return ;
}

//ͬ���ͻ���demo
int tcp_sync_echo_client()
{
    // connect several clients
    char* messages[] = { "John says hi", "so does James",
                         "Lucy just got home", "Boost.Asio is Fun!", 0
                       };
    boost::thread_group threads;
    for (char** message = messages; *message; ++message)
    {
        //�̰߳󶨷�������
        threads.create_thread(boost::bind(tcp_sync_echo, *message));
        boost::this_thread::sleep(boost::posix_time::millisec(1000));
    }
    threads.join_all();
    return 1;
}




#endif
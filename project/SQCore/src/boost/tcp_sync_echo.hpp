//-----------------------------------------------------------------------------
//
//      TCP简单同步服务器
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

//同步服务端demo
int tcp_sync_echo_server()
{
    //handle_connections
    ip::tcp::acceptor acceptor(tcp_sync_service, ip::tcp::endpoint(ip::tcp::v4(), 2001));
    char buff[1024];
    cout << "server start!" << endl;
    while (true)
    {
        //创建sock
        ip::tcp::socket sock(tcp_sync_service);
        //关联sock 阻塞直到客户端请求 ******************
        acceptor.accept(sock);
        //读取连接数据
        int bytes = read(sock, buffer(buff), boost::bind(read_complete, buff, _1, _2));
        std::string msg(buff, bytes);
        std::cout << msg << std::endl;
        //返回数据
        sock.write_some(buffer(msg));
        sock.close();
    }
    return 1;
}

void tcp_sync_echo(std::string msg)
{
    static ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    msg += "\n";
    //创建sock
    ip::tcp::socket sock(tcp_sync_service);
    //连接ip访问
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
    //发送数据
    sock.write_some(buffer(msg));
    cout << "send:" << msg << endl;
    char buf[1024];
    //读取返回数据
    int bytes = read(sock, buffer(buf), boost::bind(read_complete, buf, _1, _2));
    std::string copy(buf, bytes - 1);
    msg = msg.substr(0, msg.size() - 1);
    std::cout << "server echoed our " << msg << ": "
              << (copy == msg ? "OK" : "FAIL") << std::endl;
    sock.close();
    return ;
}

//同步客户端demo
int tcp_sync_echo_client()
{
    // connect several clients
    char* messages[] = { "John says hi", "so does James",
                         "Lucy just got home", "Boost.Asio is Fun!", 0
                       };
    boost::thread_group threads;
    for (char** message = messages; *message; ++message)
    {
        //线程绑定发送数据
        threads.create_thread(boost::bind(tcp_sync_echo, *message));
        boost::this_thread::sleep(boost::posix_time::millisec(1000));
    }
    threads.join_all();
    return 1;
}




#endif
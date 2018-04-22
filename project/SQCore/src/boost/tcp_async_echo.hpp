//-----------------------------------------------------------------------------
//
//      TCP简单异步服务器
//
//-----------------------------------------------------------------------------

#ifndef TCP_ASYNC_ECHO_HPP
#define TCP_ASYNC_ECHO_HPP

#include "stdafx.h"
#include "BoostConfig.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;

io_service tcp_async_service;

#define MEM_FN(x)       boost::bind(&self_echo_type::x, shared_from_this())
#define MEM_FN1(x,y)    boost::bind(&self_echo_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z)  boost::bind(&self_echo_type::x, shared_from_this(),y,z)

//异步服务端
class talk_echo_svr : public boost::enable_shared_from_this<talk_echo_svr>, boost::noncopyable
{
    typedef talk_echo_svr self_echo_type;
    talk_echo_svr() : sock_(tcp_async_service), started_(false) {}
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_echo_svr> ptr;

    static ptr new_()
    {
        ptr new_(new talk_echo_svr);
        return new_;
    }
    void start()
    {
        started_ = true;
        do_read();
    }
    void stop()
    {
        if (!started_) return;
        started_ = false;
        sock_.close();
    }
    ip::tcp::socket& sock()
    {
        return sock_;
    }
private:
    void on_read(const error_code& err, size_t bytes)
    {
        if (!err)
        {
            std::string msg(read_buffer_, bytes);
            // echo message back, and then stop
            do_write(msg + "\n");
        }
        stop();
    }
    void on_write(const error_code& err, size_t bytes)
    {
        //do_read();
    }
    void do_read()
    {
        std::cout << "do_read" << std::endl;
        //服务器接收数据********************
        async_read(sock_, buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
    }
    //服务器回复数据
    void do_write(const std::string& msg)
    {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, msg.size()), MEM_FN2(on_write, _1, _2));
    }
    //接收数据处理回调
    size_t read_complete(const boost::system::error_code& err, size_t bytes)
    {
        if (err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
private:
    ip::tcp::socket sock_;              //socket对象
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
};

//异步客户端
class talk_echo_client : public boost::enable_shared_from_this<talk_echo_client>, boost::noncopyable
{
    typedef talk_echo_client self_echo_type;
    talk_echo_client(const std::string& message) : sock_(tcp_async_service), started_(true), message_(message) {}
    void start(ip::tcp::endpoint ep)
    {
        //非阻塞的异步连接
        sock_.async_connect(ep, MEM_FN1(on_connect, _1));
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_echo_client> ptr;

    static ptr start(ip::tcp::endpoint ep, const std::string& message)
    {
        ptr new_(new talk_echo_client(message));
        new_->start(ep);
        return new_;
    }
    void stop()
    {
        if (!started_) return;
        started_ = false;
        sock_.close();
    }
    bool started()
    {
        return started_;
    }
private:
    //连接回调
    void on_connect(const error_code& err)
    {
        if (!err)
            do_write(message_ + "\n");
        else
            stop();
    }
    void on_read(const error_code& err, size_t bytes)
    {
        if (!err)
        {
            std::string copy(read_buffer_, bytes - 1);
            std::cout << "server echoed our " << message_ << ": "
                      << (copy == message_ ? "OK" : "FAIL") << std::endl;
        }
        stop();
    }
    void on_write(const error_code& err, size_t bytes)
    {
        do_read();
    }
    void do_read()
    {
        async_read(sock_, buffer(read_buffer_),
                   MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
    }
    //发送服务端数据**********
    void do_write(const std::string& msg)
    {
        if (!started()) return;
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, msg.size()),
                               MEM_FN2(on_write, _1, _2));
    }
    size_t read_complete(const boost::system::error_code& err, size_t bytes)
    {
        if (err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string message_;
};

ip::tcp::acceptor acceptor(tcp_async_service, ip::tcp::endpoint(ip::tcp::v4(), 2001));

void echo_handle_accept(talk_echo_svr::ptr server, const boost::system::error_code& err)
{
    server->start();
    talk_echo_svr::ptr new_client = talk_echo_svr::new_();
    acceptor.async_accept(new_client->sock(), boost::bind(echo_handle_accept, new_client, _1));
}

//启动服务器
int tcp_async_echo_server(void)
{
    //创建异步服务器对象
    talk_echo_svr::ptr server = talk_echo_svr::new_();
    //异步绑定sock
    acceptor.async_accept(server->sock(), boost::bind(echo_handle_accept, server, _1));
    tcp_async_service.run();
    return 1;
}

//启动客户端
int tcp_async_echo_client(void)
{
    // connect several clients
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    char* messages[] = { "John says hi", "so does James", "Lucy just got home", 0 };
    //为了保证发送数据显示 循环30片
    for (int i = 0; i < 30; i++)
        for (char** message = messages; *message; ++message)
        {
            //发送一次数据
            talk_echo_client::start(ep, *message);
            boost::this_thread::sleep(boost::posix_time::millisec(100));
        }
    tcp_async_service.run();
    return 1;
}




#endif
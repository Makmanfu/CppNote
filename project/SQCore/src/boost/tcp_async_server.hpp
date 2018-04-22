//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------

#ifndef TCP_ASYNC_SERVER_HPP
#define TCP_ASYNC_SERVER_HPP

#include "stdafx.h"
#include "BoostConfig.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
io_service async_svr_service;

class svr_talk_server;
typedef boost::shared_ptr<svr_talk_server> client_ptr;
typedef std::vector<client_ptr> svrarray;
svrarray clients;

#define SVR_MEM_FN(x)       boost::bind(&self_server_type::x, shared_from_this())
#define SVR_MEM_FN1(x,y)    boost::bind(&self_server_type::x, shared_from_this(),y)
#define SVR_MEM_FN2(x,y,z)  boost::bind(&self_server_type::x, shared_from_this(),y,z)


void async_svr_update_clients_changed();

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible client requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
class svr_talk_server : public boost::enable_shared_from_this<svr_talk_server>, boost::noncopyable
{
    typedef svr_talk_server self_server_type;
    svr_talk_server() : sock_(async_svr_service), started_(false), timer_(async_svr_service), clients_changed_(false) {}
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<svr_talk_server> ptr;

    void start()
    {
        started_ = true;
        clients.push_back(shared_from_this());
        last_ping = boost::posix_time::microsec_clock::local_time();
        // first, we wait for client to login
        do_read();
    }
    static ptr new_()
    {
        ptr new_(new svr_talk_server);
        return new_;
    }
    void stop()
    {
        if (!started_) return;
        started_ = false;
        sock_.close();
        ptr self = shared_from_this();
        svrarray::iterator it = std::find(clients.begin(), clients.end(), self);
        clients.erase(it);
        async_svr_update_clients_changed();
    }
    bool started() const
    {
        return started_;
    }
    ip::tcp::socket& sock()
    {
        return sock_;
    }
    std::string username() const
    {
        return username_;
    }
    void set_clients_changed()
    {
        clients_changed_ = true;
    }
private:
    void on_read(const error_code& err, size_t bytes)
    {
        if (err) stop();
        if (!started()) return;
        // process the msg
        std::string msg(read_buffer_, bytes);
        if (msg.find("login ") == 0) on_login(msg);
        else if (msg.find("ping") == 0) on_ping();
        else if (msg.find("ask_clients") == 0) on_clients();
        else std::cerr << "invalid msg " << msg << std::endl;
    }

    void on_login(const std::string& msg)
    {
        std::istringstream in(msg);
        in >> username_ >> username_;
        std::cout << username_ << " logged in" << std::endl;
        do_write("login ok\n");
        async_svr_update_clients_changed();
    }

    void on_ping()
    {
        do_write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
        clients_changed_ = false;
    }

    void on_clients()
    {
        std::string msg;
        for (svrarray::const_iterator b = clients.begin(), e = clients.end() ; b != e; ++b)
            msg += (*b)->username() + " ";
        do_write("clients " + msg + "\n");
    }

    void do_ping()
    {
        do_write("ping\n");
    }
    void do_ask_clients()
    {
        do_write("ask_clients\n");
    }

    void on_check_ping()
    {
        boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
        if ((now - last_ping).total_milliseconds() > 5000)
        {
            std::cout << "stopping " << username_ << " - no ping in time" << std::endl;
            stop();
        }
        last_ping = boost::posix_time::microsec_clock::local_time();
    }

    void post_check_ping()
    {
        timer_.expires_from_now(boost::posix_time::millisec(5000));
        timer_.async_wait(SVR_MEM_FN(on_check_ping));
    }

    void on_write(const error_code& err, size_t bytes)
    {
        do_read();
    }

    void do_read()
    {
        async_read(sock_, buffer(read_buffer_),
                   SVR_MEM_FN2(read_complete, _1, _2), SVR_MEM_FN2(on_read, _1, _2));
        post_check_ping();
    }
    void do_write(const std::string& msg)
    {
        if (!started()) return;
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_, msg.size()),
                               SVR_MEM_FN2(on_write, _1, _2));
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
    std::string username_;
    deadline_timer timer_;
    boost::posix_time::ptime last_ping;
    bool clients_changed_;
};

void async_svr_update_clients_changed()
{
    for (svrarray::iterator b = clients.begin(),
         e = clients.end(); b != e; ++b)
        (*b)->set_clients_changed();
}

ip::tcp::acceptor async_svr_acceptor(async_svr_service, ip::tcp::endpoint(ip::tcp::v4(), 8001));

void svr_handle_accept(svr_talk_server::ptr client, const boost::system::error_code& err)
{
    client->start();
    svr_talk_server::ptr new_client = svr_talk_server::new_();
    async_svr_acceptor.async_accept(new_client->sock(), boost::bind(svr_handle_accept, new_client, _1));
}

//-------------------------------------------------------------------------------

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
class svr_talk_client : public boost::enable_shared_from_this<svr_talk_client>, boost::noncopyable
{
    typedef svr_talk_client self_server_type;
    svr_talk_client(const std::string& username) : sock_(async_svr_service),
        started_(true), username_(username), timer_(async_svr_service) {}
    void start(ip::tcp::endpoint ep)
    {
        sock_.async_connect(ep, SVR_MEM_FN1(on_connect, _1));
    }
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<svr_talk_client> ptr;

    static ptr start(ip::tcp::endpoint ep, const std::string& username)
    {
        ptr new_(new svr_talk_client(username));
        new_->start(ep);
        return new_;
    }
    void stop()
    {
        if (!started_) return;
        std::cout << "stopping " << username_ << std::endl;
        started_ = false;
        sock_.close();
    }
    bool started()
    {
        return started_;
    }
private:
    void on_connect(const error_code& err)
    {
        if (!err)
            do_write("login " + username_ + "\n");
        else
            stop();
    }

    void on_read(const error_code& err, size_t bytes)
    {
        if (err) stop();
        if (!started()) return;
        // process the msg
        std::string msg(read_buffer_, bytes);
        if (msg.find("login ") == 0) on_login();
        else if (msg.find("ping") == 0) on_ping(msg);
        else if (msg.find("clients ") == 0) on_clients(msg);
        else std::cerr << "invalid msg " << msg << std::endl;
    }

    void on_login()
    {
        std::cout << username_ << " logged in" << std::endl;
        do_ask_clients();
    }

    void on_ping(const std::string& msg)
    {
        std::istringstream in(msg);
        std::string answer;
        in >> answer >> answer;
        if (answer == "client_list_changed") do_ask_clients();
        else postpone_ping();
    }

    void on_clients(const std::string& msg)
    {
        std::string clients = msg.substr(8);
        std::cout << username_ << ", new client list:" << clients ;
        postpone_ping();
    }

    void do_ping()
    {
        do_write("ping\n");
    }

    void postpone_ping()
    {
        // note: even though the server wants a ping every 5 secs, we randomly
        // don't ping that fast - so that the server will randomly disconnect us
        int millis = rand() % 7000;
        std::cout << username_ << " postponing ping " << millis
                  << " millis" << std::endl;
        timer_.expires_from_now(boost::posix_time::millisec(millis));
        timer_.async_wait(SVR_MEM_FN(do_ping));
    }

    void do_ask_clients()
    {
        do_write("ask_clients\n");
    }

    void on_write(const error_code& err, size_t bytes)
    {
        do_read();
    }

    void do_read()
    {
        async_read(sock_, buffer(read_buffer_),
                   SVR_MEM_FN2(read_complete, _1, _2), SVR_MEM_FN2(on_read, _1, _2));
    }
    void do_write(const std::string& msg)
    {
        if (!started()) return;
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some(buffer(write_buffer_,
                                      msg.size()), SVR_MEM_FN2(on_write, _1, _2));
    }
    size_t read_complete(const boost::system::error_code& err, size_t bytes)
    {
        if (err) return 0;
        bool found = std::find(read_buffer_,
                               read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
    std::string username_;
    deadline_timer timer_;
};

int async_client(void)
{
    // connect several clients
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 8001);
    char* names[] = { "John", "James", "Lucy", "Tracy", "Frank", "Abby", 0 };
    for (char** name = names; *name; ++name)
    {
        svr_talk_client::start(ep, *name);
        boost::this_thread::sleep(boost::posix_time::millisec(100));
    }
    async_svr_service.run();
    return 1;
}

int async_server(void)
{
    svr_talk_server::ptr client = svr_talk_server::new_();
    async_svr_acceptor.async_accept(client->sock(), boost::bind(svr_handle_accept, client, _1));
    async_svr_service.run();
    return 1;
}




#endif
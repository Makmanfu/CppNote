//-----------------------------------------------------------------------------
//
//
//
//-----------------------------------------------------------------------------

#ifndef TCP_SYNC_SERVER_HPP
#define TCP_SYNC_SERVER_HPP

#include "stdafx.h"
#include "BoostConfig.h"
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost::asio;
using namespace boost::posix_time;
io_service sync_svr_service;

struct talk_sync_svr;
typedef boost::shared_ptr<talk_sync_svr> svr_ptr;
typedef std::vector<svr_ptr> array_svr;
array_svr sync_svr_servers;
// thread-safe access to clients array
boost::recursive_mutex cs;

void sync_svr_update_clients_changed() ;

/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
struct talk_sync_svr : boost::enable_shared_from_this<talk_sync_svr>
{
    talk_sync_svr() : sock_(sync_svr_service), started_(false), already_read_(0)
    {
        last_ping = microsec_clock::local_time();
    }
    std::string username() const
    {
        return username_;
    }

    void answer_to_client()
    {
        try
        {
            read_request();
            process_request();
        }
        catch (boost::system::system_error&)
        {
            stop();
        }
        if (timed_out())
        {
            stop();
            std::cout << "stopping " << username_ << " - no ping in time" << std::endl;
        }
    }
    void set_clients_changed()
    {
        clients_changed_ = true;
    }
    ip::tcp::socket& sock()
    {
        return sock_;
    }
    bool timed_out() const
    {
        ptime now = microsec_clock::local_time();
        long long ms = (now - last_ping).total_milliseconds();
        return ms > 5000 ;
    }
    void stop()
    {
        // close client connection
        boost::system::error_code err;
        sock_.close(err);
    }
private:
    void read_request()
    {
        if (sock_.available())
            already_read_ += sock_.read_some(buffer(buff_ + already_read_, max_msg - already_read_));
    }
    void process_request()
    {
        bool found_enter = std::find(buff_, buff_ + already_read_, '\n')
                           < buff_ + already_read_;
        if (!found_enter)
            return; // message is not full
        // process the msg
        last_ping = microsec_clock::local_time();
        size_t pos = std::find(buff_, buff_ + already_read_, '\n') - buff_;
        std::string msg(buff_, pos);
        std::copy(buff_ + already_read_, buff_ + max_msg, buff_);
        already_read_ -= pos + 1;
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
        write("login ok\n");
        sync_svr_update_clients_changed();
    }
    void on_ping()
    {
        write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
        clients_changed_ = false;
    }
    void on_clients()
    {
        std::string msg;
        {
            boost::recursive_mutex::scoped_lock lk(cs);
            for (array_svr::const_iterator b = sync_svr_servers.begin(), e = sync_svr_servers.end() ; b != e; ++b)
                msg += (*b)->username() + " ";
        }
        write("clients " + msg + "\n");
    }

    void write(const std::string& msg)
    {
        sock_.write_some(buffer(msg));
    }
private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;
    bool clients_changed_;
    ptime last_ping;
};

void sync_svr_update_clients_changed()
{
    boost::recursive_mutex::scoped_lock lk(cs);
    for (array_svr::iterator b = sync_svr_servers.begin(), e = sync_svr_servers.end(); b != e; ++b)
        (*b)->set_clients_changed();
}

void accept_thread()
{
    ip::tcp::acceptor acceptor(sync_svr_service, ip::tcp::endpoint(ip::tcp::v4(), 2001));
    std::cout << "svr start" << std::endl;
    //·þÎñ¶Ë°ó¶¨sock
    while (true)
    {
        svr_ptr new_(new talk_sync_svr);
        acceptor.accept(new_->sock());
        boost::recursive_mutex::scoped_lock lk(cs);
        sync_svr_servers.push_back(new_);
    }
}

void handle_clients_thread()
{
    while (true)
    {
        boost::this_thread::sleep(millisec(1));
        boost::recursive_mutex::scoped_lock lk(cs);
        for (array_svr::iterator b = sync_svr_servers.begin(), e = sync_svr_servers.end(); b != e; ++b)
            (*b)->answer_to_client();
        // erase clients that timed out
        sync_svr_servers.erase(std::remove_if(sync_svr_servers.begin(), sync_svr_servers.end(),
                                              boost::bind(&talk_sync_svr::timed_out, _1)), sync_svr_servers.end());
    }
}


//--------------------------------------------------------------
/** simple connection to server:
    - logs in just with username (no password)
    - all connections are initiated by the client: client asks, server answers
    - server disconnects any client that hasn't pinged for 5 seconds

    Possible requests:
    - gets a list of all connected clients
    - ping: the server answers either with "ping ok" or "ping client_list_changed"
*/
struct talk_sync_client
{
    talk_sync_client(const std::string& username) : sock_(sync_svr_service), started_(true), username_(username) {}
    void connect(ip::tcp::endpoint ep)
    {
        sock_.connect(ep);
    }
    void loop()
    {
        // read answer to our login
        write("login " + username_ + "\n");
        read_answer();
        while (started_)
        {
            write_request();
            read_answer();
            int millis = rand() % 7000;
            std::cout << username_ << " postpone ping "
                      << millis << " ms" << std::endl;
            boost::this_thread::sleep(boost::posix_time::millisec(millis));
        }
    }
    std::string username() const
    {
        return username_;
    }
private:
    void write_request()
    {
        write("ping\n");
    }
    void read_answer()
    {
        already_read_ = 0;
        read(sock_, buffer(buff_),
             boost::bind(&talk_sync_client::read_complete, this, _1, _2));
        process_msg();
    }
    void process_msg()
    {
        std::string msg(buff_, already_read_);
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
        if (answer == "client_list_changed")
            do_ask_clients();
    }
    void on_clients(const std::string& msg)
    {
        std::string clients = msg.substr(8);
        std::cout << username_ << ", new client list:" << clients;
    }
    void do_ask_clients()
    {
        write("ask_clients\n");
        read_answer();
    }

    void write(const std::string& msg)
    {
        sock_.write_some(buffer(msg));
    }
    size_t read_complete(const boost::system::error_code& err, size_t bytes)
    {
        if (err) return 0;
        already_read_ = bytes;
        bool found = std::find(buff_, buff_ + bytes, '\n') < buff_ + bytes;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }
private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    int already_read_;
    char buff_[max_msg];
    bool started_;
    std::string username_;
};

ip::tcp::endpoint sync_svr_ep(ip::address::from_string("127.0.0.1"), 2001);

void run_client(const std::string& client_name)
{
    talk_sync_client client(client_name);
    try
    {
        client.connect(sync_svr_ep);
        client.loop();
    }
    catch (boost::system::system_error& err)
    {
        std::cout << "client terminated " << client.username()
                  << ": " << err.what() << std::endl;
    }
}

int sync_client(void)
{
    boost::thread_group threads;
    char* names[] = { "John", "James", "Lucy", "Tracy", "Frank", "Abby", 0 };
    for (char** name = names; *name; ++name)
    {
        threads.create_thread(boost::bind(run_client, *name));
        boost::this_thread::sleep(boost::posix_time::millisec(100));
    }
    threads.join_all();
    return 1;
}

int sync_server(void)
{
    boost::thread_group threads;
    threads.create_thread(accept_thread);
    threads.create_thread(handle_clients_thread);
    threads.join_all();
    return 1;
}





#endif
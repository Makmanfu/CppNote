
#include "stdafx.h"
#include "BoostConfig.h"
#include "Boost_thread.h"
#include <iostream>
using namespace std;
#define BOOST_HAS_WINTHREADS
//#define BOOST_THREAD_BUILD_DLL
#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>
extern "C" void tss_cleanup_implemented(void) {}
#include <libs/thread/src/win32/thread.cpp>
#include <libs/thread/src/win32/tss_dll.cpp>
#include <libs/thread/src/win32/tss_pe.cpp>
using namespace boost;

#include <stack>

//测试耗时函数1
void tmp_prolood(void)
{
    cout << "thread:  tmp_prolood 启动" << endl;
    for (int i = 0; i < 10; ++i)
    {
        //休息1s  类似Sleep(1000)
        this_thread::sleep(posix_time::seconds(1));
        cout << "临界区操作!" << endl;
    }
    //this_thread::interruption_point();    //允许中断
}

//测试耗时函数带参数
void tmp_prolood2(const string& f_cap)
{
    cout << f_cap << endl;
    for (int i = 0; i < 100; ++i)
    {
        this_thread::sleep(posix_time::seconds(1));
        cout << "loop:  " << i << endl;
    }
}

//测试函数 中断点
void tmp_prolood3(const string& f_cap)
{
    cout << f_cap << endl;
    int i = 0;
    while (true)
    {
        cout << ++i << endl;
        this_thread::interruption_point();      //这里允许中断
    }
}

void boost_mutex_Demo(void)
{
    //说明: 互斥量主要是防止多高线程访问同一个资源问题
    mutex mu;   //声明一个互斥量对象
    try
    {
        //锁
        mu.lock();
        //耗时函数
        tmp_prolood();
        //解锁
        mu.unlock();
    }
    catch (...)
    {
        //保证异常也要解锁
        mu.unlock();
    }
}

void boost_mutexRAII_Demo(void)
{
    mutex mu;
    mutex::scoped_lock lock(mu);    //使用RAII 锁
    tmp_prolood();
    //执行完后自动释放
}

void boost_CreatThread_Demo(int f_demo)
{
    switch (f_demo)
    {
        case 0:
        {
            cout << "线程一直等待 join" << endl;
            boost::thread td(tmp_prolood);
            td.joinable();          //打开线程等待
            td.join();              //阻塞一直等待直到跑完
            cout << "tmp_prolood 结束" << endl;
        }
        break;
        case 1:
        {
            //线程定时等待 后线程结束
            cout << "线程等待4s后强制结束" << endl;
            boost::thread td(tmp_prolood);
            td.joinable();
            td.timed_join(posix_time::seconds(4));       //等待4秒后跑后面语句
            cout << "4s 后并没有停止线程 只是解除阻塞" << endl;
        }
        break;
        case 2:
        {
            cout << "thread1 开工" << endl;
            boost::thread td(tmp_prolood);
            td.joinable();
            td.join();  //等thread1跑完跑第二线程
            cout << "thread2 开工" << endl;
            boost::thread td2(tmp_prolood);
            td2.joinable();
            td2.timed_join(posix_time::seconds(2));
            cout << "thread2 2s后解除阻塞" << endl;
            cout << "thread3 开工" << endl;
            boost::thread td3(tmp_prolood);
            td3.joinable();
            td3.join();
        }
        break;
        default:
        {
            //创建启动一个线程 完成后自动释放
            cout << "thread 最简单例子" << endl;
            boost::thread td(tmp_prolood);
            //带参数
            //boost::thread td(tmp_prolood2,"td1线程");
            td.detach();        //线程执行体分离,线程继续运行直到跑完
        }
        break;
    }
}

void boost_CreatThread_parameter(int f_demo/*=100*/)
{
    switch (f_demo)
    {
        case 0:
        {
            //线程传参数
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            boost::function<void() > ftd = boost::bind(tmp_prolood2, "td1线程");
            //此处有问题
            //thread(ftd);
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        break;
        case 1:
        {
            //CPU支持并行线程数
            cout << "CPU Thread: " << boost::thread::hardware_concurrency() << endl;
        }
        break;
        case 2:
        {
            //sleep休息 解除阻塞
            boost::thread td(boost::bind(tmp_prolood2, "td1线程"));
            boost::this_thread::sleep(posix_time::seconds(3));
            cout << "休息2秒 解除阻塞" << endl;
        }
        break;
        case 3:
        {
            //线程中断 (有中断点的才能断)
            boost::thread td(boost::bind(tmp_prolood2, "td1线程"));
            this_thread::sleep(posix_time::seconds(5));
            td.interrupt();     //线程中断
            cout << "线程中断了" << endl;
        }
        break;
        case 4:
        {
            //线程中断点 中断线程
            boost::thread td(boost::bind(tmp_prolood3, "td1线程"));
            this_thread::sleep(posix_time::seconds(7));
            td.interrupt();     //线程中断
        }
        break;
        case 5:
        {
            boost::thread td1(boost::bind(tmp_prolood2, "td1线程"));
            boost::thread td2(boost::bind(tmp_prolood2, "td2线程"));
            boost::thread td3(boost::bind(tmp_prolood2, "td3线程"));
            cout << "111" << endl;
            td1.timed_join(posix_time::seconds(7));
            cout << "222" << endl;
            td2.timed_join(posix_time::seconds(3));
            cout << "333" << endl;
            td3.join();
            cout << "444" << endl;
        }
        break;
        default:
        {
            //bind 传递参数
            boost::thread td(boost::bind(tmp_prolood2, "td1线程"));
        }
        break;
    }
}

void boost_Thread_pool(void)
{
    //创建线程池
    boost::thread_group tg;
    //根据cpu创建线程数
    int cpunum = boost::thread::hardware_concurrency();
    string t_tmp;
    for (int i = 1; i <= cpunum; ++i)
    {
        t_tmp = "线程" + boost::lexical_cast<string> (i);
        tg.create_thread(boost::bind(tmp_prolood2, t_tmp));
    }
    //tg.join_all();
    //3秒后中断所有线程
    boost::this_thread::sleep(posix_time::seconds(3));
    tg.interrupt_all();
}

#ifndef ___condition_variable______
class buffer
{
private:
    mutex mu;                                       //互斥量
    condition_variable_any cond_put;                //写入条件变量
    condition_variable_any cond_get;                //读取条件变量
    stack<int> stk;                                 //缓冲区对象
    size_t un_read, capacity;
    bool is_full()
    {
        return un_read == capacity;    //缓冲区满判断
    }
    bool is_empty()
    {
        return un_read == 0;    //缓冲区空判断
    }
public:
    buffer(size_t n) : un_read(0), capacity(n) {}          //构造函数
    void put(int x)                                 //写入数据
    {
        {
            mutex::scoped_lock lock(mu);            //锁定互斥量
            while (is_full())                       //检查缓冲区是否满
            {
                {
                    mutex io_mu;
                    mutex::scoped_lock lock(io_mu);
                    cout << "full waiting ..." << endl;
                }
                cond_put.wait(mu);                  //条件变量等待
            }                                       //条件满足 停止等待
            stk.push(x);                            //压栈,写入数据
            ++un_read;
        }                                           //自动解锁互斥量
        cond_get.notify_one();                      //通知可以读取数据
    }
    void get(int* x)                                //读取数据
    {
        {
            mutex::scoped_lock lock(mu);            //锁定互斥量
            while (is_empty())                      //检查缓冲区是否空
            {
                {
                    mutex io_mu;
                    mutex::scoped_lock lock(io_mu);
                    cout << "empty waiting ..." << endl;
                }
                cond_get.wait(mu);                  //条件变量等待
            }                                       //条件满足 停止等待
            --un_read;
            *x = stk.top();                         //读取数据
            stk.pop();                              //弹栈
        }
        cond_put.notify_one();                      //通知可以写入数据
    }
} buf(5);      //一个缓冲区对象

void producer(int n)                                //生产者
{
    for (int i = 0; i < n; ++i)
    {
        {
            mutex io_mu;
            mutex::scoped_lock lock(io_mu);
            cout << "put " << endl;
        }
        buf.put(i);                                 //写入数据
    }
}

void consumer(int n)                                //消费者
{
    int x;
    for (int i = 0; i < n; ++i)
    {
        buf.get(&x);                                //读取数据
        mutex io_mu;
        mutex::scoped_lock lock(io_mu);
        cout << "get " << x << endl;
    }
}

void boost_tdmanDemo(void)
{
    thread t1(producer, 20);
    thread t2(consumer, 10);
    thread t3(consumer, 10);
    t1.join();
    t2.join();
    t3.join();
}
#endif





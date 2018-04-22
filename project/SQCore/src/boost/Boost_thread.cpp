
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

//���Ժ�ʱ����1
void tmp_prolood(void)
{
    cout << "thread:  tmp_prolood ����" << endl;
    for (int i = 0; i < 10; ++i)
    {
        //��Ϣ1s  ����Sleep(1000)
        this_thread::sleep(posix_time::seconds(1));
        cout << "�ٽ�������!" << endl;
    }
    //this_thread::interruption_point();    //�����ж�
}

//���Ժ�ʱ����������
void tmp_prolood2(const string& f_cap)
{
    cout << f_cap << endl;
    for (int i = 0; i < 100; ++i)
    {
        this_thread::sleep(posix_time::seconds(1));
        cout << "loop:  " << i << endl;
    }
}

//���Ժ��� �жϵ�
void tmp_prolood3(const string& f_cap)
{
    cout << f_cap << endl;
    int i = 0;
    while (true)
    {
        cout << ++i << endl;
        this_thread::interruption_point();      //���������ж�
    }
}

void boost_mutex_Demo(void)
{
    //˵��: ��������Ҫ�Ƿ�ֹ����̷߳���ͬһ����Դ����
    mutex mu;   //����һ������������
    try
    {
        //��
        mu.lock();
        //��ʱ����
        tmp_prolood();
        //����
        mu.unlock();
    }
    catch (...)
    {
        //��֤�쳣ҲҪ����
        mu.unlock();
    }
}

void boost_mutexRAII_Demo(void)
{
    mutex mu;
    mutex::scoped_lock lock(mu);    //ʹ��RAII ��
    tmp_prolood();
    //ִ������Զ��ͷ�
}

void boost_CreatThread_Demo(int f_demo)
{
    switch (f_demo)
    {
        case 0:
        {
            cout << "�߳�һֱ�ȴ� join" << endl;
            boost::thread td(tmp_prolood);
            td.joinable();          //���̵߳ȴ�
            td.join();              //����һֱ�ȴ�ֱ������
            cout << "tmp_prolood ����" << endl;
        }
        break;
        case 1:
        {
            //�̶߳�ʱ�ȴ� ���߳̽���
            cout << "�̵߳ȴ�4s��ǿ�ƽ���" << endl;
            boost::thread td(tmp_prolood);
            td.joinable();
            td.timed_join(posix_time::seconds(4));       //�ȴ�4����ܺ������
            cout << "4s ��û��ֹͣ�߳� ֻ�ǽ������" << endl;
        }
        break;
        case 2:
        {
            cout << "thread1 ����" << endl;
            boost::thread td(tmp_prolood);
            td.joinable();
            td.join();  //��thread1�����ܵڶ��߳�
            cout << "thread2 ����" << endl;
            boost::thread td2(tmp_prolood);
            td2.joinable();
            td2.timed_join(posix_time::seconds(2));
            cout << "thread2 2s��������" << endl;
            cout << "thread3 ����" << endl;
            boost::thread td3(tmp_prolood);
            td3.joinable();
            td3.join();
        }
        break;
        default:
        {
            //��������һ���߳� ��ɺ��Զ��ͷ�
            cout << "thread �������" << endl;
            boost::thread td(tmp_prolood);
            //������
            //boost::thread td(tmp_prolood2,"td1�߳�");
            td.detach();        //�߳�ִ�������,�̼߳�������ֱ������
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
            //�̴߳�����
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            boost::function<void() > ftd = boost::bind(tmp_prolood2, "td1�߳�");
            //�˴�������
            //thread(ftd);
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        }
        break;
        case 1:
        {
            //CPU֧�ֲ����߳���
            cout << "CPU Thread: " << boost::thread::hardware_concurrency() << endl;
        }
        break;
        case 2:
        {
            //sleep��Ϣ �������
            boost::thread td(boost::bind(tmp_prolood2, "td1�߳�"));
            boost::this_thread::sleep(posix_time::seconds(3));
            cout << "��Ϣ2�� �������" << endl;
        }
        break;
        case 3:
        {
            //�߳��ж� (���жϵ�Ĳ��ܶ�)
            boost::thread td(boost::bind(tmp_prolood2, "td1�߳�"));
            this_thread::sleep(posix_time::seconds(5));
            td.interrupt();     //�߳��ж�
            cout << "�߳��ж���" << endl;
        }
        break;
        case 4:
        {
            //�߳��жϵ� �ж��߳�
            boost::thread td(boost::bind(tmp_prolood3, "td1�߳�"));
            this_thread::sleep(posix_time::seconds(7));
            td.interrupt();     //�߳��ж�
        }
        break;
        case 5:
        {
            boost::thread td1(boost::bind(tmp_prolood2, "td1�߳�"));
            boost::thread td2(boost::bind(tmp_prolood2, "td2�߳�"));
            boost::thread td3(boost::bind(tmp_prolood2, "td3�߳�"));
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
            //bind ���ݲ���
            boost::thread td(boost::bind(tmp_prolood2, "td1�߳�"));
        }
        break;
    }
}

void boost_Thread_pool(void)
{
    //�����̳߳�
    boost::thread_group tg;
    //����cpu�����߳���
    int cpunum = boost::thread::hardware_concurrency();
    string t_tmp;
    for (int i = 1; i <= cpunum; ++i)
    {
        t_tmp = "�߳�" + boost::lexical_cast<string> (i);
        tg.create_thread(boost::bind(tmp_prolood2, t_tmp));
    }
    //tg.join_all();
    //3����ж������߳�
    boost::this_thread::sleep(posix_time::seconds(3));
    tg.interrupt_all();
}

#ifndef ___condition_variable______
class buffer
{
private:
    mutex mu;                                       //������
    condition_variable_any cond_put;                //д����������
    condition_variable_any cond_get;                //��ȡ��������
    stack<int> stk;                                 //����������
    size_t un_read, capacity;
    bool is_full()
    {
        return un_read == capacity;    //���������ж�
    }
    bool is_empty()
    {
        return un_read == 0;    //���������ж�
    }
public:
    buffer(size_t n) : un_read(0), capacity(n) {}          //���캯��
    void put(int x)                                 //д������
    {
        {
            mutex::scoped_lock lock(mu);            //����������
            while (is_full())                       //��黺�����Ƿ���
            {
                {
                    mutex io_mu;
                    mutex::scoped_lock lock(io_mu);
                    cout << "full waiting ..." << endl;
                }
                cond_put.wait(mu);                  //���������ȴ�
            }                                       //�������� ֹͣ�ȴ�
            stk.push(x);                            //ѹջ,д������
            ++un_read;
        }                                           //�Զ�����������
        cond_get.notify_one();                      //֪ͨ���Զ�ȡ����
    }
    void get(int* x)                                //��ȡ����
    {
        {
            mutex::scoped_lock lock(mu);            //����������
            while (is_empty())                      //��黺�����Ƿ��
            {
                {
                    mutex io_mu;
                    mutex::scoped_lock lock(io_mu);
                    cout << "empty waiting ..." << endl;
                }
                cond_get.wait(mu);                  //���������ȴ�
            }                                       //�������� ֹͣ�ȴ�
            --un_read;
            *x = stk.top();                         //��ȡ����
            stk.pop();                              //��ջ
        }
        cond_put.notify_one();                      //֪ͨ����д������
    }
} buf(5);      //һ������������

void producer(int n)                                //������
{
    for (int i = 0; i < n; ++i)
    {
        {
            mutex io_mu;
            mutex::scoped_lock lock(io_mu);
            cout << "put " << endl;
        }
        buf.put(i);                                 //д������
    }
}

void consumer(int n)                                //������
{
    int x;
    for (int i = 0; i < n; ++i)
    {
        buf.get(&x);                                //��ȡ����
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





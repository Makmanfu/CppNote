//*************************************************************************
//
//
//
//                                              @阿甘 2017-07-13
//
//*************************************************************************
#ifndef THREADCONCURRENCY_H
#define THREADCONCURRENCY_H
#include <iostream>
#include <thread>
using namespace std;

//线程总结集合
namespace C11ThreadDemo
{
    struct C11ThreadTest1
    {
        static int main(int argc, char** argv)
        {
            //启动运行线程
            std::thread t(ThreadStart);
            //等待线程
            t.join();
            return 1;
        };
        static void ThreadStart(void)
        {
            cout << "ThreadStart..." << endl;
            for (int i = 1; i <= 20; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
            for (int i = 1; i <= 20; i++)
            {
                cout << i << " ";
                Sleep(100);
            }
            cout << endl;
        };
    };

    struct C11ThreadTest2
    {
        static int main(int argc, char** argv)
        {
            int some_local_state = 0;
            C11ThreadTest2 my_func(some_local_state);
            std::thread my_thread(my_func);
            my_thread.detach();                     //不等待线程完成
            return 1;
        };
    private:
        int& i;
    public:
        C11ThreadTest2(int& i_) :i(i_){};
        void operator()()
        {
            for (unsigned j = 0; j < 1000000; ++j)
                i++;
        };
    };

}






#endif

#ifndef BOOST_THREAD_H
#define BOOST_THREAD_H

//互斥量的使用
void boost_mutex_Demo(void);
//互斥量使用RAII 智能指针
void boost_mutexRAII_Demo(void);
//创建线程简单例子
void boost_CreatThread_Demo(int f_demo = 100);
//创建带参数的函数线程
void boost_CreatThread_parameter(int f_demo = 100);
//线程池的使用
void boost_Thread_pool(void);
//多线程应用例子
void boost_tdmanDemo(void);







#endif
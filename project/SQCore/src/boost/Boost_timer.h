//===================================================
//
//              boost时间和日期库的使用
//
//===================================================


#ifndef BOOST_TIMER_H
#define BOOST_TIMER_H
#include <boost/progress.hpp>
#include <boost/static_assert.hpp>

template<int N = 2>
class new_progress_timer: public boost::timer
{
public:
    new_progress_timer(std::ostream& os = std::cout) : m_os(os)
    {
        BOOST_STATIC_ASSERT(N >= 0 && N <= 10);
    }
    ~new_progress_timer(void)
    {
        try
        {
            //保存流的状态
            std::istream::fmtflags old_flags = m_os.setf(std::istream::fixed, std::istream::floatfield);
            std::streamsize old_prec = m_os.precision(N);
            //输出时间
            m_os << elapsed() << " s\n" << std::endl; //"s"秒
            //恢复流状态
            m_os.flags(old_flags);
            m_os.precision(old_prec);
        }
        catch (...) {}     //析构函数严禁抛出异常
    }
private:
    std::ostream& m_os;
};

//模板特化
template<>
class new_progress_timer<2>: public boost::progress_timer
{
};


//计时器计时的多种使用
void boost_Timer_Demo(void);
//进度条的使用
void boost_Progress_Demo(int f_num = 200);



#endif
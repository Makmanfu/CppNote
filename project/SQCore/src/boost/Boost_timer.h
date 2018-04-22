//===================================================
//
//              boostʱ������ڿ��ʹ��
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
            //��������״̬
            std::istream::fmtflags old_flags = m_os.setf(std::istream::fixed, std::istream::floatfield);
            std::streamsize old_prec = m_os.precision(N);
            //���ʱ��
            m_os << elapsed() << " s\n" << std::endl; //"s"��
            //�ָ���״̬
            m_os.flags(old_flags);
            m_os.precision(old_prec);
        }
        catch (...) {}     //���������Ͻ��׳��쳣
    }
private:
    std::ostream& m_os;
};

//ģ���ػ�
template<>
class new_progress_timer<2>: public boost::progress_timer
{
};


//��ʱ����ʱ�Ķ���ʹ��
void boost_Timer_Demo(void);
//��������ʹ��
void boost_Progress_Demo(int f_num = 200);



#endif
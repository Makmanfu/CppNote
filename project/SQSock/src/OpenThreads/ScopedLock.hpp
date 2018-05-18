
#ifndef _SCOPEDLOCK_HPP
#define _SCOPEDLOCK_HPP

namespace OpenThreads
{
    template <class M> class ScopedLock
    {
    private:
        M &m_lock;
        ScopedLock(const ScopedLock &);                 // prevent copy
        ScopedLock &operator=(const ScopedLock &);      // prevent assign
    public:
        explicit ScopedLock(M &m): m_lock(m)
        {
            m_lock.lock();
        };
        ~ScopedLock()
        {
            m_lock.unlock();
        };
    };

    template <class M> class ReverseScopedLock
    {
    private:
        M &m_lock;
        ReverseScopedLock(const ReverseScopedLock &);               // prevent copy
        ReverseScopedLock &operator=(const ReverseScopedLock &);    // prevent assign
    public:
        explicit ReverseScopedLock(M &m): m_lock(m)
        {
            m_lock.unlock();
        };
        ~ReverseScopedLock()
        {
            m_lock.lock();
        };
    };

    template <class M> class ScopedPointerLock
    {
    private:
        M *m_lock;
        ScopedPointerLock(const ScopedPointerLock &);               // prevent copy
        ScopedPointerLock &operator=(const ScopedPointerLock &);    // prevent assign
    public:
        explicit ScopedPointerLock(M *m): m_lock(m)
        {
            if (m_lock) m_lock->lock();
        };
        ~ScopedPointerLock()
        {
            if (m_lock) m_lock->unlock();
        };
    };

    template <class M> class ReverseScopedPointerLock
    {
    private:
        M *m_lock;
        ReverseScopedPointerLock(const ReverseScopedPointerLock &);             // prevent copy
        ReverseScopedPointerLock &operator=(const ReverseScopedPointerLock &);  // prevent assign
    public:
        explicit ReverseScopedPointerLock(M *m): m_lock(m)
        {
            if (m_lock) m_lock->unlock();
        };
        ~ReverseScopedPointerLock()
        {
            if (m_lock) m_lock->lock();
        };
    };

#ifndef USE_CRITICAL_SECTION

    template <int instance>
    struct WIN32MutexSpin
    {

        enum { __low_max = 30, __high_max = 1000 };
        // Low if we suspect uniprocessor, high for multiprocessor.
        static unsigned __max;
        static unsigned __last;
    };

    template <int instance>
    unsigned WIN32MutexSpin <instance>::__max = WIN32MutexSpin <instance>::__low_max;

    template <int instance>
    unsigned WIN32MutexSpin <instance>::__last = 0;

    static void _S_nsec_sleep(int __log_nsec)
    {
        if (__log_nsec <= 20)
            SwitchToThread(); //Sleep(0); // adegli replaced it Sleep by SwitchToThread
        else
            Sleep(1 << (__log_nsec - 20));
    }

#if defined(_MSC_VER) && _MSC_VER <= 1300
    template WIN32MutexSpin <0>;
#endif

#endif // USE_CRITICAL_SECTION


}

#endif

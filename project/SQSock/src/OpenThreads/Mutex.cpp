
#include "stdafx.h"
#include "Mutex.h"
#include "OpenThread.h"

namespace OpenThreads
{
#define InterlockedGet(x) InterlockedExchangeAdd(x,0)

    class Win32MutexPrivateData
    {
        friend class Mutex;
        friend class Condition;
    private:
#define USE_CRITICAL_SECTION
#ifdef USE_CRITICAL_SECTION
        CRITICAL_SECTION _cs;
#else
        volatile unsigned long mutex;
#endif
    private:
        Win32MutexPrivateData()
        {
#ifdef USE_CRITICAL_SECTION
            InitializeCriticalSection(&_cs);
#else
            mutex = 0;
#endif
        };
        ~Win32MutexPrivateData()
        {
#ifdef USE_CRITICAL_SECTION
            DeleteCriticalSection(&_cs);
#endif
        };
    };

    class Win32ConditionPrivateData
    {
    public:
        friend class Condition;
        /// number of waiters.
        long waiters_;

        Win32ConditionPrivateData()
            : waiters_(0),
            sema_(CreateSemaphore(NULL, 0, 0x7fffffff, NULL)),
            waiters_done_(CreateEvent(NULL, FALSE, FALSE, NULL)),
            was_broadcast_(0)
        {
        }

        ~Win32ConditionPrivateData()
        {}

        inline int broadcast()
        {
            int have_waiters = 0;
            long w = InterlockedGet(&waiters_);

            if (w > 0)
            {
                // we are broadcasting.
                was_broadcast_ = 1;
                have_waiters = 1;
            }

            int result = 0;
            if (have_waiters)
            {
                // Wake up all the waiters.
                ReleaseSemaphore(sema_.get(), w, NULL);

                OpenThreads::cooperativeWait(waiters_done_.get(), INFINITE);

                //end of broadcasting
                was_broadcast_ = 0;
            }
            return result;
        }

        inline int signal()
        {
            long w = InterlockedGet(&waiters_);
            int have_waiters = w > 0;

            int result = 0;

            if (have_waiters)
            {
                if (!ReleaseSemaphore(sema_.get(), 1, NULL))
                    result = -1;
            }
            return result;
        }

        inline int wait(Mutex &external_mutex, long timeout_ms)
        {

            // Prevent race conditions on the <waiters_> count.
            InterlockedIncrement(&waiters_);

            int result = 0;

            ReverseScopedLock<Mutex> lock(external_mutex);

            // wait in timeslices, giving testCancel() a change to
            // exit the thread if requested.
            try
            {
                DWORD dwResult = OpenThreads::cooperativeWait(sema_.get(), timeout_ms);
                if (dwResult != WAIT_OBJECT_0)
                    result = (int)dwResult;
            }
            catch (...)
            {
                // thread is canceled in cooperative wait , do cleanup
                long w = InterlockedDecrement(&waiters_);
                int last_waiter = was_broadcast_ && w == 0;

                if (last_waiter)  SetEvent(waiters_done_.get());
                // rethrow
                throw;
            }


            // We're ready to return, so there's one less waiter.
            long w = InterlockedDecrement(&waiters_);
            int last_waiter = was_broadcast_ && w == 0;

            if (result != -1 && last_waiter)
                SetEvent(waiters_done_.get());

            return result;
        }

    protected:

        /// Serialize access to the waiters count.
        /// Mutex waiters_lock_;
        /// Queue up threads waiting for the condition to become signaled.
        HandleHolder sema_;
        /**
        * An auto reset event used by the broadcast/signal thread to wait
        * for the waiting thread(s) to wake up and get a chance at the
        * semaphore.
        */
        HandleHolder waiters_done_;
        /// Keeps track of whether we were broadcasting or just signaling.
        size_t was_broadcast_;
    };

    class Win32BarrierPrivateData
    {
        friend class Barrier;
    private:
        Condition cond;                 // cv for waiters at barrier
        Mutex lock;                     // mutex for waiters at barrier
        volatile int maxcnt;            // number of threads to wait for
        volatile int cnt;               // number of waiting threads
        volatile int phase;             // flag to seperate two barriers
    private:
        Win32BarrierPrivateData(int mc, int c, int p) :maxcnt(mc), cnt(c), phase(p) {};
        ~Win32BarrierPrivateData(){};
    };


    HandleHolder::HandleHolder() : _handle(INVALID_HANDLE_VALUE)
    {
    }

    HandleHolder::HandleHolder(HANDLE h) : _handle(h)
    {
    }

    HandleHolder::~HandleHolder()
    {
        close();
    }

    const HANDLE & HandleHolder::get() const
    {
        return _handle;
    }

    void HandleHolder::set(HANDLE h)
    {
        if (_handle != INVALID_HANDLE_VALUE) close();
        _handle = h;
    }

    HandleHolder::operator bool()
    {
        return _handle != INVALID_HANDLE_VALUE && _handle != NULL;
    }

    void HandleHolder::close()
    {
        if (_handle != INVALID_HANDLE_VALUE) CloseHandle(_handle);
        _handle = INVALID_HANDLE_VALUE;
    }


    Mutex::Mutex(MutexType type) : _mutexType(type)
    {
        Win32MutexPrivateData *pd = new Win32MutexPrivateData();
        _prvData = static_cast<void *>(pd);
    }

    Mutex::Mutex(const Mutex &)
    {}

    Mutex & Mutex::operator=(const Mutex &/*m*/)
    {
        return *(this);
    }

    Mutex::~Mutex()
    {
        delete static_cast<Win32MutexPrivateData *>(_prvData);
    }

    Mutex::MutexType Mutex::getMutexType() const
    {
        return _mutexType;
    }

    int Mutex::lock()
    {
        Win32MutexPrivateData *pd =
            static_cast<Win32MutexPrivateData *>(_prvData);

#ifdef USE_CRITICAL_SECTION

        // Block until we can take this lock.
        EnterCriticalSection(&(pd->_cs));

        return 0;

#else

        volatile unsigned long *lock = &pd->mutex;
        // InterlockedExchange returns old value
        // if old_value  == 0 mutex wasn't locked , now it is
        if (!InterlockedExchange((long *)lock, 1L))
        {
            return 0;
        }

        unsigned my_spin_max = WIN32MutexSpin<0>::__max;
        unsigned my_last_spins = WIN32MutexSpin<0>::__last;
        volatile unsigned junk = 17;
        unsigned i;

        for (i = 0; i < my_spin_max; i++)
        {
            if (i < my_last_spins / 2 || *lock)
            {
                junk *= junk;
                junk *= junk;
                junk *= junk;
                junk *= junk;
                continue;
            }

            if (!InterlockedExchange((long *)lock, 1L))
            {
                // got it!
                // Spinning worked.  Thus we're probably not being scheduled
                // against the other process with which we were contending.
                // Thus it makes sense to spin longer the next time.
                WIN32MutexSpin<0>::__last = i;
                WIN32MutexSpin<0>::__max = WIN32MutexSpin<0>::__high_max;
                return 0;
            }
        }
        // We are probably being scheduled against the other process.  Sleep.
        WIN32MutexSpin<0>::__max = WIN32MutexSpin<0>::__low_max;
        for (i = 0;; ++i)
        {
            int __log_nsec = i + 6;
            if (__log_nsec > 27) __log_nsec = 27;
            if (!InterlockedExchange((long *)lock, 1L))
            {
                return 0;
            }
            _S_nsec_sleep(__log_nsec);
        }
        return -1;

#endif // USE_CRITICAL_SECTION
    }

    int Mutex::unlock()
    {
        Win32MutexPrivateData *pd =
            static_cast<Win32MutexPrivateData *>(_prvData);

#ifdef USE_CRITICAL_SECTION

        // Release this lock. CRITICAL_SECTION is nested, thus
        //   unlock() calls must be paired with lock() calls.
        LeaveCriticalSection(&(pd->_cs));

        return 0;

#else

        volatile unsigned long *lock = &pd->mutex;
        *lock = 0;
        // This is not sufficient on many multiprocessors, since
        // writes to protected variables and the lock may be reordered.
        return 0;

#endif // USE_CRITICAL_SECTION
    }

    int Mutex::trylock()
    {
        Win32MutexPrivateData *pd =
            static_cast<Win32MutexPrivateData *>(_prvData);

#ifdef USE_CRITICAL_SECTION

        // Take the lock if we can; regardless don't block.
        // 'result' is FALSE if we took the lock or already held
        //   it amd TRUE if another thread already owns the lock.
        BOOL result = TryEnterCriticalSection(&(pd->_cs));

        return((result == TRUE) ? 0 : 1);

#else

        volatile unsigned long *lock = &pd->mutex;

        if (!InterlockedExchange((long *)lock, 1L))
        {
            return 1; // TRUE
        }

        return 0; // FALSE

#endif // USE_CRITICAL_SECTION
    }


    Condition::Condition()
    {
        Win32ConditionPrivateData *pd =
            new Win32ConditionPrivateData();
        _prvData = static_cast<void *>(pd);
    }

    Condition::Condition(const Condition &/*c*/)
    {
    }

    Condition & Condition::operator=(const Condition &/*c*/)
    {
        return *(this);
    }

    Condition::~Condition()
    {
        Win32ConditionPrivateData *pd =
            static_cast<Win32ConditionPrivateData *>(_prvData);
        delete pd;
    }

    int Condition::wait(Mutex *mutex)
    {
        Win32ConditionPrivateData *pd =
            static_cast<Win32ConditionPrivateData *>(_prvData);
        return pd->wait(*mutex, INFINITE);
    }

    int Condition::wait(Mutex *mutex, unsigned long ms)
    {
        Win32ConditionPrivateData *pd =
            static_cast<Win32ConditionPrivateData *>(_prvData);
        return pd->wait(*mutex, ms);
    }

    int Condition::signal()
    {
        Win32ConditionPrivateData *pd =
            static_cast<Win32ConditionPrivateData *>(_prvData);
        return pd->signal();
    }

    int Condition::broadcast()
    {
        Win32ConditionPrivateData *pd =
            static_cast<Win32ConditionPrivateData *>(_prvData);
        return pd->broadcast();
    }


    Barrier::Barrier(int numThreads)
    {
        Win32BarrierPrivateData *pd = new Win32BarrierPrivateData(numThreads, 0, 0);
        _valid = true;
        _prvData = static_cast<void *>(pd);
    }

    Barrier::Barrier(const Barrier &/*b*/)
    {
    }

    Barrier & Barrier::operator=(const Barrier &/*b*/)
    {
        return *(this);
    }

    Barrier::~Barrier()
    {
        Win32BarrierPrivateData *pd =
            static_cast<Win32BarrierPrivateData *>(_prvData);
        delete pd;
    }

    void Barrier::reset()
    {
        Win32BarrierPrivateData *pd =
            static_cast<Win32BarrierPrivateData *>(_prvData);
        pd->cnt = 0;
        pd->phase = 0;
    }

    void Barrier::block(unsigned int numThreads)
    {
        Win32BarrierPrivateData *pd =
            static_cast<Win32BarrierPrivateData *>(_prvData);

        if (numThreads != 0) pd->maxcnt = numThreads;
        int my_phase;

        ScopedLock<Mutex> lock(pd->lock);
        if (_valid)
        {
            my_phase = pd->phase;
            ++pd->cnt;

            if (pd->cnt == pd->maxcnt)               // I am the last one
            {
                pd->cnt = 0;                         // reset for next use
                pd->phase = 1 - my_phase;            // toggle phase
                pd->cond.broadcast();
            }
            else
            {
                while (pd->phase == my_phase)
                {
                    pd->cond.wait(&pd->lock);
                }
            }
        }
    }

    void Barrier::invalidate()
    {
        Win32BarrierPrivateData *pd =
            static_cast<Win32BarrierPrivateData *>(_prvData);

        pd->lock.lock();
        _valid = false;
        pd->lock.unlock();
        release();
    }

    void Barrier::release()
    {
        Win32BarrierPrivateData *pd =
            static_cast<Win32BarrierPrivateData *>(_prvData);
        int my_phase;

        ScopedLock<Mutex> lock(pd->lock);
        my_phase = pd->phase;

        pd->cnt = 0;                         // reset for next use
        pd->phase = 1 - my_phase;            // toggle phase
        pd->cond.broadcast();

    }

    int Barrier::numThreadsCurrentlyBlocked()
    {
        Win32BarrierPrivateData *pd =
            static_cast<Win32BarrierPrivateData *>(_prvData);
        int numBlocked = -1;
        ScopedLock<Mutex> lock(pd->lock);
        numBlocked = pd->cnt;
        return numBlocked;
    }


    Block::Block() :_released(false)
    {
    }

    Block::Block(const Block &)
    {
    }

    Block::~Block()
    {
        release();
    }

    bool Block::block()
    {
        ScopedLock<Mutex> mutlock(_mut);
        if (!_released)
            return _cond.wait(&_mut) == 0;
        else
            return true;
    }

    bool Block::block(unsigned long timeout)
    {
        ScopedLock<Mutex> mutlock(_mut);
        if (!_released)
            return _cond.wait(&_mut, timeout) == 0;
        else
            return true;
    }

    void Block::release()
    {
        ScopedLock<Mutex> mutlock(_mut);
        if (!_released)
        {
            _released = true;
            _cond.broadcast();
        }
    }

    void Block::reset()
    {
        ScopedLock<Mutex> mutlock(_mut);
        _released = false;
    }

    void Block::set(bool doRelease)
    {
        if (doRelease != _released)
        {
            if (doRelease)
                release();
            else
                reset();
        }
    }


    BlockCount::BlockCount(unsigned int blockCount) :
        _blockCount(blockCount),
        _currentCount(0)
    {

    }

    BlockCount::BlockCount(const BlockCount &)
    {

    }

    BlockCount::~BlockCount()
    {
        _blockCount = 0;
        release();
    }

    void BlockCount::completed()
    {
        ScopedLock<Mutex> mutlock(_mut);
        if (_currentCount > 0)
        {
            --_currentCount;

            if (_currentCount == 0)
            {
                // osg::notify(osg::NOTICE)<<"Released"<<std::endl;
                _cond.broadcast();
            }
        }
    }

    void BlockCount::block()
    {
        ScopedLock<Mutex> mutlock(_mut);
        if (_currentCount)
            _cond.wait(&_mut);
    }

    void BlockCount::reset()
    {
        ScopedLock<Mutex> mutlock(_mut);
        if (_currentCount != _blockCount)
        {
            if (_blockCount == 0) _cond.broadcast();
            _currentCount = _blockCount;
        }
    }

    void BlockCount::release()
    {
        ScopedLock<Mutex> mutlock(_mut);
        if (_currentCount)
        {
            _currentCount = 0;
            _cond.broadcast();
        }
    }

    void BlockCount::setBlockCount(unsigned int blockCount)
    {
        _blockCount = blockCount;
    }

    unsigned int BlockCount::getBlockCount() const
    {
        return _blockCount;
    }

    unsigned int BlockCount::getCurrentCount() const
    {
        return _currentCount;
    }

}

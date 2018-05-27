
#ifndef _OPENTHREADS_MUTEX_
#define _OPENTHREADS_MUTEX_

#include "Config.h"
#include "ScopedLock.hpp"

namespace OpenThreads
{

    class HandleHolder
    {
    public:
        HandleHolder();
        explicit HandleHolder(HANDLE h);
        ~HandleHolder();
    private:
        // copy constructor - disallow
        HandleHolder(const HandleHolder &rhs);
        // assignment operator - disallow
        HandleHolder &operator=(const HandleHolder &rhs);
    public:
        // accessor
        const HANDLE &get() const;
        // mutator
        void set(HANDLE h);

        operator bool();
    private:
        HANDLE _handle;
        void close();
    };

    class Mutex
    {
        friend class Condition;
    public:
        enum MutexType
        {
            MUTEX_NORMAL,
            MUTEX_RECURSIVE
        };
    private:
        //Implementation-specific private data.
        void *_prvData;
        MutexType _mutexType;
    public:
        Mutex(MutexType type = MUTEX_NORMAL);
        virtual ~Mutex();
    private:
        //Private copy constructor, to prevent tampering.
        Mutex(const Mutex &/*m*/);
        //Private copy assignment, to prevent tampering.
        Mutex &operator=(const Mutex &/*m*/);;
    public:
        MutexType getMutexType() const;
        virtual int lock();
        virtual int unlock();
        virtual int trylock();
    };

    class Condition
    {
    private:
        //Implementation-specific data
        void *_prvData;
    public:
        Condition();
        virtual ~Condition();
    private:
        //Private copy constructor, to prevent tampering.
        Condition(const Condition &/*c*/);
        //Private copy assignment, to prevent tampering.
        Condition &operator=(const Condition &/*c*/);
    public:
        //Wait on a mutex.
        virtual int wait(Mutex *mutex);

        /**
        *  Wait on a mutex for a given amount of time (ms)
        *  @return 0 if normal, -1 if errno set, errno code otherwise.
        */
        virtual int wait(Mutex *mutex, unsigned long int ms);

        /**
        *  Signal a SINGLE thread to wake if it's waiting.
        *  @return 0 if normal, -1 if errno set, errno code otherwise.
        */
        virtual int signal();

        /**
        *  Wake all threads waiting on this condition.
        *  @return 0 if normal, -1 if errno set, errno code otherwise.
        */
        virtual int broadcast();
    };

    class Barrier
    {
    private:
        //Implementation-specific private data.
        void *_prvData;
        bool _valid;
    public:
        Barrier(int numThreads = 0);
        virtual ~Barrier();
    private:
        //Private copy constructor, to prevent tampering.
        Barrier(const Barrier &/*b*/);
        //Private copy assignment, to prevent tampering.
        Barrier &operator=(const Barrier &/*b*/);
    public:
        //Reset the barrier to it's original state.
        virtual void reset();
        //Block until numThreads threads have entered the barrier.
        virtual void block(unsigned int numThreads = 0);
        //Release the barrier, now.
        virtual void release();
        //Return the number of threads currently blocked in the barrier,
        virtual int numThreadsCurrentlyBlocked();
        void invalidate();
    };

    class Block//Block is a block that can be used to halt a thread that is waiting another thread to release it.
    {
    protected:
        Mutex _mut;
        Condition _cond;
        bool _released;
    public:
        Block();
        ~Block();
    private:
        Block(const Block &);
    public:
        bool block();
        bool block(unsigned long timeout);
        void release();
        void reset();
        void set(bool doRelease);
    };

    class BlockCount//BlockCount is a block that can be used to halt a thread that is waiting for a specified number of operations to be completed.
    {
    protected:
        OpenThreads::Mutex _mut;
        OpenThreads::Condition _cond;
        unsigned int _blockCount;
        unsigned int _currentCount;
    public:
        BlockCount(unsigned int blockCount);
        ~BlockCount();
    private:
        BlockCount(const BlockCount &);
    public:
        inline void completed();
        inline void block();
        inline void reset();
        inline void release();
        inline void setBlockCount(unsigned int blockCount);
        inline unsigned int getBlockCount() const;
        inline unsigned int getCurrentCount() const;
    };

    class ReentrantMutex : public OpenThreads::Mutex
    {
    public:
        ReentrantMutex() :Mutex(MUTEX_RECURSIVE)
        {}
    };

    class ReadWriteMutex
    {
    public:
        ReadWriteMutex() :_readCount(0) {}

        virtual ~ReadWriteMutex() {}

        virtual int readLock()
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readCountMutex);
            int result = 0;
            if (_readCount == 0)
            {
                result = _readWriteMutex.lock();
            }
            ++_readCount;
            return result;
        }

        virtual int readUnlock()
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_readCountMutex);
            int result = 0;
            if (_readCount > 0)
            {
                --_readCount;
                if (_readCount == 0)
                {
                    result = _readWriteMutex.unlock();
                }
            }
            return result;
        }

        virtual int writeLock()
        {
            return _readWriteMutex.lock();
        }

        virtual int writeUnlock()
        {
            return _readWriteMutex.unlock();
        }

    protected:
        ReadWriteMutex(const ReadWriteMutex &) {}
        ReadWriteMutex &operator = (const ReadWriteMutex &)
        {
            return *(this);
        }

#if 0
        ReentrantMutex  _readWriteMutex;
        ReentrantMutex  _readCountMutex;
#else
        OpenThreads::Mutex  _readWriteMutex;
        OpenThreads::Mutex  _readCountMutex;
#endif
        unsigned int    _readCount;
    };

    class ScopedReadLock
    {
    public:
        ScopedReadLock(ReadWriteMutex &mutex) : _mutex(mutex)
        {
            _mutex.readLock();
        }
        ~ScopedReadLock()
        {
            _mutex.readUnlock();
        }
    protected:
        ReadWriteMutex &_mutex;

        ScopedReadLock &operator = (const ScopedReadLock &)
        {
            return *this;
        }
    };

    class ScopedWriteLock
    {
    public:
        ScopedWriteLock(ReadWriteMutex &mutex) : _mutex(mutex)
        {
            _mutex.writeLock();
        }
        ~ScopedWriteLock()
        {
            _mutex.writeUnlock();
        }
    protected:
        ReadWriteMutex &_mutex;

        ScopedWriteLock &operator = (const ScopedWriteLock &)
        {
            return *this;
        }
    };
    
}

#endif 

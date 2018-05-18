
#include "stdafx.h"
#include <unistd.h>
#include <ulocks.h>
#include <errno.h>
#include <assert.h>

#include <OpenThreads/Mutex>
#include "SharedArena.h"
#include "SprocMutexPrivateData.h"
#include "SprocThreadPrivateActions.h"

using namespace OpenThreads;

//----------------------------------------------------------------------------
//
// Decription: Constructor
//
// Use: public.
//
Mutex::Mutex(MutexType type):
    _mutexType(type)
{

    SprocMutexPrivateData *pd = new SprocMutexPrivateData();

    pd->mutex = SharedArena::allocLock();

    _prvData = static_cast<void *>(pd);

}

//----------------------------------------------------------------------------
//
// Decription: Destructor
//
// Use: public.
//
Mutex::~Mutex()
{

    SprocMutexPrivateData *pd =
        static_cast<SprocMutexPrivateData *>(_prvData);

    SharedArena::deleteLock(pd->mutex);

    delete pd;

}

//----------------------------------------------------------------------------
//
// Decription: lock the mutex
//
// Use: public.
//
int Mutex::lock()
{

    ThreadPrivateActions::ThreadCancelTest();

    SprocMutexPrivateData *pd =
        static_cast<SprocMutexPrivateData *>(_prvData);

    int status = SharedArena::lock(pd->mutex);

    if(status == -1)
    {
        return status;
    }

    if(status == 1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
    return 0;
}

//----------------------------------------------------------------------------
//
// Decription: unlock the mutex
//
// Use: public.
//
int Mutex::unlock()
{

    SprocMutexPrivateData *pd =
        static_cast<SprocMutexPrivateData *>(_prvData);

    int status = SharedArena::unlock(pd->mutex);

    ThreadPrivateActions::ThreadCancelTest();

    return status;

}

//----------------------------------------------------------------------------
//
// Decription: test if the mutex may be locked
//
// Use: public.
//
int Mutex::trylock()
{

    SprocMutexPrivateData *pd =
        static_cast<SprocMutexPrivateData *>(_prvData);

    int status = SharedArena::trylock(pd->mutex);

    return status;

}


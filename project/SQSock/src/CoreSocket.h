
#ifndef CORESOCKET_H
#define CORESOCKET_H
#include "CoreSocket_def.h"
#include <intrin.h>

class CEvt
{
public:
    CEvt(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPCTSTR pszName = nullptr, LPSECURITY_ATTRIBUTES pSecurity = nullptr)
    {
        m_hEvent = ::CreateEvent(pSecurity, bManualReset, bInitialState, pszName);
        ASSERT(IsValid());
    }
    ~CEvt()
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hEvent));
    }
    BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hEvent));
        m_hEvent = ::OpenEvent(dwAccess, bInheritHandle, pszName);
        return (IsValid());
    }
    BOOL Pulse()
    {
        return (::PulseEvent(m_hEvent));
    }
    BOOL Reset()
    {
        return (::ResetEvent(m_hEvent));
    }
    BOOL Set()
    {
        return (::SetEvent(m_hEvent));
    }
    HANDLE& GetHandle()
    {
        return m_hEvent;
    }
    operator HANDLE()
    {
        return m_hEvent;
    }
    BOOL IsValid()
    {
        return m_hEvent != nullptr;
    }
private:
    CEvt(const CEvt&);
    CEvt operator = (const CEvt&);
private:
    HANDLE m_hEvent;
};

class CSEM
{
public:
    CSEM(LONG lMaximumCount, LONG lInitialCount = 0, LPCTSTR lpName = nullptr, LPSECURITY_ATTRIBUTES pSecurity = nullptr)
    {
        m_hsem = ::CreateSemaphore(pSecurity, lInitialCount, lMaximumCount, lpName);
        ASSERT(IsValid());
    }
    ~CSEM()
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hsem));
    }
    BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hsem));
        m_hsem = ::OpenSemaphore(dwAccess, bInheritHandle, pszName);
        return (IsValid());
    }
    void Wait(DWORD dwMilliseconds = INFINITE)
    {
        ::WaitForSingleObject(m_hsem, dwMilliseconds);
    }
    BOOL Release(LONG lReleaseCount = 1, LPLONG lpPreviousCount = nullptr)
    {
        return ::ReleaseSemaphore(m_hsem, lReleaseCount, lpPreviousCount);
    }
    HANDLE& GetHandle()
    {
        return m_hsem;
    }
    operator HANDLE()
    {
        return m_hsem;
    }
    BOOL IsValid()
    {
        return m_hsem != nullptr;
    }
private:
    CSEM(const CSEM& sem);
    CSEM operator = (const CSEM& sem);
private:
    HANDLE m_hsem;
};

class CInterCriSec
{
public:
    CInterCriSec(DWORD dwSpinCount = DEFAULT_CRISEC_SPIN_COUNT)
    {
        VERIFY(::InitializeCriticalSectionAndSpinCount(&m_crisec, dwSpinCount));
    }
    ~CInterCriSec()
    {
        ::DeleteCriticalSection(&m_crisec);
    }
    void Lock()
    {
        ::EnterCriticalSection(&m_crisec);
    }
    void Unlock()
    {
        ::LeaveCriticalSection(&m_crisec);
    }
    BOOL TryLock()
    {
        return ::TryEnterCriticalSection(&m_crisec);
    }
    DWORD SetSpinCount(DWORD dwSpinCount)
    {
        return ::SetCriticalSectionSpinCount(&m_crisec, dwSpinCount);
    }
    CRITICAL_SECTION* GetObject()
    {
        return &m_crisec;
    }
private:
    CInterCriSec(const CInterCriSec& cs);
    CInterCriSec operator = (const CInterCriSec& cs);
private:
    CRITICAL_SECTION m_crisec;
};

class CInterCriSec2
{
public:
    CInterCriSec2(DWORD dwSpinCount = DEFAULT_CRISEC_SPIN_COUNT, BOOL bInitialize = TRUE)
    {
        if (bInitialize)
        {
            m_pcrisec = new CRITICAL_SECTION;
            VERIFY(::InitializeCriticalSectionAndSpinCount(m_pcrisec, dwSpinCount));
        }
        else
            m_pcrisec = nullptr;
    }
    ~CInterCriSec2()
    {
        Reset();
    }
    void Attach(CRITICAL_SECTION* pcrisec)
    {
        Reset();
        m_pcrisec = pcrisec;
    }
    CRITICAL_SECTION* Detach()
    {
        CRITICAL_SECTION* pcrisec = m_pcrisec;
        m_pcrisec = nullptr;
        return pcrisec;
    }

    void Lock()
    {
        ::EnterCriticalSection(m_pcrisec);
    }
    void Unlock()
    {
        ::LeaveCriticalSection(m_pcrisec);
    }
    BOOL TryLock()
    {
        return ::TryEnterCriticalSection(m_pcrisec);
    }
    DWORD SetSpinCount(DWORD dwSpinCount)
    {
        return ::SetCriticalSectionSpinCount(m_pcrisec, dwSpinCount);
    }
    CRITICAL_SECTION* GetObject()
    {
        return m_pcrisec;
    }
private:
    CInterCriSec2(const CInterCriSec2& cs);
    CInterCriSec2 operator = (const CInterCriSec2& cs);
    void Reset()
    {
        if (m_pcrisec)
        {
            ::DeleteCriticalSection(m_pcrisec);
            delete m_pcrisec;
            m_pcrisec = nullptr;
        }
    }
private:
    CRITICAL_SECTION* m_pcrisec;
};

class CMTX
{
public:
    CMTX(BOOL bInitialOwner = FALSE, LPCTSTR pszName = nullptr, LPSECURITY_ATTRIBUTES pSecurity = nullptr)
    {
        m_hMutex = ::CreateMutex(pSecurity, bInitialOwner, pszName);
        ASSERT(IsValid());
    }
    ~CMTX()
    {
        if (IsValid())
            ::CloseHandle(m_hMutex);
    }
    BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hMutex));
        m_hMutex = ::OpenMutex(dwAccess, bInheritHandle, pszName);
        return (IsValid());
    }
    void Lock(DWORD dwMilliseconds = INFINITE)
    {
        ::WaitForSingleObject(m_hMutex, dwMilliseconds);
    }
    void Unlock()
    {
        ::ReleaseMutex(m_hMutex);
    }
    HANDLE& GetHandle()
    {
        return m_hMutex;
    }
    operator HANDLE()
    {
        return m_hMutex;
    }
    BOOL IsValid()
    {
        return m_hMutex != nullptr;
    }
private:
    CMTX(const CMTX& mtx);
    CMTX operator = (const CMTX& mtx);
private:
    HANDLE m_hMutex;
};

class CSpinGuard
{
public:
    CSpinGuard() : m_lFlag(0)
    {
    }
    ~CSpinGuard()
    {
        ASSERT(m_lFlag == 0);
    }
    void Lock()
    {
        for (UINT i = 0; !TryLock(); ++i)
            Pause(i);
    }
    BOOL TryLock()
    {
        if (::InterlockedCompareExchange(&m_lFlag, 1, 0) == 0)
        {
            ::_ReadWriteBarrier();
            return TRUE;
        }
        return FALSE;
    }
    void Unlock()
    {
        ASSERT(m_lFlag == 1);
        m_lFlag = 0;
    }
    static void Pause(UINT i)
    {
        if (i < DEFAULT_PAUSE_YIELD)
            YieldProcessor();
        else if (i < DEFAULT_PAUSE_CYCLE - 1)
            SwitchToThread();
        else if (i < DEFAULT_PAUSE_CYCLE)
            Sleep(1);
        else
            Pause(i & (DEFAULT_PAUSE_CYCLE - 1));
    }

private:
    CSpinGuard(const CSpinGuard& cs);
    CSpinGuard operator = (const CSpinGuard& cs);
private:
    volatile LONG m_lFlag;
};

class CReentrantSpinGuard
{
public:
    CReentrantSpinGuard(): m_dwThreadID(0)
        , m_iCount(0)
    {
    }
    ~CReentrantSpinGuard()
    {
        ASSERT(m_dwThreadID == 0);
        ASSERT(m_iCount == 0);
    }
    void Lock()
    {
        for (UINT i = 0; !_TryLock(i == 0); ++i)
            Pause(i);
    }

    BOOL TryLock()
    {
        return _TryLock(TRUE);
    }

    void Unlock()
    {
        ASSERT(m_dwThreadID == ::GetCurrentThreadId());
        if ((--m_iCount) == 0)
            m_dwThreadID = 0;
    }

    static void Pause(UINT i)
    {
        if (i < DEFAULT_PAUSE_YIELD)        YieldProcessor();
        else if (i < DEFAULT_PAUSE_CYCLE - 1)   SwitchToThread();
        else if (i < DEFAULT_PAUSE_CYCLE)       Sleep(1);
        else                                    Pause(i & (DEFAULT_PAUSE_CYCLE - 1));
    }

private:
    CReentrantSpinGuard(const CReentrantSpinGuard& cs);
    CReentrantSpinGuard operator = (const CReentrantSpinGuard& cs);

    BOOL _TryLock(BOOL bFirst)
    {
        DWORD dwCurrentThreadID = ::GetCurrentThreadId();
        if (bFirst && m_dwThreadID == dwCurrentThreadID)
        {
            ++m_iCount;
            return TRUE;
        }
        if (::InterlockedCompareExchange(&m_dwThreadID, dwCurrentThreadID, 0) == 0)
        {
            ::_ReadWriteBarrier();
            ASSERT(m_iCount == 0);
            m_iCount = 1;
            return TRUE;
        }
        return FALSE;
    }

private:
    volatile DWORD  m_dwThreadID;
    int             m_iCount;
};

class CFakeGuard
{
public:
    void Lock()     {}
    void Unlock()   {}
    BOOL TryLock()
    {
        return TRUE;
    }
};

template<class CLockObj> class CLocalLock
{
public:
    CLocalLock(CLockObj& obj) : m_lock(obj)
    {
        m_lock.Lock();
    }
    ~CLocalLock()
    {
        m_lock.Unlock();
    }
private:
    CLockObj& m_lock;
};

typedef CInterCriSec                    CCriSec;
typedef CLocalLock<CCriSec>             CCriSecLock;
typedef CLocalLock<CInterCriSec>        CInterCriSecLock;
typedef CLocalLock<CInterCriSec2>       CInterCriSecLock2;
typedef CLocalLock<CMTX>                CMutexLock;
typedef CLocalLock<CSpinGuard>          CSpinLock;
typedef CLocalLock<CReentrantSpinGuard> CReentrantSpinLock;
typedef CLocalLock<CFakeGuard>          CFakeLock;


class CPrivateHeap
{
public:
    PVOID Alloc(SIZE_T dwSize, DWORD dwFlags = 0)
    {
        return ::HeapAlloc(m_hHeap, dwFlags, dwSize);
    }

    PVOID ReAlloc(PVOID pvMemory, SIZE_T dwSize, DWORD dwFlags = 0)
    {
        return ::HeapReAlloc(m_hHeap, dwFlags, pvMemory, dwSize);
    }

    SIZE_T Size(PVOID pvMemory, DWORD dwFlags = 0)
    {
        return ::HeapSize(m_hHeap, dwFlags, pvMemory);
    }

    BOOL Free(PVOID pvMemory, DWORD dwFlags = 0)
    {
        return ::HeapFree(m_hHeap, dwFlags, pvMemory);
    }

    SIZE_T Compact(DWORD dwFlags = 0)
    {
        return ::HeapCompact(m_hHeap, dwFlags);
    }

    BOOL IsValid()
    {
        return m_hHeap != nullptr;
    }

    BOOL Reset()
    {
        if (IsValid()) ::HeapDestroy(m_hHeap);
        m_hHeap = ::HeapCreate(m_dwOptions, m_dwInitSize, m_dwMaxSize);
        return IsValid();
    }

public:
    CPrivateHeap(DWORD dwOptions = 0, SIZE_T dwInitSize = 0, SIZE_T dwMaxSize = 0)
        : m_dwOptions(dwOptions), m_dwInitSize(dwInitSize), m_dwMaxSize(dwMaxSize)
    {
        m_hHeap = ::HeapCreate(m_dwOptions, m_dwInitSize, m_dwMaxSize);
    }

    ~CPrivateHeap()
    {
        if (IsValid()) ::HeapDestroy(m_hHeap);
    }

    operator HANDLE()
    {
        return m_hHeap;
    }

private:
    CPrivateHeap(const CPrivateHeap&);
    CPrivateHeap operator = (const CPrivateHeap&);

private:
    HANDLE  m_hHeap;
    DWORD   m_dwOptions;
    SIZE_T  m_dwInitSize;
    SIZE_T  m_dwMaxSize;
};

template<class T>
class CPrivateHeapBuffer
{
public:
    CPrivateHeapBuffer(CPrivateHeap& hpPrivate,
                       SIZE_T dwSize = 0,
                       DWORD dwAllocFlags = 0,
                       DWORD dwFreeFlags = 0)
        : m_hpPrivate(hpPrivate)
        , m_dwAllocFlags(dwAllocFlags)
        , m_dwFreeFlags(dwFreeFlags)
        , m_pvMemory(nullptr)
    {
        ASSERT(m_hpPrivate.IsValid());
        Alloc(dwSize);
    }

    ~CPrivateHeapBuffer()
    {
        Free();
    }

public:

    T* Alloc(SIZE_T dwSize)
    {
        if (IsValid())
            Free();
        return m_pvMemory = (T*)m_hpPrivate.Alloc(dwSize * sizeof(T), m_dwAllocFlags);
    }

    T* ReAlloc(SIZE_T dwSize, DWORD dwFlags = 0)
    {
        return m_pvMemory = (T*)m_hpPrivate.ReAlloc(m_pvMemory, dwSize * sizeof(T), dwFlags);
    }

    SIZE_T Size(DWORD dwFlags = 0)
    {
        return m_hpPrivate.Size(m_pvMemory, dwFlags);
    }

    BOOL Free()
    {
        BOOL isOK = TRUE;
        if (IsValid())
        {
            isOK = m_hpPrivate.Free(m_pvMemory, m_dwFreeFlags);
            m_pvMemory = nullptr;
        }
        return isOK;
    }

    BOOL IsValid()
    {
        return m_pvMemory != nullptr;
    }
    operator T* ()  const
    {
        return m_pvMemory;
    }

private:
    CPrivateHeapBuffer(const CPrivateHeapBuffer&);
    CPrivateHeapBuffer operator = (const CPrivateHeapBuffer&);

private:
    CPrivateHeap&   m_hpPrivate;
    T*              m_pvMemory;
    DWORD           m_dwAllocFlags;
    DWORD           m_dwFreeFlags;
};

typedef CPrivateHeapBuffer<BYTE>    CPrivateHeapByteBuffer;
typedef CPrivateHeapBuffer<TCHAR>   CPrivateHeapStrBuffer;


class CSWMR
{
public:
    VOID WaitToRead();
    VOID WaitToWrite();
    VOID ReadDone()
    {
        Done();
    }
    VOID WriteDone()
    {
        Done();
    }

private:
    VOID Done();

public:
    CSWMR();
    ~CSWMR();

private:
    CSWMR(const CSWMR&);
    CSWMR operator = (const CSWMR&);

private:
    int m_nWaitingReaders;
    int m_nWaitingWriters;
    int m_nActive;

    CSpinGuard  m_cs;
    CSEM        m_smRead;
    CSEM        m_smWrite;
};

#if _WIN32_WINNT >= _WIN32_WINNT_WS08

class CSlimLock
{
public:
    VOID WaitToRead()
    {
        ::AcquireSRWLockShared(&m_lock);
    }
    VOID WaitToWrite()
    {
        ::AcquireSRWLockExclusive(&m_lock);
    }
    VOID ReadDone()
    {
        ::ReleaseSRWLockShared(&m_lock);
    }
    VOID WriteDone()
    {
        ::ReleaseSRWLockExclusive(&m_lock);
    }
    BOOL TryWaitToRead()
    {
        return ::TryAcquireSRWLockShared(&m_lock);
    }
    BOOL TryWaitToWrite()
    {
        return ::TryAcquireSRWLockExclusive(&m_lock);
    }

    SRWLOCK* GetObject()
    {
        return &m_lock;
    }

public:
    CSlimLock()
    {
        ::InitializeSRWLock(&m_lock);
    }
    ~CSlimLock()    {}

private:
    CSlimLock(const CSlimLock&);
    CSlimLock operator = (const CSlimLock&);

private:
    SRWLOCK m_lock;
};

#endif

class CRWLock
{
public:
    VOID WaitToRead();
    VOID WaitToWrite();
    VOID ReadDone();
    VOID WriteDone();

private:
    VOID Done(CSEM** ppSem, LONG& lCount);
    BOOL IsOwner()
    {
        return m_dwWriterTID == ::GetCurrentThreadId();
    }
    VOID SetOwner()
    {
        m_dwWriterTID = ::GetCurrentThreadId();
    }
    VOID DetachOwner()
    {
        m_dwWriterTID = 0;
    }

public:
    CRWLock();
    ~CRWLock();

private:
    CRWLock(const CRWLock&);
    CRWLock operator = (const CRWLock&);

private:
    int m_nWaitingReaders;
    int m_nWaitingWriters;
    int m_nActive;
    DWORD m_dwWriterTID;

    CSpinGuard  m_cs;
    CSEM        m_smRead;
    CSEM        m_smWrite;
};

template<class CLockObj> class CLocalReadLock
{
public:
    CLocalReadLock(CLockObj& obj) : m_wait(obj)
    {
        m_wait.WaitToRead();
    }
    ~CLocalReadLock()
    {
        m_wait.ReadDone();
    }
private:
    CLocalReadLock(const CLocalReadLock&);
    CLocalReadLock operator = (const CLocalReadLock&);
private:
    CLockObj& m_wait;
};

template<class CLockObj> class CLocalWriteLock
{
public:
    CLocalWriteLock(CLockObj& obj) : m_wait(obj)
    {
        m_wait.WaitToWrite();
    }
    ~CLocalWriteLock()
    {
        m_wait.WriteDone();
    }
private:
    CLocalWriteLock(const CLocalWriteLock&);
    CLocalWriteLock operator = (const CLocalWriteLock&);
private:
    CLockObj& m_wait;
};

#if _WIN32_WINNT >= _WIN32_WINNT_WS08
    typedef CSlimLock   CSimpleRWLock;
#else
    typedef CSWMR       CSimpleRWLock;
#endif

typedef CLocalReadLock<CSimpleRWLock>   CReadLock;
typedef CLocalWriteLock<CSimpleRWLock>  CWriteLock;
typedef CLocalReadLock<CRWLock>         CReentrantReadLock;
typedef CLocalWriteLock<CRWLock>        CReentrantWriteLock;

#if _WIN32_WINNT >= _WIN32_WINNT_WS08

class ICVCondition
{
public:
    virtual BOOL Detect() = 0;

public:
    virtual ~ICVCondition() {}
};

class CCVCriSec
{
public:
    CCVCriSec(CInterCriSec& cs)
        : m_cs(cs)
    {
        ::InitializeConditionVariable(&m_cv);
    }

    ~CCVCriSec() {}

    void WaitToRead(ICVCondition* pCondition)
    {
        Wait(pCondition);
    }

    void WaitToWrite(ICVCondition* pCondition)
    {
        Wait(pCondition);
    }

    void ReadDone()
    {
        Done();
    }

    void WriteDone()
    {
        Done();
    }

    void WakeUp()
    {
        ::WakeConditionVariable(&m_cv);
    }

    void WakeUpAll()
    {
        ::WakeAllConditionVariable(&m_cv);
    }

private:
    void Wait(ICVCondition* pCondition)
    {
        ASSERT(pCondition);
        m_cs.Lock();
        while (!pCondition->Detect())
            ::SleepConditionVariableCS(&m_cv, m_cs.GetObject(), INFINITE);
    }

    void Done()
    {
        m_cs.Unlock();
    }

private:
    CCVCriSec(const CCVCriSec& cs);
    CCVCriSec operator = (const CCVCriSec& cs);

private:
    CInterCriSec&       m_cs;
    CONDITION_VARIABLE  m_cv;
};

class CCVSlim
{
public:
    CCVSlim(CSlimLock& cs)
        : m_cs(cs)
    {
        ::InitializeConditionVariable(&m_cv);
    }

    ~CCVSlim() {}

    void WaitToRead(ICVCondition* pCondition)
    {
        ASSERT(pCondition);
        m_cs.WaitToRead();
        while (!pCondition->Detect())
            ::SleepConditionVariableSRW(&m_cv, m_cs.GetObject(), INFINITE, CONDITION_VARIABLE_LOCKMODE_SHARED);
    }

    void WaitToWrite(ICVCondition* pCondition)
    {
        ASSERT(pCondition);
        m_cs.WaitToWrite();
        while (!pCondition->Detect())
            ::SleepConditionVariableSRW(&m_cv, m_cs.GetObject(), INFINITE, 0);
    }

    void ReadDone()
    {
        m_cs.ReadDone();
    }

    void WriteDone()
    {
        m_cs.WriteDone();
    }

    void WakeUp()
    {
        ::WakeConditionVariable(&m_cv);
    }

    void WakeUpAll()
    {
        ::WakeAllConditionVariable(&m_cv);
    }

private:
    CCVSlim(const CCVSlim& cs);
    CCVSlim operator = (const CCVSlim& cs);

private:
    CSlimLock&          m_cs;
    CONDITION_VARIABLE  m_cv;
};

template<class _Lock, class _Var> class CCVGuard
{
public:
    void WaitForProduce()
    {
        m_cvP.WaitToWrite(m_pcdtProduce);
    }

    void WaitForConsume()
    {
        m_cvC.WaitToRead(m_pcdtConsume);
    }

    void ProduceDone()
    {
        m_cvP.WriteDone();
    }

    void WakeUpProduce()
    {
        m_cvP.WakeUp();
    }

    void ConsumeDone()
    {
        m_cvC.ReadDone();
    }

    void WakeUpConsume()
    {
        m_cvC.WakeUp();
    }

    void WakeUpAllConsumes()
    {
        m_cvC.WakeUpAll();
    }

public:
    CCVGuard(ICVCondition* pcdtProduce, ICVCondition* pcdtConsume)
        : m_cvP(m_cs)
        , m_cvC(m_cs)
        , m_pcdtProduce(pcdtProduce)
        , m_pcdtConsume(pcdtConsume)
    {
        ASSERT(m_pcdtConsume && m_pcdtProduce);
    }

    ~CCVGuard() {}

private:
    CCVGuard(const CCVGuard& cs);
    CCVGuard operator = (const CCVGuard& cs);

private:
    ICVCondition* m_pcdtProduce;
    ICVCondition* m_pcdtConsume;

    _Lock   m_cs;
    _Var    m_cvP;
    _Var    m_cvC;
};

template<class _GuardObj> class CConsumeLock
{
public:
    CConsumeLock(_GuardObj& obj) : m_guard(obj)
    {
        m_guard.WaitForConsume();
    }
    ~CConsumeLock()
    {
        m_guard.ConsumeDone();
    }
private:
    CConsumeLock(const CConsumeLock&);
    CConsumeLock operator = (const CConsumeLock&);
private:
    _GuardObj& m_guard;
};

template<class _GuardObj> class CProduceLock
{
public:
    CProduceLock(_GuardObj& obj) : m_guard(obj)
    {
        m_guard.WaitForProduce();
    }
    ~CProduceLock()
    {
        m_guard.ProduceDone();
    }
private:
    CProduceLock(const CProduceLock&);
    CProduceLock operator = (const CProduceLock&);
private:
    _GuardObj& m_guard;
};

typedef CCVGuard<CInterCriSec, CCVCriSec>   CCVGuardCS;
typedef CCVGuard<CSlimLock, CCVSlim>        CCVGuardSRW;
typedef CProduceLock<CCVGuardCS>            CProduceLockCS;
typedef CConsumeLock<CCVGuardCS>            CConsumeLockCS;
typedef CProduceLock<CCVGuardSRW>           CProduceLockSRW;
typedef CConsumeLock<CCVGuardSRW>           CConsumeLockSRW;

#endif

// 获取系统信息
VOID SysGetSystemInfo(LPSYSTEM_INFO pInfo);
// 获取 CPU 核数
DWORD SysGetNumberOfProcessors();
// 获取页面大小
DWORD SysGetPageSize();

#if _MSC_VER < 1800

    // 获取操作系统版本
    BOOL SysGetOSVersionInfo(LPOSVERSIONINFO pInfo, BOOL bInfoEx = FALSE);
    // 获取操作系统版本（高位双字节：主版本号；低位双字节：副版本号）
    DWORD SysGetOSVersion();

#endif

// timeGetTime() 包装方法
DWORD TimeGetTime();

//获取当前时间与原始时间的时间差 参数:dwOriginal: 原始时间（毫秒），通常用 timeGetTime() 或 GetTickCount() 获取
DWORD GetTimeGap32(DWORD dwOriginal);

#if _WIN32_WINNT >= _WIN32_WINNT_WS08
    //获取当前时间与原始时间的时间差 参数:ullOriginal: 原始时间（毫秒），通常用 GetTickCount64() 获取
    ULONGLONG GetTimeGap64(ULONGLONG ullOriginal);
#endif

//处理Windows消息 参数:bDispatchQuitMsg: 是否转发 WM_QUIT 消息返回值:TRUE:收完消息FALSE:bDispatchQuitMsg 参数为FALSE 并收到 WM_QUIT 消息
BOOL PeekMessageLoop(BOOL bDispatchQuitMsg = TRUE);

/**********************************
    描述: 等待指定时间, 同时处理Windows消息
    参数: (参考: MsgWaitForMultipleObjectsEx() )
    dwHandles       : 数组元素个数
    szHandles       : 对象句柄数组
    dwMilliseconds  : 等待时间 (毫秒)
    dwWakeMask      : 消息过滤标识
    dwFlags         : 等待类型

    返回值: (0 ~ dwHandles - 1): 等待成功
    WAIT_TIMEOUT        : 超时
    WAIT_FAILED         : 执行失败
**********************************/
DWORD WaitForMultipleObjectsWithMessageLoop(DWORD dwHandles, HANDLE szHandles[], DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
    描述: 等待指定时间, 同时处理Windows消息
    参数: (参考: MsgWaitForMultipleObjectsEx() )
    hHandle         : 对象句柄
    dwMilliseconds  : 等待时间 (毫秒)
    dwWakeMask      : 消息过滤标识
    dwFlags         : 等待类型

    返回值: TRUE: 等待成功，FALSE: 超时
**********************************/
BOOL MsgWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
    描述: 等待指定时间, 同时处理Windows消息
    参数: (参考: MsgWaitForMultipleObjectsEx() )
    dwMilliseconds  : 等待时间 (毫秒)
    dwWakeMask      : 消息过滤标识
    dwFlags         : 等待类型

    返回值: MsgWaitForMultipleObjectsEx() 函数的操作结果
**********************************/
void WaitWithMessageLoop(DWORD dwMilliseconds, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
    描述: 等待用Sleep()函数等待某个变量小于指定值
    参数:
    plWorkingItemCount      : 监视变量
    lMaxWorkingItemCount    : 指定值
    dwCheckInterval         : 检查间隔 (毫秒)

    返回值:
**********************************/
void WaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval);
/**********************************
    描述: 等待用Sleep()函数等待某个变量减小到 0
    参数:
    plWorkingItemCount      : 监视变量
    dwCheckInterval         : 检查间隔 (毫秒)

    返回值:
**********************************/
void WaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval);

/**********************************
    描述: 等待用WaitWithMessageLoop()函数等待某个变量小于指定值
    参数:
    plWorkingItemCount      : 监视变量
    lMaxWorkingItemCount    : 指定值
    dwCheckInterval         : 检查间隔 (毫秒)

    返回值:
**********************************/
void MsgWaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval = 10);
/**********************************
    描述: 等待用WaitWithMessageLoop()函数等待某个变量减小到 0
    参数:
    plWorkingItemCount      : 监视变量
    dwCheckInterval         : 检查间隔 (毫秒)

    返回值:
**********************************/
void MsgWaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval = 10);


struct TItem
{
    template<typename T>
    friend struct TSimpleList;
    friend struct TItemList;
    friend class CItemPool;
    friend struct TBuffer;
public:
    inline int Cat(const BYTE* pData, int length);
    inline int Cat(const TItem& other);
    inline int Fetch(BYTE* pData, int length);
    inline int Peek(BYTE* pData, int length);
    inline int Reduce(int length);
    inline void Reset(int first = 0, int last = 0);

    BYTE* Ptr()
    {
        return begin;
    }
    const BYTE* Ptr() const
    {
        return begin;
    }
    int Size() const
    {
        return (int)(end - begin);
    }
    int Remain() const
    {
        return capacity - (int)(end - head);
    }
    int Capacity() const
    {
        return capacity;
    }
    bool IsEmpty() const
    {
        return Size() == 0;
    }
    bool IsFull() const
    {
        return Remain() == 0;
    }
public:
    operator BYTE* ()
    {
        return Ptr();
    }
    operator const BYTE* () const
    {
        return Ptr();
    }
public:
    static TItem* Construct(CPrivateHeap& heap,
                            int capacity = DEFAULT_ITEM_CAPACITY,
                            BYTE* pData = nullptr,
                            int length = 0);
    static void Destruct(TItem* pItem);
private:
    TItem(CPrivateHeap& hp, int cap = DEFAULT_ITEM_CAPACITY, BYTE* pData = nullptr, int length = 0)
        : heap(hp), capacity(cap), begin(head), end(head), next(nullptr), last(nullptr)
    {
        if (pData != nullptr && length != 0)
            Cat(pData, length);
    }
    ~TItem() {}
    DECLARE_NO_COPY_CLASS(TItem)
public:
    static const DWORD DEFAULT_ITEM_CAPACITY;
private:
    CPrivateHeap& heap;
private:
    TItem* next;
    TItem* last;

    int     capacity;
    BYTE*   head;
    BYTE*   begin;
    BYTE*   end;
};

template<class T> struct TSimpleList
{
public:
    T* PushFront(T* pItem)
    {
        if (pFront != nullptr)
        {
            pFront->last = pItem;
            pItem->next = pFront;
        }
        else
        {
            pItem->last = nullptr;
            pItem->next = nullptr;
            pBack = pItem;
        }
        pFront = pItem;
        ++size;
        return pItem;
    }

    T* PushBack(T* pItem)
    {
        if (pBack != nullptr)
        {
            pBack->next = pItem;
            pItem->last = pBack;
        }
        else
        {
            pItem->last = nullptr;
            pItem->next = nullptr;
            pFront = pItem;
        }
        pBack = pItem;
        ++size;
        return pItem;
    }

    T* PopFront()
    {
        T* pItem = pFront;
        if (pFront != pBack)
        {
            pFront = pFront->next;
            pFront->last = nullptr;
        }
        else if (pFront != nullptr)
        {
            pFront = nullptr;
            pBack = nullptr;
        }
        if (pItem != nullptr)
        {
            pItem->next = nullptr;
            pItem->last = nullptr;
            --size;
        }
        return pItem;
    }

    T* PopBack()
    {
        T* pItem = pBack;
        if (pFront != pBack)
        {
            pBack = pBack->last;
            pBack->next = nullptr;
        }
        else if (pBack != nullptr)
        {
            pFront = nullptr;
            pBack = nullptr;
        }
        if (pItem != nullptr)
        {
            pItem->next = nullptr;
            pItem->last = nullptr;
            --size;
        }
        return pItem;
    }

    TSimpleList<T>& Shift(TSimpleList<T>& other)
    {
        if (&other != this && other.size > 0)
        {
            if (size > 0)
            {
                pBack->next = other.pFront;
                other.pFront->last = pBack;
            }
            else
                pFront = other.pFront;
            pBack = other.pBack;
            size += other.size;
            other.Reset();
        }
        return *this;
    }

    void Clear()
    {
        if (size > 0)
        {
            T* pItem;
            while ((pItem = PopFront()) != nullptr)
                T::Destruct(pItem);
        }
    }

    T*  Front()  const
    {
        return pFront;
    }
    T*  Back()  const
    {
        return pBack;
    }
    int Size()  const
    {
        return size;
    }
    bool    IsEmpty()  const
    {
        return size == 0;
    }

public:
    TSimpleList()
    {
        Reset();
    }
    ~TSimpleList()
    {
        Clear();
    }

    DECLARE_NO_COPY_CLASS(TSimpleList<T>)

private:
    void Reset()
    {
        pFront = nullptr;
        pBack = nullptr;
        size = 0;
    }

private:
    int size;
    T*  pFront;
    T*  pBack;
};

struct TItemList : public TSimpleList<TItem>
{
public:
    int Cat(const BYTE* pData, int length);
    int Cat(const TItem* pItem);
    int Cat(const TItemList& other);
    int Fetch(BYTE* pData, int length);
    int Peek(BYTE* pData, int length);
    int Reduce(int length);
    void Release();

public:
    TItemList(CItemPool& pool) : itPool(pool)
    {
    }
private:
    CItemPool& itPool;
};

struct TItemListEx : public TItemList
{
public:
    TItem* PushFront(TItem* pItem)
    {
        length += pItem->Size();
        return __super::PushFront(pItem);
    }

    TItem* PushBack(TItem* pItem)
    {
        length += pItem->Size();
        return __super::PushBack(pItem);
    }

    TItem* PopFront()
    {
        TItem* pItem = __super::PopFront();
        if (pItem != nullptr)
            length -= pItem->Size();
        return pItem;
    }

    TItem* PopBack()
    {
        TItem* pItem = __super::PopBack();
        if (pItem != nullptr)
            length -= pItem->Size();
        return pItem;
    }

    TItemListEx& Shift(TItemListEx& other)
    {
        if (&other != this && other.length > 0)
        {
            length += other.length;
            other.length = 0;
            __super::Shift(other);
        }
        return *this;
    }

    void Clear()
    {
        __super::Clear();
        length = 0;
    }

    void Release()
    {
        __super::Release();
        length = 0;
    }

public:
    int Cat(const BYTE* pData, int length)
    {
        int cat = __super::Cat(pData, length);
        this->length += cat;
        return cat;
    }

    int Cat(const TItem* pItem)
    {
        int cat = __super::Cat(pItem->Ptr(), pItem->Size());
        this->length += cat;
        return cat;
    }

    int Cat(const TItemList& other)
    {
        int cat = __super::Cat(other);
        this->length += cat;
        return cat;
    }

    int Fetch(BYTE* pData, int length)
    {
        int fetch = __super::Fetch(pData, length);
        this->length -= fetch;
        return fetch;
    }

    int Reduce(int length)
    {
        int reduce = __super::Reduce(length);
        this->length -= reduce;
        return reduce;
    }

    int Length() const
    {
        return length;
    }

public:
    TItemListEx(CItemPool& pool) : TItemList(pool), length(0)
    {
    }

    ~TItemListEx()
    {
        ASSERT(length >= 0);
    }

    DECLARE_NO_COPY_CLASS(TItemListEx)

private:
    int length;
};

class CItemPool
{
public:
    void PutFreeItem(TItem* pItem);
    void PutFreeItem(TItemList& lsItem);
    TItem* PickFreeItem();

    inline void Clear();

private:
    void CompressFreeItem(int size);

public:
    void SetItemCapacity(DWORD dwItemCapacity)
    {
        m_dwItemCapacity = dwItemCapacity;
    }
    void SetPoolSize(DWORD dwPoolSize)
    {
        m_dwPoolSize = dwPoolSize;
    }
    void SetPoolHold(DWORD dwPoolHold)
    {
        m_dwPoolHold = dwPoolHold;
    }
    DWORD GetItemCapacity()
    {
        return m_dwItemCapacity;
    }
    DWORD GetPoolSize()
    {
        return m_dwPoolSize;
    }
    DWORD GetPoolHold()
    {
        return m_dwPoolHold;
    }

public:
    CItemPool(DWORD dwPoolSize = DEFAULT_POOL_SIZE,
              DWORD dwPoolHold = DEFAULT_POOL_HOLD,
              DWORD dwItemCapacity = DEFAULT_ITEM_CAPACITY)
        : m_dwPoolSize(dwPoolSize)
        , m_dwPoolHold(dwPoolHold)
        , m_dwItemCapacity(dwItemCapacity)
        , m_lsFreeItem(*this)
    {
    }

    ~CItemPool()
    {
        Clear();
    }

    DECLARE_NO_COPY_CLASS(CItemPool)

public:
    static const DWORD DEFAULT_ITEM_CAPACITY;
    static const DWORD DEFAULT_POOL_SIZE;
    static const DWORD DEFAULT_POOL_HOLD;

private:
    CPrivateHeap    m_heap;

    DWORD           m_dwItemCapacity;
    DWORD           m_dwPoolSize;
    DWORD           m_dwPoolHold;

    CCriSec         m_csFreeItem;
    TItemList       m_lsFreeItem;
};

struct TItemPtr
{
public:
    TItem* Reset(TItem* pItem = nullptr)
    {
        if (m_pItem != nullptr)
            itPool.PutFreeItem(m_pItem);
        m_pItem = pItem;
        return m_pItem;
    }

    TItem* Attach(TItem* pItem)
    {
        return Reset(pItem);
    }

    TItem* Detach()
    {
        TItem* pItem = m_pItem;
        m_pItem = nullptr;
        return pItem;
    }

    bool IsValid()
    {
        return m_pItem != nullptr;
    }
    TItem* operator -> ()
    {
        return m_pItem;
    }
    TItem* operator = (TItem* pItem)
    {
        return Reset(pItem);
    }
    operator TItem*         ()
    {
        return m_pItem;
    }
    TItem* Ptr()
    {
        return m_pItem;
    }
    const TItem* Ptr()  const
    {
        return m_pItem;
    }
    operator const TItem*   ()  const
    {
        return m_pItem;
    }

public:
    TItemPtr(CItemPool& pool, TItem* pItem = nullptr)
        : itPool(pool), m_pItem(pItem)
    {
    }

    ~TItemPtr()
    {
        Reset();
    }

    DECLARE_NO_COPY_CLASS(TItemPtr)

private:
    CItemPool&  itPool;
    TItem*      m_pItem;
};

struct TBuffer
{
    template<typename T> friend struct TSimpleList;
    friend class CBufferPool;

public:
    static TBuffer* Construct(CBufferPool& pool, ULONG_PTR dwID);
    static void Destruct(TBuffer* pBuffer);

public:
    int Cat(const BYTE* pData, int len);
    int Cat(const TItem* pItem);
    int Cat(const TItemList& other);
    int Fetch(BYTE* pData, int length);
    int Peek(BYTE* pData, int length);
    int Reduce(int len);

public:
    CCriSec& CriSec()
    {
        return cs;
    }
    TItemList& ItemList()
    {
        return items;
    }

    ULONG_PTR ID() const
    {
        return id;
    }
    int Length() const
    {
        return length;
    }
    bool IsValid() const
    {
        return id != 0;
    }

private:
    int IncreaseLength(int len)
    {
        return (length += len);
    }
    int DecreaseLength(int len)
    {
        return (length -= len);
    }

    inline void Reset();

private:
    TBuffer(CPrivateHeap& hp, CItemPool& itPool, ULONG_PTR dwID = 0)
        : heap(hp), items(itPool), id(dwID), length(0)
    {
    }

    ~TBuffer()  {}

    DECLARE_NO_COPY_CLASS(TBuffer)

private:
    CPrivateHeap& heap;

private:
    ULONG_PTR id;
    int length;
    DWORD freeTime;

private:
    TBuffer* next;
    TBuffer* last;

    CCriSec cs;
    TItemList items;
};

typedef TSimpleList<TBuffer>                TBufferList;

typedef unordered_map<ULONG_PTR, TBuffer*>  TBufferPtrMap;
typedef TBufferPtrMap::iterator             TBufferPtrMapI;
typedef TBufferPtrMap::const_iterator       TBufferPtrMapCI;

class CBufferPool
{
public:
    void        PutFreeBuffer(ULONG_PTR dwID);
    void        PutCacheBuffer(ULONG_PTR dwID);
    TBuffer*    PickFreeBuffer(ULONG_PTR dwID);
    TBuffer*    FindCacheBuffer(ULONG_PTR dwID);

    void        Clear();

private:
    void PutFreeBuffer(TBuffer* pBuffer);
    void CompressFreeBuffer(int size);

public:
    void SetItemCapacity(DWORD dwItemCapacity)
    {
        m_itPool.SetItemCapacity(dwItemCapacity);
    }
    void SetItemPoolSize(DWORD dwItemPoolSize)
    {
        m_itPool.SetPoolSize(dwItemPoolSize);
    }
    void SetItemPoolHold(DWORD dwItemPoolHold)
    {
        m_itPool.SetPoolHold(dwItemPoolHold);
    }

    void SetBufferLockTime(DWORD dwBufferLockTime)
    {
        m_dwBufferLockTime = dwBufferLockTime;
    }
    void SetBufferPoolSize(DWORD dwBufferPoolSize)
    {
        m_dwBufferPoolSize = dwBufferPoolSize;
    }
    void SetBufferPoolHold(DWORD dwBufferPoolHold)
    {
        m_dwBufferPoolHold = dwBufferPoolHold;
    }

    DWORD GetItemCapacity()
    {
        return m_itPool.GetItemCapacity();
    }
    DWORD GetItemPoolSize()
    {
        return m_itPool.GetPoolSize();
    }
    DWORD GetItemPoolHold()
    {
        return m_itPool.GetPoolHold();
    }

    DWORD GetBufferLockTime()
    {
        return m_dwBufferLockTime;
    }
    DWORD GetBufferPoolSize()
    {
        return m_dwBufferPoolSize;
    }
    DWORD GetBufferPoolHold()
    {
        return m_dwBufferPoolHold;
    }

    TBuffer* operator [](ULONG_PTR dwID)
    {
        return FindCacheBuffer(dwID);
    }

public:
    CBufferPool(DWORD dwPoolSize = DEFAULT_BUFFER_POOL_SIZE,
                DWORD dwPoolHold = DEFAULT_BUFFER_POOL_HOLD,
                DWORD dwLockTime = DEFAULT_BUFFER_LOCK_TIME,
                DWORD dwItemCapacity = DEFAULT_ITEM_CAPACITY)
        : m_dwBufferPoolSize(dwPoolSize)
        , m_dwBufferPoolHold(dwPoolHold)
        , m_dwBufferLockTime(dwLockTime)
    {
        m_itPool.SetItemCapacity(dwItemCapacity);
    }

    ~CBufferPool()
    {
        Clear();
    }

    DECLARE_NO_COPY_CLASS(CBufferPool)

public:
    CPrivateHeap&   GetPrivateHeap()
    {
        return m_heap;
    }
    CItemPool&      GetItemPool()
    {
        return m_itPool;
    }

public:
    static const DWORD DEFAULT_ITEM_CAPACITY;
    static const DWORD DEFAULT_ITEM_POOL_SIZE;
    static const DWORD DEFAULT_ITEM_POOL_HOLD;
    static const DWORD DEFAULT_BUFFER_LOCK_TIME;
    static const DWORD DEFAULT_BUFFER_POOL_SIZE;
    static const DWORD DEFAULT_BUFFER_POOL_HOLD;

private:
    DWORD           m_dwBufferLockTime;
    DWORD           m_dwBufferPoolSize;
    DWORD           m_dwBufferPoolHold;

    CPrivateHeap    m_heap;
    CItemPool       m_itPool;

    CRWLock         m_csBufferMap;
    TBufferPtrMap   m_mpBuffer;

    CCriSec         m_csFreeBuffer;
    TBufferList     m_lsFreeBuffer;
};








class CSocket
{
public:
    CSocket();
    ~CSocket();
public:
    bool InitSocket(void);
    bool ExitSocket(void);
    bool StartServer(void);
};






#endif
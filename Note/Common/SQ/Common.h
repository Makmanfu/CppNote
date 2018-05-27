
#pragma once
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS
#endif

#if _MSC_VER >= 1400

#if defined _DEBUG && _MSC_VER < 1600
#ifndef _SECURE_SCL
#define _SECURE_SCL                 0
#endif
#ifndef _HAS_ITERATOR_DEBUGGING
#define _HAS_ITERATOR_DEBUGGING     0
#endif
#endif

#ifndef _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_DEPRECATE        1
#endif

#ifndef _SCL_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_DEPRECATE        1
#endif

#ifndef _ATL_SECURE_NO_WARNINGS
#define _ATL_SECURE_NO_WARNINGS         1
#endif

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS   1
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL                     1
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#endif

#endif

#ifndef _WIN32_WINNT
#if defined (_WIN64)
#define _WIN32_WINNT    _WIN32_WINNT_WIN7
#else
#define _WIN32_WINNT    _WIN32_WINNT_WINXP
#endif
#endif

#ifndef WINVER
#define WINVER  _WIN32_WINNT
#endif

#if _MSC_VER >= 1600
#include <SDKDDKVer.h>
#else
#if !defined(nullptr)
#define nullptr NULL
#endif
#endif

#ifdef _DETECT_MEMORY_LEAK
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#endif

#ifdef _USE_MFC

#ifndef _AFX_ALL_WARNINGS
#define _AFX_ALL_WARNINGS
#endif

#include <afxwin.h>
#include <afxext.h>
#include <afxdisp.h>

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>
#endif

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

#ifndef _NO_RIBBONS_SUPPORT
#include <afxcontrolbars.h>
#endif

#else
#include <Windows.h>
#include <WindowsX.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <atlstr.h>
#include <atltime.h>

#ifndef ASSERT
#define ASSERT(f)   ATLASSERT(f)
#endif
#ifndef VERIFY
#define VERIFY(f)   ATLVERIFY(f)
#endif

#ifndef TRACE
#include <atltrace.h>

#define TRACE                           AtlTrace
#define TRACE0(f)                       TRACE(f)
#define TRACE1(f, p1)                   TRACE(f, p1)
#define TRACE2(f, p1, p2)               TRACE(f, p1, p2)
#define TRACE3(f, p1, p2, p3)           TRACE(f, p1, p2, p3)
#define TRACE4(f, p1, p2, p3, p4)       TRACE(f, p1, p2, p3, p4)
#define TRACE5(f, p1, p2, p3, p4, p5)   TRACE(f, p1, p2, p3, p4, p5)
#endif

#endif

#ifdef _WINSOCK_SUPPORT
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#endif

#include <atlbase.h>
#include <atlconv.h>

#define SINGLETON_THIS(ClassName)       ClassName::GetThis()
#define SINGLETON_INSTANCE(ClassName)   ClassName::GetInstance()
#define SINGLETON_OBJECT(ObjName)       SINGLETON_INSTANCE(C##ObjName)

#define DEFINE_SINGLETON(ClassName)                                         \
    ClassName* ClassName::m_pThis = nullptr;

#define DEFINE_P_THIS(ClassName)                                            \
    DEFINE_SINGLETON(ClassName)

#define DECLARE_SINGLETON_INTERFACE(ClassName)                              \
public:                                                                     \
    static ClassName* GetThis()     {return m_pThis;}                       \
    static ClassName& GetInstance() {return *m_pThis;}                      \
protected:                                                                  \
    static ClassName* m_pThis;

#define DECLARE_SINGLETON_CREATE_INSTANCE(ClassName)                        \
public:                                                                     \
    static BOOL CreateInstance()                                            \
    {                                                                       \
        if(!m_pThis)                                                        \
            m_pThis = new ClassName;                                        \
        return m_pThis != nullptr;                                          \
    }                                                                       \
    static BOOL DeleteInstance()                                            \
    {                                                                       \
        if(m_pThis)                                                         \
        {                                                                   \
            delete m_pThis;                                                 \
            m_pThis = nullptr;                                              \
        }                                                                   \
        return m_pThis == nullptr;                                          \
    }

#define DECLARE_PRIVATE_DEFAULT_CONSTRUCTOR(ClassName)                      \
private:                                                                    \
    ClassName(){}

#define DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)                         \
private:                                                                    \
    ClassName(const ClassName&);                                            \
    ClassName& operator = (const ClassName&);

#define DECLARE_NO_COPY_CLASS(className)                                    \
    DECLARE_PRIVATE_COPY_CONSTRUCTOR(className)


#define DECLARE_SINGLETON_IMPLEMENT_NO_CREATE_INSTANCE(ClassName)           \
    DECLARE_SINGLETON_INTERFACE(ClassName)                                  \
    DECLARE_PRIVATE_DEFAULT_CONSTRUCTOR(ClassName)                          \
    DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)

#define DECLARE_SINGLETON_IMPLEMENT_NO_DEFAULT_CONSTRUCTOR(ClassName)       \
    DECLARE_SINGLETON_CREATE_INSTANCE(ClassName)                            \
    DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)

#define DECLARE_SINGLETON_IMPLEMENT(ClassName)                              \
    DECLARE_SINGLETON_IMPLEMENT_NO_DEFAULT_CONSTRUCTOR(ClassName)           \
    DECLARE_PRIVATE_DEFAULT_CONSTRUCTOR(ClassName)

#define DECLARE_SINGLETON_NO_DEFAULT_CONSTRUCTOR(ClassName)                 \
    DECLARE_SINGLETON_INTERFACE(ClassName)                                  \
    DECLARE_SINGLETON_IMPLEMENT_NO_DEFAULT_CONSTRUCTOR(ClassName)

#define DECLARE_SINGLETON(ClassName)                                        \
    DECLARE_SINGLETON_NO_DEFAULT_CONSTRUCTOR(ClassName)                     \
    DECLARE_PRIVATE_DEFAULT_CONSTRUCTOR(ClassName)

template<class T>
class CSingleObject
{
public:
    CSingleObject()
    {
        T::CreateInstance();
    }
    ~CSingleObject()
    {
        T::DeleteInstance();
    }
    T* GetPointer()
    {
        return T::GetThis();
    }
    T& GetObject()
    {
        return T::GetInstance();
    }
    BOOL IsValid()
    {
        return GetPointer() != nullptr;
    }
};

#define DECLARE_SINGLE_OBJECT(ClassName) CSingleObject<ClassName> _##ClassName##_Single_Object_;

class CEvt
{
public:
    CEvt(BOOL bManualReset = FALSE, BOOL bInitialState = FALSE, LPCTSTR pszName = nullptr, LPSECURITY_ATTRIBUTES pSecurity = nullptr)
    {
        m_hEvent = ::CreateEvent(pSecurity, bManualReset, bInitialState, pszName);
        ASSERT(IsValid());
    };
    ~CEvt()
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hEvent));
    };
    BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hEvent));
        m_hEvent = ::OpenEvent(dwAccess, bInheritHandle, pszName);
        return (IsValid());
    };
    BOOL Pulse()
    {
        return (::PulseEvent(m_hEvent));
    };
    BOOL Reset()
    {
        return (::ResetEvent(m_hEvent));
    };
    BOOL Set()
    {
        return (::SetEvent(m_hEvent));
    };
    BOOL IsValid()
    {
        return m_hEvent != nullptr;
    };
    HANDLE GetHandle()
    {
        return m_hEvent;
    };
    const HANDLE GetHandle()  const
    {
        return m_hEvent;
    };
    operator HANDLE()
    {
        return m_hEvent;
    };
    operator const HANDLE()  const
    {
        return m_hEvent;
    };
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
    };
    ~CSEM()
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hsem));
    };
    BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hsem));
        m_hsem = ::OpenSemaphore(dwAccess, bInheritHandle, pszName);
        return (IsValid());
    };
    void Wait(DWORD dwMilliseconds = INFINITE)
    {
        ::WaitForSingleObject(m_hsem, dwMilliseconds);
    };
    BOOL Release(LONG lReleaseCount = 1, LPLONG lpPreviousCount = nullptr)
    {
        return ::ReleaseSemaphore(m_hsem, lReleaseCount, lpPreviousCount);
    };
    HANDLE& GetHandle()
    {
        return m_hsem;
    };
    operator HANDLE()
    {
        return m_hsem;
    };
    BOOL IsValid()
    {
        return m_hsem != nullptr;
    };
private:
    CSEM(const CSEM& sem);
    CSEM operator = (const CSEM& sem);
private:
    HANDLE m_hsem;
};

#include <intrin.h>

#pragma intrinsic(_ReadBarrier)
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(_ReadWriteBarrier)

#define DEFAULT_CRISEC_SPIN_COUNT   4096

#if defined (_WIN64)
#define DEFAULT_PAUSE_RETRY     16
#define DEFAULT_PAUSE_YIELD     128
#define DEFAULT_PAUSE_CYCLE     8192
#else
#define DEFAULT_PAUSE_RETRY     4
#define DEFAULT_PAUSE_YIELD     32
#define DEFAULT_PAUSE_CYCLE     4096
#endif

#ifndef YieldProcessor
#pragma intrinsic(_mm_pause)
#define YieldProcessor _mm_pause
#endif

static inline void YieldThread(UINT i = DEFAULT_PAUSE_RETRY)
{
    if (i < DEFAULT_PAUSE_RETRY);
    else if (i < DEFAULT_PAUSE_YIELD)       YieldProcessor();
    else if (i < DEFAULT_PAUSE_CYCLE - 1)   SwitchToThread();
    else if (i < DEFAULT_PAUSE_CYCLE)       Sleep(1);
    else                                    YieldThread(i & (DEFAULT_PAUSE_CYCLE - 1));
}

class CInterCriSec
{
public:
    CInterCriSec(DWORD dwSpinCount = DEFAULT_CRISEC_SPIN_COUNT)
    {
        VERIFY(::InitializeCriticalSectionAndSpinCount(&m_crisec, dwSpinCount));
    };
    ~CInterCriSec()
    {
        ::DeleteCriticalSection(&m_crisec);
    };

    void Lock()
    {
        ::EnterCriticalSection(&m_crisec);
    };
    void Unlock()
    {
        ::LeaveCriticalSection(&m_crisec);
    };
    BOOL TryLock()
    {
        return ::TryEnterCriticalSection(&m_crisec);
    };
    DWORD SetSpinCount(DWORD dwSpinCount)
    {
        return ::SetCriticalSectionSpinCount(&m_crisec, dwSpinCount);
    };

    CRITICAL_SECTION* GetObject()
    {
        return &m_crisec;
    };
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
    };

    ~CInterCriSec2()
    {
        Reset();
    };

    void Attach(CRITICAL_SECTION* pcrisec)
    {
        Reset();
        m_pcrisec = pcrisec;
    };

    CRITICAL_SECTION* Detach()
    {
        CRITICAL_SECTION* pcrisec = m_pcrisec;
        m_pcrisec = nullptr;
        return pcrisec;
    };

    void Lock()
    {
        ::EnterCriticalSection(m_pcrisec);
    };
    void Unlock()
    {
        ::LeaveCriticalSection(m_pcrisec);
    };
    BOOL TryLock()
    {
        return ::TryEnterCriticalSection(m_pcrisec);
    };
    DWORD SetSpinCount(DWORD dwSpinCount)
    {
        return ::SetCriticalSectionSpinCount(m_pcrisec, dwSpinCount);
    };

    CRITICAL_SECTION* GetObject()
    {
        return m_pcrisec;
    };

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
    };
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
    };
    ~CMTX()
    {
        if (IsValid())
            ::CloseHandle(m_hMutex);
    };
    BOOL Open(DWORD dwAccess, BOOL bInheritHandle, LPCTSTR pszName)
    {
        if (IsValid())
            VERIFY(::CloseHandle(m_hMutex));
        m_hMutex = ::OpenMutex(dwAccess, bInheritHandle, pszName);
        return (IsValid());
    };
    void Lock(DWORD dwMilliseconds = INFINITE)
    {
        ::WaitForSingleObject(m_hMutex, dwMilliseconds);
    };
    void Unlock()
    {
        ::ReleaseMutex(m_hMutex);
    };
    HANDLE& GetHandle()
    {
        return m_hMutex;
    };
    operator HANDLE()
    {
        return m_hMutex;
    };
    BOOL IsValid()
    {
        return m_hMutex != nullptr;
    };
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
    };
    ~CSpinGuard()
    {
        ASSERT(m_lFlag == 0);
    };
    void Lock()
    {
        for (UINT i = 0; !TryLock(); ++i)
            YieldThread(i);
    };
    BOOL TryLock()
    {
        if (::InterlockedCompareExchange(&m_lFlag, 1, 0) == 0)
        {
            ::_ReadWriteBarrier();
            return TRUE;
        }
        return FALSE;
    };
    void Unlock()
    {
        ASSERT(m_lFlag == 1);
        m_lFlag = 0;
    };
private:
    CSpinGuard(const CSpinGuard& cs);
    CSpinGuard operator = (const CSpinGuard& cs);
private:
    volatile LONG m_lFlag;
};

class CReentrantSpinGuard
{
public:
    CReentrantSpinGuard()
        : m_dwThreadID(0)
        , m_iCount(0)
    {
    };
    ~CReentrantSpinGuard()
    {
        ASSERT(m_dwThreadID == 0);
        ASSERT(m_iCount == 0);
    };
    void Lock()
    {
        for (UINT i = 0; !_TryLock(i == 0); ++i)
            YieldThread(i);
    };
    BOOL TryLock()
    {
        return _TryLock(TRUE);
    };
    void Unlock()
    {
        ASSERT(m_dwThreadID == ::GetCurrentThreadId());
        if ((--m_iCount) == 0)
            m_dwThreadID = 0;
    };
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
    };
private:
    volatile DWORD m_dwThreadID;
    int m_iCount;
};

class CFakeGuard
{
public:
    void Lock() {};
    void Unlock() {};
    BOOL TryLock()
    {
        return TRUE;
    };
};

template<class CLockObj> class CLocalLock
{
public:
    CLocalLock(CLockObj& obj) : m_lock(obj)
    {
        m_lock.Lock();
    };
    ~CLocalLock()
    {
        m_lock.Unlock();
    };
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

#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <set>
#include <map>
#if _MSC_VER >= 1500
#include <unordered_set>
#include <unordered_map>

#define hash_set            unordered_set
#define hash_map            unordered_map
#define hash_multimap       unordered_multimap
#else
#include <hash_set>
#include <hash_map>

#define unordered_set       hash_set
#define unordered_map       hash_map
#define unordered_multimap  hash_multimap
#endif

using namespace std;
using namespace stdext;
using namespace std::tr1;

typedef wstring     CStdStringW;
typedef string      CStdStringA;

#ifdef _UNICODE
typedef CStdStringW     CStdString;
#else
typedef CStdStringA     CStdString;
#endif

typedef list<short>                     short_list;
typedef list<int>                       int_list;
typedef list<long>                      long_list;
typedef list<__int64>                   int64_list;
typedef list<unsigned short>            ushort_list;
typedef list<unsigned int>              uint_list;
typedef list<unsigned long>             ulong_list;
typedef list<unsigned __int64>          uint64_list;
typedef list<float>                     float_list;
typedef list<double>                    double_list;
typedef stack<short>                    short_stack;
typedef stack<int>                      int_stack;
typedef stack<long>                     long_stack;
typedef stack<__int64>                  int64_stack;
typedef stack<unsigned short>           ushort_stack;
typedef stack<unsigned int>             uint_stack;
typedef stack<unsigned long>            ulong_stack;
typedef stack<unsigned __int64>         uint64_stack;
typedef stack<float>                    float_stack;
typedef stack<double>                   double_stack;
typedef queue<short>                    short_queue;
typedef queue<int>                      int_queue;
typedef queue<long>                     long_queue;
typedef queue<__int64>                  int64_queue;
typedef queue<unsigned short>           ushort_queue;
typedef queue<unsigned int>             uint_queue;
typedef queue<unsigned long>            ulong_queue;
typedef queue<unsigned __int64>         uint64_queue;
typedef queue<float>                    float_queue;
typedef queue<double>                   double_queue;
typedef deque<short>                    short_deque;
typedef deque<int>                      int_deque;
typedef deque<long>                     long_deque;
typedef deque<__int64>                  int64_deque;
typedef deque<unsigned short>           ushort_deque;
typedef deque<unsigned int>             uint_deque;
typedef deque<unsigned long>            ulong_deque;
typedef deque<unsigned __int64>         uint64_deque;
typedef deque<float>                    float_deque;
typedef deque<double>                   double_deque;
typedef vector<short>                   short_vector;
typedef vector<int>                     int_vector;
typedef vector<long>                    long_vector;
typedef vector<__int64>                 int64_vector;
typedef vector<unsigned short>          ushort_vector;
typedef vector<unsigned int>            uint_vector;
typedef vector<unsigned long>           ulong_vector;
typedef vector<unsigned __int64>        uint64_vector;
typedef vector<float>                   float_vector;
typedef vector<double>                  double_vector;
typedef set<short>                      short_set;
typedef set<int>                        int_set;
typedef set<long>                       long_set;
typedef set<__int64>                    int64_set;
typedef set<unsigned short>             ushort_set;
typedef set<unsigned int>               uint_set;
typedef set<unsigned long>              ulong_set;
typedef set<unsigned __int64>           uint64_set;
typedef set<float>                      float_set;
typedef set<double>                     double_set;
typedef hash_set<short>                 short_hash_set;
typedef hash_set<int>                   int_hash_set;
typedef hash_set<long>                  long_hash_set;
typedef hash_set<__int64>               int64_hash_set;
typedef hash_set<unsigned short>        ushort_hash_set;
typedef hash_set<unsigned int>          uint_hash_set;
typedef hash_set<unsigned long>         ulong_hash_set;
typedef hash_set<unsigned __int64>      uint64_hash_set;
typedef hash_set<float>                 float_hash_set;
typedef hash_set<double>                double_hash_set;
typedef unordered_set<short>            short_unordered_set;
typedef unordered_set<int>              int_unordered_set;
typedef unordered_set<long>             long_unordered_set;
typedef unordered_set<__int64>          int64_unordered_set;
typedef unordered_set<unsigned short>   ushort_unordered_set;
typedef unordered_set<unsigned int>     uint_unordered_set;
typedef unordered_set<unsigned long>    ulong_unordered_set;
typedef unordered_set<unsigned __int64> uint64_unordered_set;
typedef unordered_set<float>            float_unordered_set;
typedef unordered_set<double>           double_unordered_set;

typedef list<INT_PTR>                   int_ptr_list;
typedef list<LONG_PTR>                  long_ptr_list;
typedef list<UINT_PTR>                  uint_ptr_list;
typedef list<ULONG_PTR>                 ulong_ptr_list;
typedef stack<INT_PTR>                  int_ptr_stack;
typedef stack<LONG_PTR>                 long_ptr_stack;
typedef stack<UINT_PTR>                 uint_ptr_stack;
typedef stack<ULONG_PTR>                ulong_ptr_stack;
typedef queue<INT_PTR>                  int_ptr_queue;
typedef queue<LONG_PTR>                 long_ptr_queue;
typedef queue<UINT_PTR>                 uint_ptr_queue;
typedef queue<ULONG_PTR>                ulong_ptr_queue;
typedef deque<INT_PTR>                  int_ptr_deque;
typedef deque<LONG_PTR>                 long_ptr_deque;
typedef deque<UINT_PTR>                 uint_ptr_deque;
typedef deque<ULONG_PTR>                ulong_ptr_deque;
typedef vector<INT_PTR>                 int_ptr_vector;
typedef vector<LONG_PTR>                long_ptr_vector;
typedef vector<UINT_PTR>                uint_ptr_vector;
typedef vector<ULONG_PTR>               ulong_ptr_vector;
typedef set<INT_PTR>                    int_ptr_set;
typedef set<LONG_PTR>                   long_ptr_set;
typedef set<UINT_PTR>                   uint_ptr_set;
typedef set<ULONG_PTR>                  ulong_ptr_set;
typedef hash_set<INT_PTR>               int_ptr_hash_set;
typedef hash_set<LONG_PTR>              long_ptr_hash_set;
typedef hash_set<UINT_PTR>              uint_ptr_hash_set;
typedef hash_set<ULONG_PTR>             ulong_ptr_hash_set;
typedef unordered_set<INT_PTR>          int_ptr_unordered_set;
typedef unordered_set<LONG_PTR>         long_ptr_unordered_set;
typedef unordered_set<UINT_PTR>         uint_ptr_unordered_set;
typedef unordered_set<ULONG_PTR>        ulong_ptr_unordered_set;

/*****************************************************************************/
/******************************** 容器操作函数 *******************************/

//描述: 清除普通集合 , 适用于 vector<Object> / list<Object> 参数: v : vector / list / set
template<class Set> void ClearSet(Set& v)
{
    v.clear();
}

template<class Set> struct Set_Cleaner
{
    static void Clear(Set& v)
    {
        ClearSet(v);
    }
};

//描述: 清除指针集合 (清除前先释放指针), 适用于 vector<Object*> / list<Object*> 参数: v : vector / list / set
template<class PtrSet> void ClearPtrSet(PtrSet& v)
{
    for (PtrSet::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
        delete(*it);
    v.clear();
}

template<class PtrSet> struct PtrSet_Cleaner
{
    static void Clear(PtrSet& v)
    {
        ClearPtrSet(v);
    }
};

//描述: 清除指针集合 (指针同时又指向数组), 适用于 vector<Object*[]> / list<Object*[]> 参数: v : vector / list / set
template<class PtrArraySet> void ClearPtrArraySet(PtrArraySet& v)
{
    for (PtrArraySet::iterator   it = v.begin(),
        end = v.end();
        it != end;
    ++it)
        delete[](*it);
    v.clear();
}

template<class PtrArraySet> struct PtrArraySet_Cleaner
{
    static void Clear(PtrArraySet& v)
    {
        ClearPtrArraySet(v);
    }
};

//描述: 清除普通影射 , 适用于 map<key, value>  参数: v : map
template<class Map> void ClearMap(Map& v)
{
    v.clear();
}

template<class Map> struct Map_Cleaner
{
    static void Clear(Map& v)
    {
        ClearMap(v);
    }
};

//描述: 清除指针影射 (清除前先释放指针), 适用于 map<key, Object*> 参数: v : map
template<class PtrMap> void ClearPtrMap(PtrMap& v)
{
    for (PtrMap::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
        delete it->second;
    v.clear();
}

template<class PtrMap> struct PtrMap_Cleaner
{
    static void Clear(PtrMap& v)
    {
        ClearPtrMap(v);
    }
};

//描述: 清除指针影射 (指针同时又指向数组), 适用于 map<key, Object*[]>  参数: v : map
template<class PtrArrayMap> void ClearPtrArrayMap(PtrArrayMap& v)
{
    for (PtrArrayMap::iterator   it = v.begin(),
        end = v.end();
        it != end;
    ++it)
        delete[] it->second;
    v.clear();
}

template<class PtrArrayMap> struct PtrArrayMap_Cleaner
{
    static void Clear(PtrArrayMap& v)
    {
        ClearPtrArrayMap(v);
    }
};

//描述: 清除集合-集合 (清除前先清除内部集合), 适用于 set<vector<Object>*>  参数: v : vector / list / set
template<class SetSet> void ClearSetSet(SetSet& v)
{
    for (SetSet::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        (*it)->clear();
        delete(*it);
    }
    v.clear();
}

template<class SetSet> struct SetSet_Cleaner
{
    static void Clear(SetSet& v)
    {
        ClearSetSet(v);
    }
};

//描述: 清除指针集合-集合 (清除前先清除内部指针集合), 适用于 set<vector<Object*>*> 参数: v : vector / list / set
template<class PtrSetSet> void ClearPtrSetSet(PtrSetSet& v)
{
    for (PtrSetSet::iterator it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        ClearPtrSet(**it);
        delete(*it);
    }
    v.clear();
}

template<class PtrSetSet> struct PtrSetSet_Cleaner
{
    static void Clear(PtrSetSet& v)
    {
        ClearPtrSetSet(v);
    }
};

//描述: 清除指针数组集合影射 (清除前先清除指针数组集合), 适用于 map<vector<Object*[]>*> 参数: v : vector / list / set
template<class PtrArraySetSet> void ClearPtrArraySetSet(PtrArraySetSet& v)
{
    for (PtrArraySetSet::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        ClearPtrArraySet(**it);
        delete(*it);
    }
    v.clear();
}

template<class PtrArraySetSet> struct PtrArraySetSet_Cleaner
{
    static void Clear(PtrArraySetSet& v)
    {
        ClearPtrArraySetSet(v);
    }
};

//描述: 清除集合影射 (清除前先清除集合), 适用于 map<key, vector<Object>*> 参数: v : map
template<class SetMap> void ClearSetMap(SetMap& v)
{
    for (SetMap::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        it->second->clear();
        delete it->second;
    }
    v.clear();
}

template<class SetMap> struct SetMap_Cleaner
{
    static void Clear(SetMap& v)
    {
        ClearSetMap(v);
    }
};

//描述: 清除指针集合影射 (清除前先清除指针集合), 适用于 map<key, vector<Object*>*> 参数: v : map
template<class PtrSetMap> void ClearPtrSetMap(PtrSetMap& v)
{
    for (PtrSetMap::iterator it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        ClearPtrSet(*(it->second));
        delete it->second;
    }
    v.clear();
}

template<class PtrSetMap> struct PtrSetMap_Cleaner
{
    static void Clear(PtrSetMap& v)
    {
        ClearPtrSetMap(v);
    }
};

//描述: 清除指针数组集合影射 (清除前先清除指针数组集合), 适用于 map<key, vector<Object*[]>*> 参数: v : map
template<class PtrArraySetMap> void ClearPtrArraySetMap(PtrArraySetMap& v)
{
    for (PtrArraySetMap::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        ClearPtrArraySet(*(it->second));
        delete it->second;
    }
    v.clear();
}

template<class PtrArraySetMap> struct PtrArraySetMap_Cleaner
{
    static void Clear(PtrArraySetMap& v)
    {
        ClearPtrArraySetMap(v);
    }
};

//描述: 清除映射-影射 (清除前先清除内部映射), 适用于 map<key, map<key2, Object>*> 参数: v : map
template<class MapMap> void ClearMapMap(MapMap& v)
{
    for (MapMap::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        it->second->clear();
        delete it->second;
    }
    v.clear();
}

template<class MapMap> struct MapMap_Cleaner
{
    static void Clear(MapMap& v)
    {
        ClearMapMap(v);
    }
};

//描述: 清除指针映射-影射 (清除前先清除指针内部映射), 适用于 map<key, map<key2, Object*>*>
template<class PtrMapMap> void ClearPtrMapMap(PtrMapMap& v)
{
    for (PtrMapMap::iterator it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        ClearPtrMap(*(it->second));
        delete it->second;
    }
    v.clear();
}

template<class PtrMapMap> struct PtrMapMap_Cleaner
{
    static void Clear(PtrMapMap& v)
    {
        ClearPtrMapMap(v);
    }
};

//描述: 清除指针映射-影射 (清除前先清除指针数组内部映射), 适用于 map<key, map<key2, Object*[]>*>
template<class PtrArrayMapMap> void ClearPtrArrayMapMap(PtrArrayMapMap& v)
{
    for (PtrArrayMapMap::iterator    it = v.begin(),
        end = v.end();
        it != end;
    ++it)
    {
        ClearPtrArrayMap(*(it->second));
        delete it->second;
    }
    v.clear();
}

template<class PtrArrayMapMap> struct PtrArrayMapMap_Cleaner
{
    static void Clear(PtrArrayMapMap& v)
    {
        ClearPtrArrayMapMap(v);
    }
};

/************************************************************************/
/*                           指针集合容器                               */
/************************************************************************/
template<class Set, class Cleaner> struct SetWrapper
{
    typedef typename Set::iterator          iterator;
    typedef typename Set::const_iterator    const_iterator;
    typedef typename Set::value_type        value_type;
    typedef typename Set::reference         reference;
    typedef typename Set::const_reference   const_reference;
    typedef typename Set::pointer           pointer;
    typedef typename Set::const_pointer     const_pointer;
    typedef typename Set::size_type         size_type;
    typedef typename Set::difference_type   difference_type;

    SetWrapper()
    {
    }

    virtual ~SetWrapper()
    {
        Clear();
    }

    void Clear()
    {
        if (!IsEmpty())
            Cleaner::Clear(m_set);
    }

    Set& operator * ()
    {
        return m_set;
    }
    const Set& operator * ()  const
    {
        return m_set;
    }
    Set* operator -> ()
    {
        return &m_set;
    }
    const Set* operator -> ()  const
    {
        return &m_set;
    }
    Set& Get()
    {
        return m_set;
    }
    operator Set&           ()
    {
        return m_set;
    }
    bool IsEmpty()  const
    {
        return m_set.empty();
    }
    size_t Size()  const
    {
        return m_set.size();
    }

protected:
    Set m_set;

    DECLARE_NO_COPY_CLASS(SetWrapper)
};

template<class Set, class Cleaner> struct VectorWrapper : public SetWrapper<Set, Cleaner>
{
    VectorWrapper()
    {
    }

    reference       operator [](size_type i)
    {
        return m_set[i];
    }
    const_reference operator [](size_type i)   const
    {
        return m_set[i];
    }

    DECLARE_NO_COPY_CLASS(VectorWrapper)
};

/************************************************************************/
/*                         指针数组集合容器                             */
/************************************************************************/


/************************************************************************/
/*                           指针映射容器                               */
/************************************************************************/
template<class Map, class Cleaner> struct MapWrapper
{
    typedef typename Map::iterator          iterator;
    typedef typename Map::const_iterator    const_iterator;
    typedef typename Map::key_type          key_type;
    typedef typename Map::mapped_type       mapped_type;
    typedef typename Map::value_type        value_type;
    typedef typename Map::reference         reference;
    typedef typename Map::const_reference   const_reference;
    typedef typename Map::pointer           pointer;
    typedef typename Map::size_type         size_type;
    typedef typename Map::difference_type   difference_type;

    MapWrapper()
    {
    }

    ~MapWrapper()
    {
        Clear();
    }

    void Clear()
    {
        if (!IsEmpty())
            Cleaner::Clear(m_map);
    }

    Map&                operator * ()
    {
        return m_map;
    }
    const Map&          operator * ()                      const
    {
        return m_map;
    }
    Map*                operator -> ()
    {
        return &m_map;
    }
    const Map*          operator -> ()                      const
    {
        return &m_map;
    }
    mapped_type&        operator [](const key_type& key)
    {
        return m_map[key];
    }
    const mapped_type&  operator [](const key_type& key)   const
    {
        return m_map[key];
    }
    Map& Get()
    {
        return m_map;
    }
    operator Map&       ()
    {
        return m_map;
    }
    bool IsEmpty()  const
    {
        return m_map.empty();
    }
    size_t Size()  const
    {
        return m_map.size();
    }

private:
    Map m_map;

    DECLARE_NO_COPY_CLASS(MapWrapper)
};

/************************************************************************/
/*                            比较仿函数                                */
/************************************************************************/

template<class T> struct char_comparator
{
    typedef T   row_type;
    static row_type row_type_value(const T& v)
    {
        return (row_type)v;
    }
    static bool equal_to(const T& v1, const T& v2)
    {
        return strcmp(v1, v2) == 0;
    }
};

template<class T> struct char_nc_comparator
{
    typedef T   row_type;
    static row_type row_type_value(const T& v)
    {
        return (row_type)v;
    }
    static bool equal_to(const T& v1, const T& v2)
    {
        return stricmp(v1, v2) == 0;
    }
};

template<class T> struct wchar_comparator
{
    typedef T   row_type;
    static row_type row_type_value(const T& v)
    {
        return (row_type)v;
    }
    static bool equal_to(const T& v1, const T& v2)
    {
        return wcscmp(v1, v2) == 0;
    }
};

template<class T> struct wchar_nc_comparator
{
    typedef T   row_type;
    static row_type row_type_value(const T& v)
    {
        return (row_type)v;
    }
    static bool equal_to(const T& v1, const T& v2)
    {
        return wcsicmp(v1, v2) == 0;
    }
};

template<class T> struct cstring_comparator
{
    typedef typename T::PCXSTR  row_type;
    static row_type row_type_value(const T& v)
    {
        return (row_type)v;
    }
    static bool equal_to(const T& v1, const T& v2)
    {
        return v1.Compare(v2) == 0;
    }
};

template<class T> struct cstring_nc_comparator
{
    typedef typename T::PCXSTR  row_type;
    static row_type row_type_value(const T& v)
    {
        return (row_type)v;
    }
    static bool equal_to(const T& v1, const T& v2)
    {
        return v1.CompareNoCase(v2) == 0;
    }
};

// char/wchar_t/CStringX hash function
template<class T, class H> struct str_hash_func_t
{
    struct hash
    {
        size_t operator()(const T& t) const
        {
            return hash_value(H::row_type_value(t));
        }
    };

    struct equal_to
    {
        bool operator()(const T& t1, const T& t2) const
        {
            return H::equal_to(t1, t2);
        }
    };

};

// char/wchar_t/CStringX hash function (no case)
template<class T, class H> struct str_nc_hash_func_t
{
    struct hash
    {
        size_t operator()(const T& t) const
        {
            size_t _Val = 2166136261U;
            H::row_type lpsz = H::row_type_value(t);
            char c;
            while ((c = *lpsz++) != 0)
            {
                if (c >= 'A' && c <= 'Z')
                    c += 32;
                _Val = 16777619U * _Val ^ c;
            }
            return _Val;
        }
    };

    struct equal_to
    {
        bool operator()(const T& t1, const T& t2) const
        {
            return H::equal_to(t1, t2);
        }
    };

};

typedef str_hash_func_t<LPCSTR, char_comparator<LPCSTR>>            str_hash_func;
typedef str_hash_func_t<LPCWSTR, wchar_comparator<LPCWSTR>>         wstr_hash_func;
typedef str_hash_func_t<CStringA, cstring_comparator<CStringA>>     cstringa_hash_func;
typedef str_hash_func_t<CStringW, cstring_comparator<CStringW>>     cstringw_hash_func;
typedef str_nc_hash_func_t<LPCSTR, char_comparator<LPCSTR>>         str_nc_hash_func;
typedef str_nc_hash_func_t<LPCWSTR, wchar_comparator<LPCWSTR>>      wstr_nc_hash_func;
typedef str_nc_hash_func_t<CStringA, cstring_comparator<CStringA>>  cstringa_nc_hash_func;
typedef str_nc_hash_func_t<CStringW, cstring_comparator<CStringW>>  cstringw_nc_hash_func;

#ifdef _UNICODE
typedef cstringw_hash_func      cstring_hash_func;
typedef cstringw_nc_hash_func   cstring_nc_hash_func;
#else
typedef cstringa_hash_func      cstring_hash_func;
typedef cstringa_nc_hash_func   cstring_nc_hash_func;
#endif

struct bool_comp_func
{
    bool operator()(bool v1, bool v2) const
    {
        if (!v1)
            return false;
        if (v1 == v2)
            return false;
        return true;
    }
};

template<class T>
// T -> (signed / unsigned) short / int / long / __int64
struct integer_comp_func
{
    bool operator()(T v1, T v2) const
    {
        return v1 < v2;
    }
};

typedef integer_comp_func<short>                short_comp_func;
typedef integer_comp_func<int>                  int_comp_func;
typedef integer_comp_func<long>                 long_comp_func;
typedef integer_comp_func<__int64>              int64_comp_func;
typedef integer_comp_func<unsigned short>       ushort_comp_func;
typedef integer_comp_func<unsigned int>         uint_comp_func;
typedef integer_comp_func<unsigned long>        ulong_comp_func;
typedef integer_comp_func<unsigned __int64>     uint64_comp_func;

struct float_comp_func
{
    bool operator()(float v1, float v2) const
    {
        float disc = v1 - v2;
        if (fabsf(disc) < 1E-5)
            return false;
        return disc < 0;
    }
};

struct double_comp_func
{
    bool operator()(double v1, double v2) const
    {
        double disc = v1 - v2;
        if (fabs(disc) < 1E-8)
            return false;
        return disc < 0;
    }
};

template<class T, bool CASE = false>
// T -> (unsigned) char / wchar_t
struct character_comp_func
{
    bool operator()(T v1, T v2) const
    {
        if (!CASE)
        {
            if (v1 >= 'A' && v1 <= 'Z')  v1 += 32;
            if (v2 >= 'A' && v2 <= 'Z')  v2 += 32;
        }
        return v1 < v2;
    }
};

typedef character_comp_func<char, true>             char_case_comp_func;
typedef character_comp_func<unsigned char, true>    uchar_case_comp_func;
typedef character_comp_func<wchar_t, true>          wchar_case_comp_func;
typedef character_comp_func<char, false>            char_ucase_comp_func;
typedef character_comp_func<unsigned char, false>   uchar_ucase_comp_func;
typedef character_comp_func<wchar_t, false>         wchar_ucase_comp_func;

template<class T, bool CASE = false>
// T -> TCHAR* / CString
struct str_comp_func
{
    //比较函数。
    bool operator()(const T& A, const T& B) const
    {
        if (!CASE)
            return lstrcmpi((LPCTSTR)A, (LPCTSTR)B) < 0;
        else
            return lstrcmp((LPCTSTR)A, (LPCTSTR)B) < 0;
    }
};

typedef str_comp_func<LPCTSTR, true>        case_tchar_comp_func;
typedef str_comp_func<LPCTSTR, false>       uncase_tchar_comp_func;
typedef str_comp_func<CString, true>        case_string_comp_func;
typedef str_comp_func<CString, false>       uncase_string_comp_func;
typedef case_tchar_comp_func                tchar_ptr_case_comp_func;
typedef uncase_tchar_comp_func              tchar_ptr_ucase_comp_func;
typedef case_string_comp_func               string_case_comp_func;
typedef uncase_string_comp_func             string_ucase_comp_func;
/************************************************************************/
/*                            排序仿函数                                */
/************************************************************************/
template<bool ASC = true>
struct bool_sort_func
{
    bool operator()(bool v1, bool v2) const
    {
        if (v1 == v2)
            return false;
        bool result = !v1;
        return ASC ? result : !result;
    }
};

typedef bool_sort_func<true>    bool_asc_sort_func;
typedef bool_sort_func<false>   bool_desc_sort_func;

template<class T, bool ASC = true>
// T -> (signed / unsigned) short / int / long / __int64
struct integer_sort_func
{
    bool operator()(T v1, T v2) const
    {
        if (v1 == v2)
            return false;
        bool result = v1 < v2;
        return ASC ? result : !result;
    }
};

typedef integer_sort_func<short, true>       short_asc_sort_func;
typedef integer_sort_func<unsigned short, true>       ushort_asc_sort_func;
typedef integer_sort_func<int, true>       int_asc_sort_func;
typedef integer_sort_func<unsigned int, true>       uint_asc_sort_func;
typedef integer_sort_func<long, true>       long_asc_sort_func;
typedef integer_sort_func<unsigned long, true>       ulong_asc_sort_func;
typedef integer_sort_func<__int64, true>       int64_asc_sort_func;
typedef integer_sort_func<unsigned __int64, true>       uint64_asc_sort_func;
typedef integer_sort_func<short, false>      short_desc_sort_func;
typedef integer_sort_func<unsigned short, false>      ushort_desc_sort_func;
typedef integer_sort_func<int, false>      int_desc_sort_func;
typedef integer_sort_func<unsigned int, false>      uint_desc_sort_func;
typedef integer_sort_func<long, false>      long_desc_sort_func;
typedef integer_sort_func<unsigned long, false>      ulong_desc_sort_func;
typedef integer_sort_func<__int64, false>      int64_desc_sort_func;
typedef integer_sort_func<unsigned __int64, false>      uint64_desc_sort_func;

template<bool ASC = true>
struct float_sort_func
{
    bool operator()(float v1, float v2) const
    {
        float disc = v1 - v2;
        if (fabsf(disc) < 1E-5)
            return false;
        bool result = disc < 0;
        return ASC ? result : !result;
    }
};

typedef float_sort_func<true>       float_asc_sort_func;
typedef float_sort_func<false>      float_desc_sort_func;

template<bool ASC = true>
struct double_sort_func
{
    bool operator()(double v1, double v2) const
    {
        double disc = v1 - v2;
        if (fabs(disc) < 1E-8)
            return false;
        bool result = disc < 0;
        return ASC ? result : !result;
    }
};

typedef double_sort_func<true>      double_asc_sort_func;
typedef double_sort_func<false>     double_desc_sort_func;

template<class T, bool ASC = true, bool CASE = false>
// T -> (unsigned) char / wchar_t
struct character_sort_func
{
    bool operator()(T v1, T v2) const
    {
        if (!CASE)
        {
            if (v1 >= 'A' && v1 <= 'Z')  v1 += 32;
            if (v2 >= 'A' && v2 <= 'Z')  v2 += 32;
        }
        if (v1 == v2)
            return false;
        bool result = v1 < v2;
        return ASC ? result : !result;
    }
};

typedef character_sort_func<char, true, true>               char_asc_case_sort_func;
typedef character_sort_func<unsigned char, true, true>      uchar_asc_case_sort_func;
typedef character_sort_func<wchar_t, true, true>            wchar_asc_case_sort_func;
typedef character_sort_func<char, true, false>              char_asc_ucase_sort_func;
typedef character_sort_func<unsigned char, true, false>     uchar_asc_ucase_sort_func;
typedef character_sort_func<wchar_t, true, false>           wchar_asc_ucase_sort_func;
typedef character_sort_func<char, false, true>              char_desc_case_sort_func;
typedef character_sort_func<unsigned char, false, true>     uchar_desc_case_sort_func;
typedef character_sort_func<wchar_t, false, true>           wchar_desc_case_sort_func;
typedef character_sort_func<char, false, false>             char_desc_ucase_sort_func;
typedef character_sort_func<unsigned char, false, false>    uchar_desc_ucase_sort_func;
typedef character_sort_func<wchar_t, false, false>          wchar_desc_ucase_sort_func;

template<class T, bool ASC = true, bool CASE = false>
// T -> TCHAR* / CString
struct str_sort_func
{
    bool operator()(const T& v1, const T& v2) const
    {
        bool result;
        if (CASE)
        {
            int v = lstrcmp((LPCTSTR)v1, (LPCTSTR)v2);
            if (v == 0)
                result = false;
            else
                result = v < 0;
        }
        else
        {
            int v = lstrcmpi((LPCTSTR)v1, (LPCTSTR)v2);
            if (v == 0)
                result = false;
            else
                result = v < 0;
        }
        return ASC ? result : !result;
    }
};

typedef str_sort_func<TCHAR*, true, true>       tchar_ptr_asc_case_sort_func;
typedef str_sort_func<CString, true, true>      string_asc_case_sort_func;
typedef str_sort_func<TCHAR*, true, false>      tchar_ptr_asc_ucase_sort_func;
typedef str_sort_func<CString, true, false>     string_asc_ucase_sort_func;
typedef str_sort_func<TCHAR*, false, true>      tchar_ptr_desc_case_sort_func;
typedef str_sort_func<CString, false, true>     string_desc_case_sort_func;
typedef str_sort_func<TCHAR*, false, false>     tchar_ptr_desc_ucase_sort_func;
typedef str_sort_func<CString, false, false>    string_desc_ucase_sort_func;

/************************************************************************/
/*                     smart_ptr 单实体或数组智能指针                    */
/************************************************************************/

template<class _Ty>
struct simple_deleter
{
    static void delete_ptr(_Ty* pv)
    {
        delete pv;
    }
};

template<class _Ty>
struct global_simple_deleter
{
    static void delete_ptr(_Ty* pv)
    {
        ::delete pv;
    }
};

template<class _Ty>
struct array_deleter
{
    static void delete_ptr(_Ty* pv)
    {
        delete[] pv;
    }
};

template<class _Ty>
struct global_array_deleter
{
    static void delete_ptr(_Ty* pv)
    {
        ::delete[] pv;
    }
};

template<class _Ty, class _Deleter>
class smart_ptr
{
public:
    smart_ptr(_Ty* _Ptr = 0) : _Myptr(_Ptr)              {}
    smart_ptr(smart_ptr<_Ty, _Deleter>& _Right) : _Myptr(_Right.release())  {}

    ~smart_ptr()
    {
        reset();
    }

    smart_ptr<_Ty, _Deleter>& reset(_Ty* _Ptr = 0)
    {
        if (_Ptr != _Myptr)
        {
            if (_Myptr)
                _Deleter::delete_ptr(_Myptr);
            _Myptr = _Ptr;
        }
        return *this;
    }

    smart_ptr<_Ty, _Deleter>& reset(smart_ptr<_Ty, _Deleter>& _Right)
    {
        if (this != &_Right)
            reset(_Right.release());
        return *this;
    }

    _Ty* release()
    {
        _Ty* _Ptr = _Myptr;
        _Myptr = 0;
        return _Ptr;
    }

    smart_ptr<_Ty, _Deleter>& operator = (_Ty* _Ptr)
    {
        return reset(_Ptr);
    }
    smart_ptr<_Ty, _Deleter>& operator = (smart_ptr<_Ty, _Deleter>& _Right)
    {
        return reset(_Right);
    }

    bool is_valid()  const
    {
        return _Myptr != 0;
    }
    _Ty& operator * ()  const
    {
        return *_Myptr;
    }
    _Ty* get()  const
    {
        return _Myptr;
    }
    _Ty* operator -> ()  const
    {
        return _Myptr;
    }
    operator _Ty*       ()  const
    {
        return _Myptr;
    }

private:
    template<class _Other> smart_ptr<_Ty, _Deleter>(const smart_ptr<_Ty, _Other>&);
    template<class _Other> smart_ptr<_Ty, _Deleter>&    reset(const smart_ptr<_Ty, _Other>&);
    template<class _Other> smart_ptr<_Ty, _Deleter>&    operator = (const smart_ptr<_Ty, _Other>&);

    template<class _Other> smart_ptr<_Ty, _Deleter>(const smart_ptr<_Other, _Deleter>&);
    template<class _Other> smart_ptr<_Ty, _Deleter>&    reset(const smart_ptr<_Other, _Deleter>&);
    template<class _Other> smart_ptr<_Ty, _Deleter>&    operator = (const smart_ptr<_Other, _Deleter>&);

protected:
    _Ty* _Myptr;
};


/************************************************************************/
/*                  smart_simple_ptr 单实体智能指针                      */
/************************************************************************/

template<class _Ty>
class smart_simple_ptr : public smart_ptr<_Ty, simple_deleter<_Ty>>
{
public:
    smart_simple_ptr(_Ty* _Ptr = 0) : smart_ptr(_Ptr)   {}
    smart_simple_ptr(smart_simple_ptr<_Ty>& _Right) : smart_ptr(_Right) {}
    smart_simple_ptr(smart_ptr<_Ty, simple_deleter<_Ty>>& _Right) : smart_ptr(_Right) {}

    smart_simple_ptr<_Ty>& operator = (smart_ptr<_Ty, simple_deleter<_Ty>>& _Right)
    {
        return (smart_simple_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_simple_ptr<_Ty>& operator = (smart_simple_ptr<_Ty>& _Right)
    {
        return (smart_simple_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_simple_ptr<_Ty>& operator = (_Ty* _Ptr)
    {
        return (smart_simple_ptr<_Ty>&)__super::operator = (_Ptr);
    }

private:
    template<class _Other> smart_simple_ptr<_Ty>(const smart_ptr<_Ty, _Other>&);
    template<class _Other> smart_simple_ptr<_Ty>&   operator = (const smart_ptr<_Ty, _Other>&);

    template<class _Other> smart_simple_ptr<_Ty>(const smart_simple_ptr<_Other>&);
    template<class _Other> smart_simple_ptr<_Ty>&   operator = (const smart_simple_ptr<_Other>&);
};

/************************************************************************/
/*         smart_gd_simple_ptr 单实体智能指针 (使用全局 delete)          */
/************************************************************************/

template<class _Ty>
class smart_gd_simple_ptr : public smart_ptr<_Ty, global_simple_deleter<_Ty>>
{
public:
    smart_gd_simple_ptr(_Ty* _Ptr = 0) : smart_ptr(_Ptr)   {}
    smart_gd_simple_ptr(smart_gd_simple_ptr<_Ty>& _Right) : smart_ptr(_Right) {}
    smart_gd_simple_ptr(smart_ptr<_Ty, global_simple_deleter<_Ty>>& _Right) : smart_ptr(_Right) {}

    smart_gd_simple_ptr<_Ty>& operator = (smart_ptr<_Ty, global_simple_deleter<_Ty>>& _Right)
    {
        return (smart_gd_simple_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_gd_simple_ptr<_Ty>& operator = (smart_gd_simple_ptr<_Ty>& _Right)
    {
        return (smart_gd_simple_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_gd_simple_ptr<_Ty>& operator = (_Ty* _Ptr)
    {
        return (smart_gd_simple_ptr<_Ty>&)__super::operator = (_Ptr);
    }

private:
    template<class _Other> smart_gd_simple_ptr<_Ty>(const smart_ptr<_Ty, _Other>&);
    template<class _Other> smart_gd_simple_ptr<_Ty>&    operator = (const smart_ptr<_Ty, _Other>&);

    template<class _Other> smart_gd_simple_ptr<_Ty>(const smart_gd_simple_ptr<_Other>&);
    template<class _Other> smart_gd_simple_ptr<_Ty>&    operator = (const smart_gd_simple_ptr<_Other>&);
};

/************************************************************************/
/*                   smart_array_ptr 数组智能指针                        */
/************************************************************************/

template<class _Ty>
class smart_array_ptr : public smart_ptr<_Ty, array_deleter<_Ty>>
{
public:
    smart_array_ptr(_Ty* _Ptr = 0) : smart_ptr(_Ptr)   {}
    smart_array_ptr(smart_simple_ptr<_Ty>& _Right) : smart_ptr(_Right) {}
    smart_array_ptr(smart_ptr<_Ty, array_deleter<_Ty>>& _Right) : smart_ptr(_Right) {}

    smart_array_ptr<_Ty>& operator = (smart_ptr<_Ty, array_deleter<_Ty>>& _Right)
    {
        return (smart_array_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_array_ptr<_Ty>& operator = (smart_array_ptr<_Ty>& _Right)
    {
        return (smart_array_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_array_ptr<_Ty>& operator = (_Ty* _Ptr)
    {
        return (smart_array_ptr<_Ty>&)__super::operator = (_Ptr);
    }

private:
    template<class _Other> smart_array_ptr<_Ty>(const smart_ptr<_Ty, _Other>&);
    template<class _Other> smart_array_ptr<_Ty>&    operator = (const smart_ptr<_Ty, _Other>&);

    template<class _Other> smart_array_ptr<_Ty>(const smart_array_ptr<_Other>&);
    template<class _Other> smart_array_ptr<_Ty>&    operator = (const smart_array_ptr<_Other>&);
};

/************************************************************************/
/*          smart_gd_array_ptr 单实体智能指针 (使用全局 delete)          */
/************************************************************************/

template<class _Ty>
class smart_gd_array_ptr : public smart_ptr<_Ty, global_array_deleter<_Ty>>
{
public:
    smart_gd_array_ptr(_Ty* _Ptr = 0) : smart_ptr(_Ptr)   {}
    smart_gd_array_ptr(smart_gd_array_ptr<_Ty>& _Right) : smart_ptr(_Right) {}
    smart_gd_array_ptr(smart_ptr<_Ty, global_array_deleter<_Ty>>& _Right) : smart_ptr(_Right) {}

    smart_gd_array_ptr<_Ty>& operator = (smart_ptr<_Ty, global_array_deleter<_Ty>>& _Right)
    {
        return (smart_gd_array_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_gd_array_ptr<_Ty>& operator = (smart_gd_array_ptr<_Ty>& _Right)
    {
        return (smart_gd_array_ptr<_Ty>&)__super::operator = (_Right);
    }

    smart_gd_array_ptr<_Ty>& operator = (_Ty* _Ptr)
    {
        return (smart_gd_array_ptr<_Ty>&)__super::operator = (_Ptr);
    }

private:
    template<class _Other> smart_gd_array_ptr<_Ty>(const smart_ptr<_Ty, _Other>&);
    template<class _Other> smart_gd_array_ptr<_Ty>& operator = (const smart_ptr<_Ty, _Other>&);

    template<class _Other> smart_gd_array_ptr<_Ty>(const smart_gd_array_ptr<_Other>&);
    template<class _Other> smart_gd_array_ptr<_Ty>& operator = (const smart_gd_array_ptr<_Other>&);
};



// RECT 帮助宏
#define RC_WIDTH(rc)        (rc.right - rc.left)
#define RC_HEIGHT(rc)       (rc.bottom - rc.top)
#define RC_CENTER_X(rc)     ((rc.right + rc.left) / 2)
#define RC_CENTER_Y(rc)     ((rc.bottom + rc.top) / 2)

/************************************************************************/
/*                            消息映射帮助宏                                */
/************************************************************************/

/* see: WindowsX.h */
#define HANDLE_SYS_MSG(hwnd, message, fn)   HANDLE_MSG(hwnd, message, fn)

/* LRESULT Cls_OnMessage(HWND hwnd, WPARAM wParam, LPARAM lParam) */
#define HANDLE_USER_MSG(hwnd, message, fn)                              \
    case (message): return (LRESULT)(fn)((hwnd), (wParam), (lParam))

#define FORWARD_USER_MSG(hwnd, message, wParam, lParam, fn)             \
    (LRESULT)(fn)((hwnd), (message), (wParam), (lParam))

#define GET_WND_PROC_INTERNAL(theClass, flag)   ((WNDPROC)theClass##flag##WndProc)
#define GET_DLG_PROC_INTERNAL(theClass, flag)   ((DLGPROC)theClass##flag##DlgProc)

#define DECLARE_MSG_MAP_INTERNAL(theClass, flag)        \
    static LRESULT CALLBACK theClass##flag##WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define DECLARE_DLG_MSG_MAP_INTERNAL(theClass, flag)    \
    static BOOL CALLBACK theClass##flag##DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define BEGIN_MSG_MAP_INTERNAL(theClass, flag)          \
    LRESULT theClass##flag##WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  \
    {                                                                                   \
        LRESULT result = 0;                                                             \
        switch(msg)                                                                     \
        {
#define BEGIN_DLG_MSG_MAP_INTERNAL(theClass, flag)      \
    BOOL theClass##flag##DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)     \
    {                                                                                   \
        BOOL    retVal = TRUE;                                                          \
        LRESULT result = 0;                                                             \
        \
        switch(msg)                                                                     \
        {

// 窗口过程为类中的静态成员函数
#define GET_WND_PROC(theClass)          GET_WND_PROC_INTERNAL(theClass, ::)
#define GET_DLG_PROC(theClass)          GET_DLG_PROC_INTERNAL(theClass, ::)

#define DECLARE_MSG_MAP(theClass)                   \
    public:                                             \
    DECLARE_MSG_MAP_INTERNAL(theClass, ::)

#define DECLARE_DLG_MSG_MAP(theClass)               \
    public:                                             \
    DECLARE_DLG_MSG_MAP_INTERNAL(theClass, ::)

#define BEGIN_MSG_MAP(theClass)         BEGIN_MSG_MAP_INTERNAL(theClass, ::)
#define BEGIN_DLG_MSG_MAP(theClass)     BEGIN_DLG_MSG_MAP_INTERNAL(theClass, ::)

/* 消息处理函数的声明请参考: <WindowsX.h> 的 HANDLE_MSG */
#define ADD_MSG_MAP(msg, fn)                        \
    case (msg): result = HANDLE_##msg((hWnd), (wParam), (lParam), (fn));    break;

/* LRESULT Cls_OnMessage(HWND hwnd, WPARAM wParam, LPARAM lParam) */
#define ADD_USER_MSG_MAP(msg, fn)                   \
    case (msg): result = (LRESULT)(fn)((hWnd), (wParam), (lParam));         break;

#define END_MSG_MAP()                               \
    default:                                    \
    result = ::DefWindowProc(hWnd, msg, wParam, lParam);                        \
    }                                                                               \
    \
    return result;                                                                  \
    }

#define END_DLG_MSG_MAP()                           \
    default:                                    \
    retVal = FALSE;                                                             \
    }                                                                               \
    \
    if(retVal)                                                                      \
        SetDlgMsgResult(hWnd, msg, result);                                         \
    \
    return retVal;                                                                  \
    }

// 窗口过程为全局函数
#define GET_GLOBAL_WND_PROC(theClass)           GET_WND_PROC_INTERNAL(theClass,         _)
#define DECLARE_GLOBAL_MSG_MAP(theClass)        DECLARE_MSG_MAP_INTERNAL(theClass,      _)
#define BEGIN_GLOBAL_MSG_MAP(theClass)          BEGIN_MSG_MAP_INTERNAL(theClass,        _)
#define END_GLOBAL_MSG_MAP()                    END_MSG_MAP()

#define GET_GLOBAL_DLG_PROC(theClass)           GET_DLG_PROC_INTERNAL(theClass,         _)
#define DECLARE_GLOBAL_DLG_MSG_MAP(theClass)    DECLARE_DLG_MSG_MAP_INTERNAL(theClass,  _)
#define BEGIN_GLOBAL_DLG_MSG_MAP(theClass)      BEGIN_DLG_MSG_MAP_INTERNAL(theClass,    _)
#define END_GLOBAL_DLG_MSG_MAP()                END_DLG_MSG_MAP()

// 绑定对象指针到窗口
#define ATTACH_OBJ_PTR_TO_WINDOW(hwnd, objPtr)  ::SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)objPtr)
#define GET_OBJ_PTR_FROM_WINDOW(hwnd, theClass) (theClass*)(LONG_PTR)::GetWindowLongPtr(hwnd, GWL_USERDATA)

#define DEFINE_OBJ_PTR_FROM_WINDOW(hwnd, theClass, pObj)        \
    theClass* pObj = GET_OBJ_PTR_FROM_WINDOW(hwnd, theClass);   \
    ASSERT(pObj);


/************************************************************************/
/*                        应用程序唯一实例                          */
/************************************************************************/

class COnlyOneApp
{
public:
    BOOL IsFirstApp()
    {
        return m_bIsFirstApp;
    }
    DWORD GetProcID()
    {
        return m_dwProcID;
    }

    COnlyOneApp(LPCTSTR pszAppFlag)
        : m_dwProcID(0), m_bIsFirstApp(FALSE)
    {
        m_hMap = ::CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, sizeof(DWORD), pszAppFlag);
        if (m_hMap)
        {
            if (::GetLastError() != ERROR_ALREADY_EXISTS)
            {
                m_bIsFirstApp = TRUE;
                m_dwProcID = ::GetCurrentProcessId();
                LPVOID lpBuff = ::MapViewOfFile(m_hMap, FILE_MAP_WRITE, 0, 0, sizeof(DWORD));
                ASSERT(lpBuff);
                memcpy(lpBuff, &m_dwProcID, sizeof(DWORD));
                ::UnmapViewOfFile(lpBuff);
            }
            else
            {
                m_bIsFirstApp = FALSE;
                LPVOID lpBuff = ::MapViewOfFile(m_hMap, FILE_MAP_READ, 0, 0, sizeof(DWORD));
                ASSERT(lpBuff);
                memcpy(&m_dwProcID, lpBuff, sizeof(DWORD));
                ::UnmapViewOfFile(lpBuff);
            }
        }
        else
            ASSERT(FALSE);
    }

    ~COnlyOneApp()
    {
        if (m_hMap)
        {
            ::CloseHandle(m_hMap);
            m_hMap = nullptr;
        }
    }

private:
    HANDLE  m_hMap;
    DWORD   m_dwProcID;
    BOOL    m_bIsFirstApp;

    DECLARE_NO_COPY_CLASS(COnlyOneApp)
};

class COnlyOneWndApp
{
public:
    BOOL IsFirstApp()
    {
        return m_hwndPre == nullptr;
    }
    HWND GetPreInstanceWindow()
    {
        return m_hwndPre;
    }

    COnlyOneWndApp(LPCTSTR lpszClassName, LPCTSTR lpszWindowName = nullptr)
    {
        m_hwndPre = ::FindWindow(lpszClassName, lpszWindowName);
    }

private:
    HWND m_hwndPre;

    DECLARE_NO_COPY_CLASS(COnlyOneWndApp)
};

/************************************************************************/
/*                      句柄 (HANDLE) 智能包装器                        */
/************************************************************************/

template<HANDLE NULL_VALUE>
class auto_handle
{
public:
    auto_handle(HANDLE h = NULL_VALUE)
    {
        set(h);
    }
    auto_handle(auto_handle<NULL_VALUE>& other)
    {
        set(other.release());
    }

    ~auto_handle()
    {
        reset();
    }

    auto_handle<NULL_VALUE>& reset(HANDLE h = NULL_VALUE)
    {
        if (h != m_h)
        {
            if (is_valid())
                VERIFY(::CloseHandle(m_h));
            set(h);
        }
        return *this;
    }

    auto_handle<NULL_VALUE>& reset(auto_handle<NULL_VALUE>& other)
    {
        if (this != &other)
            reset(other.release());
        return *this;
    }

    HANDLE release()
    {
        HANDLE h = get();
        set(NULL_VALUE);
        return h;
    }

    operator HANDLE()  const
    {
        return m_h;
    }
    HANDLE get()  const
    {
        return m_h;
    }
    HANDLE& get_ref()  const
    {
        return m_h;
    }
    bool is_valid()  const
    {
        return m_h != NULL_VALUE;
    }

    auto_handle<NULL_VALUE>& operator = (HANDLE h)
    {
        return reset(h);
    }
    auto_handle<NULL_VALUE>& operator = (auto_handle<NULL_VALUE>& other)
    {
        return reset(other);
    }

    bool operator == (HANDLE h)                             const
    {
        return m_h == h;
    }
    bool operator != (HANDLE h)                             const
    {
        return !(operator == (h));
    }
    bool operator == (const auto_handle<NULL_VALUE>& other) const
    {
        return m_h == other.m_h;
    }
    bool operator != (const auto_handle<NULL_VALUE>& other) const
    {
        return !(operator == (other));
    }

private:
    void set(HANDLE h)
    {
        m_h = h;
    }

    // ~! do not define these conversion, because it's very easy to making mistake !~
    template<HANDLE _Other> auto_handle(const auto_handle<_Other>&);
    template<HANDLE _Other> auto_handle<NULL_VALUE>& operator = (const auto_handle<_Other>&);

private:
    HANDLE  m_h;
};

typedef auto_handle<INVALID_HANDLE_VALUE>   auto_file_handle;   // 文件智能句柄
typedef auto_handle<nullptr>                auto_res_handle;    // 普通资源智能句柄

/************************************************************************/
/*                          DC 智能包装器                               */
/************************************************************************/

class auto_dc
{
public:
    auto_dc(HDC h = nullptr, HWND w = nullptr, bool is_create = false)
    {
        set(h, w, is_create);
    }

    auto_dc(auto_dc& other)
    {
        set(other.m_h, other.m_w, other.m_is_create);
        other.release();
    }

    ~auto_dc()
    {
        reset();
    }

    HDC GetDC(HWND hWnd)
    {
        HDC h = ::GetDC(hWnd);
        reset(h, hWnd, false);
        return h;
    }

    HDC GetWindowDC(HWND hWnd)
    {
        HDC h = ::GetWindowDC(hWnd);
        reset(h, hWnd, false);
        return h;
    }

    HDC GetDCEx(HWND hWnd, HRGN hrgnClip, DWORD flags)
    {
        HDC h = ::GetDCEx(hWnd, hrgnClip, flags);
        reset(h, hWnd, false);
        return h;
    }

    HDC CreateDC(LPCTSTR lpszDriver, LPCTSTR lpszDevice, LPCTSTR lpszOutput, CONST DEVMODE* lpInitData)
    {
        HDC h = ::CreateDC(lpszDriver, lpszDevice, lpszOutput, lpInitData);
        reset(h, nullptr, true);
        return h;
    }

    HDC CreateCompatibleDC(HDC hSrc)
    {
        HDC h = ::CreateCompatibleDC(hSrc);
        reset(h, nullptr, true);
        return h;
    }

    HGDIOBJ GetCurrentObject(UINT uObjectType)
    {
        return ::GetCurrentObject(m_h, uObjectType);
    }

    HBITMAP _GetCurrentBitmap()
    {
        return (HBITMAP)GetCurrentObject(OBJ_BITMAP);
    }

    HBRUSH _GetCurrentBrush()
    {
        return (HBRUSH)GetCurrentObject(OBJ_BRUSH);
    }

    HPALETTE _GetCurrentPalette()
    {
        return (HPALETTE)GetCurrentObject(OBJ_PAL);
    }

    HPEN _GetCurrentPen()
    {
        return (HPEN)GetCurrentObject(OBJ_PEN);
    }

    HFONT _GetCurrentFont()
    {
        return (HFONT)GetCurrentObject(OBJ_FONT);
    }

    int SelectClipRgn(HRGN hrgn)
    {
        return ::SelectClipRgn(m_h, hrgn);
    }

    int ExtSelectClipRgn(HRGN hrgn, int mode)
    {
        return ::ExtSelectClipRgn(m_h, hrgn, mode);
    }

    HGDIOBJ SelectObject(HGDIOBJ hgdiobj)
    {
        return ::SelectObject(m_h, hgdiobj);
    }

    HPALETTE SelectPalette(HPALETTE hpal, BOOL bForceBackground)
    {
        return ::SelectPalette(m_h, hpal, bForceBackground);
    }

    HBITMAP _SelectBitmap(HBITMAP hbm)
    {
        return (HBITMAP)SelectObject(hbm);
    }

    HBRUSH _SelectBrush(HBRUSH hbr)
    {
        return (HBRUSH)SelectObject(hbr);
    }

    HPEN _SelectPen(HPEN hpen)
    {
        return (HPEN)SelectObject(hpen);
    }

    HRGN _SelectRgn(HRGN hrgn)
    {
        return (HRGN)SelectObject(hrgn);
    }

    HFONT _SelectFont(HFONT hf)
    {
        return (HFONT)SelectObject(hf);
    }

    auto_dc& reset(HDC h = nullptr, HWND w = nullptr, bool is_create = false)
    {
        if (h != m_h || w != m_w)
        {
            if (is_valid())
            {
                if (m_is_create)
                    VERIFY(::DeleteDC(m_h));
                else
                    VERIFY(::ReleaseDC(m_w, m_h));
            }
            set(h, w, is_create);
        }
        return *this;
    }

    auto_dc& reset(auto_dc& other)
    {
        if (this != &other)
        {
            reset(other.m_h, other.m_w, other.m_is_create);
            other.release();
        }
        return *this;
    }

    HDC release()
    {
        HDC h = get_dc();
        set(nullptr, nullptr, false);
        return h;
    }

    /*
    auto_dc& operator = (HDC h)
    {
    return reset(h);
    }
    */

    auto_dc& operator = (auto_dc& other)
    {
        return reset(other);
    }

    operator HDC()  const
    {
        return m_h;
    }
    HDC         get_dc()  const
    {
        return m_h;
    }
    const HDC&  get_dc_ref()  const
    {
        return m_h;
    }
    HWND        get_wnd()  const
    {
        return m_w;
    }
    bool        is_valid()  const
    {
        return m_h != nullptr;
    }
    bool        is_create()  const
    {
        return m_is_create;
    }

private:
    void set(HDC h, HWND w, bool is_create)
    {
        m_h = h;
        m_w = w;
        m_is_create = is_create;
    }

private:
    HDC     m_h;
    HWND    m_w;
    bool    m_is_create;
};

class paint_dc
{
public:
    paint_dc(HWND hwnd) : m_hwnd(hwnd)
    {
        VERIFY(m_hdc = ::BeginPaint(m_hwnd, &m_ps));
    }

    ~paint_dc()
    {
        VERIFY(::EndPaint(m_hwnd, &m_ps));
    }

    operator HDC() const
    {
        return m_hdc;
    }
    bool is_valid() const
    {
        return m_hdc != nullptr;
    }

public:
    PAINTSTRUCT m_ps;
    HWND        m_hwnd;
    HDC         m_hdc;
};
/************************************************************************/
/*                          GDI Object 智能包装器                       */
/************************************************************************/

template<class T>
class auto_gdi_obj
{
public:
    auto_gdi_obj(T obj = nullptr)
    {
        set(obj);
    }
    auto_gdi_obj(auto_gdi_obj<T>& other)
    {
        set(other.release());
    }

    ~auto_gdi_obj()
    {
        reset();
    }

    auto_gdi_obj<T>& reset(T obj = nullptr)
    {
        if (obj != m_obj)
        {
            if (is_valid())
                VERIFY(::DeleteObject(m_obj));
            set(obj);
        }
        return *this;
    }

    auto_gdi_obj<T>& reset(auto_gdi_obj<T>& other)
    {
        if (this != &other)
            reset(other.release());
        return *this;
    }

    T release()
    {
        T obj = get();
        set(nullptr);
        return obj;
    }

    auto_gdi_obj<T>& operator = (T obj)
    {
        return reset(obj);
    }
    auto_gdi_obj<T>& operator = (auto_gdi_obj<T>& other)
    {
        return reset(other);
    }

    operator T()  const
    {
        return m_obj;
    }
    T           get()  const
    {
        return m_obj;
    }
    const T&    get_ref()  const
    {
        return m_obj;
    }
    bool        is_valid()  const
    {
        return m_obj != nullptr;
    }

private:
    void set(T obj)
    {
        m_obj = obj;
    }

protected:
    T m_obj;
};

typedef auto_gdi_obj<HBITMAP>       auto_bitmap_base;
typedef auto_gdi_obj<HBRUSH>        auto_brush_base;
typedef auto_gdi_obj<HPALETTE>      auto_palette_base;
typedef auto_gdi_obj<HPEN>          auto_pen_base;
typedef auto_gdi_obj<HRGN>          auto_rgn_base;
typedef auto_gdi_obj<HFONT>         auto_font_base;

class auto_bitmap : public auto_bitmap_base
{
public:
    auto_bitmap(HBITMAP obj = nullptr) : auto_bitmap_base(obj) {}

    auto_bitmap& operator = (HBITMAP obj)
    {
        return (auto_bitmap&)reset(obj);
    }

    HBITMAP CreateBitmap(int nWidth, int nHeight, UINT cPlanes, UINT cBitsPerPel, CONST VOID* lpvBits)
    {
        HBITMAP obj = ::CreateBitmap(nWidth, nHeight, cPlanes, cBitsPerPel, lpvBits);
        reset(obj);
        return obj;
    }

    HBITMAP CreateBitmapIndirect(CONST BITMAP* lpbm)
    {
        HBITMAP obj = ::CreateBitmapIndirect(lpbm);
        reset(obj);
        return obj;
    }

    HBITMAP CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight)
    {
        HBITMAP obj = ::CreateCompatibleBitmap(hdc, nWidth, nHeight);
        reset(obj);
        return obj;
    }

    HBITMAP CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT iUsage, void** ppvBits, HANDLE hSection, DWORD dwOffset)
    {
        HBITMAP obj = ::CreateDIBSection(hdc, pbmi, iUsage, ppvBits, hSection, dwOffset);
        reset(obj);
        return obj;
    }

    HBITMAP LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmapName)
    {
        HBITMAP obj = ::LoadBitmap(hInstance, lpBitmapName);
        reset(obj);
        return obj;
    }

    int GetBitmap(BITMAP* pBitMap)
    {
        ASSERT(m_obj != nullptr);
        return ::GetObject(m_obj, sizeof(BITMAP), pBitMap);
    }

    int GetDIBSection(DIBSECTION* pDIBSection)
    {
        ASSERT(m_obj != nullptr);
        return ::GetObject(m_obj, sizeof(DIBSECTION), pDIBSection);
    }
};

class auto_brush : public auto_brush_base
{
public:
    auto_brush(HBRUSH obj = nullptr) : auto_brush_base(obj) {}

    auto_brush& operator = (HBRUSH obj)
    {
        return (auto_brush&)reset(obj);
    }

    HBRUSH CreateDIBPatternBrushPt(const void* lpPackedDIB, UINT iUsage)
    {
        HBRUSH obj = ::CreateDIBPatternBrushPt(lpPackedDIB, iUsage);
        reset(obj);
        return obj;
    }

    HBRUSH CreatePatternBrush(HBITMAP hbmp)
    {
        HBRUSH obj = ::CreatePatternBrush(hbmp);
        reset(obj);
        return obj;
    }

    HBRUSH CreateSolidBrush(COLORREF crColor)
    {
        HBRUSH obj = ::CreateSolidBrush(crColor);
        reset(obj);
        return obj;
    }

    int GetLogBrush(LOGBRUSH* pLogBrush)
    {
        ASSERT(m_obj != nullptr);
        return ::GetObject(m_obj, sizeof(LOGBRUSH), pLogBrush);
    }
};

class auto_palette : public auto_palette_base
{
public:
    auto_palette(HPALETTE obj = nullptr) : auto_palette_base(obj) {}

    auto_palette& operator = (HPALETTE obj)
    {
        return (auto_palette&)reset(obj);
    }

    HPALETTE CreatePalette(CONST LOGPALETTE* lplgpl)
    {
        HPALETTE obj = ::CreatePalette(lplgpl);
        reset(obj);
        return obj;
    }

    int GetEntryCount()
    {
        ASSERT(m_obj != nullptr);
        WORD nEntries;
        ::GetObject(m_obj, sizeof(WORD), &nEntries);
        return (int)nEntries;
    }
};

class auto_pen : public auto_pen_base
{
public:
    auto_pen(HPEN obj = nullptr) : auto_pen_base(obj) {}

    auto_pen& operator = (HPEN obj)
    {
        return (auto_pen&)reset(obj);
    }

    HPEN CreatePen(int fnPenStyle, int nWidth, COLORREF crColor)
    {
        HPEN obj = ::CreatePen(fnPenStyle, nWidth, crColor);
        reset(obj);
        return obj;
    }

    HPEN CreatePenIndirect(const LOGPEN* lplgpn)
    {
        HPEN obj = ::CreatePenIndirect(lplgpn);
        reset(obj);
        return obj;
    }

    int GetLogPen(LOGPEN* pLogPen)
    {
        ASSERT(m_obj != nullptr);
        return ::GetObject(m_obj, sizeof(LOGPEN), pLogPen);
    }
};

class auto_rgn : public auto_rgn_base
{
public:
    auto_rgn(HRGN obj = nullptr) : auto_rgn_base(obj) {}

    auto_rgn& operator = (HRGN obj)
    {
        return (auto_rgn&)reset(obj);
    }

    HRGN CreateRectRgn(int nLeftRect, int nTopRect, int nRightRect, int nBottomRect)
    {
        HRGN obj = ::CreateRectRgn(nLeftRect, nTopRect, nRightRect, nBottomRect);
        reset(obj);
        return obj;
    }

    HRGN CreateRectRgnIndirect(const RECT* lprc)
    {
        HRGN obj = ::CreateRectRgnIndirect(lprc);
        reset(obj);
        return obj;
    }
};

class auto_font : public auto_font_base
{
public:
    auto_font(HFONT obj = nullptr) : auto_font_base(obj) {}

    auto_font& operator = (HFONT obj)
    {
        return (auto_font&)reset(obj);
    }

    HFONT CreateFont(
        int nHeight,                // height of font
        int nWidth,                 // average character width
        int nEscapement,            // angle of escapement
        int nOrientation,           // base-line orientation angle
        int fnWeight,               // font weight
        DWORD bItalic,              // italic attribute option
        DWORD bUnderline,           // underline attribute option
        DWORD cStrikeOut,           // strikeout attribute option
        DWORD nCharSet,             // character set identifier
        DWORD nOutPrecision,        // output precision
        DWORD nClipPrecision,       // clipping precision
        DWORD nQuality,             // output quality
        DWORD nPitchAndFamily,      // pitch and family
        LPCTSTR lpszFace           // typeface name
        )
    {
        HFONT obj = ::CreateFont(
            nHeight,
            nWidth,
            nEscapement,
            nOrientation,
            fnWeight,
            bItalic,
            bUnderline,
            cStrikeOut,
            nCharSet,
            nOutPrecision,
            nClipPrecision,
            nQuality,
            nPitchAndFamily,
            lpszFace
            );
        reset(obj);
        return obj;
    }

    HFONT CreateFontIndirect(const LOGFONT* lplf)
    {
        HFONT obj = ::CreateFontIndirect(lplf);
        reset(obj);
        return obj;
    }

    int GetLogFont(LOGFONT* pLogFont)
    {
        ASSERT(m_obj != nullptr);
        return ::GetObject(m_obj, sizeof(LOGFONT), pLogFont);
    }
};

class CPrivateHeapImpl
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
    CPrivateHeapImpl(DWORD dwOptions = 0, SIZE_T dwInitSize = 0, SIZE_T dwMaxSize = 0)
        : m_dwOptions(dwOptions), m_dwInitSize(dwInitSize), m_dwMaxSize(dwMaxSize)
    {
        m_hHeap = ::HeapCreate(m_dwOptions, m_dwInitSize, m_dwMaxSize);
    }

    ~CPrivateHeapImpl()
    {
        if (IsValid()) ::HeapDestroy(m_hHeap);
    }

    operator HANDLE()
    {
        return m_hHeap;
    }

private:
    CPrivateHeapImpl(const CPrivateHeapImpl&);
    CPrivateHeapImpl operator = (const CPrivateHeapImpl&);

private:
    HANDLE  m_hHeap;
    DWORD   m_dwOptions;
    SIZE_T  m_dwInitSize;
    SIZE_T  m_dwMaxSize;
};

class CGlobalHeapImpl
{
public:
    PVOID Alloc(SIZE_T dwSize, DWORD dwFlags = 0)
    {
        PVOID pv = malloc(dwSize);
        if (pv && (dwFlags & HEAP_ZERO_MEMORY))
            ZeroMemory(pv, dwSize);
        return pv;
    }

    PVOID ReAlloc(PVOID pvMemory, SIZE_T dwSize, DWORD dwFlags = 0)
    {
        PVOID pv = realloc(pvMemory, dwSize);
        if (pv && (dwFlags & HEAP_ZERO_MEMORY))
            ZeroMemory(pv, dwSize);
        else if (!pv)
            free(pvMemory);
        return pv;
    }

    BOOL Free(PVOID pvMemory, DWORD dwFlags = 0)
    {
        if (pvMemory)
        {
            free(pvMemory);
            return TRUE;
        }
        return FALSE;
    }

    SIZE_T Compact(DWORD dwFlags = 0)
    {
        return 0;
    }
    SIZE_T Size(PVOID pvMemory, DWORD dwFlags = 0)
    {
        return _msize(pvMemory);
    }

    BOOL IsValid()
    {
        return TRUE;
    }
    BOOL Reset()
    {
        return TRUE;
    }

public:
    CGlobalHeapImpl(DWORD dwOptions = 0, SIZE_T dwInitSize = 0, SIZE_T dwMaxSize = 0) {}
    ~CGlobalHeapImpl()  {}

    operator HANDLE()
    {
        return nullptr;
    }

private:
    CGlobalHeapImpl(const CGlobalHeapImpl&);
    CGlobalHeapImpl operator = (const CGlobalHeapImpl&);
};

#ifndef _NOT_USE_PRIVATE_HEAP
typedef CPrivateHeapImpl    CPrivateHeap;
#else
typedef CGlobalHeapImpl     CPrivateHeap;
#endif

template<class T> class CPrivateHeapBuffer
{
public:
    CPrivateHeapBuffer(CPrivateHeap& hpPrivate, SIZE_T dwSize = 0)
        : m_hpPrivate(hpPrivate)
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
    T* Alloc(SIZE_T dwSize, DWORD dwFlags = 0)
    {
        if (IsValid())
            Free();
        if (dwSize > 0)
            m_pvMemory = (T*)m_hpPrivate.Alloc(dwSize * sizeof(T), dwFlags);
        return m_pvMemory;
    }

    T* ReAlloc(SIZE_T dwSize, DWORD dwFlags = 0)
    {
        return m_pvMemory = (T*)m_hpPrivate.ReAlloc(m_pvMemory, dwSize * sizeof(T), dwFlags);
    }

    SIZE_T Size(DWORD dwFlags = 0)
    {
        return m_hpPrivate.Size(m_pvMemory, dwFlags) / sizeof(T);
    }

    BOOL Free(DWORD dwFlags = 0)
    {
        BOOL isOK = TRUE;
        if (IsValid())
        {
            isOK = m_hpPrivate.Free(m_pvMemory, dwFlags);
            m_pvMemory = nullptr;
        }
        return isOK;
    }

    BOOL IsValid()
    {
        return m_pvMemory != nullptr;
    }
    operator T* ()          const
    {
        return m_pvMemory;
    }
    T& operator [](int i)  const
    {
        return *(m_pvMemory + i);
    }

private:
    CPrivateHeapBuffer(const CPrivateHeapBuffer&);
    CPrivateHeapBuffer operator = (const CPrivateHeapBuffer&);

private:
    CPrivateHeap&   m_hpPrivate;
    T*              m_pvMemory;
};

typedef CPrivateHeapBuffer<BYTE>    CPrivateHeapByteBuffer;
typedef CPrivateHeapBuffer<TCHAR>   CPrivateHeapStrBuffer;

template<class T, size_t MAX_CACHE_SIZE = 0>
class CBufferPtrT
{
public:
    explicit CBufferPtrT(size_t size = 0, bool zero = false)
    {
        Reset();
        Malloc(size, zero);
    }
    explicit CBufferPtrT(const T* pch, size_t size)
    {
        Reset();
        Copy(pch, size);
    }
    CBufferPtrT(const CBufferPtrT& other)
    {
        Reset();
        Copy(other);
    }
    template<size_t S> CBufferPtrT(const CBufferPtrT<T, S>& other)
    {
        Reset();
        Copy(other);
    }

    ~CBufferPtrT()
    {
        Free();
    }

    T* Malloc(size_t size = 1, bool zero = false)
    {
        Free();
        return Alloc(size, zero, false);
    }

    T* Realloc(size_t size, bool zero = false)
    {
        return Alloc(size, zero, true);
    }

    void Free()
    {
        if (m_pch)
        {
            free(m_pch);
            Reset();
        }
    }

    template<size_t S> CBufferPtrT& Copy(const CBufferPtrT<T, S>& other)
    {
        if ((void*)&other != (void*)this)
            Copy(other.Ptr(), other.Size());
        return *this;
    }

    CBufferPtrT& Copy(const T* pch, size_t size)
    {
        Malloc(size);
        if (m_pch)
            memcpy(m_pch, pch, size * sizeof(T));
        return *this;
    }

    template<size_t S> CBufferPtrT& Cat(const CBufferPtrT<T, S>& other)
    {
        if ((void*)&other != (void*)this)
            Cat(other.Ptr(), other.Size());
        return *this;
    }

    CBufferPtrT& Cat(const T* pch, size_t size = 1)
    {
        size_t pre_size = m_size;
        Realloc(m_size + size);
        if (m_pch)
            memcpy(m_pch + pre_size, pch, size * sizeof(T));
        return *this;
    }

    template<size_t S> bool Equal(const CBufferPtrT<T, S>& other) const
    {
        if ((void*)&other == (void*)this)
            return true;
        else if (m_size != other.Size())
            return false;
        else if (m_size == 0)
            return true;
        else
            return (memcmp(m_pch, other.Ptr(), m_size * sizeof(T)) == 0);
    }

    bool Equal(T* pch) const
    {
        if (m_pch == pch)
            return true;
        else if (!m_pch || !pch)
            return false;
        else
            return (memcmp(m_pch, pch, m_size * sizeof(T)) == 0);
    }

    T*          Ptr()
    {
        return m_pch;
    }
    const T*    Ptr()           const
    {
        return m_pch;
    }
    T&          Get(int i)
    {
        return *(m_pch + i);
    }
    const T&    Get(int i)      const
    {
        return *(m_pch + i);
    }
    size_t      Size()          const
    {
        return m_size;
    }
    bool        IsValid()       const
    {
        return m_pch != 0;
    }

    operator T*  ()
    {
        return Ptr();
    }
    operator const                      T*  ()          const
    {
        return Ptr();
    }
    T& operator                         [](int i)
    {
        return Get(i);
    }
    const T& operator                   [](int i)     const
    {
        return Get(i);
    }
    bool operator                       == (T* pv)     const
    {
        return Equal(pv);
    }
    template<size_t S> bool operator    == (const CBufferPtrT<T, S>& other)
    {
        return Equal(other);
    }
    CBufferPtrT& operator               = (const CBufferPtrT& other)
    {
        return Copy(other);
    }
    template<size_t S> CBufferPtrT& operator = (const CBufferPtrT<T, S>& other)
    {
        return Copy(other);
    }

private:
    void Reset()
    {
        m_pch = 0;
        m_size = 0;
        m_capacity = 0;
    }
    size_t GetAllocSize(size_t size)
    {
        return max(size, min(size * 2, m_size + MAX_CACHE_SIZE));
    }

    T* Alloc(size_t size, bool zero = false, bool is_realloc = false)
    {
        if (size >= 0 && size != m_size)
        {
            size_t rsize = GetAllocSize(size);
            if (size > m_capacity || rsize < m_size)
            {
                m_pch = is_realloc ?
                    (T*)realloc(m_pch, rsize * sizeof(T)) :
                    (T*)malloc(rsize * sizeof(T));
                if (m_pch || rsize == 0)
                {
                    m_size = size;
                    m_capacity = rsize;
                }
                else
                    Reset();
            }
            else
                m_size = size;
        }
        if (zero && m_pch)
            memset(m_pch, 0, m_size * sizeof(T));
        return m_pch;
    }

private:
    T*      m_pch;
    size_t  m_size;
    size_t  m_capacity;
};

typedef CBufferPtrT<char>           CCharBufferPtr;
typedef CBufferPtrT<wchar_t>        CWCharBufferPtr;
typedef CBufferPtrT<unsigned char>  CByteBufferPtr;
typedef CByteBufferPtr              CBufferPtr;

#ifdef _UNICODE
typedef CWCharBufferPtr         CTCharBufferPtr;
#else
typedef CCharBufferPtr          CTCharBufferPtr;
#endif

#include <eh.h>

#define MapSEx2CppEx()  CSE::MapSE2CE()

class CSE
{
public:
    // 结构化异常转换成 C++ 异常.
    static void MapSE2CE()
    {
        _set_se_translator(TranslateSE2CE);
    }

    operator DWORD()
    {
        return m_er.ExceptionCode;
    }

    CSE(PEXCEPTION_POINTERS pep)
        : m_er(*pep->ExceptionRecord)
        , m_context(*pep->ContextRecord)
    {
    }

    static void _cdecl TranslateSE2CE(UINT ec, PEXCEPTION_POINTERS pep)
    {
        throw CSE(pep);
    }

private:
    EXCEPTION_RECORD    m_er;           // CPU independent exception informaiton
    CONTEXT             m_context;      // CPU dependent exception informaiton
};


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

class CSlimRWLock
{
public:
    VOID WaitToRead();
    VOID WaitToWrite();
    VOID ReadDone();
    VOID WriteDone();

private:
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
    CSlimRWLock();
    ~CSlimRWLock();

private:
    CSlimRWLock(const CSlimRWLock&);
    CSlimRWLock operator = (const CSlimRWLock&);

private:
    int m_nActive;
    int m_nReadCount;
    DWORD m_dwWriterTID;

    CSpinGuard  m_cs;
    CSlimLock   m_smLock;
};

#endif

class CSEMRWLock
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
    CSEMRWLock();
    ~CSEMRWLock();

private:
    CSEMRWLock(const CSEMRWLock&);
    CSEMRWLock operator = (const CSEMRWLock&);

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

typedef CSEMRWLock                      CRWLock;
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



#pragma once

#define CACHE_LINE      64
#define PACK_SIZE_OF(T) (CACHE_LINE - sizeof(T) % CACHE_LINE)

#if !defined (_WIN64)
#pragma pack(push, 4)
#endif

template <class T, class _PutGuard = CCriSec, class _GetGuard = CCriSec> class CRingBuffer
{
public:
    static const UINT DEFAULT_EXPECT = 4096;

public:
    BOOL Put(T* pElement)
    {
        ASSERT(pElement != nullptr);
        {
            CLocalLock<_PutGuard> locallock(m_csPut);
            ULONGLONG seqPut = m_seqPut;
            WaitForPut(seqPut);
            if (!IsValid()) return FALSE;
            DoPut(pElement, seqPut);
        }
        return TRUE;
    }

    BOOL TryPut(T* pElement)
    {
        ASSERT(pElement != nullptr);
        if (!IsValid() || !HasPutSpace(m_seqPut))
            return FALSE;
        {
            CLocalLock<_PutGuard> locallock(m_csPut);
            ULONGLONG seqPut = m_seqPut;
            if (!IsValid() || !HasPutSpace(seqPut))
                return FALSE;
            DoPut(pElement, seqPut);
        }
        return TRUE;
    }

    BOOL PutBatch(T* pElements[], int& iCount)
    {
        ASSERT(pElements != nullptr && iCount > 0);
        {
            CLocalLock<_PutGuard> locallock(m_csPut);
            ULONGLONG seqPut = m_seqPut;
            for (int i = 0; i < iCount; ++i)
            {
                WaitForPut(seqPut);
                if (!IsValid())
                {
                    iCount = i;
                    return FALSE;
                }
                DoPut(*(pElements + i), seqPut);
            }
        }
        return TRUE;
    }

    BOOL TryPutBatch(T* pElements[], int& iCount)
    {
        ASSERT(pElements != nullptr && iCount > 0);
        if (!IsValid() || !HasPutSpace(m_seqPut))
        {
            iCount = 0;
            return FALSE;
        }
        {
            CLocalLock<_PutGuard> locallock(m_csPut);
            ULONGLONG seqPut = m_seqPut;
            for (int i = 0; i < iCount; ++i)
            {
                if (!IsValid() || !HasPutSpace(seqPut))
                {
                    iCount = i;
                    return FALSE;
                }
                DoPut(*(pElements + i), seqPut);
            }
        }
        return TRUE;
    }

    BOOL Get(T** pElement)
    {
        ASSERT(pElement != nullptr);
        {
            CLocalLock<_GetGuard> locallock(m_csGet);
            ULONGLONG seqGet = m_seqGet;
            WaitForGet(seqGet);
            if (!IsValid()) return FALSE;
            DoGet(pElement, seqGet);
        }
        return TRUE;
    }

    BOOL TryGet(T** pElement)
    {
        ASSERT(pElement != nullptr);
        if (!IsValid() || !HasGetSpace(m_seqGet))
            return FALSE;
        {
            CLocalLock<_GetGuard> locallock(m_csGet);
            ULONGLONG seqGet = m_seqGet;
            if (!IsValid() || !HasGetSpace(seqGet))
                return FALSE;
            DoGet(pElement, seqGet);
        }
        return TRUE;
    }

    BOOL GetBatch(T* pElements[], int& iCount)
    {
        ASSERT(pElements != nullptr && iCount > 0);
        {
            CLocalLock<_GetGuard> locallock(m_csGet);
            ULONGLONG seqGet = m_seqGet;
            for (int i = 0; i < iCount; ++i)
            {
                WaitForGet(seqGet);
                if (!IsValid())
                {
                    iCount = i;
                    return FALSE;
                }
                DoGet(pElements + i, seqGet);
            }
        }
        return TRUE;
    }

    BOOL TryGetBatch(T* pElements[], int& iCount)
    {
        ASSERT(pElements != nullptr && iCount > 0);
        if (!IsValid() || !HasGetSpace(m_seqGet))
        {
            iCount = 0;
            return FALSE;
        }
        {
            CLocalLock<_GetGuard> locallock(m_csGet);
            ULONGLONG seqGet = m_seqGet;
            for (int i = 0; i < iCount; ++i)
            {
                if (!IsValid() || !HasGetSpace(seqGet))
                {
                    iCount = i;
                    return FALSE;
                }
                DoGet(pElements + i, seqGet);
            }
        }
        return TRUE;
    }

    BOOL Peek(T** pElement)
    {
        ASSERT(pElement != nullptr);
        ULONGLONG seqGet = m_seqGet;
        if (!IsValid() || !HasGetSpace(seqGet))
            return FALSE;
        DoPeek(pElement, seqGet);
        return TRUE;
    }

private:
    void DoPut(T* pElement, ULONGLONG& seqPut)
    {
        DWORD index = seqPut & (m_dwReal - 1);
        *(m_pv + index) = pElement;
        ++seqPut;
        m_seqPut = seqPut;
    }

    void DoGet(T** pElement, ULONGLONG& seqGet)
    {
        DWORD index = seqGet & (m_dwReal - 1);
        *(pElement) = *(m_pv + index);
        ++seqGet;
        m_seqGet = seqGet;
    }

    void DoPeek(T** pElement, ULONGLONG& seqGet)
    {
        DWORD index = seqGet & (m_dwReal - 1);
        *(pElement) = *(m_pv + index);
    }

    BOOL HasPutSpace(ULONGLONG seqPut)
    {
        return (seqPut - m_seqGet < m_dwReal);
    }

    void WaitForPut(ULONGLONG seqPut)
    {
        for (DWORD w = 0; IsValid(); ++w)
        {
            if (HasPutSpace(seqPut))
                break;
            ::YieldThread(w);
        }
    }

    BOOL HasGetSpace(ULONGLONG seqGet)
    {
        return (m_seqPut - seqGet > 0);
    }

    void WaitForGet(ULONGLONG seqGet)
    {
        for (DWORD w = 0; IsValid(); ++w)
        {
            if (HasGetSpace(seqGet))
                break;
            ::YieldThread(w);
        }
    }

    DWORD Revise(DWORD dwExpect)
    {
        int index = 0;
        int shift = sizeof(DWORD) * 8 - 1;
        for (int i = shift; i >= 0; i--)
        {
            if (index == 0)
            {
                if (dwExpect & (1 << i))
                {
                    index = i;
                    if (index == shift)
                        break;
                }
            }
            else
            {
                if (dwExpect & (1 << i))
                    ++index;
                break;
            }
        }
        return 1 << index;
    }

public:
    CRingBuffer(DWORD uiExpect = DEFAULT_EXPECT)
        : m_pv(nullptr)
        , m_dwReal(0)
        , m_seqPut(0)
        , m_seqGet(0)
    {
        Reset(uiExpect);
    }

    ~CRingBuffer()
    {
        Reset(0);
    }

    void Reset(DWORD uiExpect = DEFAULT_EXPECT)
    {
        if (IsValid())
            Destroy();
        if (uiExpect > 0)
            Create(uiExpect);
    }

    BOOL IsValid()
    {
        return m_pv != nullptr;
    }

private:
    void Create(DWORD dwExpect = DEFAULT_EXPECT)
    {
        ASSERT(!IsValid() && dwExpect > 0);
        m_seqPut = 0;
        m_seqGet = 0;
        m_dwReal = Revise(dwExpect);
        m_pv = (T**)malloc(m_dwReal * sizeof(T*));
    }

    void Destroy()
    {
        ASSERT(IsValid());
        CLocalLock<_PutGuard> locallock1(m_csPut);
        CLocalLock<_GetGuard> locallock2(m_csGet);
        free((void*)m_pv);
        m_pv = nullptr;
        m_dwReal = 0;
        m_seqPut = 0;
        m_seqGet = 0;
    }

private:
    CRingBuffer(const CRingBuffer&);
    CRingBuffer operator = (const CRingBuffer&);

private:
    DWORD               m_dwReal;
    T**                 m_pv;
    char                pack1[PACK_SIZE_OF(T**)];
    volatile ULONGLONG  m_seqPut;
    char                pack4[PACK_SIZE_OF(ULONGLONG)];
    volatile ULONGLONG  m_seqGet;
    char                pack5[PACK_SIZE_OF(ULONGLONG)];
    _PutGuard           m_csPut;
    char                pack2[PACK_SIZE_OF(_PutGuard)];
    _GetGuard           m_csGet;
    char                pack3[PACK_SIZE_OF(_GetGuard)];
};

typedef CRingBuffer<void, CCriSec, CCriSec>             CCSRingBuffer;
typedef CRingBuffer<void, CInterCriSec, CInterCriSec>   CICSRingBuffer;
typedef CRingBuffer<void, CSpinGuard, CSpinGuard>       CSGRingBuffer;
typedef CRingBuffer<void, CFakeGuard, CFakeGuard>       CFKRingBuffer;

// ------------------------------------------------------------------------------------------------------------- //

template <class T, class index_type = DWORD, bool adjust_index = false> class CRingCache
{
public:

    enum EnGetResult { GR_FAIL = -1, GR_INVALID = 0, GR_VALID = 1 };

    typedef T*                                  TPTR;
    typedef volatile T*                         VTPTR;

    typedef unordered_set<index_type>           IndexSet;
    typedef typename IndexSet::const_iterator   IndexSetCI;
    typedef typename IndexSet::iterator         IndexSetI;

    static TPTR const E_EMPTY;
    static TPTR const E_LOCKED;
    static TPTR const E_MAX_STATUS;

public:

    static index_type& INDEX_INC(index_type& dwIndex)
    {
        if (adjust_index) ++dwIndex;
        return dwIndex;
    }
    static index_type& INDEX_DEC(index_type& dwIndex)
    {
        if (adjust_index) --dwIndex;
        return dwIndex;
    }

private:

    index_type& INDEX_V2R(index_type& dwIndex)
    {
        dwIndex %= m_dwSize;
        if (dwIndex == 0) dwIndex = m_dwSize;
        return dwIndex;
    }
    VTPTR& INDEX_VAL(index_type dwIndex)
    {
        return *(m_pv + dwIndex);
    }

public:

    BOOL Put(TPTR pElement, index_type& dwIndex)
    {
        ASSERT(pElement != nullptr);
        if (!IsValid()) return FALSE;
        BOOL isOK = FALSE;
        while (true)
        {
            if (!HasSpace())
                break;
            DWORD dwCurSeq = m_dwCurSeq;
            index_type dwCurIndex = dwCurSeq % m_dwSize;
            VTPTR& pValue = INDEX_VAL(dwCurIndex);
            if (pValue == E_EMPTY)
            {
                if (::InterlockedCompareExchangePointer((volatile PVOID*)&pValue, pElement, E_EMPTY) == E_EMPTY)
                {
                    ::InterlockedIncrement(&m_dwCount);
                    ::InterlockedCompareExchange(&m_dwCurSeq, dwCurSeq + 1, dwCurSeq);
                    dwIndex = INDEX_INC(dwCurIndex);
                    isOK = TRUE;
                    if (pElement != E_LOCKED)
                        EmplaceIndex(dwIndex);
                    break;
                }
            }
            ::InterlockedCompareExchange(&m_dwCurSeq, dwCurSeq + 1, dwCurSeq);
        }
        return isOK;
    }

    EnGetResult GetEx(index_type dwIndex, TPTR* ppElement)
    {
        return Get(INDEX_V2R(dwIndex), ppElement);
    }

    EnGetResult Get(index_type dwIndex, TPTR* ppElement)
    {
        ASSERT(dwIndex <= m_dwSize);
        ASSERT(ppElement != nullptr);
        if (!IsValid() || INDEX_DEC(dwIndex) >= m_dwSize)
        {
            *ppElement = nullptr;
            return GR_FAIL;
        }
        *ppElement = (TPTR)INDEX_VAL(dwIndex);
        return IsValidElement(*ppElement) ? GR_VALID : GR_INVALID;
    }

    BOOL SetEx(index_type dwIndex, TPTR pElement, TPTR* ppOldElement = nullptr)
    {
        return Set(INDEX_V2R(dwIndex), pElement, ppOldElement);
    }

    BOOL Set(index_type dwIndex, TPTR pElement, TPTR* ppOldElement = nullptr)
    {
        TPTR pElement2 = nullptr;
        if (Get(dwIndex, &pElement2) == GR_FAIL)
            return FALSE;
        if (ppOldElement != nullptr)
            *ppOldElement = pElement2;
        if (pElement == pElement2)
            return FALSE;
        int f1 = 0;
        int f2 = 0;
        if (pElement == E_EMPTY)
        {
            if (pElement2 == E_LOCKED)
                f1 = -1;
            else
                f1 = f2 = -1;
        }
        else if (pElement == E_LOCKED)
        {
            if (pElement2 == E_EMPTY)
                f1 = 1;
            else
                f2 = -1;
        }
        else
        {
            if (pElement2 == E_EMPTY)
                f1 = f2 = 1;
            else if (pElement2 == E_LOCKED)
                f2 = 1;
        }
        BOOL bSetValueFirst = (f1 + f2 >= 0);
        index_type dwOuterIndex = dwIndex;
        INDEX_DEC(dwIndex);
        if (bSetValueFirst)  INDEX_VAL(dwIndex) = pElement;
        if (f1 > 0)          ::InterlockedIncrement(&m_dwCount);
        if (f2 != 0)(f2 > 0) ? EmplaceIndex(dwOuterIndex) : EraseIndex(dwOuterIndex);
        if (f1 < 0)          ::InterlockedDecrement(&m_dwCount);
        if (!bSetValueFirst) INDEX_VAL(dwIndex) = pElement;
        ASSERT(Spaces() <= Size());
        return TRUE;
    }

    BOOL RemoveEx(index_type dwIndex, TPTR* ppElement = nullptr)
    {
        return Remove(INDEX_V2R(dwIndex), ppElement);
    }

    BOOL Remove(index_type dwIndex, TPTR* ppElement = nullptr)
    {
        return Set(dwIndex, E_EMPTY, ppElement);
    }

    BOOL AcquireLock(index_type& dwIndex)
    {
        return Put(E_LOCKED, dwIndex);
    }

    BOOL ReleaseLock(index_type dwIndex, TPTR pElement)
    {
        ASSERT(pElement == nullptr || IsValidElement(pElement));
        TPTR pElement2 = nullptr;
        Get(dwIndex, &pElement2);
        ASSERT(pElement2 == E_LOCKED);
        if (pElement2 != E_LOCKED)
            return FALSE;
        return Set(dwIndex, pElement);
    }

public:

    void Reset(DWORD dwSize = 0)
    {
        if (IsValid())
            Destroy();
        if (dwSize > 0)
            Create(dwSize);
    }

    BOOL GetAllElementIndexes(index_type ids[], DWORD& dwCount, BOOL bCopy = TRUE)
    {
        if (ids == nullptr || dwCount == 0)
        {
            dwCount = Elements();
            return FALSE;
        }
        IndexSet* pIndexes = nullptr;
        IndexSet indexes;
        if (bCopy)
            pIndexes = &CopyIndexes(indexes);
        else
            pIndexes = &m_indexes;
        BOOL isOK = FALSE;
        DWORD dwSize = (DWORD)pIndexes->size();
        if (dwSize > 0 && dwSize <= dwCount)
        {
            IndexSetCI it = pIndexes->begin();
            IndexSetCI end = pIndexes->end();
            for (int i = 0; it != end; ++it, ++i)
                ids[i] = *it;
            isOK = TRUE;
        }
        dwCount = dwSize;
        return isOK;
    }

    unique_ptr<index_type[]> GetAllElementIndexes(DWORD& dwCount, BOOL bCopy = TRUE)
    {
        IndexSet* pIndexes = nullptr;
        IndexSet indexes;
        if (bCopy)
            pIndexes = &CopyIndexes(indexes);
        else
            pIndexes = &m_indexes;
        unique_ptr<index_type[]> ids;
        dwCount = (DWORD)pIndexes->size();
        if (dwCount > 0)
        {
            ids.reset(new index_type[dwCount]);
            IndexSetCI it = pIndexes->begin();
            IndexSetCI end = pIndexes->end();
            for (int i = 0; it != end; ++it, ++i)
                ids[i] = *it;
        }
        return ids;
    }

    static BOOL IsValidElement(TPTR pElement)
    {
        return pElement > E_MAX_STATUS;
    }

    DWORD Size()
    {
        return m_dwSize;
    }
    DWORD Elements()
    {
        return (DWORD)m_indexes.size();
    }
    DWORD Spaces()
    {
        return m_dwSize - m_dwCount;
    }
    BOOL HasSpace()
    {
        return m_dwCount < m_dwSize;
    }
    BOOL IsEmpty()
    {
        return m_dwCount == 0;
    }
    BOOL IsValid()
    {
        return m_pv != nullptr;
    }

private:

    void Create(DWORD dwSize)
    {
        ASSERT(!IsValid() && dwSize > 0);
        m_dwCurSeq = 0;
        m_dwCount = 0;
        m_dwSize = dwSize;
        m_pv = (VTPTR*)malloc(m_dwSize * sizeof(TPTR));
        ::ZeroMemory(m_pv, m_dwSize * sizeof(TPTR));
    }

    void Destroy()
    {
        ASSERT(IsValid());
        m_indexes.clear();
        free((void*)m_pv);
        m_pv = nullptr;
        m_dwSize = 0;
        m_dwCount = 0;
        m_dwCurSeq = 0;
    }

    IndexSet& CopyIndexes(IndexSet& indexes)
    {
        {
            CReadLock locallock(m_cs);
            indexes = m_indexes;
        }
        return indexes;
    }

    void EmplaceIndex(index_type dwIndex)
    {
        CWriteLock locallock(m_cs);
        m_indexes.emplace(dwIndex);
    }

    void EraseIndex(index_type dwIndex)
    {
        CWriteLock locallock(m_cs);
        m_indexes.erase(dwIndex);
    }

public:
    CRingCache(DWORD dwSize = 0)
        : m_pv(nullptr)
        , m_dwSize(0)
        , m_dwCount(0)
        , m_dwCurSeq(0)
    {
        Reset(dwSize);
    }

    ~CRingCache()
    {
        Reset(0);
    }

private:
    CRingCache(const CRingCache&);
    CRingCache operator = (const CRingCache&);

private:
    DWORD               m_dwSize;
    VTPTR*              m_pv;
    char                pack1[PACK_SIZE_OF(VTPTR*)];
    volatile DWORD      m_dwCurSeq;
    char                pack2[PACK_SIZE_OF(DWORD)];
    volatile DWORD      m_dwCount;
    char                pack3[PACK_SIZE_OF(DWORD)];

    CSimpleRWLock       m_cs;
    IndexSet            m_indexes;
};

template <class T, class index_type, bool adjust_index> T* const CRingCache<T, index_type, adjust_index>::E_EMPTY = (T*)0x00;
template <class T, class index_type, bool adjust_index> T* const CRingCache<T, index_type, adjust_index>::E_LOCKED = (T*)0x01;
template <class T, class index_type, bool adjust_index> T* const CRingCache<T, index_type, adjust_index>::E_MAX_STATUS = (T*)0x0F;

// ------------------------------------------------------------------------------------------------------------- //

template <class T, class index_type = DWORD, bool adjust_index = false> class CRingCache2
{
public:

    enum EnGetResult { GR_FAIL = -1, GR_INVALID = 0, GR_VALID = 1 };

    typedef T*                                  TPTR;
    typedef volatile T*                         VTPTR;

    typedef unordered_set<index_type>           IndexSet;
    typedef typename IndexSet::const_iterator   IndexSetCI;
    typedef typename IndexSet::iterator         IndexSetI;

    static TPTR const E_EMPTY;
    static TPTR const E_LOCKED;
    static TPTR const E_MAX_STATUS;
    static DWORD const MAX_SIZE;

public:

    static index_type& INDEX_INC(index_type& dwIndex)
    {
        if (adjust_index) ++dwIndex;
        return dwIndex;
    }
    static index_type& INDEX_DEC(index_type& dwIndex)
    {
        if (adjust_index) --dwIndex;
        return dwIndex;
    }

    index_type& INDEX_R2V(index_type& dwIndex)
    {
        dwIndex += *(m_px + dwIndex) * m_dwSize;
        return dwIndex;
    }

    BOOL INDEX_V2R(index_type& dwIndex)
    {
        index_type m = dwIndex % m_dwSize;
        BYTE x = *(m_px + m);
        if (dwIndex / m_dwSize != x)
            return FALSE;
        dwIndex = m;
        return TRUE;
    }


private:

    VTPTR& INDEX_VAL(index_type dwIndex)
    {
        return *(m_pv + dwIndex);
    }

public:

    BOOL Put(TPTR pElement, index_type& dwIndex)
    {
        ASSERT(pElement != nullptr);
        if (!IsValid()) return FALSE;
        BOOL isOK = FALSE;
        while (true)
        {
            if (!HasSpace())
                break;
            DWORD dwCurSeq = m_dwCurSeq;
            index_type dwCurIndex = dwCurSeq % m_dwSize;
            VTPTR& pValue = INDEX_VAL(dwCurIndex);
            if (pValue == E_EMPTY)
            {
                if (::InterlockedCompareExchangePointer((volatile PVOID*)&pValue, pElement, E_EMPTY) == E_EMPTY)
                {
                    ::InterlockedIncrement(&m_dwCount);
                    ::InterlockedCompareExchange(&m_dwCurSeq, dwCurSeq + 1, dwCurSeq);
                    dwIndex = INDEX_INC(INDEX_R2V(dwCurIndex));
                    isOK = TRUE;
                    if (pElement != E_LOCKED)
                        EmplaceIndex(dwIndex);
                    break;
                }
            }
            ::InterlockedCompareExchange(&m_dwCurSeq, dwCurSeq + 1, dwCurSeq);
        }
        return isOK;
    }

    EnGetResult Get(index_type dwIndex, TPTR* ppElement, index_type* pdwRealIndex = nullptr)
    {
        ASSERT(ppElement != nullptr);
        if (!IsValid() || !INDEX_V2R(INDEX_DEC(dwIndex)))
        {
            *ppElement = nullptr;
            return GR_FAIL;
        }
        *ppElement = (TPTR)INDEX_VAL(dwIndex);
        if (pdwRealIndex) *pdwRealIndex = dwIndex;
        return IsValidElement(*ppElement) ? GR_VALID : GR_INVALID;
    }

    BOOL Set(index_type dwIndex, TPTR pElement, TPTR* ppOldElement = nullptr, index_type* pdwRealIndex = nullptr)
    {
        TPTR pElement2 = nullptr;
        if (pdwRealIndex == nullptr)
            pdwRealIndex = (index_type*)_alloca(sizeof(index_type));
        if (Get(dwIndex, &pElement2, pdwRealIndex) == GR_FAIL)
            return FALSE;
        if (ppOldElement != nullptr)
            *ppOldElement = pElement2;
        if (pElement == pElement2)
            return FALSE;
        int f1 = 0;
        int f2 = 0;
        if (pElement == E_EMPTY)
        {
            if (pElement2 == E_LOCKED)
                f1 = -1;
            else
                f1 = f2 = -1;
        }
        else if (pElement == E_LOCKED)
        {
            if (pElement2 == E_EMPTY)
                f1 = 1;
            else
                f2 = -1;
        }
        else
        {
            if (pElement2 == E_EMPTY)
                f1 = f2 = 1;
            else if (pElement2 == E_LOCKED)
                f2 = 1;
        }
        BOOL bSetValueFirst = (f1 + f2 >= 0);
        index_type dwRealIndex = *pdwRealIndex;
        if (bSetValueFirst)  INDEX_VAL(dwRealIndex) = pElement;
        if (f1 > 0)          ::InterlockedIncrement(&m_dwCount);
        if (f2 != 0)(f2 > 0) ? EmplaceIndex(dwIndex) : EraseIndex(dwIndex);
        if (f1 < 0)
        {
            ::InterlockedDecrement(&m_dwCount);
            ++(*(m_px + dwRealIndex));
        }
        if (!bSetValueFirst) INDEX_VAL(dwRealIndex) = pElement;
        ASSERT(Spaces() <= Size());
        return TRUE;
    }

    BOOL Remove(index_type dwIndex, TPTR* ppElement = nullptr)
    {
        return Set(dwIndex, E_EMPTY, ppElement);
    }

    BOOL AcquireLock(index_type& dwIndex)
    {
        return Put(E_LOCKED, dwIndex);
    }

    BOOL ReleaseLock(index_type dwIndex, TPTR pElement)
    {
        ASSERT(pElement == nullptr || IsValidElement(pElement));
        TPTR pElement2 = nullptr;
        Get(dwIndex, &pElement2);
        ASSERT(pElement2 == E_LOCKED);
        if (pElement2 != E_LOCKED)
            return FALSE;
        return Set(dwIndex, pElement);
    }

public:

    void Reset(DWORD dwSize = 0)
    {
        if (IsValid())
            Destroy();
        if (dwSize > 0)
            Create(dwSize);
    }

    BOOL GetAllElementIndexes(index_type ids[], DWORD& dwCount, BOOL bCopy = TRUE)
    {
        if (ids == nullptr || dwCount == 0)
        {
            dwCount = Elements();
            return FALSE;
        }
        IndexSet* pIndexes = nullptr;
        IndexSet indexes;
        if (bCopy)
            pIndexes = &CopyIndexes(indexes);
        else
            pIndexes = &m_indexes;
        BOOL isOK = FALSE;
        DWORD dwSize = (DWORD)pIndexes->size();
        if (dwSize > 0 && dwSize <= dwCount)
        {
            IndexSetCI it = pIndexes->begin();
            IndexSetCI end = pIndexes->end();
            for (int i = 0; it != end; ++it, ++i)
                ids[i] = *it;
            isOK = TRUE;
        }
        dwCount = dwSize;
        return isOK;
    }

    unique_ptr<index_type[]> GetAllElementIndexes(DWORD& dwCount, BOOL bCopy = TRUE)
    {
        IndexSet* pIndexes = nullptr;
        IndexSet indexes;
        if (bCopy)
            pIndexes = &CopyIndexes(indexes);
        else
            pIndexes = &m_indexes;
        unique_ptr<index_type[]> ids;
        dwCount = (DWORD)pIndexes->size();
        if (dwCount > 0)
        {
            ids.reset(new index_type[dwCount]);
            IndexSetCI it = pIndexes->begin();
            IndexSetCI end = pIndexes->end();
            for (int i = 0; it != end; ++it, ++i)
                ids[i] = *it;
        }
        return ids;
    }

    static BOOL IsValidElement(TPTR pElement)
    {
        return pElement > E_MAX_STATUS;
    }

    DWORD Size()
    {
        return m_dwSize;
    }
    DWORD Elements()
    {
        return (DWORD)m_indexes.size();
    }
    DWORD Spaces()
    {
        return m_dwSize - m_dwCount;
    }
    BOOL HasSpace()
    {
        return m_dwCount < m_dwSize;
    }
    BOOL IsEmpty()
    {
        return m_dwCount == 0;
    }
    BOOL IsValid()
    {
        return m_pv != nullptr;
    }

private:

    void Create(DWORD dwSize)
    {
        ASSERT(!IsValid() && dwSize > 0 && dwSize <= MAX_SIZE);
        m_dwCurSeq = 0;
        m_dwCount = 0;
        m_dwSize = dwSize;
        m_pv = (VTPTR*)malloc(m_dwSize * sizeof(TPTR));
        m_px = (BYTE*)malloc(m_dwSize * sizeof(BYTE));
        ::ZeroMemory(m_pv, m_dwSize * sizeof(TPTR));
        ::ZeroMemory(m_px, m_dwSize * sizeof(BYTE));
    }

    void Destroy()
    {
        ASSERT(IsValid());
        m_indexes.clear();
        free((void*)m_pv);
        free((void*)m_px);
        m_pv = nullptr;
        m_px = nullptr;
        m_dwSize = 0;
        m_dwCount = 0;
        m_dwCurSeq = 0;
    }

    IndexSet& CopyIndexes(IndexSet& indexes)
    {
        {
            CReadLock locallock(m_cs);
            indexes = m_indexes;
        }
        return indexes;
    }

    void EmplaceIndex(index_type dwIndex)
    {
        CWriteLock locallock(m_cs);
        m_indexes.emplace(dwIndex);
    }

    void EraseIndex(index_type dwIndex)
    {
        CWriteLock locallock(m_cs);
        m_indexes.erase(dwIndex);
    }

public:
    CRingCache2(DWORD dwSize = 0)
        : m_pv(nullptr)
        , m_px(nullptr)
        , m_dwSize(0)
        , m_dwCount(0)
        , m_dwCurSeq(0)
    {
        Reset(dwSize);
    }

    ~CRingCache2()
    {
        Reset(0);
    }

private:
    CRingCache2(const CRingCache2&);
    CRingCache2 operator = (const CRingCache2&);

private:
    DWORD               m_dwSize;
    VTPTR*              m_pv;
    char                pack1[PACK_SIZE_OF(VTPTR*)];
    BYTE*               m_px;
    char                pack2[PACK_SIZE_OF(BYTE*)];
    volatile DWORD      m_dwCurSeq;
    char                pack3[PACK_SIZE_OF(DWORD)];
    volatile DWORD      m_dwCount;
    char                pack4[PACK_SIZE_OF(DWORD)];

    CSimpleRWLock       m_cs;
    IndexSet            m_indexes;
};

template <class T, class index_type, bool adjust_index> T* const CRingCache2<T, index_type, adjust_index>::E_EMPTY = (T*)0x00;
template <class T, class index_type, bool adjust_index> T* const CRingCache2<T, index_type, adjust_index>::E_LOCKED = (T*)0x01;
template <class T, class index_type, bool adjust_index> T* const CRingCache2<T, index_type, adjust_index>::E_MAX_STATUS = (T*)0x0F;

template <class T, class index_type, bool adjust_index> DWORD const CRingCache2<T, index_type, adjust_index>::MAX_SIZE =
#if !defined(_WIN64)
0x00FFFFFF
#else
0xFFFFFFFF
#endif
;
// ------------------------------------------------------------------------------------------------------------- //

template <class T> class CRingPool
{
private:

    typedef T*          TPTR;
    typedef volatile T* VTPTR;

    static TPTR const E_EMPTY;
    static TPTR const E_LOCKED;
    static TPTR const E_RELEASED;
    static TPTR const E_OCCUPIED;
    static TPTR const E_MAX_STATUS;

private:

    VTPTR& INDEX_VAL(DWORD dwIndex)
    {
        return *(m_pv + dwIndex);
    }

public:

    BOOL TryPut(TPTR pElement)
    {
        ASSERT(pElement != nullptr);
        if (!IsValid()) return FALSE;
        BOOL isOK = FALSE;
        while (true)
        {
            BOOL bOccupy = FALSE;
            DWORD seqPut = m_seqPut;
            if (!HasPutSpace(seqPut))
                break;
            DWORD dwIndex = seqPut % m_dwSize;
            VTPTR& pValue = INDEX_VAL(dwIndex);
            if (pValue == E_RELEASED)
            {
                if (::InterlockedCompareExchangePointer((volatile PVOID*)&pValue, E_OCCUPIED, E_RELEASED) == E_RELEASED)
                    bOccupy = TRUE;
                else
                    continue;
            }
            if (pValue == E_EMPTY || bOccupy)
            {
                if (::InterlockedCompareExchange(&m_seqPut, seqPut + 1, seqPut) == seqPut)
                {
                    pValue = pElement;
                    isOK = TRUE;
                    break;
                }
            }
            else if (pValue == E_LOCKED)
                break;
        }
        return isOK;
    }

    BOOL TryGet(TPTR* ppElement)
    {
        ASSERT(ppElement != nullptr);
        if (!IsValid()) return FALSE;
        BOOL isOK = FALSE;
        while (true)
        {
            DWORD seqGet = m_seqGet;
            if (!HasGetSpace(seqGet))
                break;
            DWORD dwIndex = seqGet % m_dwSize;
            VTPTR& pValue = INDEX_VAL(dwIndex);
            if (pValue == E_LOCKED)
                break;
            else if (pValue != E_EMPTY && pValue != E_RELEASED && pValue != E_OCCUPIED)
            {
                if (::InterlockedCompareExchange(&m_seqGet, seqGet + 1, seqGet) == seqGet)
                {
                    ASSERT(pValue > E_MAX_STATUS);
                    *(ppElement) = (TPTR)pValue;
                    pValue = E_EMPTY;
                    isOK = TRUE;
                    break;
                }
            }
        }
        return isOK;
    }

    BOOL TryLock(TPTR* ppElement, DWORD& dwIndex)
    {
        ASSERT(ppElement != nullptr);
        if (!IsValid()) return FALSE;
        BOOL isOK = FALSE;
        while (true)
        {
            DWORD seqGet = m_seqGet;
            if (!HasGetSpace(seqGet))
                break;
            dwIndex = seqGet % m_dwSize;
            VTPTR& pValue = INDEX_VAL(dwIndex);
            if (pValue == E_LOCKED)
                break;
            else if (pValue != E_EMPTY && pValue != E_RELEASED && pValue != E_OCCUPIED)
            {
                if (::InterlockedCompareExchange(&m_seqGet, seqGet + 1, seqGet) == seqGet)
                {
                    ASSERT(pValue > E_MAX_STATUS);
                    *(ppElement) = (TPTR)pValue;
                    pValue = E_LOCKED;
                    isOK = TRUE;
                    break;
                }
            }
        }
        return isOK;
    }

    BOOL ReleaseLock(TPTR pElement, DWORD dwIndex)
    {
        ASSERT(dwIndex < m_dwSize);
        ASSERT(pElement == nullptr || pElement > E_MAX_STATUS);
        if (!IsValid()) return FALSE;
        VTPTR& pValue = INDEX_VAL(dwIndex);
        VERIFY(pValue == E_LOCKED);
        if (pElement != nullptr)
        {
            for (DWORD i = 0;; i++)
            {
                if (TryPut(pElement))
                    break;
                DWORD dwPutIndex = m_seqPut % m_dwSize;
                if (dwIndex == dwPutIndex)
                {
                    pValue = pElement;
                    ::InterlockedIncrement(&m_seqPut);
                    return TRUE;
                }
                ::YieldThread(i);
            }
        }
        pValue = E_RELEASED;
        return TRUE;
    }

public:

    void Reset(DWORD dwSize = 0)
    {
        if (IsValid())
            Destroy();
        if (dwSize > 0)
            Create(dwSize);
    }

    DWORD Size()
    {
        return m_dwSize;
    }
    DWORD Elements()
    {
        return m_seqPut - m_seqGet;
    }
    BOOL IsFull()
    {
        return Elements() == Size();
    }
    BOOL IsEmpty()
    {
        return Elements() == 0;
    }
    BOOL IsValid()
    {
        return m_pv != nullptr;
    }

private:

    BOOL HasPutSpace(DWORD seqPut)
    {
        return ((int)(seqPut - m_seqGet) < (int)m_dwSize);
    }

    BOOL HasGetSpace(DWORD seqGet)
    {
        return ((int)(m_seqPut - seqGet) > 0);
    }

    void Create(DWORD dwSize)
    {
        ASSERT(!IsValid() && dwSize > 0);
        m_seqPut = 0;
        m_seqGet = 0;
        m_dwSize = dwSize;
        m_pv = (VTPTR*)malloc(m_dwSize * sizeof(TPTR));
        ::ZeroMemory(m_pv, m_dwSize * sizeof(TPTR));
    }

    void Destroy()
    {
        ASSERT(IsValid());
        free((void*)m_pv);
        m_pv = nullptr;
        m_dwSize = 0;
        m_seqPut = 0;
        m_seqGet = 0;
    }

public:
    CRingPool(DWORD dwSize = 0)
        : m_pv(nullptr)
        , m_dwSize(0)
        , m_seqPut(0)
        , m_seqGet(0)
    {
        Reset(dwSize);
    }

    ~CRingPool()
    {
        Reset(0);
    }

private:
    CRingPool(const CRingPool&);
    CRingPool operator = (const CRingPool&);

private:
    DWORD               m_dwSize;
    VTPTR*              m_pv;
    char                pack1[PACK_SIZE_OF(VTPTR*)];
    volatile DWORD      m_seqPut;
    char                pack2[PACK_SIZE_OF(DWORD)];
    volatile DWORD      m_seqGet;
    char                pack3[PACK_SIZE_OF(DWORD)];
};

template <class T> T* const CRingPool<T>::E_EMPTY = (T*)0x00;
template <class T> T* const CRingPool<T>::E_LOCKED = (T*)0x01;
template <class T> T* const CRingPool<T>::E_RELEASED = (T*)0x02;
template <class T> T* const CRingPool<T>::E_OCCUPIED = (T*)0x03;
template <class T> T* const CRingPool<T>::E_MAX_STATUS = (T*)0x0F;

// ------------------------------------------------------------------------------------------------------------- //

template <class T> class CCASQueue
{
private:
    struct Node;
    typedef Node*           NPTR;
    typedef volatile Node*  VNPTR;
    typedef volatile ULONG  VLONG;

    struct Node
    {
        T*      pValue;
        VNPTR   pNext;

        Node(T* val, NPTR next = nullptr)
            : pValue(val), pNext(next)
        {
        }
    };

public:

    void PushBack(T* pVal)
    {
        ASSERT(pVal != nullptr);
        VNPTR pTail = nullptr;
        NPTR pNode = new Node(pVal);
        while (true)
        {
            pTail = m_pTail;
            if (::InterlockedCompareExchangePointer((volatile PVOID*)&m_pTail, (PVOID)pNode, (PVOID)pTail) == pTail)
            {
                pTail->pNext = pNode;
                break;
            }
        }
        ::InterlockedIncrement(&m_lSize);
    }

    void UnsafePushBack(T* pVal)
    {
        ASSERT(pVal != nullptr);
        NPTR pNode = new Node(pVal);
        m_pTail->pNext = pNode;
        m_pTail = pNode;
        ::InterlockedIncrement(&m_lSize);
    }

    BOOL PopFront(T** ppVal)
    {
        ASSERT(ppVal != nullptr);
        if (IsEmpty())
            return FALSE;
        BOOL isOK = FALSE;
        NPTR pHead = nullptr;
        NPTR pNext = nullptr;
        while (true)
        {
            while (::InterlockedCompareExchange(&m_lLock, 1, 0) != 0)
                ::YieldProcessor();
            pHead = (NPTR)m_pHead;
            pNext = (NPTR)pHead->pNext;
            if (pNext == nullptr)
            {
                m_lLock = 0;
                break;
            }
            *ppVal = pNext->pValue;
            m_pHead = pNext;
            m_lLock = 0;
            isOK = TRUE;
            ::InterlockedDecrement(&m_lSize);
            delete pHead;
            break;
        }
        return isOK;
    }

    BOOL UnsafePopFront(T** ppVal)
    {
        if (!UnsafePeekFront(ppVal))
            return FALSE;
        NPTR pHead = (NPTR)m_pHead;
        NPTR pNext = (NPTR)pHead->pNext;
        m_pHead = pNext;
        ::InterlockedDecrement(&m_lSize);
        delete pHead;
        return TRUE;
    }

    BOOL UnsafePeekFront(T** ppVal)
    {
        ASSERT(ppVal != nullptr);
        NPTR pNext = (NPTR)m_pHead->pNext;
        if (pNext == nullptr)
            return FALSE;
        *ppVal = pNext->pValue;
        return TRUE;
    }

public:

    ULONG Size()
    {
        return m_lSize;
    }
    BOOL IsEmpty()
    {
        return m_lSize == 0;
    }

public:

    CCASQueue() : m_lLock(0), m_lSize(0)
    {
        NPTR pHead = new Node(nullptr);
        m_pHead = m_pTail = pHead;
    }

    ~CCASQueue()
    {
        ASSERT(m_lLock == 0);
        ASSERT(m_lSize == 0);
        ASSERT(m_pHead != nullptr);
        ASSERT(m_pHead->pNext == nullptr);
        while (m_pHead != nullptr)
        {
            VNPTR pNode = m_pHead->pNext;
            delete m_pHead;
            m_pHead = pNode;
        }
    }

private:
    VLONG   m_lLock;
    VLONG   m_lSize;
    VNPTR   m_pHead;
    VNPTR   m_pTail;
};

#if !defined (_WIN64)
#pragma pack(pop)
#endif


/* timeGetTime() 包装方法 */
DWORD TimeGetTime();

/**********************************
描述: 获取当前时间与原始时间的时间差
参数:
dwOriginal	: 原始时间（毫秒），通常用 timeGetTime() 或 GetTickCount() 获取

返回值:	与当前 timeGetTime() 之间的时间差
**********************************/
DWORD GetTimeGap32(DWORD dwOriginal);

#if _WIN32_WINNT >= _WIN32_WINNT_WS08
/**********************************
描述: 获取当前时间与原始时间的时间差
参数:
ullOriginal	: 原始时间（毫秒），通常用 GetTickCount64() 获取

返回值:	与当前 GetTickCount64() 之间的时间差
**********************************/
ULONGLONG GetTimeGap64(ULONGLONG ullOriginal);
#endif

/**********************************
描述: 处理Windows消息
参数:
bDispatchQuitMsg	: 是否转发 WM_QUIT 消息
TRUE : 转发（默认）
FALSE: 不转发，并返回 FALSE

返回值:		TRUE  : 收完消息
FALSE : bDispatchQuitMsg 参数为 FALSE 并收到 WM_QUIT 消息
**********************************/
BOOL PeekMessageLoop(BOOL bDispatchQuitMsg = TRUE);

/**********************************
描述: 等待指定时间, 同时处理Windows消息
参数: (参考: MsgWaitForMultipleObjectsEx() )
dwHandles		: 数组元素个数
szHandles		: 对象句柄数组
dwMilliseconds	: 等待时间 (毫秒)
dwWakeMask		: 消息过滤标识
dwFlags			: 等待类型

返回值: (0 ~ dwHandles - 1): 等待成功
WAIT_TIMEOUT		: 超时
WAIT_FAILED			: 执行失败
**********************************/
DWORD WaitForMultipleObjectsWithMessageLoop(DWORD dwHandles, HANDLE szHandles[], DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
描述: 等待指定时间, 同时处理Windows消息
参数: (参考: MsgWaitForMultipleObjectsEx() )
hHandle			: 对象句柄
dwMilliseconds	: 等待时间 (毫秒)
dwWakeMask		: 消息过滤标识
dwFlags			: 等待类型

返回值: TRUE: 等待成功，FALSE: 超时
**********************************/
BOOL MsgWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
描述: 等待指定时间, 同时处理Windows消息
参数: (参考: MsgWaitForMultipleObjectsEx() )
dwMilliseconds	: 等待时间 (毫秒)
dwWakeMask		: 消息过滤标识
dwFlags			: 等待类型

返回值: MsgWaitForMultipleObjectsEx() 函数的操作结果
**********************************/
void WaitWithMessageLoop(DWORD dwMilliseconds, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
描述: 等待用Sleep()函数等待某个变量小于指定值
参数:
plWorkingItemCount		: 监视变量
lMaxWorkingItemCount	: 指定值
dwCheckInterval			: 检查间隔 (毫秒)

返回值:
**********************************/
void WaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval);
/**********************************
描述: 等待用Sleep()函数等待某个变量减小到 0
参数:
plWorkingItemCount		: 监视变量
dwCheckInterval			: 检查间隔 (毫秒)

返回值:
**********************************/
void WaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval);

/**********************************
描述: 等待用WaitWithMessageLoop()函数等待某个变量小于指定值
参数:
plWorkingItemCount		: 监视变量
lMaxWorkingItemCount	: 指定值
dwCheckInterval			: 检查间隔 (毫秒)

返回值:
**********************************/
void MsgWaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval = 10);
/**********************************
描述: 等待用WaitWithMessageLoop()函数等待某个变量减小到 0
参数:
plWorkingItemCount		: 监视变量
dwCheckInterval			: 检查间隔 (毫秒)

返回值:
**********************************/
void MsgWaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval = 10);


#include <tlhelp32.h>
#include <malloc.h>

enum EnCodePage
{
    XCP_ACP = CP_ACP,
    XCP_GB2312 = 936,
#ifdef _WIN32_WCE
    XCP_BIG5 = CP_ACP,
#else
    XCP_BIG5 = 950,
#endif
    XCP_GBK = XCP_GB2312,
    XCP_UTF7 = CP_UTF7,
    XCP_UTF8 = CP_UTF8,
};

/************************************* 字符操作 *************************************/

// 把字符转换成其表示的值: 如字符 'F' 转换成 数值 15
#define CHARTOVALUE(c)              (c <= '9' ? c - '0' : (c <= 'F' ? c - 'A' + 0x0A : c - 'a' + 0X0A))
// 把双字符转换成其表示的值: 如字符 '1F' 转换成 数值 31
#define DOUBLECHARTOVALUE(pc)       (((CHARTOVALUE(*(pc))) << 4) | (CHARTOVALUE(*(pc + 1))))

// 把数值转换成其十六进制字符: 如数值 15 转换成 字符 'F'
#define VALUETOCHAR(n)              (n <= 9 ? n + '0' : (n <= 'F' ? n + 'A' - 0X0A : n + 'a' - 0X0A))
// 把数值字符转换成其两个十六进制字符: 如数值 11 转换成 字符 '0B'
#define VALUETODOUBLECHAR(pc, n)    {*(pc) = VALUETOCHAR((n >> 4)); *((pc) + 1) = VALUETOCHAR((n & 0X0F));}

// 把双字符转换成其表示的值: 如字符 '1F' 转换成 数值 31
BYTE DoubleCharToByte(LPCTSTR psValue);
// 把数值字符转换成其两个十六进制字符: 如数值 11 转换成 字符 '0B'
LPTSTR ByteToDoubleChar(BYTE b, LPTSTR des);
// 把十六进制转换成其表示的值: 如字符 '601F' 转换成 数值 24607
UINT HexStrToInt(LPCTSTR pHexText, int len = -1);
// 把十进制转换成其表示的值: 如字符 '0000024607' 转换成 数值 24607
UINT DecStrToInt(LPCTSTR pDecText, int len = -1);
// 把数值换成其十六进制字符串表示的值: 如数值 24607 转换成 字符 '601F'
CString& IntToHexStr(CString& dest, UINT v, int len = -1);
// 把数值换成其十进制字符串表示的值: 如数值 24607 转换成 字符 '24607'
CString& IntToDecStr(CString& dest, UINT v, int len = -1);
// 把十六进制表示的地址改为十进制表示的地址: 如 "000C35CE" 转换成 '0000800236'
CString& HexAddrToDecAddr(CString& dest, LPCTSTR src, int destlen = 10, int srclen = -1);
// 把十进制表示的地址改为十六进制表示的地址: 如 "0000800236" 转换成 '000C35CE'
CString& DecAddrToHexAddr(CString& dest, LPCTSTR src, int destlen = 8, int srclen = -1);

// Code Page Name -> Code Page Value
EnCodePage GetCodePageByName(LPCTSTR lpszCodePageName);

// MBCS -> UNICODE
BOOL MbcsToUnicode(const char* pszInString, WCHAR** ptrOutWStr, int& nSizeCount);
// UNICODE -> MBCS
BOOL UnicodeToMbcs(const WCHAR* pwzInString, char** ptrOutStr, int& nSizeCount);
// UTF8 -> UNICODE
BOOL Utf8ToUnicode(const char* pszInString, WCHAR** ptrOutWStr, int& nSizeCount);
// UNICODE -> UTF8
BOOL UnicodeToUtf8(const WCHAR* pwzInString, char** ptrOutStr, int& nSizeCount);
// CP_XXX -> UNICODE
BOOL CPToUni(const char* pszInString, WCHAR** ptrOutWStr, unsigned int nCodePage, int& nSizeCount);
// UNICODE -> CP_XXX
BOOL UniToCP(const WCHAR* pwzInString, char** ptrOutStr, unsigned int nCodePage, int& nSizeCount);

// 把字节数组转换成16进制字符串
int BytesToHex(const BYTE* pBytes, int nLength, LPTSTR* lpszDest);
// 把16进制字符串转换成字节数组
int HexToBytes(LPCTSTR lpszHex, BYTE** ppBytes, int* pnLength);

// 把普通字符串转换成16进制字符串
CString& StrToHex(const TCHAR* src, CString& des);
// 把16进制字符串转换成普通字符串
CString& HexToStr(const TCHAR* src, CString& des);
// 把普通字符串转换成UTF8字符串, 然后再把该UTF8字符串转换成16进制字符串
CString& StrToUtf8Hex(const TCHAR* src, CString& strDec);
// 把16进制字符串转换成UTF8字符串, 然后再把该UTF8字符串转换成普通字符串
CString& HexUtf8ToStr(const TCHAR* src, CString& strDec);

// 取得系统错误代码的描述信息
CString GetSystemErrorDesc(DWORD dwCode);

// 分隔字符串
BOOL SplitStr(LPCTSTR pszSrc, vector<CString>& vtItem, LPCTSTR pszSepectors = nullptr, LPCTSTR pszQMarks = nullptr);

// 提取文件名称
CString ExtractFileName(LPCTSTR lpszFullFileName);
// 提取文件路径
CString ExtractPath(LPCTSTR lpszFullFileName);
// 提取文件路径
CString ExtractPath(LPCTSTR lpszFullFileName);
// 提取当前模块路径
CString ExtractModulePath(HMODULE hModule = nullptr);

// 启动进程
BOOL RunProcess(LPCTSTR sFileName, LPCTSTR cmdline = nullptr, BOOL bHide = TRUE, LPCTSTR dir = nullptr, BOOL bWait = TRUE, DWORD dwWaitTime = INFINITE);
BOOL ShellRunExe(LPCTSTR lpszPath, LPCTSTR lpszParams = nullptr, int iShow = SW_SHOWNORMAL, HANDLE* phProcess = nullptr, BOOL bWait = FALSE, DWORD dwWaitTime = INFINITE);

// 查找进程
BOOL FindProcess(LPCTSTR pszProcessName);

// 查找进程句柄
HANDLE FindProcessHandle(LPCTSTR pszProcessName, DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION, BOOL bInheritHandle = FALSE);

typedef map<DWORD, PROCESSENTRY32*>                                 ProcessInfos;
typedef MapWrapper<ProcessInfos, PtrMap_Cleaner<ProcessInfos>>      ProcessInfoMap;

// 查找当前运行的信息
BOOL FindRunningProcessesInfo(ProcessInfoMap& infoMap);

// 查找进程的主窗口
HWND FindProcessMainWindow(DWORD dwProcID, LPCTSTR lpszClassName = nullptr);
// 查找进程的主窗口
HWND FindProcessMainWindow(HANDLE hProcess, LPCTSTR lpszClassName = nullptr);

BOOL TerminateProcessFairily(HANDLE hProcess, DWORD dwWait = INFINITE);

// 简单记录日志
void WriteLog(LPCTSTR pszLogFileName, LPCTSTR pszLog);

#ifndef _WIN32_WCE
// 设置当前路径
BOOL SetCurrentPathToModulePath(HMODULE hModule = nullptr);
// 查找进程
BOOL FindProcessEx(LPCTSTR pszProcessName);
// 读取 INI 配置
CString GetIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpFileName, DWORD dwSize);
#endif

#ifdef _AFX
// 秒转换成时间字符串
CString SecondToTimeStr(DWORD dwSeconds, BOOL bDayOnly = FALSE);
#endif

// 设置注册表项
BOOL GetRegistryValue(HKEY hRoot, LPCTSTR wcSubKey, LPCTSTR wcName, LPBYTE pValue, DWORD* pdwSize, DWORD* pdwType);
// 获取注册表项
BOOL SetRegistryValue(HKEY hRoot, LPCTSTR wcSubKey, LPCTSTR wcName, LPBYTE pValue, DWORD dwSize, DWORD dwType);


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



struct TItem
{
    template<typename T> friend struct  TSimpleList;
    template<typename T> friend class   CNodePoolT;

    friend struct   TItemList;
    friend struct   TBuffer;

public:
    inline int Cat(const BYTE* pData, int length);
    inline int Cat(const TItem& other);
    inline int Fetch(BYTE* pData, int length);
    inline int Peek(BYTE* pData, int length);
    inline int Reduce(int length);
    inline void Reset(int first = 0, int last = 0);

    BYTE*       Ptr()
    {
        return begin;
    }
    const BYTE* Ptr()  const
    {
        return begin;
    }
    int         Size()  const
    {
        return (int)(end - begin);
    }
    int         Remain()  const
    {
        return capacity - (int)(end - head);
    }
    int         Capacity()  const
    {
        return capacity;
    }
    bool        IsEmpty()  const
    {
        return Size() == 0;
    }
    bool        IsFull()  const
    {
        return Remain() == 0;
    }

public:
    operator BYTE*   ()
    {
        return Ptr();
    }
    operator const  BYTE*   () const
    {
        return Ptr();
    }

public:
    static TItem* Construct(CPrivateHeap& heap,
        int     capacity = DEFAULT_ITEM_CAPACITY,
        BYTE*   pData = nullptr,
        int     length = 0);

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

    T*      Front()  const
    {
        return pFront;
    }
    T*      Back()  const
    {
        return pBack;
    }
    int     Size()  const
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

template<class T> class CNodePoolT
{
public:
    void PutFreeItem(T* pItem)
    {
        ASSERT(pItem != nullptr);
        if (!m_lsFreeItem.TryPut(pItem))
            T::Destruct(pItem);
    }

    void PutFreeItem(TSimpleList<T>& lsItem)
    {
        if (lsItem.IsEmpty())
            return;
        T* pItem;
        while ((pItem = lsItem.PopFront()) != nullptr)
            PutFreeItem(pItem);
    }

    T* PickFreeItem()
    {
        T* pItem = nullptr;
        if (m_lsFreeItem.TryGet(&pItem))
            pItem->Reset();
        else
            pItem = T::Construct(m_heap, m_dwItemCapacity);
        return pItem;
    }

    inline void Prepare()
    {
        m_lsFreeItem.Reset(m_dwPoolHold);
    }

    inline void Clear()
    {
        T* pItem = nullptr;
        while (m_lsFreeItem.TryGet(&pItem))
            T::Destruct(pItem);
        VERIFY(m_lsFreeItem.IsEmpty());
        m_lsFreeItem.Reset();
        m_heap.Reset();
    }

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
    CNodePoolT(DWORD dwPoolSize = DEFAULT_POOL_SIZE,
        DWORD dwPoolHold = DEFAULT_POOL_HOLD,
        DWORD dwItemCapacity = DEFAULT_ITEM_CAPACITY)
        : m_dwPoolSize(dwPoolSize)
        , m_dwPoolHold(dwPoolHold)
        , m_dwItemCapacity(dwItemCapacity)
    {
    }

    ~CNodePoolT()
    {
        Clear();
    }

    DECLARE_NO_COPY_CLASS(CNodePoolT)

public:
    static const DWORD DEFAULT_ITEM_CAPACITY;
    static const DWORD DEFAULT_POOL_SIZE;
    static const DWORD DEFAULT_POOL_HOLD;

private:
    CPrivateHeap    m_heap;

    DWORD           m_dwItemCapacity;
    DWORD           m_dwPoolSize;
    DWORD           m_dwPoolHold;

    CRingPool<T>    m_lsFreeItem;
};

template<class T> const DWORD CNodePoolT<T>::DEFAULT_ITEM_CAPACITY = TItem::DEFAULT_ITEM_CAPACITY;
template<class T> const DWORD CNodePoolT<T>::DEFAULT_POOL_SIZE = 300;
template<class T> const DWORD CNodePoolT<T>::DEFAULT_POOL_HOLD = 1200;

typedef CNodePoolT<TItem>   CItemPool;

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
    CCriSec&    CriSec()
    {
        return cs;
    }
    TItemList&  ItemList()
    {
        return items;
    }

    ULONG_PTR ID()  const
    {
        return id;
    }
    int Length()  const
    {
        return length;
    }
    bool IsValid()  const
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
    CPrivateHeap&   heap;

private:
    ULONG_PTR       id;
    int             length;
    DWORD           freeTime;

private:
    TBuffer*        next;
    TBuffer*        last;

    CCriSec         cs;
    TItemList       items;
};

class CBufferPool
{
    typedef CRingPool<TBuffer>                      TBufferList;
    typedef CCASQueue<TBuffer>                      TBufferQueue;

    typedef CRingCache<TBuffer, ULONG_PTR, true>    TBufferCache;

public:
    void        PutFreeBuffer(ULONG_PTR dwID);
    TBuffer*    PutCacheBuffer(ULONG_PTR dwID);
    TBuffer*    FindCacheBuffer(ULONG_PTR dwID);
    TBuffer*    PickFreeBuffer(ULONG_PTR dwID);
    void        PutFreeBuffer(TBuffer* pBuffer);

    void        Prepare();
    void        Clear();

private:
    void ReleaseGCBuffer(BOOL bForce = FALSE);

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

    void SetMaxCacheSize(DWORD dwMaxCacheSize)
    {
        m_dwMaxCacheSize = dwMaxCacheSize;
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

    DWORD GetMaxCacheSize()
    {
        return m_dwMaxCacheSize;
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
        DWORD dwMaxCacheSize = DEFAULT_MAX_CACHE_SIZE)
        : m_dwBufferPoolSize(dwPoolSize)
        , m_dwBufferPoolHold(dwPoolHold)
        , m_dwBufferLockTime(dwLockTime)
        , m_dwMaxCacheSize(dwMaxCacheSize)
    {
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
    static const DWORD DEFAULT_MAX_CACHE_SIZE;
    static const DWORD DEFAULT_ITEM_CAPACITY;
    static const DWORD DEFAULT_ITEM_POOL_SIZE;
    static const DWORD DEFAULT_ITEM_POOL_HOLD;
    static const DWORD DEFAULT_BUFFER_LOCK_TIME;
    static const DWORD DEFAULT_BUFFER_POOL_SIZE;
    static const DWORD DEFAULT_BUFFER_POOL_HOLD;

private:
    DWORD           m_dwMaxCacheSize;
    DWORD           m_dwBufferLockTime;
    DWORD           m_dwBufferPoolSize;
    DWORD           m_dwBufferPoolHold;

    CPrivateHeap    m_heap;
    CItemPool       m_itPool;

    TBufferCache    m_bfCache;

    TBufferList     m_lsFreeBuffer;
    TBufferQueue    m_lsGCBuffer;
};


#include <process.h>

template<class T, class P = VOID> class CThread
{
private:
    typedef UINT(T::*F)(P*);

public:
    BOOL Start(T* pRunner, F pFunc, P* pArg = nullptr, int iPriority = THREAD_PRIORITY_NORMAL, UINT uiStackSize = 0, LPSECURITY_ATTRIBUTES lpThreadAttributes = nullptr)
    {
        BOOL isOK = TRUE;
        if (!IsRunning())
        {
            Release();
            m_pRunner = pRunner;
            m_pFunc = pFunc;
            m_pArg = pArg;
            if (iPriority == THREAD_PRIORITY_NORMAL)
                m_hThread = (HANDLE)_beginthreadex(lpThreadAttributes, uiStackSize, ThreadProc, (LPVOID)this, 0, &m_uiThreadID);
            else
            {
                m_hThread = (HANDLE)_beginthreadex(lpThreadAttributes, uiStackSize, ThreadProc, (LPVOID)this, CREATE_SUSPENDED, &m_uiThreadID);
                if (IsValid())
                {
                    ::SetThreadPriority(m_hThread, iPriority);
                    ::ResumeThread(m_hThread);
                }
            }
            if (!IsValid())
            {
                ::SetLastError(ERROR_CREATE_FAILED);
                isOK = FALSE;
            }
        }
        else
        {
            ::SetLastError(ERROR_INVALID_STATE);
            isOK = FALSE;
        }
        return isOK;
    }

    BOOL Join(BOOL bWithMessageLoop = FALSE, DWORD dwMilliseconds = INFINITE)
    {
        BOOL isOK = bWithMessageLoop ?
            ::MsgWaitForSingleObject(m_hThread, dwMilliseconds) :
            ::WaitForSingleObject(m_hThread, dwMilliseconds) == WAIT_OBJECT_0;
        Release();
        return isOK;
    }

    BOOL IsRunning()
    {
        if (!IsValid())
            return FALSE;
        DWORD dwExitCode;
        if (GetExitCode(&dwExitCode))
            return dwExitCode == STILL_ACTIVE;
        return FALSE;
    }

    VOID Release()
    {
        if (IsValid())
        {
            ::CloseHandle(m_hThread);
            Reset();
        }
    }

    HANDLE Detatch()
    {
        HANDLE h = m_hThread;
        Reset();
        return h;
    }

    BOOL Terminate(DWORD dwExitCode)
    {
        return ::TerminateThread(m_hThread, dwExitCode);
    }
    BOOL GetExitCode(LPDWORD lpExitCode)
    {
        return ::GetExitCodeThread(m_hThread, lpExitCode);
    }
    DWORD Suspend()
    {
        return ::SuspendThread(m_hThread);
    }
    DWORD Resume()
    {
        return ::ResumeThread(m_hThread);
    }

    BOOL IsValid()
    {
        return m_hThread != nullptr;
    }
    T* GetRunner()
    {
        return m_pRunner;
    }
    F GetFunc()
    {
        return m_pFunc;
    }
    P* GetArg()
    {
        return m_pArg;
    }
    DWORD GetThreadID()
    {
        return m_uiThreadID;
    }

    HANDLE& GetThreadHandle()
    {
        return m_hThread;
    }
    const HANDLE& GetThreadHandle() const
    {
        return m_hThread;
    }

public:
    CThread()
    {
        Reset();
    }

    virtual ~CThread()
    {
        Release();
    }

private:
    static UINT WINAPI ThreadProc(LPVOID pv)
    {
        CThread* pThis = (CThread*)pv;
        return ((pThis->m_pRunner)->*(pThis->m_pFunc))(pThis->m_pArg);
    }

    void Reset()
    {
        m_uiThreadID = 0;
        m_hThread = nullptr;
        m_pRunner = nullptr;
        m_pFunc = nullptr;
        m_pArg = nullptr;
    }

private:
    UINT    m_uiThreadID;
    HANDLE  m_hThread;
    T*      m_pRunner;
    F       m_pFunc;
    P*      m_pArg;

    DECLARE_NO_COPY_CLASS(CThread)
};

template<class T, typename construct_param_type = void*> class CTlsObj
{
    typedef unordered_map<DWORD, T*>            TLocalMap;
    typedef typename TLocalMap::iterator        TLocalMapI;
    typedef typename TLocalMap::const_iterator  TLocalMapCI;

public:
    T* TryGet()
    {
        T* pValue = nullptr;
        {
            CReadLock locallock(m_lock);
            TLocalMapCI it = m_map.find(::GetCurrentThreadId());
            if (it != m_map.end())
                pValue = it->second;
        }
        return pValue;
    }

    T* Get()
    {
        T* pValue = TryGet();
        if (pValue == nullptr)
        {
            pValue = Construct();
            CWriteLock locallock(m_lock);
            m_map[::GetCurrentThreadId()] = pValue;
        }
        return pValue;
    }

    T& GetRef()
    {
        return *Get();
    }

    T* Get(construct_param_type& construct_param)
    {
        T* pValue = TryGet();
        if (pValue == nullptr)
        {
            pValue = ConstructWithParam(construct_param);
            CWriteLock locallock(m_lock);
            m_map[::GetCurrentThreadId()] = pValue;
        }
        return pValue;
    }

    T& GetRef(construct_param_type& construct_param)
    {
        return *Get(construct_param);
    }

    T* SetNewAndGetOld(T* pValue)
    {
        T* pOldValue = TryGet();
        if (pValue != pOldValue)
        {
            if (pValue == nullptr)
                DoRemove();
            else
            {
                CWriteLock locallock(m_lock);
                m_map[::GetCurrentThreadId()] = pValue;
            }
        }
        return pOldValue;
    }

    void Set(T* pValue)
    {
        T* pOldValue = SetNewAndGetOld(pValue);
        if (pValue != pOldValue)
            DoDelete(pOldValue);
    }

    void Remove()
    {
        T* pValue = TryGet();
        if (pValue != nullptr)
        {
            DoDelete(pValue);
            DoRemove();
        }
    }

    void Clear()
    {
        CWriteLock locallock(m_lock);
        if (!IsEmpty())
        {
            for (TLocalMapCI it = m_map.begin(), end = m_map.end(); it != end; ++it)
                DoDelete(it->second);
            m_map.clear();
        }
    }

    TLocalMap&          GetLocalMap()
    {
        return m_map;
    }
    const TLocalMap&    GetLocalMap()   const
    {
        return m_map;
    }

    CTlsObj&    operator = (T* p)
    {
        Set(p);
        return *this;
    }
    T*          operator -> ()
    {
        return Get();
    }
    const T*    operator -> ()  const
    {
        return Get();
    }
    T&          operator * ()
    {
        return GetRef();
    }
    const T&    operator * ()  const
    {
        return GetRef();
    }
    size_t Size()              const
    {
        return m_map.size();
    }
    bool IsEmpty()              const
    {
        return m_map.empty();
    }

public:
    CTlsObj()
    {
    }

    CTlsObj(T* pValue)
    {
        Set(pValue);
    }

    ~CTlsObj()
    {
        Clear();
    }

private:
    inline void DoRemove()
    {
        CWriteLock locallock(m_lock);
        m_map.erase(::GetCurrentThreadId());
    }

    static inline void DoDelete(T* pValue)
    {
        if (pValue != nullptr)
            delete pValue;
    }

    static inline T* Construct()
    {
        return new T;
    }

    static inline T* ConstructWithParam(construct_param_type& construct_param)
    {
        return new T(construct_param);
    }

private:
    CSimpleRWLock   m_lock;
    TLocalMap       m_map;

    DECLARE_NO_COPY_CLASS(CTlsObj)
};

template<class T> class CTlsSimple
{
    typedef unordered_map<DWORD, T>             TLocalMap;
    typedef typename TLocalMap::iterator        TLocalMapI;
    typedef typename TLocalMap::const_iterator  TLocalMapCI;

    static const T DEFAULT = (T)(0);

public:
    BOOL TryGet(T& tValue)
    {
        BOOL isOK = FALSE;
        {
            CReadLock locallock(m_lock);
            TLocalMapCI it = m_map.find(::GetCurrentThreadId());
            if (it != m_map.end())
            {
                tValue = it->second;
                isOK = TRUE;
            }
        }
        return isOK;
    }

    T Get(T tDefault = DEFAULT)
    {
        T tValue;
        if (TryGet(tValue))
            return tValue;
        Set(tDefault);
        return tDefault;
    }

    T SetNewAndGetOld(T tValue)
    {
        T tOldValue;
        if (!TryGet(tOldValue))
            tOldValue = DEFAULT;
        else if (tValue != tOldValue)
            Set(tValue);
        return tOldValue;
    }

    void Set(T tValue)
    {
        CWriteLock locallock(m_lock);
        m_map[::GetCurrentThreadId()] = tValue;
    }

    void Remove()
    {
        T tValue;
        if (TryGet(tValue))
        {
            CWriteLock locallock(m_lock);
            m_map.erase(::GetCurrentThreadId());
        }
    }

    void Clear()
    {
        CWriteLock locallock(m_lock);
        if (!IsEmpty())
            m_map.clear();
    }

    TLocalMap&          GetLocalMap()
    {
        return m_map;
    }
    const TLocalMap&    GetLocalMap()   const
    {
        return m_map;
    }

    CTlsSimple& operator = (T t)
    {
        Set(t);
        return *this;
    }
    BOOL operator       == (T t)
    {
        return Get() == t;
    }
    BOOL operator       != (T t)
    {
        return Get() != t;
    }
    BOOL operator       >= (T t)
    {
        return Get() >= t;
    }
    BOOL operator       <= (T t)
    {
        return Get() <= t;
    }
    BOOL operator        > (T t)
    {
        return Get() > t;
    }
    BOOL operator        < (T t)
    {
        return Get() < t;
    }

    size_t Size()  const
    {
        return m_map.size();
    }
    bool IsEmpty()  const
    {
        return m_map.empty();
    }

public:
    CTlsSimple()
    {
    }

    CTlsSimple(T tValue)
    {
        Set(tValue);
    }

    ~CTlsSimple()
    {
        Clear();
    }

    DECLARE_NO_COPY_CLASS(CTlsSimple)

private:
    CSimpleRWLock   m_lock;
    TLocalMap       m_map;
};

#if defined (_DEBUG) && defined (_DETECT_MEMORY_LEAK)
#include "debug/win32_crtdbg.h"
#endif
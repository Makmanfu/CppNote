
#ifndef CORESOCKET_DEF_H
#define CORESOCKET_DEF_H

#ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN                            // 从 Windows 头文件中排除极少使用的资料
#endif
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN                     // 从 Windows 头文件中排除极少使用的资料
#endif

#if _MSC_VER >= 1400                            // VC 8.0 以上版本

    #ifdef _DEBUG
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

#endif

#if _MSC_VER < 1600

    #if !defined(nullptr)
        #define nullptr NULL    // 定义 nulllptr
    #endif

#endif

#ifndef _WIN32_WINNT
    #if defined (_WIN64)
        #define _WIN32_WINNT    _WIN32_WINNT_WIN7   // 64 位系统默认版本为 Windows 7
    #else
        #define _WIN32_WINNT    _WIN32_WINNT_WINXP  // 32 位系统默认版本为 Windows XP
    #endif
#endif

#ifndef WINVER
    #define WINVER          _WIN32_WINNT
#endif

#ifdef _DETECT_MEMORY_LEAK
    #ifndef _CRTDBG_MAP_ALLOC
        #define _CRTDBG_MAP_ALLOC
    #endif
#endif

#if _MSC_VER >= 1600
    #include <SDKDDKVer.h>
#endif

#ifdef _USE_MFC

    #ifndef _AFX_ALL_WARNINGS
        #define _AFX_ALL_WARNINGS
    #endif
    #include <afxwin.h>             // MFC core and standard components
    #include <afxext.h>             // MFC extensions
    #include <afxdisp.h>            // MFC Automation classes
    #ifndef _AFX_NO_OLE_SUPPORT
        #include <afxdtctl.h>       // MFC support for Internet Explorer 4 Common Controls
    #endif
    #ifndef _AFX_NO_AFXCMN_SUPPORT
        #include <afxcmn.h>         // MFC support for Windows Common Controls
    #endif
    #ifndef _NO_RIBBONS_SUPPORT
        #include <afxcontrolbars.h> // MFC support for ribbons and control bars
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
#include <memory.h>
#include <malloc.h>
//#include "Singleton.h"
//#include "Event.h"
//#include "Semaphore.h"
//#include "CriticalSection.h"
//#include "STLHelper.h"
//#include "Win32Helper.h"
//#include "PrivateHeap.h"
//#include "bufferptr.h"

#if defined (_DEBUG) && defined (_DETECT_MEMORY_LEAK)
    #if defined (_WIN32_WCE)
        #include "debug/ce_crtdbg.h"
    #else
        #include "debug/win32_crtdbg.h"
    #endif
#endif


#define SINGLETON_THIS(ClassName)       ClassName::GetThis()
#define SINGLETON_INSTANCE(ClassName)   ClassName::GetInstance()
#define SINGLETON_OBJECT(ObjName)       SINGLETON_INSTANCE(C##ObjName)

#define DEFINE_SINGLETON(ClassName)                                         \
    ClassName* ClassName::m_pThis = nullptr;

#define DEFINE_P_THIS(ClassName)                                            \
    DEFINE_SINGLETON(ClassName)

#define DECLARE_SINGLETON_INTERFACE(ClassName)                              \
    public:                                                                 \
    static ClassName* GetThis()     {return m_pThis;}                       \
    static ClassName& GetInstance() {return *m_pThis;}                      \
    protected:                                                              \
    static ClassName* m_pThis;

#define DECLARE_SINGLETON_CREATE_INSTANCE(ClassName)                        \
    public:                                                                 \
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
    private:                                                                \
    ClassName(){}
#define DECLARE_PRIVATE_COPY_CONSTRUCTOR(ClassName)                         \
    private:                                                                \
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

#pragma intrinsic(_ReadBarrier)
#pragma intrinsic(_WriteBarrier)
#pragma intrinsic(_ReadWriteBarrier)

#define DEFAULT_CRISEC_SPIN_COUNT   4096

#if defined (_WIN64)
    #define DEFAULT_PAUSE_YIELD     32
    #define DEFAULT_PAUSE_CYCLE     8192
#else
    #define DEFAULT_PAUSE_YIELD     8
    #define DEFAULT_PAUSE_CYCLE     4096
#endif

#ifndef YieldProcessor
    #pragma intrinsic(_mm_pause)
    #define YieldProcessor _mm_pause
#endif


#ifndef STLHelper

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
#include <hash_set>
#include <hash_map>
#include <unordered_set>
#include <unordered_map>

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

//描述: 清除普通集合 , 适用于 vector<Object> / list<Object>  参数: vector / list / set
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

//描述: 清除指针集合 (清除前先释放指针), 适用于 vector<Object*> / list<Object*>    参数:vector / list / set
template<class PtrSet> void ClearPtrSet(PtrSet& v)
{
    for (PtrSet::iterator    it = v.begin();
         it != v.end();
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

//描述: 清除指针集合 (指针同时又指向数组), 适用于 vector<Object*[]> / list<Object*[]>  参数:vector / list / set
template<class PtrArraySet> void ClearPtrArraySet(PtrArraySet& v)
{
    for (PtrArraySet::iterator   it = v.begin();
         it != v.end();
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

//描述: 清除普通影射 , 适用于 map<key, value>  参数:map
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

//描述: 清除指针影射 (清除前先释放指针), 适用于 map<key, Object*>  参数: map
template<class PtrMap> void ClearPtrMap(PtrMap& v)
{
    for (PtrMap::iterator    it = v.begin();
         it != v.end();
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

//描述: 清除指针影射 (指针同时又指向数组), 适用于 map<key, Object*[]> 参数:map
template<class PtrArrayMap> void ClearPtrArrayMap(PtrArrayMap& v)
{
    for (PtrArrayMap::iterator   it = v.begin();
         it != v.end();
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

//描述: 清除集合-集合 (清除前先清除内部集合), 适用于 set<vector<Object>*>  参数:vector / list / set
template<class SetSet> void ClearSetSet(SetSet& v)
{
    for (SetSet::iterator    it = v.begin();
         it != v.end();
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

//描述: 清除指针集合-集合 (清除前先清除内部指针集合), 适用于 set<vector<Object*>*>  参数:vector / list / set
template<class PtrSetSet> void ClearPtrSetSet(PtrSetSet& v)
{
    for (PtrSetSet::iterator it = v.begin();
         it != v.end();
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

//描述: 清除指针数组集合影射 (清除前先清除指针数组集合), 适用于 map<vector<Object*[]>*>  参数:vector / list / set
template<class PtrArraySetSet> void ClearPtrArraySetSet(PtrArraySetSet& v)
{
    for (PtrArraySetSet::iterator    it = v.begin();
         it != v.end();
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

//描述: 清除集合影射 (清除前先清除集合), 适用于 map<key, vector<Object>*>  参数:map
template<class SetMap> void ClearSetMap(SetMap& v)
{
    for (SetMap::iterator    it = v.begin();
         it != v.end();
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

//描述: 清除指针集合影射 (清除前先清除指针集合), 适用于 map<key, vector<Object*>*>  参数:map
template<class PtrSetMap> void ClearPtrSetMap(PtrSetMap& v)
{
    for (PtrSetMap::iterator it = v.begin();
         it != v.end();
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

//描述: 清除指针数组集合影射 (清除前先清除指针数组集合), 适用于 map<key, vector<Object*[]>*>  参数:map
template<class PtrArraySetMap> void ClearPtrArraySetMap(PtrArraySetMap& v)
{
    for (PtrArraySetMap::iterator    it = v.begin();
         it != v.end();
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

//描述: 清除映射-影射 (清除前先清除内部映射), 适用于 map<key, map<key2, Object>*>  参数:map
template<class MapMap> void ClearMapMap(MapMap& v)
{
    for (MapMap::iterator    it = v.begin();
         it != v.end();
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

//描述: 清除指针映射-影射 (清除前先清除指针内部映射), 适用于 map<key, map<key2, Object*>*>  参数: map
template<class PtrMapMap> void ClearPtrMapMap(PtrMapMap& v)
{
    for (PtrMapMap::iterator it = v.begin(); it != v.end(); ++it)
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

//描述: 清除指针映射-影射 (清除前先清除指针数组内部映射), 适用于 map<key, map<key2, Object*[]>*>  参数: map
template<class PtrArrayMapMap> void ClearPtrArrayMapMap(PtrArrayMapMap& v)
{
    for (PtrArrayMapMap::iterator it = v.begin(); it != v.end(); ++it)
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



#endif


// RECT 帮助宏
#define RC_WIDTH(rc)        (rc.right - rc.left)
#define RC_HEIGHT(rc)       (rc.bottom - rc.top)
#define RC_CENTER_X(rc)     ((rc.right + rc.left) / 2)
#define RC_CENTER_Y(rc)     ((rc.bottom + rc.top) / 2)

/************************************************************************/
/*                            消息映射帮助宏                            */
/************************************************************************/

/* see: WindowsX.h */
#define HANDLE_SYS_MSG(hwnd, message, fn) HANDLE_MSG(hwnd, message, fn)

/* LRESULT Cls_OnMessage(HWND hwnd, WPARAM wParam, LPARAM lParam) */
#define HANDLE_USER_MSG(hwnd, message, fn) case (message): return (LRESULT)(fn)((hwnd), (wParam), (lParam))
#define FORWARD_USER_MSG(hwnd, message, wParam, lParam, fn) (LRESULT)(fn)((hwnd), (message), (wParam), (lParam))
#define GET_WND_PROC_INTERNAL(theClass, flag) ((WNDPROC)theClass##flag##WndProc)
#define GET_DLG_PROC_INTERNAL(theClass, flag) ((DLGPROC)theClass##flag##DlgProc)
#define DECLARE_MSG_MAP_INTERNAL(theClass, flag) static LRESULT CALLBACK theClass##flag##WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#define DECLARE_DLG_MSG_MAP_INTERNAL(theClass, flag) static BOOL CALLBACK theClass##flag##DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define BEGIN_MSG_MAP_INTERNAL(theClass, flag)                                          \
    LRESULT theClass##flag##WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)  \
    {                                                                                   \
        LRESULT result = 0;                                                             \
        switch(msg)                                                                     \
        {
#define BEGIN_DLG_MSG_MAP_INTERNAL(theClass, flag)                                      \
    BOOL theClass##flag##DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)     \
    {                                                                                   \
        BOOL    retVal = TRUE;                                                          \
        LRESULT result = 0;                                                             \
        switch(msg)                                                                     \
        {

// 窗口过程为类中的静态成员函数
#define GET_WND_PROC(theClass)          GET_WND_PROC_INTERNAL(theClass, ::)
#define GET_DLG_PROC(theClass)          GET_DLG_PROC_INTERNAL(theClass, ::)

#define DECLARE_MSG_MAP(theClass)                                                       \
    public:                                                                             \
    DECLARE_MSG_MAP_INTERNAL(theClass, ::)
#define DECLARE_DLG_MSG_MAP(theClass)                                                   \
    public:                                                                             \
    DECLARE_DLG_MSG_MAP_INTERNAL(theClass, ::)

#define BEGIN_MSG_MAP(theClass)         BEGIN_MSG_MAP_INTERNAL(theClass, ::)
#define BEGIN_DLG_MSG_MAP(theClass)     BEGIN_DLG_MSG_MAP_INTERNAL(theClass, ::)

// 消息处理函数的声明请参考: <WindowsX.h> 的 HANDLE_MSG
#define ADD_MSG_MAP(msg, fn) case (msg): result = HANDLE_##msg((hWnd), (wParam), (lParam), (fn)); break;

// LRESULT Cls_OnMessage(HWND hwnd, WPARAM wParam, LPARAM lParam)
#define ADD_USER_MSG_MAP(msg, fn) case (msg): result = (LRESULT)(fn)((hWnd), (wParam), (lParam)); break;

#define END_MSG_MAP()                                                                   \
    default:                                                                            \
    result = ::DefWindowProc(hWnd, msg, wParam, lParam);                                \
    }                                                                                   \
    return result;                                                                      \
    }

#define END_DLG_MSG_MAP()                                                               \
    default:                                                                            \
    retVal = FALSE;                                                                     \
    }                                                                                   \
    if(retVal)                                                                          \
        SetDlgMsgResult(hWnd, msg, result);                                             \
    return retVal;                                                                      \
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


//************************************************************************
//*                        应用程序唯一实例
//************************************************************************

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

//************************************************************************
//*                      句柄 (HANDLE) 智能包装器                        *
//************************************************************************

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

//************************************************************************
//*                          DC 智能包装器                               *
//************************************************************************

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
//************************************************************************
//*                          GDI Object 智能包装器                       *
//************************************************************************

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

#ifndef _WIN32_WCE
    HBITMAP CreateBitmapIndirect(CONST BITMAP* lpbm)
    {
        HBITMAP obj = ::CreateBitmapIndirect(lpbm);
        reset(obj);
        return obj;
    }
#else
    HBITMAP SHLoadDIBitMap(LPCTSTR lpszFileName)
    {
        HBITMAP obj = ::SHLoadDIBitmap(lpszFileName);
        reset(obj);
        return obj;
    }
#endif

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
#ifndef _WIN32_WCE
        DWORD bItalic,              // italic attribute option
        DWORD bUnderline,           // underline attribute option
        DWORD cStrikeOut,           // strikeout attribute option
        DWORD nCharSet,             // character set identifier
        DWORD nOutPrecision,        // output precision
        DWORD nClipPrecision,       // clipping precision
        DWORD nQuality,             // output quality
        DWORD nPitchAndFamily,      // pitch and family
#else
        BYTE bItalic,               // italic attribute option
        BYTE bUnderline,            // underline attribute option
        BYTE cStrikeOut,            // strikeout attribute option
        BYTE nCharSet,              // character set identifier
        BYTE nOutPrecision,         // output precision
        BYTE nClipPrecision,        // clipping precision
        BYTE nQuality,              // output quality
        BYTE nPitchAndFamily,       // pitch and family

#endif
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






#endif
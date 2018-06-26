

#include "stdafx.h"
#include "CoreSocket.h"


CSWMR::CSWMR()
    : m_smRead(MAXLONG)
    , m_smWrite(1)
    , m_nWaitingReaders(0)
    , m_nWaitingWriters(0)
    , m_nActive(0)
{
}

CSWMR::~CSWMR()
{
    ASSERT(m_nWaitingReaders == 0);
    ASSERT(m_nWaitingWriters == 0);
    ASSERT(m_nActive == 0);
}

VOID CSWMR::WaitToRead()
{
    BOOL fResourceWritePending;
    {
        CSpinLock locallock(m_cs);
        fResourceWritePending = (m_nWaitingWriters || (m_nActive < 0));
        if (fResourceWritePending)
            ++m_nWaitingReaders;
        else
            ++m_nActive;
    }
    if (fResourceWritePending)
        m_smRead.Wait();
}

VOID CSWMR::WaitToWrite()
{
    BOOL fResourceOwned;
    {
        CSpinLock locallock(m_cs);
        fResourceOwned = (m_nActive != 0);
        if (fResourceOwned)
            ++m_nWaitingWriters;
        else
            m_nActive = -1;
    }
    if (fResourceOwned)
        m_smWrite.Wait();
}

VOID CSWMR::Done()
{
    CSEM* pSem = nullptr;
    LONG lCount = 1;
    {
        CSpinLock locallock(m_cs);
        if (m_nActive > 0)
            --m_nActive;
        else
            ++m_nActive;
        if (m_nActive == 0)
        {
            if (m_nWaitingWriters > 0)
            {
                m_nActive = -1;
                --m_nWaitingWriters;
                pSem = &m_smWrite;
            }
            else if (m_nWaitingReaders > 0)
            {
                m_nActive = m_nWaitingReaders;
                m_nWaitingReaders = 0;
                pSem = &m_smRead;
                lCount = m_nActive;
            }
        }
    }
    if (pSem != nullptr)
        pSem->Release(lCount);
}


CRWLock::CRWLock()
    : m_smRead(MAXLONG)
    , m_smWrite(1)
    , m_nWaitingReaders(0)
    , m_nWaitingWriters(0)
    , m_nActive(0)
    , m_dwWriterTID(0)
{
}

CRWLock::~CRWLock()
{
    ASSERT(m_nActive == 0);
    ASSERT(m_dwWriterTID == 0);
}

VOID CRWLock::WaitToRead()
{
    BOOL bWait = FALSE;
    {
        CSpinLock locallock(m_cs);
        if (m_nActive > 0)
            ++m_nActive;
        else if (m_nActive == 0)
        {
            if (m_nWaitingWriters == 0)
                ++m_nActive;
            else
            {
                ++m_nWaitingReaders;
                bWait = TRUE;
            }
        }
        else
        {
            if (!IsOwner())
            {
                ++m_nWaitingReaders;
                bWait = TRUE;
            }
        }
    }
    if (bWait)
        m_smRead.Wait();
}

VOID CRWLock::WaitToWrite()
{
    BOOL bWait = FALSE;
    {
        CSpinLock locallock(m_cs);
        if (m_nActive > 0)
        {
            ++m_nWaitingWriters;
            bWait = TRUE;
        }
        else if (m_nActive == 0)
        {
            --m_nActive;
            SetOwner();
        }
        else
        {
            if (IsOwner())
                --m_nActive;
            else
            {
                ++m_nWaitingWriters;
                bWait = TRUE;
            }
        }
    }
    if (bWait)
    {
        m_smWrite.Wait();
        SetOwner();
    }
}

VOID CRWLock::ReadDone()
{
    CSEM* pSem = nullptr;
    LONG  lCount = 0;
    {
        CSpinLock locallock(m_cs);
        ASSERT(m_nActive != 0);
        if (m_nActive > 0)
        {
            if (--m_nActive == 0)
                Done(&pSem, lCount);
        }
        else
            ASSERT(IsOwner());
    }
    if (pSem != nullptr)
        pSem->Release(lCount);
}

VOID CRWLock::WriteDone()
{
    CSEM* pSem = nullptr;
    LONG  lCount = 0;
    {
        CSpinLock locallock(m_cs);
        ASSERT(m_nActive < 0);
        if (++m_nActive == 0)
        {
            DetachOwner();
            Done(&pSem, lCount);
        }
        else
            ASSERT(IsOwner());
    }
    if (pSem != nullptr)
        pSem->Release(lCount);
}

VOID CRWLock::Done(CSEM** ppSem, LONG& lCount)
{
    ASSERT(m_nActive == 0);
    ASSERT(m_dwWriterTID == 0);
    if (m_nWaitingWriters > 0)
    {
        --m_nActive;
        --m_nWaitingWriters;
        lCount = 1;
        *ppSem = &m_smWrite;
    }
    else if (m_nWaitingReaders > 0)
    {
        m_nActive = m_nWaitingReaders;
        m_nWaitingReaders = 0;
        lCount = m_nActive;
        *ppSem = &m_smRead;
    }
}


VOID SysGetSystemInfo(LPSYSTEM_INFO pInfo)
{
    ASSERT(pInfo != nullptr);
    ::GetNativeSystemInfo(pInfo);
}

DWORD SysGetNumberOfProcessors()
{
    SYSTEM_INFO si;
    SysGetSystemInfo(&si);
    return si.dwNumberOfProcessors;
}

DWORD SysGetPageSize()
{
    SYSTEM_INFO si;
    SysGetSystemInfo(&si);
    return si.dwPageSize;
}

#if _MSC_VER < 1800

BOOL SysGetOSVersionInfo(LPOSVERSIONINFO pInfo, BOOL bInfoEx)
{
    ASSERT(pInfo != nullptr);
    pInfo->dwOSVersionInfoSize = bInfoEx ? sizeof(LPOSVERSIONINFOEX) : sizeof(LPOSVERSIONINFO);
    return ::GetVersionEx(pInfo);
}

DWORD SysGetOSVersion()
{
    OSVERSIONINFO vi;
    DWORD dwOSVersion = 0;
    if (SysGetOSVersionInfo(&vi))
        dwOSVersion = (vi.dwMajorVersion << 16) + vi.dwMinorVersion;
    return dwOSVersion;
}

#endif


// Ö§³Ö timeGetTime() º¯Êý
#include <MmSystem.h>
#pragma comment(lib, "Winmm")

DWORD TimeGetTime()
{
    return ::timeGetTime();
}

DWORD GetTimeGap32(DWORD dwOriginal)
{
    return ::timeGetTime() - dwOriginal;
}

#if _WIN32_WINNT >= _WIN32_WINNT_WS08
ULONGLONG GetTimeGap64(ULONGLONG ullOriginal)
{
    return ::GetTickCount64() - ullOriginal;
}
#endif

BOOL PeekMessageLoop(BOOL bDispatchQuitMsg)
{
    BOOL value = TRUE;
    MSG msg;
    while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT && !bDispatchQuitMsg)
        {
            value = FALSE;
            break;
        }
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }
    return value;
}

DWORD WaitForMultipleObjectsWithMessageLoop(DWORD dwHandles, HANDLE szHandles[], DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
    DWORD dwResult = WAIT_FAILED;
    DWORD dwBeginTime = (dwMilliseconds == INFINITE) ? INFINITE : ::timeGetTime();
    while (TRUE)
    {
        int iWaitTime;
        if (dwBeginTime != INFINITE)
        {
            iWaitTime = dwMilliseconds - (GetTimeGap32(dwBeginTime));
            if (iWaitTime <= 0)
            {
                dwResult = WAIT_TIMEOUT;
                break;
            }
        }
        else
            iWaitTime = INFINITE;
        dwResult = ::MsgWaitForMultipleObjectsEx(dwHandles, szHandles, iWaitTime, dwWakeMask, dwFlags);
        ASSERT(dwResult != WAIT_FAILED);
        if (dwResult == (WAIT_OBJECT_0 + dwHandles))
            PeekMessageLoop();
        else
            break;
    }
    return dwResult;
}

BOOL MsgWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
    DWORD dwResult = WaitForMultipleObjectsWithMessageLoop(1, &hHandle, dwMilliseconds, dwWakeMask, dwFlags);
    switch (dwResult)
    {
        case WAIT_OBJECT_0:
            return TRUE;
        case WAIT_FAILED:
            ASSERT(FALSE);
        case WAIT_TIMEOUT:
            return FALSE;
        default:
            ASSERT(FALSE);
    }
    return FALSE;
}

void WaitWithMessageLoop(DWORD dwMilliseconds, DWORD dwWakeMask, DWORD dwFlags)
{
    static CEvt evWait;
    VERIFY(MsgWaitForSingleObject(evWait, dwMilliseconds, dwWakeMask, dwFlags) == FALSE);
}

void WaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval)
{
    while (*plWorkingItemCount > lMaxWorkingItemCount)
        ::Sleep(dwCheckInterval);
}

void WaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval)
{
    WaitForWorkingQueue(plWorkingItemCount, 0, dwCheckInterval);
}

void MsgWaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval)
{
    while (*plWorkingItemCount > lMaxWorkingItemCount)
        WaitWithMessageLoop(dwCheckInterval);
}

void MsgWaitForComplete(long* plWorkingItemCount, DWORD dwCheckInterval)
{
    MsgWaitForWorkingQueue(plWorkingItemCount, 0, dwCheckInterval);
}


const DWORD TItem::DEFAULT_ITEM_CAPACITY = ::SysGetPageSize();
const DWORD CItemPool::DEFAULT_ITEM_CAPACITY = TItem::DEFAULT_ITEM_CAPACITY;
const DWORD CItemPool::DEFAULT_POOL_SIZE = 300;
const DWORD CItemPool::DEFAULT_POOL_HOLD = 900;
const DWORD CBufferPool::DEFAULT_ITEM_CAPACITY = CItemPool::DEFAULT_ITEM_CAPACITY;
const DWORD CBufferPool::DEFAULT_ITEM_POOL_SIZE = CItemPool::DEFAULT_POOL_SIZE;
const DWORD CBufferPool::DEFAULT_ITEM_POOL_HOLD = CItemPool::DEFAULT_POOL_HOLD;
const DWORD CBufferPool::DEFAULT_BUFFER_LOCK_TIME = 5000;
const DWORD CBufferPool::DEFAULT_BUFFER_POOL_SIZE = 150;
const DWORD CBufferPool::DEFAULT_BUFFER_POOL_HOLD = 450;

TItem* TItem::Construct(CPrivateHeap& heap, int capacity, BYTE* pData, int length)
{
    ASSERT(capacity > 0);
    int item_size = sizeof(TItem);
    TItem* pItem = (TItem*)heap.Alloc(item_size + capacity);
    pItem->head = (BYTE*)pItem + item_size;
    pItem->TItem::TItem(heap, capacity, pData, length);
    return pItem;
}

void TItem::Destruct(TItem* pItem)
{
    ASSERT(pItem != nullptr);
    CPrivateHeap& heap = pItem->heap;
    pItem->TItem::~TItem();
    heap.Free(pItem);
}

inline int TItem::Cat(const BYTE* pData, int length)
{
    ASSERT(pData != nullptr && length > 0);
    int cat = min(Remain(), length);
    if (cat > 0)
    {
        memcpy(end, pData, cat);
        end += cat;
    }
    return cat;
}

inline int TItem::Cat(const TItem& other)
{
    ASSERT(this != &other);
    return Cat(other.Ptr(), other.Size());
}

inline int TItem::Fetch(BYTE* pData, int length)
{
    ASSERT(pData != nullptr && length > 0);
    int fetch = min(Size(), length);
    memcpy(pData, begin, fetch);
    begin += fetch;
    return fetch;
}

inline int TItem::Peek(BYTE* pData, int length)
{
    ASSERT(pData != nullptr && length > 0);
    int peek = min(Size(), length);
    memcpy(pData, begin, peek);
    return peek;
}

inline int TItem::Reduce(int length)
{
    ASSERT(length > 0);
    int reduce = min(Size(), length);
    begin += reduce;
    return reduce;
}

inline void TItem::Reset(int first, int last)
{
    ASSERT(first >= -1 && first <= capacity);
    ASSERT(last >= -1 && last <= capacity);
    if (first >= 0)  begin = head + min(first, capacity);
    if (last >= 0)   end = head + min(last, capacity);
}

int TItemList::Cat(const BYTE* pData, int length)
{
    int remain = length;
    while (remain > 0)
    {
        TItem* pItem = Back();
        if (pItem == nullptr || pItem->IsFull())
            pItem = PushBack(itPool.PickFreeItem());
        int cat = pItem->Cat(pData, remain);
        pData += cat;
        remain -= cat;
    }
    return length;
}

int TItemList::Cat(const TItem* pItem)
{
    return Cat(pItem->Ptr(), pItem->Size());
}

int TItemList::Cat(const TItemList& other)
{
    ASSERT(this != &other);
    int length = 0;
    for (TItem* pItem = other.Front(); pItem != nullptr; pItem = pItem->next)
        length += Cat(pItem);
    return length;
}

int TItemList::Fetch(BYTE* pData, int length)
{
    int remain = length;
    while (remain > 0 && Size() > 0)
    {
        TItem* pItem = Front();
        int fetch = pItem->Fetch(pData, remain);
        pData += fetch;
        remain -= fetch;
        if (pItem->IsEmpty())
            itPool.PutFreeItem(PopFront());
    }
    return length - remain;
}

int TItemList::Peek(BYTE* pData, int length)
{
    int remain = length;
    TItem* pItem = Front();
    while (remain > 0 && pItem != nullptr)
    {
        int peek = pItem->Peek(pData, remain);
        pData += peek;
        remain -= peek;
        pItem = pItem->next;
    }
    return length - remain;
}

int TItemList::Reduce(int length)
{
    int remain = length;
    while (remain > 0 && Size() > 0)
    {
        TItem* pItem = Front();
        remain -= pItem->Reduce(remain);
        if (pItem->IsEmpty())
            itPool.PutFreeItem(PopFront());
    }
    return length - remain;
}

void TItemList::Release()
{
    itPool.PutFreeItem(*this);
}

void CItemPool::PutFreeItem(TItem* pItem)
{
    ASSERT(pItem != nullptr);
    DWORD size = m_lsFreeItem.Size();
    if (size < m_dwPoolHold)
    {
        CCriSecLock locallock(m_csFreeItem);
        m_lsFreeItem.PushBack(pItem);
    }
    else
    {
        TItem::Destruct(pItem);
        CompressFreeItem(m_dwPoolSize);
    }
}

void CItemPool::PutFreeItem(TItemList& lsItem)
{
    DWORD addSize = lsItem.Size();
    if (addSize > 0)
    {
        DWORD cacheSize = m_lsFreeItem.Size();
        DWORD totalSize = addSize + cacheSize;
        if (totalSize <= m_dwPoolHold)
        {
            CCriSecLock locallock(m_csFreeItem);
            m_lsFreeItem.Shift(lsItem);
        }
        else
        {
            lsItem.Clear();
            if (cacheSize >= m_dwPoolHold)
                CompressFreeItem(m_dwPoolSize);
        }
    }
}

TItem* CItemPool::PickFreeItem()
{
    TItem* pItem = nullptr;
    if (m_lsFreeItem.Size() > 0)
    {
        CCriSecLock locallock(m_csFreeItem);
        if (m_lsFreeItem.Size() > 0)
            pItem = m_lsFreeItem.PopFront();
    }
    if (pItem == nullptr)
        pItem = TItem::Construct(m_heap, m_dwItemCapacity);
    else
        pItem->Reset();
    return pItem;
}

inline void CItemPool::Clear()
{
    {
        CCriSecLock locallock(m_csFreeItem);
        m_lsFreeItem.Clear();
    }
    m_heap.Reset();
}

void CItemPool::CompressFreeItem(int size)
{
    CCriSecLock locallock(m_csFreeItem);
    while (m_lsFreeItem.Size() > size)
        TItem::Destruct(m_lsFreeItem.PopFront());
}

TBuffer* TBuffer::Construct(CBufferPool& pool, ULONG_PTR dwID)
{
    ASSERT(dwID != 0);
    CPrivateHeap& heap = pool.GetPrivateHeap();
    TBuffer* pBuffer = (TBuffer*)heap.Alloc(sizeof(TBuffer));
    pBuffer->TBuffer::TBuffer(heap, pool.GetItemPool(), dwID);
    return pBuffer;
}

void TBuffer::Destruct(TBuffer* pBuffer)
{
    ASSERT(pBuffer != nullptr);
    CPrivateHeap& heap = pBuffer->heap;
    pBuffer->TBuffer::~TBuffer();
    heap.Free(pBuffer);
}

inline void TBuffer::Reset()
{
    id = 0;
    length = 0;
    freeTime = ::TimeGetTime();
}

int TBuffer::Cat(const BYTE* pData, int len)
{
    items.Cat(pData, len);
    return IncreaseLength(len);
}

int TBuffer::Cat(const TItem* pItem)
{
    items.Cat(pItem);
    return IncreaseLength(pItem->Size());
}

int TBuffer::Cat(const TItemList& other)
{
    ASSERT(&items != &other);
    for (TItem* pItem = other.Front(); pItem != nullptr; pItem = pItem->next)
        Cat(pItem);
    return length;
}

int TBuffer::Fetch(BYTE* pData, int len)
{
    int fetch = items.Fetch(pData, len);
    DecreaseLength(fetch);
    return fetch;
}

int TBuffer::Peek(BYTE* pData, int len)
{
    return items.Peek(pData, len);
}

int TBuffer::Reduce(int len)
{
    int reduce = items.Reduce(len);
    DecreaseLength(reduce);
    return reduce;
}

void CBufferPool::PutFreeBuffer(ULONG_PTR dwID)
{
    ASSERT(dwID != 0);
    TBuffer* pBuffer = FindCacheBuffer(dwID);
    if (pBuffer != nullptr)
    {
        {
            CReentrantWriteLock locallock(m_csBufferMap);
            m_mpBuffer.erase(dwID);
        }
        PutFreeBuffer(pBuffer);
    }
}

void CBufferPool::PutFreeBuffer(TBuffer* pBuffer)
{
    if (pBuffer->IsValid())
    {
        BOOL bOK = FALSE;
        {
            CCriSecLock locallock(pBuffer->cs);
            if (pBuffer->IsValid())
            {
                pBuffer->Reset();
                bOK = TRUE;
            }
        }
        if (bOK)
        {
            m_itPool.PutFreeItem(pBuffer->items);
            {
                CCriSecLock locallock(m_csFreeBuffer);
                m_lsFreeBuffer.PushBack(pBuffer);
            }
            if ((DWORD)m_lsFreeBuffer.Size() > m_dwBufferPoolHold)
                CompressFreeBuffer(m_dwBufferPoolSize);
        }
    }
}

void CBufferPool::CompressFreeBuffer(int size)
{
    CCriSecLock locallock(m_csFreeBuffer);
    DWORD now = ::TimeGetTime();
    while (m_lsFreeBuffer.Size() > size)
    {
        TBuffer* pBuffer = m_lsFreeBuffer.Front();
        if (now - pBuffer->freeTime >= m_dwBufferLockTime)
        {
            m_lsFreeBuffer.PopFront();
            TBuffer::Destruct(pBuffer);
        }
        else
            break;
    }
}

void CBufferPool::PutCacheBuffer(ULONG_PTR dwID)
{
    ASSERT(dwID != 0);
    CReentrantWriteLock locallock(m_csBufferMap);
    m_mpBuffer[dwID] = PickFreeBuffer(dwID);
}

TBuffer* CBufferPool::PickFreeBuffer(ULONG_PTR dwID)
{
    ASSERT(dwID != 0);
    TBuffer* pBuffer = nullptr;
    if (m_lsFreeBuffer.Size() > 0)
    {
        CCriSecLock locallock(m_csFreeBuffer);
        if (m_lsFreeBuffer.Size() > 0)
        {
            pBuffer = m_lsFreeBuffer.Front();
            if (::GetTimeGap32(pBuffer->freeTime) >= m_dwBufferLockTime)
                m_lsFreeBuffer.PopFront();
            else
                pBuffer = nullptr;
        }
    }
    if (pBuffer) pBuffer->id = dwID;
    else        pBuffer = TBuffer::Construct(*this, dwID);
    ASSERT(pBuffer);
    return pBuffer;
}

TBuffer* CBufferPool::FindCacheBuffer(ULONG_PTR dwID)
{
    ASSERT(dwID != 0);
    TBuffer* pBuffer = nullptr;
    CReentrantReadLock locallock(m_csBufferMap);
    TBufferPtrMapCI it = m_mpBuffer.find(dwID);
    if (it != m_mpBuffer.end())
        pBuffer = it->second;
    return pBuffer;
}

void CBufferPool::Clear()
{
    {
        CReentrantWriteLock locallock(m_csBufferMap);
        for (TBufferPtrMapCI it = m_mpBuffer.begin(); it != m_mpBuffer.end(); ++it)
            TBuffer::Destruct(it->second);
        m_mpBuffer.clear();
    }
    {
        CCriSecLock locallock(m_csFreeBuffer);
        m_lsFreeBuffer.Clear();
    }
    m_itPool.Clear();
    m_heap.Reset();
}














CSocket::CSocket()
{
    InitSocket();
}

CSocket::~CSocket()
{
    ExitSocket();
}

bool CSocket::InitSocket(void)
{
    return true;
}

bool CSocket::ExitSocket(void)
{
    return false;
}

bool CSocket::StartServer(void)
{
    return false;
}








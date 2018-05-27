
#pragma once

#include "TcpAgent.h"
#include "Common.h"

template<class T> class CTcpPullAgentT : public IPullSocket, public T
{
public:
    virtual EnFetchResult Fetch(CONNID dwConnID, BYTE* pData, int iLength)
    {
        TBuffer* pBuffer = m_bfPool[dwConnID];
        return ::FetchBuffer(pBuffer, pData, iLength);
    }

    virtual EnFetchResult Peek(CONNID dwConnID, BYTE* pData, int iLength)
    {
        TBuffer* pBuffer = m_bfPool[dwConnID];
        return ::PeekBuffer(pBuffer, pData, iLength);
    }

protected:
    virtual EnHandleResult DoFireHandShake(TSocketObj* pSocketObj)
    {
        EnHandleResult result = __super::DoFireHandShake(pSocketObj);
        if (result != HR_ERROR)
        {
            TBuffer* pBuffer = m_bfPool.PutCacheBuffer(pSocketObj->connID);
            VERIFY(SetConnectionReserved(pSocketObj, pBuffer));
        }
        return result;
    }

    virtual EnHandleResult DoFireReceive(TSocketObj* pSocketObj, const BYTE* pData, int iLength)
    {
        TBuffer* pBuffer = nullptr;
        GetConnectionReserved(pSocketObj, (PVOID*)&pBuffer);
        ASSERT(pBuffer && pBuffer->IsValid());
        pBuffer->Cat(pData, iLength);
        return __super::DoFireReceive(pSocketObj, pBuffer->Length());
    }

    virtual EnHandleResult DoFireClose(TSocketObj* pSocketObj, EnSocketOperation enOperation, int iErrorCode)
    {
        EnHandleResult result = __super::DoFireClose(pSocketObj, enOperation, iErrorCode);
        TBuffer* pBuffer = nullptr;
        GetConnectionReserved(pSocketObj, (PVOID*)&pBuffer);
        if (pBuffer != nullptr)
            m_bfPool.PutFreeBuffer(pBuffer);
        return result;
    }

    virtual EnHandleResult DoFireShutdown()
    {
        EnHandleResult result = __super::DoFireShutdown();
        m_bfPool.Clear();
        return result;
    }

    virtual void PrepareStart()
    {
        __super::PrepareStart();
        m_bfPool.SetMaxCacheSize(GetMaxConnectionCount());
        m_bfPool.SetItemCapacity(GetSocketBufferSize());
        m_bfPool.SetItemPoolSize(GetFreeBufferObjPool());
        m_bfPool.SetItemPoolHold(GetFreeBufferObjHold());
        m_bfPool.SetBufferLockTime(GetFreeSocketObjLockTime());
        m_bfPool.SetBufferPoolSize(GetFreeSocketObjPool());
        m_bfPool.SetBufferPoolHold(GetFreeSocketObjHold());
        m_bfPool.Prepare();
    }

public:
    CTcpPullAgentT(ITcpAgentListener* pListener)
        : T(pListener)
    {
    }

    virtual ~CTcpPullAgentT()
    {
        Stop();
    }

private:
    CBufferPool m_bfPool;
};

typedef CTcpPullAgentT<CTcpAgent> CTcpPullAgent;

#ifdef _SSL_SUPPORT

#include "SSLAgent.h"
typedef CTcpPullAgentT<CSSLAgent> CSSLPullAgent;

#endif

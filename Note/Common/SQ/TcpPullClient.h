
#pragma once

#include "TcpClient.h"
#include "MiscHelper.h"
#include "Common.h"

template<class T> class CTcpPullClientT : public IPullClient, public T
{
public:
    virtual EnFetchResult Fetch(BYTE* pData, int iLength)
    {
        return ::FetchBuffer(&m_lsBuffer, pData, iLength);
    }

    virtual EnFetchResult Peek(BYTE* pData, int iLength)
    {
        return ::PeekBuffer(&m_lsBuffer, pData, iLength);
    }

protected:
    virtual EnHandleResult DoFireReceive(ITcpClient* pSender, const BYTE* pData, int iLength)
    {
        m_lsBuffer.Cat(pData, iLength);
        return __super::DoFireReceive(pSender, m_lsBuffer.Length());
    }

    virtual void Reset()
    {
        m_lsBuffer.Clear();
        __super::Reset();
    }

public:
    CTcpPullClientT(ITcpClientListener* pListener)
        : T(pListener)
        , m_lsBuffer(m_itPool)
    {
    }

    virtual ~CTcpPullClientT()
    {
        Stop();
    }

private:
    TItemListEx m_lsBuffer;
};

typedef CTcpPullClientT<CTcpClient> CTcpPullClient;

#ifdef _SSL_SUPPORT

#include "SSLClient.h"
typedef CTcpPullClientT<CSSLClient> CSSLPullClient;

#endif


#pragma once

#include "TcpClient.h"
#include "MiscHelper.h"

template<class T> class CTcpPackClientT : public IPackClient, public T
{
public:
    virtual BOOL SendPackets(const WSABUF pBuffers[], int iCount)
    {
        int iNewCount = iCount + 1;
        unique_ptr<WSABUF[]> buffers(new WSABUF[iNewCount]);
        DWORD header;
        if (!::AddPackHeader(pBuffers, iCount, buffers, m_dwMaxPackSize, m_usHeaderFlag, header))
            return FALSE;
        return __super::SendPackets(buffers.get(), iNewCount);
    }

protected:
    virtual EnHandleResult DoFireReceive(ITcpClient* pSender, const BYTE* pData, int iLength)
    {
        return ParsePack(this, &m_pkInfo, &m_lsBuffer, pSender, m_dwMaxPackSize, m_usHeaderFlag, pData, iLength);
    }

    virtual BOOL CheckParams()
    {
        if ((m_dwMaxPackSize > 0 && m_dwMaxPackSize <= TCP_PACK_MAX_SIZE_LIMIT)    &&
            (m_usHeaderFlag >= 0 && m_usHeaderFlag <= TCP_PACK_HEADER_FLAG_LIMIT))
            return __super::CheckParams();
        SetLastError(SE_INVALID_PARAM, __FUNCTION__, ERROR_INVALID_PARAMETER);
        return FALSE;
    }

    virtual void Reset()
    {
        m_lsBuffer.Clear();
        m_pkInfo.Reset();
        __super::Reset();
    }

public:
    virtual void SetMaxPackSize(DWORD dwMaxPackSize)
    {
        m_dwMaxPackSize = dwMaxPackSize;
    }
    virtual void SetPackHeaderFlag(USHORT usPackHeaderFlag)
    {
        m_usHeaderFlag  = usPackHeaderFlag;
    }
    virtual DWORD GetMaxPackSize()
    {
        return m_dwMaxPackSize;
    }
    virtual USHORT GetPackHeaderFlag()
    {
        return m_usHeaderFlag;
    }

private:
    EnHandleResult DoFireSuperReceive(ITcpClient* pSender, const BYTE* pData, int iLength)
    {
        return __super::DoFireReceive(pSender, pData, iLength);
    }

    friend EnHandleResult ParsePack<> (CTcpPackClientT* pThis, TPackInfo<TItemListEx>* pInfo, TItemListEx* pBuffer, ITcpClient* pSocket,
                                       DWORD dwMaxPackSize, USHORT usPackHeaderFlag, const BYTE* pData, int iLength);

public:
    CTcpPackClientT(ITcpClientListener* pListener)
        : T(pListener)
        , m_dwMaxPackSize(TCP_PACK_DEFAULT_MAX_SIZE)
        , m_usHeaderFlag(TCP_PACK_DEFAULT_HEADER_FLAG)
        , m_pkInfo(nullptr)
        , m_lsBuffer(m_itPool)
    {
    }

    virtual ~CTcpPackClientT()
    {
        Stop();
    }

private:
    DWORD   m_dwMaxPackSize;
    USHORT  m_usHeaderFlag;

    TPackInfo<TItemListEx>  m_pkInfo;
    TItemListEx             m_lsBuffer;
};

typedef CTcpPackClientT<CTcpClient> CTcpPackClient;

#ifdef _SSL_SUPPORT

#include "SSLClient.h"
typedef CTcpPackClientT<CSSLClient> CSSLPackClient;

#endif
//*************************************************************************
//
//                  SOCK������װ
//
//
//
//*************************************************************************

#ifndef SockUnitH
#define SockUnitH

#include "CSock_def.h"


//Socket�������ܷ�װ��
class BaseSock
{
public:
    SOCKET m_sock;
private:
    bool m_bUDP;                        //UDP��ʽΪ1 
    bool m_bConnected;                  //����״̬
    string m_strHost;
    unsigned short m_nPort;
public:
    BaseSock();
    virtual ~BaseSock();
public:
    //����Socket
    bool Create(bool bUDP = false);
    //����
    virtual bool Connect(const string& host, unsigned short port);

    virtual bool Bind(unsigned short nPort);
    virtual bool Accept(BaseSock& client);
    virtual void Close();

    virtual long Send(const char* buf, long buflen);
    virtual long Recv(char* buf, long buflen);
    virtual long SendTo(const char* buf, int len, const struct sockaddr_in* toaddr, int tolen);
    virtual long RecvFrom(char* buf, int len, struct sockaddr_in* fromaddr, int* fromlen);

    virtual bool GetPeerName(string& strIP, unsigned short& nPort);
private:
    //�������״̬
    virtual bool isConnected(void);
    virtual bool isUDP(void);
    SOCKET GetHandle();

};



#endif



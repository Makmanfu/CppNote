//*************************************************************************
//
//                  SOCK基本封装
//
//
//
//*************************************************************************

#ifndef SockUnitH
#define SockUnitH

#include "CSock_def.h"


//Socket基本功能封装类
class BaseSock
{
public:
    SOCKET m_sock;
private:
    bool m_bUDP;                        //UDP方式为1 
    bool m_bConnected;                  //连接状态
    string m_strHost;
    unsigned short m_nPort;
public:
    BaseSock();
    virtual ~BaseSock();
public:
    //创建Socket
    bool Create(bool bUDP = false);
    //连接
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
    //检查连接状态
    virtual bool isConnected(void);
    virtual bool isUDP(void);
    SOCKET GetHandle();

};



#endif



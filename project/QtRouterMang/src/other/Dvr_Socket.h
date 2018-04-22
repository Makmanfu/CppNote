//********************************************************************
//
// GetMsg为处理协议
//
//
//********************************************************************/

#ifndef __DVRSOCKET_H
#define __DVRSOCKET_H
#include "SocketInterface.h"
#include "HPSocket_Core.h"
#include <iostream>
using namespace std;

class CSvrSocket: public CTcpServerListener
{
public:
    CSvrSocket(const char* f_IP, int f_Port);
    CSvrSocket();
    virtual ~CSvrSocket();
protected:
    USHORT PORT;
    LPCTSTR ADDRESS;
    bool SvrState;
    CTcpServer mServer;
private:
    virtual EnHandleResult OnPrepareListen(SOCKET soListen);
    virtual EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient);
    virtual EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength);
    virtual EnHandleResult OnReceive(CONNID dwConnID, const BYTE* pData, int iLength);
    virtual EnHandleResult OnClose(CONNID dwConnID);
    virtual EnHandleResult OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
    virtual EnHandleResult OnShutdown();
    virtual EnHandleResult GetMsg(CONNID dwConnID, const char* f_txt) = 0;
    void PrintCout(char* f_txt);
public:
    bool OnSvrStartOrEnd(bool f_rr = true);
    void SetIPandPort(const char* f_IP, int f_Port);
};

class CSvrSocket_t: public CSvrSocket
{
public:
    CSvrSocket_t() {};
    ~CSvrSocket_t() {};
    EnHandleResult GetMsg(CONNID dwConnID, const char* f_txt);
};


#endif
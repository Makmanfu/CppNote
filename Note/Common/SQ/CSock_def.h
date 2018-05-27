//*************************************************************************
//
// 
//
//
//
//*************************************************************************

#ifndef CSOCK_DEF_H
#define CSOCK_DEF_H

#ifdef WIN32
    #include <winsock2.h>
#else
    #include <unistd.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <sys/select.h>
    #include <sys/time.h>
    #include <sys/types.h>
    #include <unistd.h>
    #include <netdb.h>

    #define SOCKET int
#endif

#pragma warning (disable: 4786)

#include <string>
#include <vector>
#include <map>
using namespace std;


#endif



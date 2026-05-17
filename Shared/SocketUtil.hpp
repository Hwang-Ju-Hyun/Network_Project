#pragma once
#include "header.hpp"
#include "UDPSocket.hpp"
#include "TCPSocket.hpp"

class SocketUtil
{
    static UDPSocketPtr  CreateUDPSocket(int _vIP)
    {
        int s=socket(_vIP,SOCK_DGRAM,IPPROTO_UDP);        
        return UDPSocketPtr(new UDPSocket(s));
    }
    static TCPSocketPtr CreateTCPSocket(int _vIP)
    {
        int s= socket(_vIP,SOCK_STREAM,IPPROTO_TCP);        
        return TCPSocketPtr(new TCPSocket(s));
    }
};
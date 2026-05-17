#pragma once
#include "header.hpp"
#include <memory>

class  SocketAddress;

class UDPSocket
{
public:
    int Bind(const SocketAddress& _inAddr);
    int SendTo(const void* _inData, size_t _length, const SocketAddress& _inAddr);
    int ReceiveFrom( void* _outData, size_t _Maxlength,  SocketAddress& _outAddr);
private:
    SOCKET m_Socket;
    UDPSocket(SOCKET _socket);
    friend class SocketUtil;
};

using UDPSocketPtr=std::shared_ptr<UDPSocket>;
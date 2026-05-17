#pragma once
#include "header.hpp"
#include <memory>
#include "SocketAddress.hpp"

class TCPSocket;
using TCPSocketPtr=std::shared_ptr<TCPSocket>;

class TCPSocket
{
public:
    int Bind(const SocketAddress& _inAddr);    
    int Listen(int _inBackLog=32);
    int Connect(const SocketAddress& _inAddress);    
    TCPSocketPtr Accept(SocketAddress& _fromAddress);
    int Send(const void* _inData,size_t _inLen);
    int Receive(void* _inData,size_t _inLen);   
private:
    SOCKET m_Socket;
    TCPSocket(SOCKET _socket);
    friend class SocketUtil;
};